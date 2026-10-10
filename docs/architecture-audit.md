# Compositor 官方项目与 Windows 移植工程架构审计报告

> **审计执行团队**：首席软件架构师、Windows 桌面应用开发专家、Swift/C++/图形渲染与 GPU 计算专家、软件安全与开源合规专家  
> **审计日期**：2026-10-10  
> **官方基准版本**：Robbie Tilton / Compositor v1.4.8 (Git Commit `75147a2`)  
> **Windows 移植基准**：Compositor for Windows v0.1.5 (Git Commit `1ca4dcb`)

---

## 1. 执行摘要与审计结论

本报告对图像编辑器 **Compositor** 官方仓库（macOS 原生）与现有 Windows 移植工程进行了深度源码级技术审计。

### 核心结论速览：
1. **官方版本绝对无法直接在 Windows 上编译运行**：官方项目为 100% macOS 原生架构，深度绑定 Swift 6、SwiftUI、AppKit、Metal 着色器、Core Image、Core Graphics、vImage 及 Apple Vision 框架。这些技术在 Windows 平台上均无官方运行时或直接替代物。
2. **现有的 Windows 移植项目具备极高成熟度与生产级架构**：该移植项目基于 **C++20 + Qt 6.8 (QtWidgets) + Direct3D 11 + Direct2D / DirectWrite + WIC + ONNX Runtime (BiRefNet-lite)** 重构了完整的编辑引擎与用户界面，复用了原版所有核心 C 语言底层算法（`AdjustPixels.c`、`BrushPixels.c`、`ContentFill.c` 等），并建立了包含 932 个测试用例的测试套件、WiX MSI 与便携版打包管线。
3. **最佳工程决策**：以 `CompositorWindows`（v0.1.5）作为 Windows 版本的长期维护基线，以官方仓库（v1.4.8）为 `upstream` 参考基线，建立上游增量追踪机制。
4. **当前急需解决的核心短板**：
   - **完全缺失国际化与中文支持**：目前所有界面文案均硬编码为英文，无 `QTranslator` / `.ts` / `.qm` 支持，需立即建立双语动态切换体系。
   - **缺失自动化 CI/CD 构建发布流程**：原有 Windows 移植仓库无 `.github/workflows`，完全依赖本地环境打包，必须构建 GitHub Actions Windows Runner 自动化矩阵。
   - **上游 v1.4.5 到 v1.4.8 增量特性吸纳**：包括 Command Palette 升级、Navigator 迷你图悬浮窗（从 300% 缩放显示）、Scanlines 滤镜升级、PDF 导出与 Export As 透明棋盘预览等。

---

## 2. 官方仓库深度审计（Robbie Tilton / Compositor）

### 2.1 语言与框架技术栈
- **核心开发语言**：Swift 6.x (配合 Objective-C/C Bridging Header)。
- **GUI 框架**：macOS 原生 `SwiftUI` 与 `AppKit` (`NSWindow`, `NSView`, `NSHostingView`, `NSUndoManager`, `NSPasteboard`, `NSColorPicker`, `NSMenu`, `NSDocument`)。
- **渲染子系统**：`Metal` 图形与计算管线（`MTLDevice`, `MTLCommandQueue`, `MTLRenderPipelineState`, `MTLComputePipelineState`, `MTLTexture`），着色器采用 Metal Shading Language (`.metal`)。
- **核心图像处理**：
  - Apple 专有框架：`Core Image` (CIFilter 链条), `Core Graphics` (CGImage, CGContext), `Accelerate` (vImage 几何与色彩变换)。
  - Apple Vision 框架：`VNGenerateForegroundInstanceMaskRequest` 用于主体扣图与背景移除。
- **纯 C 模块**（原版可跨平台部分）：
  - `AdjustPixels.c` / `AdjustPixels.h`：色彩通道映射与对比度计算。
  - `BrushPixels.c` / `BrushPixels.h`：画笔覆盖掩模与羽化衰减。
  - `ContentFill.c` / `ContentFill.h`：内容识别填充邻域补全。
  - `DitherPixels.c` / `DitherPixels.h`：抖动处理。
  - `HealPixels.c` / `HealPixels.h`：污点修复与纹理融合泊松方程。
  - `LensPixels.c` / `LensPixels.h`：镜头色散与几何畸变。
  - `LevelsPixels.c` / `LevelsPixels.h`：色阶黑白场直方图重映射。
  - `NoisePixels.c` / `NoisePixels.h`：单色/高斯噪点生成。
  - `WandPixels.c` / `WandPixels.h`：魔棒选区泛洪算法。
- **工程组织**：Xcode 单体工程 `Compositor.xcodeproj`，包含 `Compositor`（主 App）、`CompositorTests`（310+ 个 XCTest 单元测试）和 `CompositorUITests`。

