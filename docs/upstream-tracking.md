# Compositor 官方上游仓库同步跟踪报告 (Upstream Tracking)

> **跟踪配置**：
> - 官方上游仓库：`https://github.com/robbietilton/Compositor`
> - Windows 维护基准：`Compositor for Windows`
> - 原始基准提交：`a19db9011282399785dc18efcfded904627bdcc2` (Mac v1.0.4)
> - 当前对齐目标：`75147a2` (Mac v1.4.8)
> - 最近一次审计与同步时间：`2026-11-09 08:56:41 UTC`

---

## 1. 上游同步状态概览

| 维度 | 当前记录状态 |
| :--- | :--- |
| **官方最新 HEAD** | `02e53989dc499d749d83f33023ff249de1c7b4eb` |
| **基准与最新提交差异** | 共包含 647 次上游提交 |
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
02e5398 Publish update feed for Compositor 1.6.6
2f50780 Compositor 1.6.6
2facf90 An image opened as a document goes back out as it came: its tab takes its name, and Export and Save start with its name, folder and format; Export As remembers its format between launches and opens on ⌥⇧⌘S (⌥⇧⌘W too), in place of Export JPEG
b46c993 Publish update feed for Compositor 1.6.5
a3276b1 Compositor 1.6.5
4585446 Tool bar pills fade when disabled, like the buttons beside them, rather than staying lighter
3761be8 Color Picker: the sampling hint returns at the footer's left with Cancel and OK together on the right; the hue strip gets 5 pt corners and its arrows stop 1 pt short of its ends
34ac9f0 Color Picker matches the other panels: inset padding and divider, Cancel left and OK right, without the sampling hint (the eyedropper pointer says it); Hue/Saturation's Range menu sits flush left and its value fields line up
9943ed8 Color Picker: a rounded field with a pointer filled with its color, a slim hue strip marked by pin-shaped arrows, the new color over the current one (click it to go back), and Cancel and OK in a footer like the other panels
d8f020a Sliders: the fill ends at the knob's center, ramping to empty and full over the last few points at each end, so the glass knob shows none at the minimum and all at the maximum
c214ee6 Sliders: one slim slider throughout, with a narrow upright knob that turns to Liquid Glass while held, in place of the system's wide glass knob
acfc5eb Painting far past a scaled-down layer paints it at its displayed size rather than growing it at its own resolution, and a stroke the GPU fails carries on on the CPU (#334)
8e6bca6 Stroke effects reach as far in every direction: a circle's stroke is even all the way round, inside or outside, where a square reach flattened its sides and thinned it on the diagonals (#335)
3e81af3 Publish update feed for Compositor 1.6.4
f2add91 Compositor 1.6.4
a376ade Tool rail: the reset button is a small diagonally split square, and reset and swap have larger click areas
f327645 Color swatches are one rounded square throughout, with a thin outline instead of a thick white border, and the tool rail's reset and swap move beneath them; tool bars drop "px" and label sizes before their fields; layer effect panels match the other panels' layout
70bc6d6 Tool bars: Expand, Contract and Feather share one pill, as do Flip Horizontal and Flip Vertical; the selection amount dialog's slider loses its tick marks
820b8fa Status bar shows a selection's feather beside its size, since the marching ants look the same at any feather
9986d12 Selection tools: Expand, Contract and Feather are icon buttons sharing one amount, 5 px by default; Feather adds up as it repeats; Expand and Contract keep corners sharp, so a rectangle no longer rounds or loses a corner
... 及其余 627 条提交记录
```
