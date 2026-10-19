# Compositor 官方上游仓库同步跟踪报告 (Upstream Tracking)

> **跟踪配置**：
> - 官方上游仓库：`https://github.com/robbietilton/Compositor`
> - Windows 维护基准：`Compositor for Windows`
> - 原始基准提交：`a19db9011282399785dc18efcfded904627bdcc2` (Mac v1.0.4)
> - 当前对齐目标：`75147a2` (Mac v1.4.8)
> - 最近一次审计与同步时间：`2026-10-19 07:12:11 UTC`

---

## 1. 上游同步状态概览

| 维度 | 当前记录状态 |
| :--- | :--- |
| **官方最新 HEAD** | `365b5745eda537f984f3d5bed1e53c361dbc9c91` |
| **基准与最新提交差异** | 共包含 454 次上游提交 |
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
365b574 Publish update feed for Compositor 1.5.3
554104c Version 1.5.3
9329ab8 Accelerate large Camera Raw previews with verified Metal lookup
cf0ffa1 Reduce Camera Raw scope sampling and benchmark preview latency
f4b7009 Keep Camera Raw previews at native resolution during adjustments
fc4c273 Update zoom cursors immediately when drag direction changes
92eba05 Add middle-mouse dragging to scroll the layer list
9b3f48e Disable smooth corners when rectangle radius is zero
9f1d158 Add smooth rectangle corners and simplify shape controls
43d691b Publish update feed for Compositor 1.5.2
f2d5754 Version 1.5.2
a3a0e29 RAW import: Camera Raw… imports, then opens the Camera Raw Filter
735c2cc Camera Raw Shadows adapts to the picture as Photoshop's does
e8c9e29 Move Camera Raw scrollbar clear of the controls
8d3f2fe Merge pull request #187 from Fvzion/feature/camera-raw-section-reset
b8ff682 Show Camera Raw section reset only alongside its eye icon
87e2283 Keyboard Shortcuts: clear a shortcut, keys for CMYK proofing, two fixes
f2747c4 Update feed: list 1.5's highlights with 1.5.1's changes
5e9033f Update alert in the wide layout every time
5a18e8b Publish update feed for Compositor 1.5.1
... 及其余 434 条提交记录
```
