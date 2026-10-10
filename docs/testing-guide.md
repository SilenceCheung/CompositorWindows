# Compositor 自动化测试、质量保障与回归验证指南 (Testing Guide)

> **原则**：根据最高优先级原则 3 与原则 4，严禁通过删除测试或跳过错误来伪造测试通过。任何现存差异必须如实报告。

---

## 1. 测试套件体系与分类

Compositor for Windows 拥有分层自动化测试金字塔：

```text
                  ▲
                 / \
                / UI\          - Qt 离屏交互与窗口事件测试 (QTest)
               /-----\
              / 像素  \         - RGBA 像素精度、混合模式与滤镜测试
             /---------\
            / 模块单元测试 \     - 历史记录、图层树、变换矩阵、文本排版
           /-------------\
          /  核心 C 算法核 \   - Upstream C 模块单测 (Wand, ContentFill 等)
         /-----------------\
```

---

## 2. 常用测试执行命令

### 2.1 运行所有正常回归测试
```powershell
ctest --preset windows-x64-release --output-on-failure
```
> 目前基准测试包含 932 个用例，其中 926 个正常通过，6 个细微差异已在 `docs/known-limitations.md` 披露。

### 2.2 运行特定模块针对性测试
```powershell
# 窗口控件与外观测试
ctest --preset windows-x64-release -R "window_chrome" --output-on-failure

# 选区手势与运算测试
ctest --preset windows-x64-release -R "selection" --output-on-failure

# 图层混合模式与合成测试
ctest --preset windows-x64-release -R "blend" --output-on-failure

# 项目持久化与恢复测试
ctest --preset windows-x64-release -R "project" --output-on-failure
```

### 2.3 运行本地化翻译校验
```powershell
python tests/localization/check_translations.py
```
> 必须 100% 通过（0 错误），方可合并代码。

---

## 3. 像素级比对标准与公差 (Pixel Comparison Standard)

当对比 Direct3D / CPU 渲染结果与参考图像时，遵循以下客观指标：

1. **确定性算子**（如反相、通道分离、硬切选区）：
   - 容差要求：`ΔE = 0`，全像素字节必须严格相同。
2. **浮点与空间变换算子**（如双三次缩放、高斯模糊、色调曲线）：
   - 由于 CPU AVX2 SIMD 指令集与 GPU 浮点舍入微小差异，允许单通道误差 $\le 1/255$。
   - 总体通道峰值信噪比 $PSNR > 52\text{ dB}$。
3. **神经抠图主体掩码**：
   - 核心主体交并比（IoU）达到 $\ge 92\%$。

---

## 4. 真实 Windows 硬件与输入设备测试

对于无法在自动化 CI 运行器中覆盖的硬件场景，在发布正式预览版前应执行人工抽样验证：
1. **触控与数位板压感**：Windows Ink / Wacom 笔触压感平滑度与延迟。
2. **高分屏高 DPI**：在 125%、150%、200% 缩放显示器下，确认界面图标矢量清晰、无文本模糊或截断。
3. **多显示器混合 DPI**：在不同 DPI 的双显示器间拖动窗口，确认画布无抖动与错位。
4. **中文输入法 (IME)**：在文字图层输入汉字，确认候选词浮窗位置准确、上屏流畅无乱码。
