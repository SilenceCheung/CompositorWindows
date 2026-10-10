# Compositor 官方上游仓库同步跟踪报告 (Upstream Tracking)

> **跟踪配置**：
> - 官方上游仓库：`https://github.com/robbietilton/Compositor`
> - Windows 维护基准：`Compositor for Windows`
> - 原始基准提交：`a19db9011282399785dc18efcfded904627bdcc2` (Mac v1.0.4)
> - 当前对齐目标：`75147a2` (Mac v1.4.8)
> - 最近一次审计与同步时间：`2026-10-10 03:34:48 UTC`

---

## 1. 上游同步状态概览

| 维度 | 当前记录状态 |
| :--- | :--- |
| **官方最新 HEAD** | `75147a27d90c681215151a6301373dd2b07b7aa2` |
| **基准与最新提交差异** | 共包含 381 次上游提交 |
| **Windows 移植对齐层级** | 核心可观察特性已对齐至 Mac v1.4.8 |
| **底层 C 算法同步状态** | `AdjustPixels.c`, `BrushPixels.c`, `ContentFill.c`, `HealPixels.c`, `LensPixels.c`, `LevelsPixels.c`, `NoisePixels.c`, `WandPixels.c` 均保持最新同步 |
| **下一次同步评估计划** | 持续通过 GitHub Actions 定时追踪官方新 Tag |

---

## 2. 上游主要演进特性移植状态矩阵

| 官方版本 | 核心提交摘要 | Windows 对应模块 | 状态 |
| :--- | :--- | :--- | :--- |
| **v1.1 ~ v1.2** | 图层组、剪贴蒙版、高级混合模式 | `src/layers/LayerOperations.cpp` | 已移植并通过测试 |
| **v1.3 ~ v1.4** | DirectWrite 文字排版、矢量形状、PSD 文本读取 | `src/text/`, `src/imaging/psd/` | 已移植并通过测试 |
| **v1.4.5** | Command Palette 全局命令面板检索 | `src/ui/CommandRegistry.cpp` | 已移植并通过测试 |
| **v1.4.6** | Canvas Only (F 键全黑沉浸模式) | `src/ui/NativeCanvas.cpp` | 已移植并通过测试 |
| **v1.4.7** | Rotate Canvas 90° CW / CCW, Don't Save 按钮样式 | `src/ui/DrawingActions.cpp` | 已移植并通过测试 |
| **v1.4.8** | Scanlines 滤镜升级、Navigator 迷你视口、Export As 透明预览 | `src/filters/`, `src/ui/` | 已移植并通过测试 |

---

## 3. 最近上游提交清单 (最新 20 条记录)

```text
75147a2 Camera Raw: adaptive Contrast, Highlights and Shadows; Dehaze, Texture, Clarity and Sharpening nearer Photoshop
57ab3bb Camera Raw's Light and Color sliders match Photoshop's Camera Raw Filter
e03923f Command or Control with a middle drag zooms, as in Blender
22c7b8d Publish update feed for Compositor 1.4.8
24ef288 Version 1.4.8
3e09948 Navigator as a minimap in the canvas's corner from 300% zoom
3c6cf5e Merge #199: Navigator
e1a5210 README lists Dither and Scanlines among the filters
cdde327 Scanlines show on solid colors: Black Level, glow that eases off near white, no glow by default
1187736 Scanlines: smoothed displacement with lines hiding those behind, ±100 px, and Dots by tone
06c3a22 Filter › Scanlines: its own filter, with Displace, Threshold, Thickness and Color Split
b1db93d Export As previews transparency on the canvas's checkerboard
450af5f File › Export As…: PNG, JPEG or PDF, sized and previewed
aac6e02 Merge #208: export a PDF
b5f9b80 Slider snap test draws its slider before pressing it (#210)
06313a9 Layers panel doesn't flash gray for a click, a stroke or a moment's work
b4e147b Layers panel reveal: no separate test file
41bd281 Merge #235: reveal canvas-selected layers in the Layers panel
cea9083 README: translations are on hold
4e035bf Don't Save matches Cancel, leaving Save the one colored button
... 及其余 361 条提交记录
```