### 2.2 上游版本提交演进（v1.0.4 至 v1.4.8）
- 官方基线共产生 381 次提交。
- 主要版本里程碑：
  - **v1.1 ~ v1.2**：图层组、剪贴蒙版、高级混合模式（Overlay, Soft Light 等）、参考线与吸附。
  - **v1.3 ~ v1.4**：文字工具与 DirectWrite/CoreText 对接、矢量形状工具、PSD/PSB 水平文字导入、Camera Raw Filter。
  - **v1.4.5 ~ v1.4.8**（最新）：
    - `06c3a22`, `1187736`, `cdde327`: Filter › Scanlines 重构（位移、阈值、厚度、色彩分离）。
    - `3e09948`: Navigator 迷你视口（当画布放大到 300% 以上时在角落浮现）。
    - `450af5f`, `b1db93d`, `aac6e02`: File › Export As…（支持透明棋盘网格实时预览，PNG/JPEG/PDF 导出）。
    - `ad4bddf`: Canvas Only 沉浸模式（F 键切换全黑背景，隐藏所有面板）。
    - `754f188`: Image › Rotate Canvas 90° 顺时针/逆时针画布旋转。
    - `cea9083`: 明确记录官方暂不接受外部多语言 PR（"translations are on hold"），确认 Windows 移植项目必须独立维护本地化资源。

---

## 3. Windows 移植项目深度审计（Cengokill / CompositorWindows）

### 3.1 架构与技术栈
- **编程语言**：C++20 (MSVC 14.44 / /std:c++20 / /permissive- / /utf-8)。
- **GUI 框架**：Qt 6.8.3 (QtWidgets)，包含 `NativeCanvas`、`MainWindow`、`LayerPanel`、`SubjectPanel`、`CameraRawPanel` 等 89 个 UI 源文件。窗口采用沉浸式暗色设计，默认采用 Windows 11 原生标题栏，可选 Mac 风格标题栏。
- **图形与 GPU 渲染**：
  - Direct3D 11 (`d3d11`, `dxgi`, `d3dcompiler`)：GPU 画笔覆盖率计算、HLSL 着色器（`shaders/BrushCoverage.hlsl`）、交互画布合成交换链。
  - Direct2D (`d2d1`) 与 DirectWrite (`dwrite`)：高性能文本排版与光栅化渲染、矢量形状、标尺与测量辅助线。
- **图像与计算扩展**：
  - WIC (`windowscodecs.lib`)：原生系统级编解码器（PNG, JPEG, TIFF, RAW）。
  - `libheif` + `libde265`：处理 HEIC 格式。
  - `ONNX Runtime 1.30.0`：无缝集成 `BiRefNet-lite` 神经网络模型（`models/birefnet-lite.onnx`），无需 Python 或云端服务即可在离线环境下完成高精度背景抠图与主体提取。
- **项目持久化**：
  - 完全遵循 `.comp` 目录包规范（`src/persistence/ProjectStore.cpp`），支持 v1 至 v12 格式版本解析与保存，包含资产校验与安全边界限制。
- **打包与发布工具**：
  - WiX Toolset v4：构建标准 MSI x64 安装包（`scripts/msi/build-msi.ps1`）。
  - PowerShell 脚本（`scripts/package.ps1`）：生成便携版 Portable ZIP、SHA256 校验清单、软件物料清单 (SBOM) 与依赖许可证汇总。

### 3.2 现有移植完成度与测试状态
- 核心代码量超过 50,000 行 C++，测试代码量超过 30,000 行。
- 自动化测试用例共计 932 个：
  - 926 个用例正常通过。
  - 6 个已知细微差异（详见 `KNOWN-ISSUES.md`）：缩略图光标移动、高斯边框 alpha 浮点舍入、历史保留的 levels 取消基准对比。
  - 无致命崩溃或项目数据损坏缺陷。

---

## 4. 关键问题逐项论证与决策（审计报告必须回答的 12 项问题）

### 1. 原版能否通过简单的跨平台构建实现 Windows 版本？
**答：绝对不能。**  
原版大量使用苹果私有专有库（SwiftUI、AppKit、Metal、Core Image、Vision、vImage），这些 API 在 Windows 操作系统上完全不存在。任何仅改动编译器配置或增加头文件别名的尝试都会在编译前缀阶段彻底失败。

### 2. 如果不能，具体障碍是什么？
**答：主要存在四大阻断性障碍：**
1. **界面层**：SwiftUI 与 AppKit 绑定 macOS 窗口系统，Windows 没有官方可用的 SwiftUI 宿主。
2. **渲染层**：Metal API 是 Apple 专有驱动接口，Windows 仅支持 DirectX (D3D11/D3D12) 和 Vulkan。
3. **图像与滤镜框架**：Core Image 的 GPU 滤镜流水线与 Apple Vision 抠图模型无法脱离 macOS 闭源框架运行。
4. **系统基础设施**：文件包装器 (`FileWrapper`)、撤销管理器 (`NSUndoManager`)、剪贴板 (`NSPasteboard`)、字体系统 (`CoreText`) 全属 macOS 专有实现。

