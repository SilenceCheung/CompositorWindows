# Compositor 官方上游仓库同步跟踪报告 (Upstream Tracking)

> **跟踪配置**：
> - 官方上游仓库：`https://github.com/robbietilton/Compositor`
> - Windows 维护基准：`Compositor for Windows`
> - 原始基准提交：`a19db9011282399785dc18efcfded904627bdcc2` (Mac v1.0.4)
> - 当前对齐目标：`75147a2` (Mac v1.4.8)
> - 最近一次审计与同步时间：`2026-10-12 08:25:10 UTC`

---

## 1. 上游同步状态概览

| 维度 | 当前记录状态 |
| :--- | :--- |
| **官方最新 HEAD** | `f5ae5bad0b25b32dda9f254e62d2f07f59478698` |
| **基准与最新提交差异** | 共包含 425 次上游提交 |
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
f5ae5ba Export As: JPEG and PNG first, PDF and EXR under More
5a2e57f HDR layers convert to 16-bit; depth tag reads "16-Bit" and refreshes
b9186f7 HDR: EXR export, and adjustment layers that reveal highlights live
a10fcb7 HDR layers keep their highlights through edits
7f239dc HDR layers: import EXR and Radiance HDR, keeping light brighter than white
e580107 A layer dragged into a new project sizes the canvas to it
d3d0ce7 Drop anywhere on the tab bar to open in a new project
067fca4 Layers panel: adjustment layer button before layer effects
2522e3b Paste an image on the New canvas screen to make the canvas
a65313a Give each test process its own pasteboard
2fe3524 Convert layers between 8 and 16 bits; 16-bit gradients and fills
3d2ada4 16-bit layers: Camera Raw and other 8-bit tools without banding
93f8e8d 16-bit blurs run once in Core Image; keep pixel buffers alive in C calls
f01e298 Blur tool on a 16-bit layer: no banding while painting
2b9603f 16-bit layers: Hue/Saturation and color adjustments without banding
6aea90a 16-bit layers: adjustments, Liquify, Smudge and Blur keep 16 bits
d9cdb81 16-bit layers: imported 16-bit images keep their precision
216be97 Ignore the local HDR plans (HDR_*.md)
bdcedf2 16-bit images are dithered to 8 bits on import, not rounded
a1bd74c Publish update feed for Compositor 1.4.10
... 及其余 405 条提交记录
```
