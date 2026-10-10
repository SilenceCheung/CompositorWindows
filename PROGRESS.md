# Current checkpoint

Windows preview **0.1.7** is aligned with upstream Mac release **v1.4.9** (`0ab3ff58`), incorporating WebP image import decoding and dialog filters, full PSD/PSB drag-and-drop and open-as-project decoding via native pure C++ PSD engine, Command/Control middle-button drag zoom matching upstream Blender-like gesture, upstream dependency lock pin updated to official release commit `0ab3ff58a3dcf3d52a455fe3ab0336ac95923edd`, and bilingual localization (Simplified Chinese & English) validation 100% passed.

The 0.1.7 production release packages are built on `windows-2022` MSVC toolchain, verified, and placed in `./dist`:
- `CompositorWindows-0.1.7-x64.msi` (`a8dab0e14a2395d9026b1f1d46cd30a0ab1f1935993c5f8c2bcaa171054ebd5a`)
- `CompositorWindows-0.1.7-portable.zip` (`de6895edcabddc284ab3611854375cbed78b6f2afc59bf3425a80f7a9fe65cce`)
- `CompositorWindows-0.1.7-demo.zip` (`0ad53ef1a6eb07b74934fd8d8c83d90ad4d601481b8061df14e39de160317301`)
- `CompositorWindows-0.1.7-source.zip` (`26a952e66ae247bd92b34b53925593ffcc6ec759fe28de804ef23dbc2b7d1f27`)
- Manifest and SHA256SUMS.txt integrity verified 100%. All runtime dependencies, neural models, and translations are self-contained with no target host requirements.

Windows preview **0.1.6** is aligned with upstream Mac release **v1.4.8** (`75147a2`), incorporating destructive Camera Raw Filter restyling, Scanlines filter, single-page lossless PDF export, Canvas 90° Clockwise and Counterclockwise rotation, bilingual runtime support (Simplified Chinese 简体中文 and English) with zero-restart dynamic switching, upstream synchronization tracking, and automated GitHub Actions CI/CD workflows for Windows x64 Release MSI/ZIP packaging.

The 0.1.6 production release packages are built on `windows-2022` MSVC toolchain, verified, and placed in `./dist`:
- `CompositorWindows-0.1.6-x64.msi` (`24e0306ce2759083fe27b4c99f8024d3568cf3f032e588b1bc49f076be4424c0`)
- `CompositorWindows-0.1.6-portable.zip` (`7aab9d2514b53777ea2962c12f2307e973fbe30d232922ff435b7fe0e71eb023`)
- `CompositorWindows-0.1.6-demo.zip` (`b0cdf0df89e68fb6faaebc781de7dc17892889470b2e0b006abb60c49f4d6b19`)
- `CompositorWindows-0.1.6-source.zip` (`77edee31cd4df07dd07ae68603b589b50ca8193e26976013cd14b3293bda64f5`)
- Manifest and SHA256SUMS.txt integrity verified 100%. All runtime dependencies, neural models, and translations are self-contained with no target host requirements.

Windows preview **0.1.5** retained Camera Raw Filter restyling. Windows preview **0.1.4** was based on Mac Compositor 1.0.4 at `a19db9011282399785dc18efcfded904627bdcc2`. The public repository is https://github.com/SilenceCheung/CompositorWindows. The complete existing Windows commit history is retained.

The current interface has a vector tool rail, custom dark controls, Inter typography, compact adjustment panels, a themed project picker, and Mac-informed layer rows. Native Windows title bars are the default; View → Appearance → Mac-style title bar enables the saved optional appearance. This preference only changes window chrome.

The initial publication consolidates user/build documentation, adds contributor guidance, and removes 712 generated or obsolete tracked files from the current tree. Local raw evidence and archived material are private to the development workspace. Historical commits remain intact. Known limitations are in KNOWN-ISSUES.md; actual checks are summarized in VALIDATION.md.

The 0.1.4 package identifies source commit `be63afd9ac9ed1b3fdc25c3692da17be456548ee`, with 490 corresponding source files and `sourceDirty: false`. The MSI and portable archive include the runtime, model, licenses, and dependency sources. The matching source ZIP, demo ZIP, hashes, and manifest accompany the release. Local MSI upgrade and installed runtime health pass. Default installation is `%LOCALAPPDATA%\Programs\CompositorWindows\Compositor.exe`.

Maintenance intent: stay aligned with Robbie's core features, editing behavior, design, and project format. The Windows preview now implements the observable behavior from upstream 1.0.4 through 1.4.5 that applies on this port: the later blend modes, folder opacity, guides, finishing adjustments and filters, CPU layer effects, DirectWrite text, line shapes, and PSD, SVG, and RAW import. The dependency pin remains `a19db901` until a project written by the Mac 1.4.5 tree is read here. Mac exchange is unverified. The Metal compositor is not ported; rendering stays on the existing CPU and Direct3D paths.

Keep this as the single current checkpoint. Follow AGENTS.md for documentation boundaries. No implementation backlog is implied by historical reports or old tests; start with the user's current request.
