# Compositor Windows 软件发布规范与发布流程 (Release Process)

> **原则**：根据最高优先级原则 6 与原则 7，每一次发布必须具有可追溯的 Git 提交、完整的依赖 SBOM、经过校验的 SHA-256 签名以及真实可用的构建产物。

---

## 1. 版本号命名规范 (Semantic Versioning)

版本号遵循三段式语义版本号 `MAJOR.MINOR.PATCH`：
- **上游对应关系**：
  - Windows 移植项目当前预览版采用 `0.1.x` 序列（如 `0.1.5`）。
  - 当功能矩阵达到 95% 以上且双语完全稳定后，步入 `1.0.0` 正式里程碑。
- **构建元数据与标签**：
  - Git Release Tag 统一使用 `v` 前缀：例如 `v0.1.5`、`v1.0.0`。
  - 发布清单 `package-manifest.json` 中必须显式记录：
    - `sourceRevision`（Git 完整 Commit SHA）
    - `sourceDirty`（源码是否处于未提交脏状态，发布包必须为 `false`）
    - `upstream`（对应的官方 Git Commit SHA，如 `75147a2`）

---

## 2. 正式发布流程清单 (Step-by-Step Checklist)

```mermaid
flowchart LR
    A["1. 冻结发布分支"] --> B["2. 运行回归测试"]
    B --> C["3. 校验多语言TS"]
    C --> D["4. 打包 MSI 与 ZIP"]
    D --> E["5. 校验 SHA256"]
    E --> F["6. 纯净环境安装验证"]
    F --> G["7. 发布 GitHub Release"]
```

### 步骤 1：准备与冻结分支
1. 在 `main` 分支确认所有特性与修复已合并且 CI 绿灯。
2. 创建版本分支 `release/v0.1.5`。

### 步骤 2：执行自动化本地化与功能测试
```powershell
# 1. 验证中英文本地化完整性
python tests/localization/check_translations.py

# 2. 运行核心功能测试套件
ctest --preset windows-x64-release --output-on-failure
```

### 步骤 3：执行官方上游同步审计
```powershell
python scripts/upstream-sync.py
```
确保 `docs/upstream-tracking.md` 已记录当前最新状态。

### 步骤 4：生成正式发布包
```powershell
./scripts/package.ps1 -Version 0.1.5
```

### 步骤 5：发布产物校验
检查产物目录：
1. `CompositorWindows-0.1.5-x64.msi` 存在且体积约 40~60MB。
2. `CompositorWindows-0.1.5-portable.zip` 存在且包含所有运行时 DLL、`models/birefnet-lite.onnx`、`shaders/BrushCoverage.hlsl`。
3. `SHA256SUMS.txt` 完整包含所有安装包与归档包的校验值。
4. `package-manifest.json` 记录 `sourceDirty: false`。

### 步骤 6：纯净环境安装体验验证
在没有 Visual Studio 或开发工具的全新 Windows 11 虚拟机中进行验证：
1. **安装**：双击 MSI，安装至 `%LOCALAPPDATA%\Programs\CompositorWindows\`。
2. **快捷方式**：检查桌面与开始菜单图标。
3. **文件关联**：双击 `.comp` 工程文件能否正确启动并打开项目。
4. **运行与双语**：正常启动程序，在菜单中切换为“简体中文”，确认所有菜单、面板与工具提示即刻变为中文。
5. **退出与卸载**：正常保存并退出；在 Windows 设置中卸载，确认用户工程文件完好无损保留。

### 步骤 7：推送 Tag 触发 GitHub Release
```bash
git tag -a v0.1.5 -m "Release Compositor for Windows v0.1.5"
git push origin v0.1.5
```
GitHub Actions 工作流将自动生成 Release，并上传 MSI、ZIP 及校验清单。
