# Compositor Windows 编译与本地开发指南 (Build Guide)

> **目标平台**：Windows 11 x64 (推荐版本 23H2 / 24H2)  
> **工具链要求**：Visual Studio 2022 C++ 工具 (MSVC 14.44 / v143)、Windows 11 SDK (10.0.26100.0)、CMake 3.31+、PowerShell 7、Python 3.12

---

## 1. 开发前准备与依赖工具

在初次构建前，请确保开发机安装了以下工具：

1. **Visual Studio 2022**（社区版/专业版/企业版）：
   - 安装工作负荷：**“使用 C++ 的桌面开发”**。
   - 包含组件：MSVC v143 - VS 2022 C++ x64/x86 生成工具（最新版）、Windows 11 SDK (10.0.26100.0)、适用于 Windows 的 C++ CMake 工具。
2. **Python 3.12**：
   - 官方 Python 3.12 x64（安装时勾选 "Add python.exe to PATH"）。
   - 用于初次离线模型格式转换与测试数据验证。
3. **PowerShell 7**（推荐）：
   - Windows 11 自带 Windows PowerShell 5.1 亦可运行，但推荐 `pwsh` 7.x。
4. **.NET 8+ SDK / WiX Toolset v4**（仅构建 MSI 安装包时必需）：
   ```powershell
   dotnet tool install --global wix --version 4.0.5
   wix extension add WixToolset.UI.wixext/4.0.5 --global
   ```

---

## 2. 自动化引导依赖 (Bootstrap)

在项目根目录下打开 PowerShell 执行引导脚本：

```powershell
# 自动拉取锁定的 Qt 6.8.3、编译依赖库头文件及转换离线 BiRefNet 模型
./scripts/bootstrap.ps1 -Python (Get-Command python).Source
```

### 引导脚本主要动作：
1. 校验并定位 Visual Studio 2022 环境变量（`Launch-VsDevShell.ps1`）。
2. 下载锁定的 Qt 6.8.3 运行时并解压到 `dependencies/qt`。
3. 校验并拉取 `libheif`、`libde265` 及 `ONNX Runtime 1.30.0` 对应依赖。
4. 校验离线背景抠图神经网络模型 `dependencies/imaging/model/birefnet-lite.onnx`（SHA256: `c0faf38f5504f2239f1e6481ce4ac166b17435b38ea35e480d811a47bc1aba80`）。

---

## 3. 使用 CMake 构建应用程序

本项目提供标准 `CMakePresets.json` 预设，推荐使用 Visual Studio 生成器或 Ninja。

### 3.1 Release 构建（推荐日常与打包）

```powershell
# 1. 配置工程
cmake --preset windows-x64-release

# 2. 编译主程序 Compositor
cmake --build --preset windows-x64-release --target Compositor --parallel 8
```

编译产物位于：`build/release/Release/Compositor.exe`。

### 3.2 部署依赖动态库到生成目录

直接在生成目录运行 `Compositor.exe` 前，需部署 Qt 运行时动态库：

```powershell
dependencies\qt\bin\windeployqt.exe --release build\release\Release\Compositor.exe
```

并在 `build\release\Release\` 目录下确保存在 `models\birefnet-lite.onnx` 与 `shaders\BrushCoverage.hlsl`。

### 3.3 Debug 构建（断点调试）

```powershell
cmake --preset windows-x64-debug
cmake --build --preset windows-x64-debug --target Compositor --parallel 8
```

---

## 4. 运行自动化测试

编译并执行原生测试套件：

```powershell
# 编译所有测试目标
cmake --build --preset windows-x64-release --parallel 8

# 运行受影响模块的测试（例如窗口与选区测试）
ctest --preset windows-x64-release -R "window_chrome|selection" --output-on-failure

# 运行本地化完整性测试
python tests/localization/check_translations.py
```

---

## 5. 打包生成 MSI 安装包与便携版 ZIP

在完成 Release 编译后，执行发布打包脚本：

```powershell
./scripts/package.ps1 -Version 0.1.5
```

执行完毕后，产物将生成在 `dist/CompositorWindows-0.1.5-preview-<时间戳>/` 目录：
- `CompositorWindows-0.1.5-x64.msi`（WiX 原生 MSI 安装包）
- `CompositorWindows-0.1.5-portable.zip`（便携版免安装绿色压缩包）
- `SHA256SUMS.txt`（哈希校验清单）
- `package-manifest.json`（SBOM 与构建元数据记录）
