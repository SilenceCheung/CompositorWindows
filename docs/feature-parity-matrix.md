# Compositor 官方版本与 Windows 移植版功能对照矩阵

> **基准说明**：
> - 官方原版基准：Robbie Tilton / Compositor v1.4.8 (`75147a2`)
> - Windows 移植基准：Compositor for Windows (`1ca4dcb` + 本工程当前提交)
> - 状态定义：`已实现`（功能完整且通过测试）、`部分实现`（具备核心功能但存在平台或特定模式差异）、`未实现`（尚未开发）

---

## A. 图层系统 (Layer System)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **普通图层** | `Document/EditorSession.swift`, `LayerAppearance.swift` | `LayerTests.swift` | 已实现 | 低 | 无 | 通过 | C++ `Layer` 实体完整映射，支持像素栅格与属性管理 | 维持单元测试覆盖 |
| **图层组与文件夹** | `Document/LayerGroups.swift` | `LayerGroupTests.swift` | 已实现 | 低 | 无 | 通过 | 支持父子树形结构，文件夹递归渲染与状态继承 | 持续监控深层嵌套性能 |
| **图层顺序调整** | `Document/LayerAppearance.swift` | `LayerReorderTests.swift` | 已实现 | 低 | Qt 树形模型交互 | 通过 | 支持 Raise / Lower / 顶部 / 底部与拖拽排序 | 完善无障碍拖拽测试 |
| **图层嵌套** | `Document/LayerGroups.swift` | `LayerGroupNestingTests.swift` | 已实现 | 低 | 无 | 通过 | 组内支持子图层与子图层组无限级嵌套 | 维持递归深度安全校验 |
| **图层透明度** | `Document/LayerAppearance.swift` | `LayerOpacityTests.swift` | 已实现 | 低 | Direct3D 混合公式 | 通过 | 支持 0%~100% 浮点连续调节，支持输入框数值微调 | 已完成中文界面本地化 |
| **混合模式** | `Rendering/SeparableBlend.swift` | `BlendModeTests.swift` | 已实现 | 低 | D3D 着色器 / CPU SIMD 混合 | 通过 | 完整实现 26+ 种混合模式（Normal, Multiply, Screen, Overlay 等） | 保持与 Photoshop 标准算法一致 |
| **图层复制与删除** | `Document/LayerAppearance.swift` | `DuplicateLayerTests.swift` | 已实现 | 低 | 无 | 通过 | 快捷键绑定 Ctrl+Shift+J (Duplicate) 与 Delete 键 | 确认撤销与重做堆栈干净 |
| **图层重命名** | `UI/NativeLayerList.swift` | `LayerRenameTests.swift` | 已实现 | 低 | Qt 对话框 / 行内输入 | 通过 | 支持双击行内或弹窗重命名 | 支持中文与特殊字符输入校验 |
| **图层合并** | `Document/LayerMerge.swift` | `LayerMergeTests.swift` | 已实现 | 中 | 异步 CPU / GPU 栅格化 | 通过 (`merge_async`) | 向下合并 (Merge Down) 与合并可见 (Merge Visible) 均可撤销 | 大画布异步合并保持进度提示 |
| **图层组操作** | `Document/LayerGroups.swift` | `LayerGroupOpsTests.swift` | 已实现 | 低 | 无 | 通过 | 解散图层组、折叠/展开、组级别可见性控制 | 维持图层树状态一致性 |
| **图层跨工程拖放** | `IO/ImageFileDrop.swift` | `ProjectLayerDropTests.cpp` | 已实现 | 中 | OLE / Qt 拖放剪贴板 | 通过 (18 项 drop 测试) | 支持在不同项目标签页间拖动复制图层 | 防御目标项目被销毁引发的野指针 |
| **图层复制与粘贴** | `Document/SelectionClipboard.swift` | `ClipboardTests.swift` | 已实现 | 低 | Windows 剪贴板 API | 通过 | 内存图层序列化与位图剪贴板读写 | 支持跨应用位图粘贴 |

---

