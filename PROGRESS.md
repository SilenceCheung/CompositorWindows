# Current checkpoint

Windows preview **0.1.8** is aligned with upstream Mac release **v1.4.10** (`eb1fda73`), incorporating Robbie Tilton Compositor's authentic official application icon (peeling color page over transparent grid, bundled across 16x16 to 256x256 multi-resolution ICO and high-DPI RGBA PNGs), digital camera HEIF/HIF image file import support (`.hif`), batch folder import as project layers with natural locale name ordering, upstream dependency lock pin updated to official release commit `eb1fda73e97dbaf5a56ddbf237b712721b37050a`, and full test & packaging verification 100% passed.

The 0.1.8 production release packages are built on `windows-2022` MSVC toolchain, verified, and placed in `./dist`:
- `CompositorWindows-0.1.8-x64.msi` (`6369b7c20c7e5ada55056aa061d548c60ee62b2a28ac15792560577fae6e1096`)
- `CompositorWindows-0.1.8-portable.zip` (`ae82e465788801a473be80894b6aed70396b22f3ca0939a1dbbcde2b0fa66cf3`)
- `CompositorWindows-0.1.8-demo.zip` (`4e7861acea4696a9249a79ff9b99c487c8e09da5b307f492ff3a75a29ce4f0d4`)
- `CompositorWindows-0.1.8-source.zip` (`07cc16e784b1407ebe7767a93f297dc7b71930076387283d7faa9c8139b8306a`)
- Manifest and SHA256SUMS.txt integrity verified 100%. All runtime dependencies, neural models, and translations are self-contained with no target host requirements.

Windows preview **0.1.7** is aligned with upstream Mac release **v1.4.9** (`0ab3ff58`), incorporating WebP image import decoding and dialog filters, full PSD/PSB drag-and-drop and open-as-project decoding via native pure C++ PSD engine, Command/Control middle-button drag zoom matching upstream Blender-like gesture, upstream dependency lock pin updated to official release commit `0ab3ff58a3dcf3d52a455fe3ab0336ac95923edd`, and bilingual localization (Simplified Chinese & English) validation 100% passed.

The 0.1.7 production release packages are built on `windows-2022` MSVC toolchain, verified, and placed in `./dist`:
- `CompositorWindows-0.1.7-x64.msi` (`91dfe2d14c1f6350321ad3eafdd004ac26327baa6f98d2b8c79ff4b5e1ec7525`)
- `CompositorWindows-0.1.7-portable.zip` (`7b6b1d202c26e62b9ef6b4675c3e4a7c1108b855c8191a3bac9975f54de78295`)
- `CompositorWindows-0.1.7-demo.zip` (`0bbff5ceb9acb153ee80420c84efceae946f341ec882e16b3807f87c6a1da1af`)
- `CompositorWindows-0.1.7-source.zip` (`f1dc34cdfa6ebac51a696db8d4a76ec46a8ddd6a8ec3ad9721a0179b5ce2a9a3`)
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