### 3. 现有 Windows 移植项目已经实现了什么？
**答：已经完成了核心骨架与绝大多数业务逻辑的原生移植：**
- 完整移植了画布交互、图层系统、选区工具、画笔引擎、修图工具、调色滤镜与 Camera Raw 滤镜。
- 将 Metal 替换为 Direct3D 11 + HLSL，将 CoreText 替换为 DirectWrite，将 Vision 替换为 ONNX Runtime + BiRefNet-lite。
- 完整保留并运行了所有原生 C 像素计算函数。
- 具备完整的 `.comp` 读写管线和基于 WiX 的 MSI / ZIP 打包系统。

### 4. 哪个仓库更适合作为初始开发基线？
**答：必须以 `CompositorWindows`（v0.1.5）作为 Windows 版本的初始开发基线。**  
从官方纯 Swift/macOS 源码重新移植等于推倒重来，违背最高优先级原则 5。`CompositorWindows` 经过深度验证与测试，是最高效、质量最高、风险最低的基线。

### 5. 是否需要更换 UI 框架或渲染架构？
**答：不需要，且严禁更换。**  
当前采用的 **Qt 6.8 (QtWidgets) + Direct3D 11 + Direct2D** 架构是 Windows 专业级 2D 图形软件的最佳实践组合（如 Photoshop、Substance Painter 早期均采用类似成熟技术）。Qt Widgets 保证亚毫秒级输入延迟和完善的高 DPI/无障碍支持，D3D11 保证 GPU 加速绘制效率与低端显卡极佳兼容性。

### 6. 哪些功能可以直接复用？
**答：**
- 官方原生 C 语言算法模块（`AdjustPixels.c`, `BrushPixels.c`, `ContentFill.c`, `HealPixels.c`, `LensPixels.c`, `LevelsPixels.c`, `NoisePixels.c`, `WandPixels.c`）。
- 图像测试输入测试向量与断言。
- 核心数学几何公式（贝塞尔曲线求值、色阶映射公式、仿射变换矩阵）。

### 7. 哪些功能需要重新实现？
**答：**
- **软件国际化与简体中文支持**（原版与当前移植版均无，需全新开发）。
- **CI/CD 自动化构建流程**（GitHub Actions 流程需从零建立）。
- **上游 v1.4.5 ~ v1.4.8 增量功能**（Navigator 小地图视口、Scanlines 滤镜升级、Export As 窗口与 PDF 导出等）。

### 8. 哪些依赖无法在 Windows 上使用？
**答：**
- `Metal`、`MetalKit`（由 Direct3D 11 替代）。
- `CoreImage`、`Accelerate/vImage`（由 C++ SIMD 及 D3D 着色器替代）。
- `Vision.framework`（由 ONNX Runtime + BiRefNet-lite 替代）。
- `AppKit`、`SwiftUI`（由 Qt 6 QtWidgets 替代）。
- `CoreText`（由 DirectWrite 替代）。
- `Sparkle.framework`（由内置 `src/update/Updater.cpp` 替代）。

### 9. 哪些功能的实现成本最高？
**答：**
1. **背景移除（Subject Removal / Object Selection）**：需维持 ONNX 模型加载、CPU/GPU 推理内存占用与多线程响应。
2. **大型画笔亚毫秒级响应（Large Soft Brush Latency）**：4000px 画布下 800px 软画笔需要高度优化的双缓冲与脏矩形合并。
3. **全面界面文字提取与中文本地化**：需要对 89 个 UI 文件进行文案统一提取与动态语言刷新机制改造。

### 10. 如何降低未来同步官方更新的成本？
**答：**
- 配置 Git 双远端：`origin`（定制维护仓）与 `upstream`（官方原版仓）。
- 设立自动化脚本 `scripts/upstream-sync.py`，自动抓取官方 Commit 并识别哪些改动影响了核心 C 代码或算法定义。
- 业务逻辑与系统抽象分层隔离，确保官方算法变更能快速同步到 `src/graphics/upstream/`。

### 11. 如何验证 Windows 版与原版之间的功能差异？
**答：**
- 使用 `reference/` 下的对比脚本工具（`capture_mac.py`, `compare_rgba.py`）。
- 执行跨平台标准图层工程（`.comp`）双向保存与读取验证。
- 像素级 RGBA8 差异矩阵对比，针对浮点与颜色空间舍入制定公差阈值（`ΔE < 1.0` 或容差 `<= 1/255`）。

### 12. 当前有哪些工作能够立即实施？
**答：**
1. **实施阶段 4 & 5**：建立规范的 Git 分支、远端配置和多语言资源架构。
2. **实施阶段 8（本地化）**：为整个 UI 框架建立 `I18n` 基础设施，提取并编写完整的简体中文与英文语言字典。
3. **实施阶段 10（CI/CD）**：创建 `.github/workflows/windows-build-release.yml`，在 GitHub Actions Windows Runner 上自动完成拉取依赖、CMake 构建、测试与 MSI 打包。
4. **实施阶段 7（补齐增量功能）**：同步官方 1.4.8 的 Command Palette 快捷键映射与 Scanlines/Navigator 视口增强。
