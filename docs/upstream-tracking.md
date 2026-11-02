# Compositor 官方上游仓库同步跟踪报告 (Upstream Tracking)

> **跟踪配置**：
> - 官方上游仓库：`https://github.com/robbietilton/Compositor`
> - Windows 维护基准：`Compositor for Windows`
> - 原始基准提交：`a19db9011282399785dc18efcfded904627bdcc2` (Mac v1.0.4)
> - 当前对齐目标：`75147a2` (Mac v1.4.8)
> - 最近一次审计与同步时间：`2026-11-02 08:43:55 UTC`

---

## 1. 上游同步状态概览

| 维度 | 当前记录状态 |
| :--- | :--- |
| **官方最新 HEAD** | `63f10138938ad72252131d888ba51a34b44be05f` |
| **基准与最新提交差异** | 共包含 599 次上游提交 |
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
63f1013 The status bar shows the selection's size after the canvas's: the Marquee as it's drawn, then the box around any selection, cut to the canvas
95d3d63 Volumetric Light opens the gaps between fingers, and between an arm and the body, that Vision's whole-photo masks fill in: it works the subject mask out again on a crop around each hand and between each elbow and hip, found from the body's pose, and takes away only what's narrow within an oval in each, so a chest a crop leaves out behind a fist stays
603cbb9 No Services in the Compositor menu: it acts on selected text and files, which an image editor rarely has
e584c8c Volumetric Light's Keep Subject Clear, on by default, keeps the subject clear of the haze; off, the subject takes its own depth from the model, averaged over it, so a figure far off is misted like everything at its distance while its edge stays crisp
ead9a0e Selection outlines lose every edge that doesn't divide selected from unselected: spurs along the canvas edge and cut-outs drawn over the outline instead of merged into it, left where Core Graphics' edges miss each other by a millionth of a pixel. Edges go onto a 1/4096 px grid and opposite pairs cancel, so the fill is unchanged
6793557 Selections drop the hairline pieces Core Graphics leaves where two edges meet exactly, so taking away a box dragged past the canvas no longer leaves a line of marching ants along its edge
9cd704d ⌘H outside the Move tool is View › Show Extras: it hides the selection's outline and the guides together, and works with nothing selected; guides turned off with ⌘; stay off
9d31d1f Volumetric Light uses round 4 of the depth model, from the depth-model-4 release: 512 px at the shape nearest the photo's proportions, 6-bit, better on dark scenes and people in front of what's behind them
bb5c8cc ⌘H hides the selection's outline outside the Move tool (View › Show Selection Edges); the selection still limits edits, and any new selection, or none, shows outlines again
76b7921 Volumetric Light's depth uses the person mask only when it covers most of what the general mask found, so a small or dim figure Vision's person segmentation misses keeps its head
f1b67ae Volumetric Light's depth finds strands of hair: heads, found from each person's pose, take the person's mask refined over 30 px and kept within the general mask; the rest of the person keeps the crisp outline
50d632b Volumetric Light's depth outlines people with Vision's accurate person segmentation, which follows shoulders and hair where the general subject mask bulges past them; objects keep the general mask
bdc84f4 Volumetric Light's Extract Depth and OK wait while it gets the depth model or finds the subject; Cancel stays
0623a67 Volumetric Light uses round 2 of the depth model, better on dark and cinematic scenes, from the depth-model-2 release
92da38d Volumetric Light's depth takes the subject's edge from Vision's own mask, as Remove Background's Basic does, so straight edges stay straight against a busy backdrop
b902958 Volumetric Light previews keep what the sliders don't change (the scene, depth and beams), so moving Haze, Distance, Intensity or the color takes about 10 ms instead of 180
92f5225 Volumetric Light: a photo's own lights glow by how they stand out from around them, so a bright scene's surfaces no longer all glow
de0c568 Volumetric Light: Depth Contrast spreads depths bunched together, such as a distant city, so the haze builds through them in steps; off by default
7e98437 Credit Depth Anything V2 Small and torchvision, which DepthMini is derived from
039d445 Volumetric Light: real depth from DepthMini (2 MB, distilled from Depth Anything V2 Small), downloaded and checked the first time, with the subject's mask setting its edge; Distance sets where the haze begins; Extract Depth adds the depth as a layer
... 及其余 579 条提交记录
```