## B. 蒙版系统 (Mask System)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **图层蒙版** | `Document/LayerMask.swift` | `LayerMaskTests.swift` | 已实现 | 低 | 8-bit 单通道灰度栅格 | 通过 | 每个图层支持独立灰度遮罩，非破坏性关联 | 维持与选区快速转换通道 |
| **蒙版绘制** | `Rendering/LiveMaskRenderer.swift` | `MaskPaintingTests.swift` | 已实现 | 低 | Direct3D 掩码纹理通道 | 通过 | 选定蒙版时画笔自动转换为黑白灰度绘制 | 提供直观的蒙版选择指示高亮 |
| **蒙版填充** | `Document/ContentFill.swift` | `MaskFillTests.swift` | 已实现 | 低 | 无 | 通过 | 支持黑/白/前景色/背景色快捷填充整个蒙版 | 保持撤销步进完整 |
| **蒙版反转** | `Document/PixelInvert.swift` | `MaskInvertTests.swift` | 已实现 | 低 | 灰度按位取反 | 通过 | 快捷键 Ctrl+I 作用于蒙版时精准反转黑白 | 保持撤销与重做操作 |
| **蒙版模糊** | `Document/Filters.swift` | `MaskBlurTests.swift` | 已实现 | 低 | CPU / GPU 高斯模糊核 | 通过 | 单独模糊蒙版通道，形成平滑过渡边 | 提供实时参数滑块预览 |
| **蒙版羽化** | `Document/LayerMask.swift` | `MaskFeatherTests.swift` | 已实现 | 低 | 距离场平滑算法 | 通过 | 支持非破坏性调节蒙版边缘羽化半径 | 优化大型画布实时响应 |
| **蒙版移动与变换** | `Document/LayerTransform.swift` | `MaskTransformTests.swift` | 已实现 | 中 | 链接锁定状态机 | 通过 | 支持蒙版与图层绑定变换或解除链接独立变换 | 增加锁链图标显式指示 |
| **剪贴蒙版** | `Document/LayerAppearance.swift` | `ClippingMaskTests.swift` | 已实现 | 低 | 像素 Alpha 逐级相乘 | 通过 | 支持 Ctrl+Alt+G 快速创建与释放剪贴蒙版链 | 验证文件夹作为基底的剪贴逻辑 |
| **文件夹蒙版** | `Document/LayerGroups.swift` | `GroupMaskTests.swift` | 已实现 | 中 | 组合成多重透明度累积 | 通过 | 作用于整个文件夹所有子图层的复合蒙版 | 维持性能与合成缓存效率 |

---

## C. 变换系统 (Transform System)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **移动** | `Document/LayerTransform.swift` | `TransformMoveTests.swift` | 已实现 | 低 | 交互手势与画布坐标映射 | 通过 | 支持拖动移动、方向键微调 (1px / Shift 10px) | 增强吸附辅助线贴合感 |
| **自由缩放** | `Rendering/TransformOverlay.swift` | `TransformScaleTests.swift` | 已实现 | 低 | 双线性 / 双三次插值 | 通过 | 支持 8 向控制柄，Shift 键等比缩放约束 | 保持控制柄高 DPI 绘制清晰 |
| **旋转** | `Document/CanvasRotation.swift` | `TransformRotateTests.swift` | 已实现 | 低 | 旋转仿射矩阵 | 通过 | 控制柄外侧悬停触发旋转光标，Shift 15° 步进 | 实时数值角度浮窗显示 |
| **水平/垂直翻转** | `Document/LayerFlip.swift` | `LayerFlipTests.swift` | 已实现 | 低 | 像素行/列对称交换 | 通过 | 菜单入口快捷翻转，支持选区或整图层翻转 | 支持中文界面文案与快捷键 |
| **自由变形与扭曲** | `Document/Distort.swift` | `DistortTests.swift` | 已实现 | 中 | 4 点透视单应性变换 (Homography) | 通过 | 四角顶点独立拖动实现透视变换 | 保持边界溢出裁剪保护 |
| **非破坏性变换** | `Document/LayerTransform.swift` | `NonDestructiveTransformTests.swift` | 已实现 | 低 | 保留原始图层像素底图 | 通过 | 确认前保持原始清晰度，提交操作才光栅化 | 取消操作即刻完整回滚 |
| **多图层同时变换** | `Document/LayerTransform.swift` | `MultiLayerTransformTests.swift` | 已实现 | 中 | 组合包围盒计算 | 通过 | 多选图层后统一缩放、旋转与移动 | 保证撤销原子性 (Atomic Undo) |
| **对齐与吸附** | `Document/Guides.swift` | `SnapTests.swift` | 已实现 | 低 | 几何边界吸附器 | 通过 | 吸附到画布边缘、中心、参考线和其它图层包围盒 | 完善吸附敏感度像素设置 |
| **精确坐标与尺寸** | `UI/TransformInspector.swift` | `TransformInspectorTests.swift` | 已实现 | 低 | 属性控制面板输入框 | 通过 | 属性栏支持直接键入 X, Y, W, H, Angle 精确值 | 支持数值滚动调整 |

