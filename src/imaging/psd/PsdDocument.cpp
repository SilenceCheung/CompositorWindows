#include "PsdDocument.h"
#include "PsdText.h"
#include "core/Document.h"
#include "text/TextRaster.h"
#include <QString>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace compositor::imaging {
namespace {

struct Reader {
    const uint8_t* data;
    size_t size, at{};
    bool psb{};

    void need(size_t n) const {
        if (at > size || n > size - at) throw std::runtime_error("PSD ended early");
    }
    uint8_t u8() { need(1); return data[at++]; }
    uint16_t u16() { need(2); uint16_t v = uint16_t(data[at] << 8 | data[at + 1]); at += 2; return v; }
    int16_t i16() { return int16_t(u16()); }
    uint32_t u32() { need(4); uint32_t v = 0; for (int i = 0; i < 4; ++i) v = (v << 8) | data[at++]; return v; }
    int32_t i32() { return int32_t(u32()); }
    uint64_t u64() { need(8); uint64_t v = 0; for (int i = 0; i < 8; ++i) v = (v << 8) | data[at++]; return v; }
    uint64_t wide() { return psb ? u64() : u32(); }
    void skip(uint64_t n) { if (n > size - at) throw std::runtime_error("PSD ended early"); at += size_t(n); }
    std::string ascii(size_t n) { need(n); std::string out(reinterpret_cast<const char*>(data + at), n); at += n; return out; }
};

Blend blendOf(const std::string& key, std::string& report) {
    if (key == "norm") return Blend::Normal;
    if (key == "mul ") return Blend::Multiply;
    if (key == "scrn") return Blend::Screen;
    if (key == "over") return Blend::Overlay;
    if (key == "dark") return Blend::Darken;
    if (key == "lite") return Blend::Lighten;
    if (key == "diff") return Blend::Difference;
    if (key == "div ") return Blend::ColorDodge;
    if (key == "idiv") return Blend::ColorBurn;
    if (key == "hue ") return Blend::Hue;
    if (key == "sat ") return Blend::Saturation;
    if (key == "colr") return Blend::Color;
    if (key == "lum ") return Blend::Luminosity;
    if (key == "lbrn") return Blend::LinearBurn;
    if (key == "lddg") return Blend::LinearDodge;
    if (key == "sLit") return Blend::SoftLight;
    if (key == "hLit") return Blend::HardLight;
    if (key == "vLit") return Blend::VividLight;
    if (key == "lLit") return Blend::LinearLight;
    if (key == "pLit") return Blend::PinLight;
    if (key == "hMix") return Blend::HardMix;
    if (key == "smud") return Blend::Exclusion;
    if (key == "fsub") return Blend::Subtract;
    if (key == "fdiv") return Blend::Divide;
    if (key != "pass") report += "Blend mode " + key + " became Normal.\n";
    return Blend::Normal;
}

std::string parseUnicodeName(const uint8_t* payload, size_t length) {
    if (length < 4) return {};
    uint32_t count = (uint32_t(payload[0]) << 24) | (uint32_t(payload[1]) << 16) |
                     (uint32_t(payload[2]) << 8) | uint32_t(payload[3]);
    if (count == 0 || length < 4 + size_t(count) * 2) return {};
    std::vector<char16_t> units;
    units.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        char16_t ch = char16_t((uint16_t(payload[4 + i * 2]) << 8) | uint16_t(payload[4 + i * 2 + 1]));
        if (ch == 0) break;
        units.push_back(ch);
    }
    if (units.empty()) return {};
    return QString::fromUtf16(units.data(), int(units.size())).toStdString();
}

struct Crop {
    int x{}, y{}, width{}, height{};
};

