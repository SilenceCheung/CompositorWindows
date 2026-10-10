# Compositor for Windows (中文 / English)

> **Compositor for Windows** 是基于 **[Robbie Tilton](https://robbietilton.com/compositor)** 创作的 macOS 开源图像编辑器 **Compositor** 的专业原生 Windows 11 移植版本，内置完善的**简体中文与英文双语支持**、**Direct3D 11 硬件加速渲染**、**离线神经网络智能抠图**与**全自动持续维护构建体系**。
>
> An independent, native Windows 11 port of Compositor by Robbie Tilton, featuring bilingual English & Simplified Chinese support, Direct3D 11 accelerated canvas, offline AI background removal, and sustainable upstream tracking.

![Compositor on Windows, editing the After the wind demonstration](docs/images/editor.png)

---

## 快速导航 (Quick Navigation)

- **[下载 Windows 预览版 (Releases)](https://github.com/IAmTheBlurr/CompositorWindows/releases)**
- **[架构审计报告 (Architecture Audit)](docs/architecture-audit.md)**
- **[功能对照矩阵 (Feature Parity Matrix)](docs/feature-parity-matrix.md)**
- **[移植与实施路线图 (Porting Roadmap)](docs/porting-roadmap.md)**
- **[本地化与术语规范 (Localization Guide)](docs/localization-guide.md)**
- **[编译开发指南 (Build Guide)](docs/build-windows.md)**
- **[测试与质量保障指南 (Testing Guide)](docs/testing-guide.md)**
- **[上游同步跟踪报告 (Upstream Tracking)](docs/upstream-tracking.md)**
- **[已知限制与风险清单 (Known Limitations)](docs/known-limitations.md)**

---

## 核心特性与亮点 (Key Features)

1. **原汁原味还原专业级图像编辑体验**：
   - **图层与蒙版**：无限层级图层组/文件夹、26 种工业标准混合模式、图层蒙版、剪贴蒙版、透明度与拖拽重排。
   - **选区与变换**：矩形/椭圆选框、自由/多边形套索、魔棒、羽化、扩展收缩、自由变换与透视扭曲。
   - **绘画与修图**：支持压感和软画笔、橡皮擦、泊松污点修复画笔、仿制图章、模糊/涂抹、渐变工具、吸管。
   - **专业级调色与滤镜**：曲线、色阶直方图、色相/饱和度、曝光度、胶片颗粒、高斯模糊、动态模糊、Scanlines 扫描线、Camera Raw Filter。
2. **完整支持简体中文与英文动态切换**：
   - 菜单栏配备 **Language / 语言** 一键切换入口。
   - 无需重启程序，所有主菜单、工具栏提示、属性面板文案瞬间动态重载。
   - 采用符合中国摄影师与设计师习惯的专业中文术语（参考 Photoshop 标准规范）。
   - 用户语言设置通过 `QSettings` 自动持久化保存，初次启动自适应 Windows 系统区域。
3. **原生 Windows 11 性能与渲染架构**：
   - **Direct3D 11 + HLSL**：硬件加速笔刷覆盖率与画布交互交换链，低延迟高刷新。
   - **Direct2D / DirectWrite**：高质量矢量路径渲染与文字排版（打包开源 InterVariable 字体）。
   - **WIC + libheif/libde265**：原生解码 PNG、JPEG、TIFF、HEIC 与数码相机 RAW 文件。
   - **PSD / PSB 导入**：支持 8-bit RGB Photoshop 工程读取，可编辑水平文本图层解析。
4. **离线高精度神经主体抠图 (Offline AI Background Removal)**：
   - 内置 `ONNX Runtime 1.30` 与轻量级 `BiRefNet-lite` 深度学习模型。
   - 纯本地 CPU/GPU 推理，**无需注册账号、无需联网、无需配置 Python 环境**，一键抠图并生成图层蒙版。
5. **开箱即用原生分发安装包**：
   - **标准安装包 (`.msi`)**：基于 WiX Toolset 构建的标准 64 位安装程序，自动配置快捷方式与 `.comp` 项目扩展名关联。
   - **便携绿色版 (`.zip`)**：解压即用，内置完整 VC++ 运行时动态库与模型文件，普通 Windows 电脑双击即运行。

---

## 运行环境要求 (System Requirements)

- **操作系统**：Windows 11 x64 (23H2 / 24H2) 推荐；Windows 10 x64 ($\ge 19045$) 兼容。
- **内存**：4 GB RAM 基础；推荐 8 GB 以上。
- **磁盘空间**：安装包约 50 MB；解压/安装占用约 150 MB。
- **免开发依赖**：最终交付包已内置全部依赖，**普通用户无需安装 Visual Studio、CMake 或 Python**。

---

## 上游关系与维护机制 (Upstream Relationship & Maintenance)

- **原始项目致谢**：原始应用架构、核心编辑理念与 UI 设计归属于 **[Robbie Tilton](https://robbietilton.com/compositor)** 及其开源仓库 [robbietilton/Compositor](https://github.com/robbietilton/Compositor)。
- **独立维护声明**：本工程为独立维护的 Windows 移植与本地化分支，保留原作者的版权声明与 MIT 开源许可证。
- **上游版本同步**：
  - 核心底层 C 算法模块（`AdjustPixels.c`、`BrushPixels.c`、`ContentFill.c` 等）完全同构对齐。
  - 自动化同步脚本（`scripts/upstream-sync.py`）定期追踪官方提交动态，并在 `docs/upstream-tracking.md` 记录演进差异。
  - 官方已宣布暂不接受外部多语言 PR（"translations are on hold"），因此 Windows 版的多语言资源由本项目独立演进与长期维护。

---

## 源码编译与开发者指南 (Building from Source)

使用 Windows 11 x64、PowerShell 7、Visual Studio 2022 C++ 工具、Windows SDK 10.0.26100.0 与 Python 3.12：

```powershell
# 1. 引导依赖与离线模型
. ./scripts/bootstrap.ps1 -Python (Get-Command python).Source

# 2. CMake 配置与编译
cmake --preset windows-x64-release
cmake --build --preset windows-x64-release --target Compositor --parallel 8

# 3. 运行测试套件与本地化校验
python tests/localization/check_translations.py
ctest --preset windows-x64-release --output-on-failure

# 4. 生成 MSI 安装包与便携版 ZIP
. ./scripts/package.ps1 -Version 0.1.5
```

详细环境配置与断点调试请参阅 **[docs/build-windows.md](docs/build-windows.md)**。

---

## 开源许可证与版权声明 (License & Credits)

- 本软件主程序遵循 [MIT 许可证](LICENSE)，保留 **Copyright (c) 2026 Wonder Assembly LLC**。
- Qt 6.8 采用 LGPL-3.0 许可，对应源码与 SBOM 随安装包归档提供。
- `libheif` 与 `libde265` 遵循 LGPL-3.0 许可。
- `ONNX Runtime` 遵循 MIT 许可；`BiRefNet-lite` 模型遵循 MIT 许可。
- Inter 字体遵循 SIL Open Font License 1.1。
- 完整第三方许可证清单与物料清单详见 `dependencies/` 与 `scripts/PACKAGE-NOTICES.txt`。