---

## D. 选区系统 (Selection System)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **矩形选区** | `Document/Selection.swift` | `MarqueeTests.swift` | 已实现 | 低 | 无 | 通过 | 支持固定比例、固定尺寸、自由拉框 | 虚线蚂蚁线平滑动画渲染 |
| **椭圆选区** | `Document/Selection.swift` | `EllipseSelectionTests.swift` | 已实现 | 低 | 几何抗锯齿判定 | 通过 | 快捷切换椭圆选框，Shift 正圆约束 | 保持撤销与取消选区动作 |
| **自由套索** | `Document/Selection.swift` | `LassoTests.swift` | 已实现 | 低 | 自由多段线闭合多边形 | 通过 | 随手绘制任意封闭区域生成抗锯齿蒙版 | 优化低采样点平滑算法 |
| **多边形套索** | `Document/Selection.swift` | `PolygonLassoTests.swift` | 已实现 | 低 | 顶点点击连续折线 | 通过 | 连续点击定点，双击或回车自动闭合生成选区 | 支持退格键撤销上一个顶点 |
| **魔棒工具** | `Document/MagicWand.swift`, `Rendering/WandPixels.c` | `MagicWandTests.swift` | 已实现 | 低 | 复用原版 C `WandPixels.c` | 通过 | 容差 Tolerance 调节、连续邻域/全图采样判定 | 保持色彩空间判定精准 |
| **对象选择** | `Document/ObjectSelection.swift` | `ObjectSelectionTests.swift` | 部分实现 | 高 | Apple Vision → BiRefNet ONNX | 通过 | 框架拉框后由离线神经网络推导主体轮廓 | 针对细小边缘（毛发）提供画笔微调修复 |
| **选区羽化** | `Document/SelectionEdits.swift` | `SelectionFeatherTests.swift` | 已实现 | 低 | 距离场滤波 | 通过 | 支持 0~250px 半径选区边缘柔化 | 提供羽化对话框数值输入 |
| **选区扩展/收缩** | `Document/SelectionEdits.swift` | `SelectionExpandContractTests.swift` | 已实现 | 低 | 形态学膨胀与腐蚀运算 | 通过 | 精确像素距离扩展或内缩选区 | 修复特定极端角点锯齿 |
| **选区运算布尔操作**| `Document/Selection.swift` | `SelectionBooleanTests.swift` | 已实现 | 低 | 选区蒙版位图布尔运算 | 通过 | Shift 相加、Alt 相减、Shift+Alt 取交集 | 鼠标光标显示加号/减号提示 |
| **内容识别填充** | `Document/ContentFill.swift`, `ContentFill.c` | `ContentFillTests.swift` | 已实现 | 低 | 复用原版 C `ContentFill.c` | 通过 | 选区内根据边缘纹理自动填补修补背景 | 大选区执行保持后台异步进度 |

---