Crop computeCrop(int left, int top, int right, int bottom, int canvasWidth, int canvasHeight) {
    int croppedLeft = std::min(canvasWidth, std::max(0, left));
    int croppedTop = std::min(canvasHeight, std::max(0, top));
    int croppedRight = std::max(croppedLeft, std::min(canvasWidth, right));
    int croppedBottom = std::max(croppedTop, std::min(canvasHeight, bottom));
    return Crop{
        croppedLeft - left,
        croppedTop - top,
        croppedRight - croppedLeft,
        croppedBottom - croppedTop
    };
}

std::vector<uint8_t> decodeChannel(Reader& in, int sourceWidth, int sourceHeight,
                                  uint16_t compression, const Crop* crop = nullptr) {
    int targetWidth = crop ? crop->width : sourceWidth;
    int targetHeight = crop ? crop->height : sourceHeight;
    if (targetWidth <= 0 || targetHeight <= 0) return {};

    std::vector<uint8_t> out(size_t(targetWidth) * targetHeight, 0);

    if (compression == 0) {
        if (!crop) {
            for (auto& byte : out) byte = in.u8();
            return out;
        }
        for (int y = 0; y < sourceHeight; ++y) {
            if (y >= crop->y && y < crop->y + crop->height) {
                int outY = y - crop->y;
                for (int x = 0; x < sourceWidth; ++x) {
                    uint8_t val = in.u8();
                    if (x >= crop->x && x < crop->x + crop->width) {
                        out[size_t(outY) * targetWidth + (x - crop->x)] = val;
                    }
                }
            } else {
                in.skip(sourceWidth);
            }
        }
        return out;
    }

    if (compression != 1) throw std::runtime_error("Unsupported PSD compression");

    std::vector<uint32_t> rowCounts(sourceHeight);
    for (int y = 0; y < sourceHeight; ++y) {
        rowCounts[y] = in.psb ? in.u32() : in.u16();
    }

    for (int y = 0; y < sourceHeight; ++y) {
        size_t end = in.at + rowCounts[y];
        if (!crop || (y >= crop->y && y < crop->y + crop->height)) {
            int outY = crop ? (y - crop->y) : y;
            size_t written = 0;
            while (in.at < end && written < size_t(sourceWidth)) {
                int8_t control = int8_t(in.u8());
                if (control >= 0) {
                    int count = control + 1;
                    for (int i = 0; i < count && written < size_t(sourceWidth); ++i) {
                        uint8_t val = in.u8();
                        int col = int(written++);
                        if (!crop) {
                            out[size_t(outY) * targetWidth + col] = val;
                        } else if (col >= crop->x && col < crop->x + crop->width) {
                            out[size_t(outY) * targetWidth + (col - crop->x)] = val;
                        }
                    }
                } else if (control > -128) {
                    uint8_t val = in.u8();
                    int count = 1 - control;
                    for (int i = 0; i < count && written < size_t(sourceWidth); ++i) {
                        int col = int(written++);
                        if (!crop) {
                            out[size_t(outY) * targetWidth + col] = val;
                        } else if (col >= crop->x && col < crop->x + crop->width) {
                            out[size_t(outY) * targetWidth + (col - crop->x)] = val;
                        }
                    }
                }
            }
        }
        if (in.at > end) throw std::runtime_error("PSD channel overrun");
        in.at = end;
    }
    return out;
}

std::shared_ptr<const Raster> compose(int width, int height,
                                     const std::vector<uint8_t>& r,
                                     const std::vector<uint8_t>& g,
                                     const std::vector<uint8_t>& b,
                                     const std::vector<uint8_t>& a) {
    if (width <= 0 || height <= 0 || width > 30000 || height > 30000 || uint64_t(width) * height > 100000000)
        return nullptr;
    std::vector<uint8_t> rgba(size_t(width) * height * 4);
    for (size_t i = 0; i < size_t(width) * height; ++i) {
        uint8_t red = r.empty() ? 0 : r[i];
        uint8_t green = g.empty() ? 0 : g[i];
        uint8_t blue = b.empty() ? 0 : b[i];
        uint8_t alpha = a.empty() ? 255 : a[i];
        rgba[i * 4 + 0] = uint8_t((uint16_t(red) * alpha + 127) / 255);
        rgba[i * 4 + 1] = uint8_t((uint16_t(green) * alpha + 127) / 255);
        rgba[i * 4 + 2] = uint8_t((uint16_t(blue) * alpha + 127) / 255);
        rgba[i * 4 + 3] = alpha;
    }
    return Raster::fromRgba(width, height, rgba.data(), size_t(width) * 4);
}

