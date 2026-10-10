# Compositor 软件本地化与多语言开发指南 (Localization Guide)

> **适用范围**：本文档为 Compositor for Windows 的多语言架构说明、新增字符串翻译规范与本地化测试指南。

---

## 1. 本地化架构设计

Compositor for Windows 采用动态、零重启多语言架构，以核心组件 `compositor::ui::I18nManager` 为中心：

1. **统一中枢 (`I18nManager`)**：
   - 维护当前的活动语言（`Language::English` 或 `Language::SimplifiedChinese`）。
   - 提供高效的 $O(1)$ 查找字典与动态翻译方法 `ui::tr(sourceText)`。
   - 通过 `QSettings` 在用户注册表/配置文件中持久化用户偏好（键名 `General/Language`）。
   - 首次启动自动读取操作系统区域（`QLocale::system().name()`），若为中文环境则自适应激活简体中文。
2. **实时刷新机制**：
   - 窗口与面板通过 `registerRetranslateCallback` 订阅语言变化。
   - 用户在界面菜单切换语言时，所有活动窗口、菜单标题、子项文案、工具栏气泡提示、侧边栏标题即刻同步刷新，无需重启应用程序。
3. **双重格式支持**：
   - 运行时内置完整字典，避免因缺失外部文件导致翻译丢失。
   - 源码树提供标准 Qt Linguist TS 文件（`translations/compositor_zh_CN.ts` 与 `translations/compositor_en_US.ts`），方便专业本地化团队协同审阅。

---

## 2. 开发者翻译规范

在新增任何界面文案或功能时，必须遵守以下铁律：

### 2.1 严禁在业务代码中硬编码中文字符串
- 业务代码应使用英文作为基准源键（Source Key）。
- UI 显示文案必须通过 `ui::tr("...")` 进行翻译封装：
  ```cpp
  // 错误示范：硬编码中文或裸英文
  auto* action = menu->addAction("新建图层");
  auto* action = menu->addAction("New Layer");

  // 正确示范：使用 ui::tr 并在 I18nManager 中登记
  auto* action = menu->addAction(ui::tr("New Layer"));
  action->setProperty("rawText", "New Layer");
  ```

### 2.2 严格遵守术语对照表
- 翻译必须严格遵循 `docs/localization-glossary.md`。
- 不得随意发明非标准翻译（例如将“混合模式”误译为“融合方案”，将“蒙版”误译为“面罩”）。

### 2.3 快捷键与助记符规范
- 菜单栏助记符（Mnemonic `&`）在中文环境下应放在术语后括号内：
  - 英文：`&File`、`&Edit`、`&Layer`
  - 中文：`文件(&F)`、`编辑(&E)`、`图层(&L)`
- 严禁擅自修改已绑定的通用全局快捷键（如 Ctrl+Z、Ctrl+C、Ctrl+T 等）。

### 2.4 参数化格式化字符串规范
- 格式化参数必须使用 Qt 标准占位符 `%1`、`%2`：
  - 英文：`Could not open %1.`
  - 中文：`无法打开项目 %1。`
- 翻译字符串中的占位符集合必须与源文本绝对一一对应，严禁丢失或变更参数序号。

---

## 3. 本地化自动化测试流程

每次提交涉及文案修改的代码前，必须在本地或 CI 流程中执行翻译校验脚本：

```bash
python3 tests/localization/check_translations.py
```

### 校验项包括：
1. **XML 语法合法性**：确保 TS 文件符合 Qt 2.1 规范。
2. **非空检查**：确保所有 `<translation>` 标签包含有效文本，不得为空。
3. **占位符一致性**：确保 `%1`、`%2` 等参数在源文本和译文中严格匹配。
4. **乱码防护**：扫描文本中是否存在编码转换损坏的 Unicode 替换符（`\ufffd`）或典型乱码片段。

如果校验器报告任何问题，CI 门禁将自动拦截构建。