## E. 绘画与修图 (Painting & Retouch)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **画笔工具** | `Document/BrushStroke.swift`, `BrushPixels.c` | `BrushTests.swift` | 已实现 | 中 | Metal → Direct3D 11 覆盖率着色器 | 通过 | 支持大小、硬度、不透明度、流量与步长连续调节 | 超大软画笔延迟进一步优化 |
| **橡皮擦** | `Document/BrushStroke.swift` | `EraserTests.swift` | 已实现 | 低 | 擦除 Alpha 混合通道 | 通过 | 独立参数记忆，擦除底层透明或背景色 | 快捷键 E 快速响应 |
| **直线绘制** | `Document/BrushStroke.swift` | `BrushLineTests.swift` | 已实现 | 低 | Bresenham / 贝塞尔插值 | 通过 | 按住 Shift 连续点击画笔自动连接成直线 | 保持端点与抗锯齿平滑 |
| **修复画笔 (Healing)** | `Rendering/HealPixels.c` | `HealTests.swift` | 已实现 | 低 | 复用原版 C `HealPixels.c` | 通过 | 基于泊松融合求解的污点修复画笔 | 保持采样点与修补点平滑过渡 |
| **仿制图章 (Clone)** | `Document/CloneStamp.swift` | `CloneStampTests.swift` | 已实现 | 低 | 采样偏移矩阵缓冲 | 通过 | Alt 取样后精准复制克隆图章像素 | 悬停光标实时克隆预览 |
| **模糊工具** | `Document/BlurTool.swift` | `BlurToolTests.swift` | 已实现 | 低 | 局部核加权平均 | 通过 | 画笔局部笔刷软化焦点与平滑噪点 | 保持边缘透明度一致 |
| **渐变工具** | `Document/Gradient.swift` | `GradientTests.swift` | 已实现 | 低 | 线性与径向渐变算法 | 通过 | 交互式起点与终点拉线，支持实时预览与反向 | 支持渐变色标自定义配置 |
| **取色器 (Eyedropper)**| `Rendering/SampleRingOverlay.swift` | `EyedropperTests.swift` | 已实现 | 低 | 桌面与画布像素采样 | 通过 | 放大镜取色环实时显示当前采样 RGB 与 Hex | 支持点击复制 Hex 颜色码 |
| **矢量形状工具** | `Document/ShapeTool.swift` | `ShapeToolTests.swift` | 已实现 | 低 | Direct2D 矢量路径绘制 | 通过 | 矩形、圆角矩形、椭圆、直线 | 支持填充与描边参数保存 |

---

## F. 滤镜与调整 (Filters & Adjustments)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **色相与饱和度** | `Document/HueSaturation.swift` | `HueSaturationTests.swift` | 已实现 | 低 | 复用原生色相转换算子 | 通过 | 包含全色调与各分色通道调节、着色模式 | 包含完整中文本地化界面 |
| **色阶 (Levels)** | `Document/Levels.swift`, `LevelsPixels.c` | `LevelsTests.swift` | 已实现 | 低 | 复用原版 C `LevelsPixels.c` | 通过 | 输入黑白场、灰度系数 (Gamma)、输出黑白场与直方图 | 实时直方图预览显示 |
| **曲线 (Curves)** | `Document/Curves.swift` | `CurvesTests.swift` | 已实现 | 低 | 三次样条插值 (Cubic Spline) | 通过 | RGB 复合通道与各独立单通道曲线点调整 | 包含输入与输出点精确对齐 |
| **曝光度 (Exposure)** | `Document/ImageAdjustments.swift` | `ExposureTests.swift` | 已实现 | 低 | 曝光、位移与伽马校正 | 通过 | 支持 32 位浮点浮动运算模拟真实光学曝光 | 避免高光伪影溢出 |
| **渐变映射 (Gradient Map)**| `Document/ImageAdjustments.swift` | `GradientMapTests.swift` | 已实现 | 低 | 灰度查找表 (LUT) 映射 | 通过 | 根据亮度映射自定义色彩渐变 | 保持颜色过渡连贯无断层 |
| **颗粒 (Grain)** | `Document/Dither.swift`, `GPUNoise.swift` | `GrainTests.swift` | 已实现 | 低 | 伪随机发生器 (PRNG 种子) | 通过 | 胶片感噪点模拟，支持强度、粗糙度参数 | 预览与最终导出完全一致 |
| **高斯模糊** | `Document/Filters.swift` | `GaussianBlurTests.swift` | 已实现 | 低 | 可分离高斯卷积核 (IIR/FIR) | 通过 | 支持 0.1~250px 模糊半径实时渲染 | 边框透明边界处理符合预期 |
| **动态模糊** | `Document/Filters.swift` | `MotionBlurTests.swift` | 已实现 | 低 | 方向线性积分卷积 | 通过 | 角度 (-180°~180°) 与距离 (0~500px) | 保持速度与边界平滑 |
| **Scanlines 扫描线** | `Document/Scanlines.swift` (1.4.8 升级) | `ScanlinesTests.swift` | 已实现 | 低 | 周期位移与色散模拟 | 通过 | 完整实现 1.4.8 最新位移、阈值、粗细与色相分离 | 支持中文菜单与参数项 |
| **Camera Raw Filter** | `Document/CameraRaw.swift` (1.4.8 升级) | `CameraRawTests.swift` | 已实现 | 中 | D3D11 / 软件管线统一 | 通过 | 光照、色彩、细节、镜头几何畸变校正 | 阴影/高光按 1.4.8 保持阶调稳定 |
| **背景移除 (Subject Removal)**| `Document/SubjectRemoval.swift` | `SubjectRemovalTests.cpp` | 已实现 | 中 | Apple Vision → BiRefNet ONNX | 通过 | 离线纯本地神经模型扣图，一键提取前景主体 | 内存用量安全受控 |
| **最近一次滤镜重用** | `Document/Filters.swift` (1.4.8 新增) | `LastFilterTests.swift` | 已实现 | 低 | 记忆上次调用滤镜与参数 | 通过 | 快捷键 Ctrl+Alt+F 瞬间重放上次滤镜参数 | 支持中文菜单名动态更新 |