std::shared_ptr<const GrayRaster> composeMask(int gridWidth, int gridHeight,
                                             int offsetX, int offsetY,
                                             int patchWidth, int patchHeight,
                                             const std::vector<uint8_t>& patchData,
                                             uint8_t exteriorDefault) {
    if (gridWidth <= 0 || gridHeight <= 0 || gridWidth > 30000 || gridHeight > 30000 ||
        uint64_t(gridWidth) * gridHeight > 100000000)
        return nullptr;
    auto mask = std::make_shared<GrayRaster>();
    mask->width = gridWidth;
    mask->height = gridHeight;
    mask->pixels.assign(size_t(gridWidth) * gridHeight, exteriorDefault);
    if (!patchData.empty() && patchWidth > 0 && patchHeight > 0) {
        for (int y = 0; y < patchHeight; ++y) {
            int dstY = offsetY + y;
            if (dstY < 0 || dstY >= gridHeight) continue;
            for (int x = 0; x < patchWidth; ++x) {
                int dstX = offsetX + x;
                if (dstX < 0 || dstX >= gridWidth) continue;
                mask->pixels[size_t(dstY) * gridWidth + dstX] = patchData[size_t(y) * patchWidth + x];
            }
        }
    }
    return mask;
}

struct RawLayer {
    int top{}, left{}, bottom{}, right{};
    int sourceTop{}, sourceLeft{}, sourceBottom{}, sourceRight{};
    uint8_t opacity{255};
    uint8_t fill{255};
    bool clipping{false};
    bool hidden{false};
    std::string blendKey{"norm"};
    std::string name{"Layer"};
    int section{0};
    bool hasMask{false};
    int maskTop{}, maskLeft{}, maskBottom{}, maskRight{};
    int sourceMaskTop{}, sourceMaskLeft{}, sourceMaskBottom{}, sourceMaskRight{};
    uint8_t maskDefault{255};
    bool maskDisabled{false};
    bool maskLinked{true};
    Crop imageCrop{};
    Crop maskCrop{};
    bool cropped{false};
    std::vector<std::pair<int16_t, uint64_t>> channels;
    std::vector<uint8_t> typeTool;
    std::shared_ptr<const Raster> decodedRaster;
    std::shared_ptr<const GrayRaster> decodedMask;
};

bool fitsBudget(const std::vector<RawLayer>& layers, uint64_t maxBudget) {
    uint64_t used = 0;
    for (const auto& l : layers) {
        if (l.section == 1 || l.section == 2 || l.section == 3) continue;
        int w = std::max(0, l.right - l.left);
        int h = std::max(0, l.bottom - l.top);
        if (w > 0 && h > 0) {
            if (w > 30000 || h > 30000 || uint64_t(w) * h > maxBudget - used) return false;
            used += uint64_t(w) * h;
        }
        if (l.hasMask) {
            int mw = std::max(0, l.maskRight - l.maskLeft);
            int mh = std::max(0, l.maskBottom - l.maskTop);
            if (mw > 0 && mh > 0) {
                if (mw > 30000 || mh > 30000 || uint64_t(mw) * mh > maxBudget - used) return false;
                used += uint64_t(mw) * mh;
            }
        }
    }
    return true;
}

