param([switch]$Offline,[string]$Python,[string]$RuntimeDirectory)
$ErrorActionPreference = 'Stop'
$taskRoot = Split-Path $PSScriptRoot -Parent
$taskVS = if ($env:VSINSTALLDIR -and (Test-Path $env:VSINSTALLDIR)) {
    $env:VSINSTALLDIR.TrimEnd('\')
} else {
    $taskVswhere = 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path $taskVswhere) {
        & $taskVswhere -version '[17.0,19.0)' -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath | Select-Object -First 1
    }
}
if (-not $taskVS) { throw 'Install Visual Studio 2022/2025 C++ x64 tools and Windows 11 SDK. No privileged installation is performed by bootstrap.' }
$devShell = Join-Path $taskVS 'Common7\Tools\Launch-VsDevShell.ps1'
if (Test-Path $devShell) {
    & $devShell -Arch amd64 -HostArch amd64 -SkipAutomaticLocation
}
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) { $env:PATH = "$taskVS\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;$env:PATH" }
$taskLock = Get-Content (Join-Path $taskRoot 'dependencies.lock.json') -Raw | ConvertFrom-Json
$taskArchive = Join-Path $taskRoot 'dependencies\downloads\qtbase.7z'
$taskQt = Join-Path $taskRoot 'dependencies\qt'
if (-not (Test-Path (Join-Path $taskQt 'lib\cmake\Qt6\Qt6Config.cmake'))) {
    if (-not (Test-Path $taskArchive)) {
        if ($Offline) { throw 'Locked Qt archive is missing. Run bootstrap with network access once.' }
        New-Item -ItemType Directory -Force (Split-Path $taskArchive) | Out-Null
        Invoke-WebRequest -Uri $taskLock.qt.url -OutFile $taskArchive
    }
    if ((Get-FileHash $taskArchive -Algorithm SHA256).Hash -ne $taskLock.qt.sha256) { throw 'Qt archive SHA256 mismatch.' }
    New-Item -ItemType Directory -Force $taskQt | Out-Null
    Push-Location $taskQt
    try { & cmake -E tar xf $taskArchive; if ($LASTEXITCODE) { throw 'Qt extraction failed.' } } finally { Pop-Location }
}
$env:PATH = "$taskQt\bin;$env:PATH"
$env:PATH = "$taskRoot\dependencies\imaging\install\bin;$taskRoot\dependencies\imaging\onnxruntime-win-x64-1.30.0\lib;$env:PATH"
$env:QT_PLUGIN_PATH = Join-Path $taskQt 'plugins'
$taskImaging = Join-Path $taskRoot 'dependencies\imaging'
$taskHeif = Join-Path $taskImaging 'install\bin\heif.dll'
if ((Test-Path -LiteralPath $taskHeif) -and ((Get-FileHash -LiteralPath $taskHeif -Algorithm SHA256).Hash -ine '172e241ed638c4ec250deb8e3fe35869087330463adc8f7ada791d1ba2844fba')) {
    Remove-Item -Force $taskHeif
}
$taskDe265 = Join-Path $taskImaging 'install\bin\libde265.dll'
if ((Test-Path -LiteralPath $taskDe265) -and ((Get-FileHash -LiteralPath $taskDe265 -Algorithm SHA256).Hash -ine '064ea0c514e8bbd53ba2a0a4f1febb4b7c35edc92fe654efa81d320ca25122b0')) {
    Remove-Item -Force $taskDe265
}
$taskRequired = @('install\bin\heif.dll','install\bin\libde265.dll','install\lib\heif.lib','install\lib\de265.lib','install\include\libheif\heif.h','onnxruntime-win-x64-1.30.0\lib\onnxruntime.dll','onnxruntime-win-x64-1.30.0\lib\onnxruntime_providers_shared.dll','onnxruntime-win-x64-1.30.0\lib\onnxruntime.lib','onnxruntime-win-x64-1.30.0\include\onnxruntime_cxx_api.h','model\birefnet-lite.onnx')
$taskMissing = @($taskRequired | Where-Object { -not (Test-Path -LiteralPath (Join-Path $taskImaging $_)) })
if ($taskMissing.Count) {
    if ($Offline) { throw "Offline imaging dependencies missing: $($taskMissing -join ', '). Run bootstrap with network access and Python 3.12 to build the locked dependencies/model." }
    if (-not $Python) {
        $taskBundledPython = Join-Path $env:USERPROFILE '.cache\codex-runtimes\codex-primary-runtime\dependencies\python\python.exe'
        if (Test-Path -LiteralPath $taskBundledPython) { $Python = $taskBundledPython } else { throw 'Pass -Python with an existing Python 3.12 executable for the locked offline model conversion.' }
    }
    & (Join-Path $PSScriptRoot 'bootstrap-imaging.ps1') -VsRoot $taskVS -Python $Python
    if ($LASTEXITCODE) { throw 'Imaging bootstrap failed.' }
}
if ($RuntimeDirectory) {
    & (Join-Path $PSScriptRoot 'restore-imaging-runtime.ps1') -RuntimeDirectory $RuntimeDirectory
}
if ((Get-FileHash -LiteralPath (Join-Path $taskImaging 'model\birefnet-lite.onnx') -Algorithm SHA256).Hash -ine 'c0faf38f5504f2239f1e6481ce4ac166b17435b38ea35e480d811a47bc1aba80') { throw 'Foreground model SHA256 mismatch.' }
if ((Get-FileHash -LiteralPath (Join-Path $taskImaging 'install\bin\heif.dll') -Algorithm SHA256).Hash -ine '172e241ed638c4ec250deb8e3fe35869087330463adc8f7ada791d1ba2844fba') { throw 'heif.dll SHA256 mismatch.' }
if ((Get-FileHash -LiteralPath (Join-Path $taskImaging 'install\bin\libde265.dll') -Algorithm SHA256).Hash -ine '064ea0c514e8bbd53ba2a0a4f1febb4b7c35edc92fe654efa81d320ca25122b0') { throw 'libde265.dll SHA256 mismatch.' }
& cmake --version
Write-Host "Ready: Qt $($taskLock.qt.version), MSVC $env:VCToolsVersion, SDK $env:WindowsSDKVersion. Run cmake --preset windows-x64-debug from $taskRoot."