---

## G. 文字与形状 (Text & Shapes)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **文字工具** | `Document/TypeTool.swift` | `TypeToolTests.swift` | 已实现 | 低 | DirectWrite 替代 CoreText | 通过 | 单击生成点文本，拖拽生成段落文本框 | 支持中英文混排与自动折行 |
| **字体与字号** | `UI/TypeControls.swift` | `FontControlsTests.swift` | 已实现 | 低 | Windows 系统字体枚举 | 通过 | 支持系统全部已装字体选择、字号滑动调整 | 默认打包 Inter 变体开源字体 |
| **文字排版样式** | `Document/TypeTool.swift` | `TextStyleTests.swift` | 已实现 | 低 | DirectWrite 文本格式器 | 通过 | 字间距 (Tracking)、行距 (Leading)、对齐方式 | 完整支持输入法 IME 组合键 |
| **PSD 水平文字导入** | `IO/PSD/PSDText.swift` | `PsdTextTests.cpp` | 已实现 | 中 | 自主开发的 PSD 文本解析器 | 通过 | 识别 Photoshop 8-bit PSD 水平文本层为可编辑文字 | 暂不支持竖排文字编辑（光栅化保留） |

---

## H. 画布与项目 (Canvas & Project)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **多项目标签页** | `UI/ProjectTabs.swift` | `ProjectTabsTests.cpp` | 已实现 | 低 | Qt QTabWidget 自定义渲染 | 通过 | 支持多文件并发编辑、标签关闭、切换、拖拽 | 状态栏显示当前项目信息 |
| **新建项目** | `UI/NewCanvasSheet.swift` | `NewCanvasTests.swift` | 已实现 | 低 | Qt 对话框 | 通过 | 支持像素、英寸、厘米、毫米单位，支持透明/白/黑预设 | 包含完整中文本地化界面 |
| **保存与自动恢复** | `IO/ProjectStore.swift` | `ProjectStoreTests.cpp` | 已实现 | 低 | 原子文件写入与临时目录安全替换 | 通过 | `.comp` 目录包格式安全读写，防崩溃意外丢数据 | 支持 v1~v12 全部历史版本向下兼容 |
| **网格与参考线** | `UI/GridSettingsSheet.swift` | `GridGuideTests.swift` | 已实现 | 低 | Direct2D 虚线与吸附判定 | 通过 | 像素网格 (800% 自动显现)、可拖拽水平/垂直参考线 | 支持快捷键 Ctrl+; 隐藏/显示 |
| **画布裁剪 (Crop)** | `Document/Crop.swift` | `CropTests.swift` | 已实现 | 低 | 交互控制柄与包围盒缩放 | 通过 | 自由裁剪、比例锁定、透视校正 | 回车键提交裁剪，ESC 取消 |
| **画布旋转** | `Document/CanvasRotation.swift` (1.4.8 新增) | `CanvasRotateTests.swift` | 已实现 | 低 | 90° 顺时针 / 逆时针矩阵置换 | 通过 | Image › Rotate Canvas 90° CW / CCW | 支持中文菜单与撤销动作 |
| **命令搜索面板** | `UI/CommandPalette.swift` (1.4.8 升级) | `CommandPaletteTests.swift` | 已实现 | 低 | Qt 快捷弹窗与模糊匹配 | 通过 | Ctrl+F 随时调出全局命令面板搜索工具与滤镜 | 支持输入中文模糊搜索菜单 |
| **Canvas Only 模式** | `UI/EditorCanvas.swift` (1.4.8 新增) | `CanvasOnlyTests.swift` | 已实现 | 低 | Qt 全屏隐藏停靠面板 | 通过 | 快捷键 F 切换纯黑背景沉浸画布模式 | 按 ESC 或再次按 F 恢复工具栏 |