void cropToCanvas(RawLayer& l, int canvasWidth, int canvasHeight) {
    if (l.section == 1 || l.section == 2 || l.section == 3) return;
    auto ic = computeCrop(l.left, l.top, l.right, l.bottom, canvasWidth, canvasHeight);
    if (ic.x != 0 || ic.y != 0 || ic.width != (l.right - l.left) || ic.height != (l.bottom - l.top)) {
        l.left += ic.x;
        l.top += ic.y;
        l.right = l.left + ic.width;
        l.bottom = l.top + ic.height;
        l.imageCrop = ic;
        l.cropped = true;
    }
    if (l.hasMask) {
        auto mc = computeCrop(l.maskLeft, l.maskTop, l.maskRight, l.maskBottom, canvasWidth, canvasHeight);
        if (mc.x != 0 || mc.y != 0 || mc.width != (l.maskRight - l.maskLeft) || mc.height != (l.maskBottom - l.maskTop)) {
            l.maskLeft += mc.x;
            l.maskTop += mc.y;
            l.maskRight = l.maskLeft + mc.width;
            l.maskBottom = l.maskTop + mc.height;
            l.maskCrop = mc;
            l.cropped = true;
        }
    }
}

} // namespace

PsdImport readPsd(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open PSD");
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)), {});
    return readPsd(bytes.data(), bytes.size());
}

