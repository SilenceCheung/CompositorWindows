# Compositor 官方上游仓库同步跟踪报告 (Upstream Tracking)

> **跟踪配置**：
> - 官方上游仓库：`https://github.com/robbietilton/Compositor`
> - Windows 维护基准：`Compositor for Windows`
> - 原始基准提交：`a19db9011282399785dc18efcfded904627bdcc2` (Mac v1.0.4)
> - 当前对齐目标：`75147a2` (Mac v1.4.8)
> - 最近一次审计与同步时间：`2026-10-26 07:18:37 UTC`

---

## 1. 上游同步状态概览

| 维度 | 当前记录状态 |
| :--- | :--- |
| **官方最新 HEAD** | `2d75d9172835e1dd6111cb2b659958ec33fa5abf` |
| **基准与最新提交差异** | 共包含 560 次上游提交 |
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
2d75d91 Publish update feed for Compositor 1.5.10
99a8fc1 Compositor 1.5.10
b0f0b2b Sharpen: an unsharp mask with Amount and Radius, as a filter and an adjustment layer (project format 19)
f7ad0f9 Merge #241: crop to a custom width:height ratio, and a 2.39:1 preset
a304465 Crop ratios grouped by shape with 2.39:1 in place of 9:20, and Original renamed Canvas
bbe485f Merge main into custom crop ratios
122e585 Tool headers: flip buttons as icons, and number fields left-aligned and sized for their digits
002737a Merge #191: align and distribute layers, from one Align button in the Move bar and the Layer menu
93a8aeb Align is one button in the Move bar, a panel of icons, horizontal over vertical, that a move in progress leaves available
4a73a1d Merge main into align and distribute
8cdc2b6 Publish update feed for Compositor 1.5.9
efc06ff Compositor 1.5.9
500e133 Adjustment layer test compares only the later kinds' own settings on reopening
338a609 Lens Effects layers on the canvas show exactly what Copy Merged and the export make, with matching angles and spike sizes
7651033 Starburst shares its work: each direction turned once, each length's lights spread once for all three colors
71bb6f0 Bokeh's Round, Hexagon and Oval discs share one table of weights, worked out once a frame
79f98de RGB Split with blur runs in two passes, streaks then spectrum: several times faster, looking the same
6a28e42 Canvas keeps each adjustment layer's result while nothing under or about it changes, and Bokeh runs on the GPU without reading back
7b5ba97 Vignette, Lens Effects and Tonal Contrast as adjustment layers (project format 18), and the Filter menu splits those that can be layers from the rest
c9c7531 Publish update feed for Compositor 1.5.8
... 及其余 540 条提交记录
```