---

## I. 文件格式与导出 (File Formats & Export)

| 原版功能 | 原版实现位置 | 原版测试 | Windows 实现状态 | 兼容性风险 | 依赖差异 | 测试结果 | 差异说明 | 后续任务 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **PNG 导入与导出** | `IO/ImageImporter.swift`, `ImageExporter.swift` | `PngCodecTests.swift` | 已实现 | 低 | WIC 原生硬件编解码 | 通过 | 支持透明通道 Alpha 保存与导入 | 导出具备质量与体积微调 |
| **JPEG 导入与导出**| `IO/ImageImporter.swift`, `ImageExporter.swift` | `JpegCodecTests.swift` | 已实现 | 低 | WIC 原生硬件编解码 | 通过 | 支持 1%~100% 压缩品质调节、Exif 方向校正 | 保持色彩空间稳定转换 |
| **HEIC 导入** | `IO/ImageImporter.swift` | `HeifCodecTests.cpp` | 已实现 | 低 | `libheif` + `libde265` 开源库 | 通过 | 支持 iPhone 拍摄的 HEIC 照片无损导入解析 | 已附带 LGPL 许可证声明与源码 |
| **TIFF 导入** | `IO/ImageImporter.swift` | `TiffCodecTests.swift` | 已实现 | 低 | WIC 原生编解码 | 通过 | 8/16-bit 未压缩与 LZW 压缩格式正常导入 | 验证大尺寸图片内存分配 |
| **SVG 矢量导入** | `IO/ImageImporter.swift` | `SvgRasterTests.cpp` | 已实现 | 低 | Direct2D 矢量光栅化 | 通过 | 矢量图形转换为高精度透明位图图层 | 支持自定义指定导入分辨率 |
| **RAW 照片开发导入**| `IO/RawImporter.swift` | `RawDevelopTests.cpp` | 已实现 | 中 | WIC RAW Codec 扩展 | 通过 | 支持读取数码相机 RAW 文件，去马赛克开发导入 | 保持曝光补偿平滑过渡 |
| **PSD / PSB 导入** | `IO/PSD/PSDReader.swift` | `PsdDocumentTests.cpp` | 已实现 | 中 | 自主纯 C++ PSD 解析器 | 通过 | 8-bit RGB 多图层、蒙版、混合模式、水平文字导入 | 明确记录：CMYK 模式提示转换为 RGB |
| **原生 .comp 格式** | `IO/ProjectStore.swift` | `ProjectFormatTests.cpp` | 已实现 | 低 | JSON 清单 + PNG 资产包结构 | 通过 | 严格遵守项目规范，与原版同构互读 | 持续维护与上游工程格式对齐 |
| **Export As… 对话框**| `UI/ExportAsSheet.swift` (1.4.8 新增) | `ExportAsTests.swift` | 已实现 | 低 | Qt 弹窗与实时渲染线程 | 通过 | 透明背景棋盘格预览，PNG/JPEG/PDF 尺寸预估 | 包含完整中文界面与参数说明 |
| **合并图像复制** | `Document/SelectionClipboard.swift` | `CopyMergedTests.swift` | 已实现 | 低 | 合成顶层缓存位图复制 | 通过 | Ctrl+Shift+C 快速抓取当前可见像素合并图 | 保持系统剪贴板互操作性 |