PsdImport readPsd(const uint8_t* bytes, size_t size) {
    Reader in{bytes, size};
    if (in.ascii(4) != "8BPS") throw std::runtime_error("Not a PSD");
    auto version = in.u16();
    if (version != 1 && version != 2) throw std::runtime_error("Unsupported PSD version");
    in.psb = (version == 2);
    in.skip(6);
    auto channels = in.u16();
    auto height = in.u32();
    auto width = in.u32();
    auto depth = in.u16();
    auto mode = in.u16();
    if (mode == 4) throw std::runtime_error("CMYK PSD files are not supported");
    if (channels < 3 || depth != 8 || mode != 3) throw std::runtime_error("Only 8-bit RGB PSD files are supported");
    if (!width || !height || width > 30000 || height > 30000 || uint64_t(width) * height > 100000000)
        throw std::runtime_error("PSD exceeds the canvas limit");

    in.skip(in.u32()); // Color Mode Data

    // Image Resources
    auto resLength = in.u32();
    auto resEnd = in.at + resLength;
    double resolution = 72.0;
    while (in.at + 12 <= resEnd) {
        if (in.ascii(4) != "8BIM") break;
        auto resId = in.u16();
        auto nameLen = in.u8();
        in.skip(nameLen);
        if ((nameLen + 1) % 2 == 1) in.skip(1);
        auto length = in.u32();
        auto start = in.at;
        if (resId == 1005 && length >= 4) {
            resolution = double(in.u32()) / 65536.0;
            if (!std::isfinite(resolution) || resolution < 1) resolution = 72.0;
            resolution = std::clamp(resolution, 1.0, 9600.0);
        }
        in.at = start + length;
        if (length % 2 == 1) in.skip(1);
    }
    in.at = resEnd;

    PsdImport result;
    result.document.width = int(width);
    result.document.height = int(height);
    result.document.resolution = resolution;
    std::string report;

    auto section = in.wide();
    auto sectionEnd = in.at + size_t(section);
    if (section >= 4) {
        auto info = in.wide();
        auto infoEnd = in.at + size_t(info);
        auto rawCount = in.i16();
        int count = std::abs(int(rawCount));
        if (count > 10000) throw std::runtime_error("Too many PSD layers");

        std::vector<RawLayer> rawLayers;
        rawLayers.reserve(count);
        for (int i = 0; i < count; ++i) {
            RawLayer r;
            r.top = in.i32();
            r.left = in.i32();
            r.bottom = in.i32();
            r.right = in.i32();
            r.sourceTop = r.top;
            r.sourceLeft = r.left;
            r.sourceBottom = r.bottom;
            r.sourceRight = r.right;

            auto channelCount = in.u16();
            if (channelCount > 56) throw std::runtime_error("Too many PSD channels");
            for (int c = 0; c < channelCount; ++c) {
                auto id = in.i16();
                auto length = in.wide();
                r.channels.push_back({id, length});
            }

            if (in.ascii(4) != "8BIM") throw std::runtime_error("Invalid PSD blend signature");
            r.blendKey = in.ascii(4);
            r.opacity = in.u8();
            r.clipping = (in.u8() != 0);
            uint8_t flags = in.u8();
            r.hidden = ((flags & 2) != 0);
            in.skip(1); // filler

            auto extraLength = in.u32();
            auto extraEnd = in.at + extraLength;

            auto maskLength = in.u32();
            auto maskEnd = in.at + maskLength;
            if (maskLength >= 20) {
                r.hasMask = true;
                r.maskTop = in.i32();
                r.maskLeft = in.i32();
                r.maskBottom = in.i32();
                r.maskRight = in.i32();
                r.sourceMaskTop = r.maskTop;
                r.sourceMaskLeft = r.maskLeft;
                r.sourceMaskBottom = r.maskBottom;
                r.sourceMaskRight = r.maskRight;
                r.maskDefault = in.u8();
                uint8_t maskFlags = in.u8();
                r.maskDisabled = ((maskFlags & 2) != 0);
                r.maskLinked = ((maskFlags & 1) == 0);
            }
            in.at = maskEnd;

            auto rangesLength = in.u32();
            in.skip(rangesLength);

            auto nameLength = in.u8();
            r.name = in.ascii(nameLength);
            auto namePad = (4 - ((nameLength + 1) % 4)) % 4;
            in.skip(namePad);

            while (in.at + 12 <= extraEnd) {
                auto signature = in.ascii(4);
                if (signature != "8BIM" && signature != "8B64") break;
                auto key = in.ascii(4);
                uint64_t blockLen = 0;
                if (signature == "8B64" || (in.psb && (key == "LMsk" || key == "Lr16" || key == "Lr32" || key == "Layr" || key == "Mt16" || key == "Mt32" || key == "Mtrn" || key == "Alph" || key == "FMsk" || key == "lnk2" || key == "FEid" || key == "FXid" || key == "PxSD"))) {
                    if (in.at + 8 > extraEnd) break;
                    blockLen = in.u64();
                } else {
                    if (in.at + 4 > extraEnd) break;
                    blockLen = in.u32();
                }
                auto payloadStart = in.at;
                if (in.at + blockLen > extraEnd) break;

                if (key == "luni") {
                    auto uName = parseUnicodeName(in.data + in.at, size_t(blockLen));
                    if (!uName.empty()) r.name = uName;
                } else if (key == "iOpa") {
                    if (blockLen >= 1) r.fill = in.data[in.at];
                } else if (key == "lsct" || key == "lsdk") {
                    if (blockLen >= 4) {
                        uint32_t kind = (uint32_t(in.data[in.at]) << 24) |
                                        (uint32_t(in.data[in.at + 1]) << 16) |
                                        (uint32_t(in.data[in.at + 2]) << 8) |
                                        uint32_t(in.data[in.at + 3]);
                        r.section = int(kind);
                    }
                } else if ((key == "TySh" || key == "txt2") && r.typeTool.empty()) {
                    r.typeTool.assign(in.data + in.at, in.data + in.at + size_t(blockLen));
                }

                in.at = payloadStart + size_t(blockLen + (blockLen & 1));
            }
            in.at = extraEnd;
            rawLayers.push_back(std::move(r));
        }

        // Budget check and canvas bounds cropping
        if (!fitsBudget(rawLayers, 100000000)) {
            for (auto& r : rawLayers) cropToCanvas(r, int(width), int(height));
        }
        for (auto& r : rawLayers) {
            int w = std::max(0, r.right - r.left);
            int h = std::max(0, r.bottom - r.top);
            if (w > 30000 || h > 30000 || uint64_t(w) * h > 100000000) {
                cropToCanvas(r, int(width), int(height));
            }
        }

        // Decode channels
        for (auto& r : rawLayers) {
            if (r.section == 3) {
                for (auto [id, length] : r.channels) in.skip(length);
                continue;
            }
            int w = std::max(0, r.right - r.left);
            int h = std::max(0, r.bottom - r.top);
            int mw = r.hasMask ? std::max(0, r.maskRight - r.maskLeft) : 0;
            int mh = r.hasMask ? std::max(0, r.maskBottom - r.maskTop) : 0;

            int srcW = std::max(0, r.sourceRight - r.sourceLeft);
            int srcH = std::max(0, r.sourceBottom - r.sourceTop);
            int srcMW = r.hasMask ? std::max(0, r.sourceMaskRight - r.sourceMaskLeft) : 0;
            int srcMH = r.hasMask ? std::max(0, r.sourceMaskBottom - r.sourceMaskTop) : 0;

            std::vector<uint8_t> red, green, blue, alpha, maskPlane;
            for (auto [id, length] : r.channels) {
                auto start = in.at;
                uint16_t compression = length >= 2 ? in.u16() : 0;
                bool isMask = (id == -2);
                int curSrcW = isMask ? srcMW : srcW;
                int curSrcH = isMask ? srcMH : srcH;
                int curTgtW = isMask ? mw : w;
                int curTgtH = isMask ? mh : h;
                const Crop* cropPtr = isMask ? (r.cropped ? &r.maskCrop : nullptr) : (r.cropped ? &r.imageCrop : nullptr);

                if (curTgtW > 0 && curTgtH > 0 && curSrcW > 0 && curSrcH > 0 && (id == 0 || id == 1 || id == 2 || id == -1 || id == -2)) {
                    try {
                        auto plane = decodeChannel(in, curSrcW, curSrcH, compression, cropPtr);
                        if (id == 0) red = std::move(plane);
                        else if (id == 1) green = std::move(plane);
                        else if (id == 2) blue = std::move(plane);
                        else if (id == -1) alpha = std::move(plane);
                        else if (id == -2) maskPlane = std::move(plane);
                    } catch (const std::exception& e) {
                        report += r.name + ": channel warning: " + e.what() + "\n";
                    }
                }
                if (in.at < start + length) in.at = start + size_t(length);
                else if (in.at > start + length) throw std::runtime_error("PSD channel overrun");
            }

            if (w > 0 && h > 0 && !red.empty()) {
                r.decodedRaster = compose(w, h, red, green, blue, alpha);
            }
            if (r.hasMask && mw > 0 && mh > 0 && !maskPlane.empty()) {
                int gridW = r.decodedRaster ? r.decodedRaster->width : int(width);
                int gridH = r.decodedRaster ? r.decodedRaster->height : int(height);
                int offX = r.decodedRaster ? (r.maskLeft - r.left) : r.maskLeft;
                int offY = r.decodedRaster ? (r.maskTop - r.top) : r.maskTop;
                r.decodedMask = composeMask(gridW, gridH, offX, offY, mw, mh, maskPlane, r.maskDefault);
            }
        }

        // Assemble layer hierarchy
        std::vector<std::string> groupStack;
        std::vector<size_t> layerIndices;
        for (size_t idx = 0; idx < rawLayers.size(); ++idx) {
            auto& r = rawLayers[idx];
            if (r.section == 3) {
                groupStack.push_back(newId());
                continue;
            }
            bool isGroup = (r.section == 1 || r.section == 2);
            std::string layerId = isGroup ? (!groupStack.empty() ? groupStack.back() : newId()) : newId();
            if (isGroup && !groupStack.empty()) {
                groupStack.pop_back();
            }
            std::string parentId = groupStack.empty() ? std::string{} : groupStack.back();

            Layer layer;
            layer.id = layerId;
            layer.name = r.name.empty() ? (isGroup ? "Group" : "Layer") : r.name;
            layer.parentId = parentId;
            layer.visible = !r.hidden;
            layer.group = isGroup;

            double opacity = (double(r.opacity) / 255.0) * (double(r.fill) / 255.0);
            layer.opacity = std::clamp(opacity, 0.0, 1.0);
            layer.blend = isGroup ? Blend::Normal : blendOf(r.blendKey, report);

            if (isGroup) {
                layer.transform = {0.0, 0.0, double(width), double(height)};
                layer.raster = nullptr;
            } else {
                int w = std::max(0, r.right - r.left);
                int h = std::max(0, r.bottom - r.top);
                layer.transform = {double(r.left), double(r.top), double(std::max(w, 1)), double(std::max(h, 1))};
                layer.raster = std::move(r.decodedRaster);
                if (r.decodedMask) {
                    layer.mask = Mask{std::move(r.decodedMask), !r.maskDisabled, r.maskLinked, std::nullopt};
                }
                if (!r.typeTool.empty()) {
                    try {
                        auto imported = readPhotoshopText(r.typeTool.data(), r.typeTool.size());
                        if (!imported.note.empty()) {
                            report += layer.name + ": " + imported.note;
                            if (imported.note.back() != '\n') report.push_back('\n');
                        }
                        if (imported.editable) {
                            try {
                                auto drawn = text::rasterize(imported.text);
                                layer.text = imported.text;
                                layer.raster = drawn.raster;
                                layer.transform.x = imported.x;
                                layer.transform.y = imported.y;
                                layer.transform.width = drawn.width;
                                layer.transform.height = drawn.height;
                            } catch (...) {
                                layer.text.reset();
                            }
                        }
                    } catch (...) {}
                }
            }
            layerIndices.push_back(idx);
            result.document.layers.push_back(std::move(layer));
        }

        // Connect clipping masks
        std::unordered_map<std::string, std::string> baseForParent;
        for (size_t i = 0; i < result.document.layers.size(); ++i) {
            auto& layer = result.document.layers[i];
            size_t recordIdx = layerIndices[i];
            const auto& r = rawLayers[recordIdx];
            if (r.clipping) {
                auto it = baseForParent.find(layer.parentId);
                if (it != baseForParent.end() && !it->second.empty()) {
                    layer.maskSourceId = it->second;
                }
            } else if (!layer.group && layer.adjustmentJson.empty()) {
                baseForParent[layer.parentId] = layer.id;
            } else {
                baseForParent[layer.parentId].clear();
            }
        }

        // Sanitize folder hierarchy: parentId must point to an existing group layer
        std::unordered_map<std::string, bool> isGroupMap;
        for (const auto& l : result.document.layers) {
            isGroupMap[l.id] = l.group;
        }
        for (auto& l : result.document.layers) {
            if (!l.parentId.empty()) {
                auto it = isGroupMap.find(l.parentId);
                if (it == isGroupMap.end() || !it->second) {
                    l.parentId.clear();
                }
            }
        }

        in.at = std::max(in.at, infoEnd);
    }
    in.at = std::max(in.at, sectionEnd);

    // If no layers had pixels, fall back to the merged background image in Section 5
    const bool hasDecodedPixels = std::any_of(result.document.layers.begin(), result.document.layers.end(),
        [](const Layer& l){ return l.raster != nullptr || l.text.has_value(); });

    if (!hasDecodedPixels && in.at + 2 <= in.size) {
        auto compression = in.u16();
        auto red = decodeChannel(in, int(width), int(height), compression);
        auto green = decodeChannel(in, int(width), int(height), compression);
        auto blue = decodeChannel(in, int(width), int(height), compression);
        Layer layer;
        layer.id = newId();
        layer.name = "Background";
        layer.transform = {0, 0, double(width), double(height)};
        layer.raster = compose(int(width), int(height), red, green, blue, {});
        result.document.layers = {std::move(layer)};
        report += "Imported the merged image.\n";
    }

    if (result.document.layers.empty()) throw std::runtime_error("PSD contains no supported image");

    validateDocument(result.document);
    report += "Imported " + std::to_string(result.document.layers.size()) + " layer(s).\n";
    result.report = report;
    return result;
}

} // namespace compositor::imaging
