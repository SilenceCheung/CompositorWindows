# Compositor 中英文软件术语对照表 (Localization Glossary)

> **制定原则**：符合中国设计师与摄影师主流使用习惯，严密参考 Adobe Photoshop、Affinity Photo 行业标准中文术语，兼顾语义严谨性与界面排版紧凑性。

---

## 1. 核心概念与工作区 (Core Concepts & Workspace)

| 英文原词 (English) | 简体中文 (Simplified Chinese) | 备注说明 |
| :--- | :--- | :--- |
| Canvas | 画布 | 图像编辑区域主体 |
| Document / Project | 项目 / 工程 | `.comp` 原生工程包 |
| Workspace | 工作区 | 窗口、面板与工具布局集合 |
| Viewport | 视口 | 画布显示与导航区域 |
| Zoom | 缩放 | 视图比例调整 |
| Actual Pixels | 实际像素 (100%) | 1:1 显示 |
| Fit Canvas | 缩放至适合画布 | 自适应窗口 |
| Pixel Grid | 像素网格 | 放大到特定倍率时显示的像素网格线 |
| Guides | 参考线 | 辅助对齐标尺线 |
| Snap | 吸附 | 边界与参考线自动吸附 |
| Command Palette | 命令面板 | 全局模糊搜索命令快捷窗 (Ctrl+F) |
| Canvas Only | 纯黑全屏模式 | 隐藏周围所有面板与边缘 (F) |
| History | 历史记录 | 撤销与重做操作栈 |
| Undo / Redo | 撤销 / 重做 | 快捷键 Ctrl+Z / Ctrl+Shift+Z |

---

## 2. 图层与蒙版系统 (Layers & Masks)

| 英文原词 (English) | 简体中文 (Simplified Chinese) | 备注说明 |
| :--- | :--- | :--- |
| Layer | 图层 | 核心合成单元 |
| New Layer | 新建图层 | Ctrl+Shift+N |
| Duplicate Layer | 复制图层 | Ctrl+Shift+J |
| Delete Layer | 删除图层 | Delete |
| Rename Layer | 重命名图层 | 修改图层名称 |
| Raise / Lower Layer | 上移 / 下移图层 | 调整图层堆叠顺序 |
| Layer Group | 图层组 / 文件夹 | 图层组织容器 |
| Group Selected | 编组图层 | Ctrl+G |
| Ungroup | 取消编组 | Ctrl+Shift+G |
| Layer Mask | 图层蒙版 | 基于灰度的非破坏性像素遮罩 |
| Clipping Mask | 剪贴蒙版 | 以基底图层 Alpha 限制上方内容 |
| Merge Layers (Merge Down) | 向下合并图层 | Ctrl+E |
| Merge Visible | 合并可见图层 | 将所有可见图层拼合成单层 |
| Copy Merged | 合并复制 | 抓取当前可见区域像素 (Ctrl+Shift+C) |
| Opacity | 不透明度 | 0%~100% 连续透明度调节 |
| Blend Mode | 混合模式 | 图层像素混合计算方式 |
| Normal | 正常 | 基础混合模式 |
| Multiply | 正片叠底 | 变暗组 |
| Screen | 滤色 | 变亮组 |
| Overlay | 叠加 | 对比组 |
| Soft Light / Hard Light | 柔光 / 强光 | 对比组 |
| Difference / Exclusion | 差值 / 排除 | 比较组 |
| Hue / Saturation / Color / Luminosity | 色相 / 饱和度 / 颜色 / 明度 | 色彩组 |

---

## 3. 选区与变换 (Selection & Transform)

| 英文原词 (English) | 简体中文 (Simplified Chinese) | 备注说明 |
| :--- | :--- | :--- |
| Marquee | 选框 (矩形/椭圆) | 规则几何选区 |
| Lasso | 套索 (自由/多边形) | 自由徒手选区 |
| Magic Wand | 魔棒 | 色彩容差连续选取 |
| Object Selection | 对象选择 | 智能主体包围盒识别选区 |
| Marching Ants | 蚂蚁线 | 动态虚线选区边界指示 |
| Feather | 羽化 | 边缘柔和过渡半径 |
| Expand / Contract | 扩展 / 收缩 | 形态学扩大或缩小选区 |
| Color Range | 色彩范围 | 基于色相亮度采样生成选区 |
| Invert Selection | 反向选择 | Ctrl+Shift+I |
| Free Transform | 自由变换 | 包含缩放、旋转、斜切 (Ctrl+T) |
| Distort | 扭曲 | 自由四角透视变形 |
| Flip Horizontally / Vertically | 水平翻转 / 垂直翻转 | 镜像置换 |
| Content-Aware Fill | 内容识别填充 | 智能利用周围纹理补全选区 (Shift+Delete) |

---

## 4. 绘画与修图工具 (Painting & Retouch Tools)

| 英文原词 (English) | 简体中文 (Simplified Chinese) | 备注说明 |
| :--- | :--- | :--- |
| Brush | 画笔 | 核心栅格涂抹工具 (B) |
| Eraser | 橡皮擦 | 擦除 Alpha 像素 (E) |
| Hardness | 硬度 | 画笔边缘衰减陡峭度 (0%~100%) |
| Flow | 流量 | 每次笔画叠加上限 |
| Spacing | 间距 / 步长 | 笔触采样圆间距 |
| Clone Stamp | 仿制图章 | Alt 取样后复制像素 (S) |
| Spot Healing Brush | 污点修复画笔 | 泊松表面融合自动修图 (J) |
| Blur / Sharpen / Smudge | 模糊 / 锐化 / 涂抹 | 局部像素滤波修改 |
| Gradient | 渐变 | 线性或径向过渡着色 (G) |
| Eyedropper | 吸管 / 取色器 | 画布采样颜色 (I) |
| Foreground / Background Color | 前景色 / 背景色 | 当前活动绘制颜色对 |

---

## 5. 滤镜与调色 (Filters & Adjustments)

| 英文原词 (English) | 简体中文 (Simplified Chinese) | 备注说明 |
| :--- | :--- | :--- |
| Levels | 色阶 | 黑场/灰场/白场直方图输入输出重映射 (Ctrl+L) |
| Curves | 曲线 | 样条调色曲线 (Ctrl+M) |
| Hue/Saturation | 色相/饱和度 | 色彩三要素调整 (Ctrl+U) |
| Exposure | 曝光度 | 曝光、位移与伽马校正 |
| Gradient Map | 渐变映射 | 亮度灰度重映射色彩梯级 |
| Gaussian Blur | 高斯模糊 | 经典平滑模糊滤波 |
| Motion Blur | 动态模糊 | 具有角度与距离的方向模糊 |
| Add Noise | 添加杂色 | 单色或彩色随机分布噪点 |
| Lens Correction | 镜头校正 | 消除鱼眼桶形、枕形畸变与色差 |
| Vignette | 晕影 | 四周暗角或亮角效果 |
| Bloom | 泛光 (Bloom) | 高光漫反射光晕扩散 |
| Scanlines | 扫描线 | 复古 CRT 阴极射线管纹理模拟 |
| Camera Raw Filter | Camera Raw 滤镜 | 摄影级专业光照与光学细节调色面板 |
| Remove Background | 移除背景 | 离线神经网络一键扣图生成蒙版 |
| Last Filter | 上次滤镜 | 按原参数重复执行上次滤镜 (Ctrl+Alt+F) |
