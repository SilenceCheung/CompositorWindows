#include "I18nManager.h"
#include <QLocale>
#include <QSettings>

namespace compositor::ui {

I18nManager& I18nManager::instance() {
    static I18nManager manager;
    return manager;
}

I18nManager::I18nManager() {
    initDictionary();

    // Read stored language preference, or auto-detect from system locale
    QSettings settings("CompositorWindows", "Compositor");
    QString savedLang = settings.value("General/Language").toString();
    if (savedLang.isEmpty()) {
        QString sysLocale = QLocale::system().name();
        if (sysLocale.startsWith("zh", Qt::CaseInsensitive)) {
            currentLanguage_ = Language::SimplifiedChinese;
        } else {
            currentLanguage_ = Language::English;
        }
    } else {
        if (savedLang == "zh_CN" || savedLang == "zh") {
            currentLanguage_ = Language::SimplifiedChinese;
        } else {
            currentLanguage_ = Language::English;
        }
    }
}

QString I18nManager::languageCode() const {
    return currentLanguage_ == Language::SimplifiedChinese ? "zh_CN" : "en_US";
}

void I18nManager::setLanguage(Language lang) {
    if (currentLanguage_ == lang) return;
    currentLanguage_ = lang;

    // Persist user selection
    QSettings settings("CompositorWindows", "Compositor");
    settings.setValue("General/Language", languageCode());

    // Invoke registered UI retranslation callbacks
    for (const auto& entry : callbacks_) {
        if (entry.callback) {
            entry.callback();
        }
    }

    emit languageChanged(lang);
}

void I18nManager::registerRetranslateCallback(void* owner, std::function<void()> callback) {
    unregisterRetranslateCallback(owner);
    callbacks_.push_back({owner, std::move(callback)});
}

void I18nManager::unregisterRetranslateCallback(void* owner) {
    for (auto it = callbacks_.begin(); it != callbacks_.end(); ) {
        if (it->owner == owner) {
            it = callbacks_.erase(it);
        } else {
            ++it;
        }
    }
}

QString I18nManager::translate(const char* sourceText) const {
    if (!sourceText || *sourceText == '\0') return QString();
    if (currentLanguage_ == Language::English) {
        return QString::fromUtf8(sourceText);
    }
    auto it = zhDictionary_.find(sourceText);
    if (it != zhDictionary_.end()) {
        return QString::fromUtf8(it->second.c_str());
    }
    return QString::fromUtf8(sourceText);
}

QString I18nManager::translate(const QString& sourceText) const {
    if (sourceText.isEmpty()) return QString();
    if (currentLanguage_ == Language::English) {
        return sourceText;
    }
    std::string utf8 = sourceText.toUtf8().constData();
    auto it = zhDictionary_.find(utf8);
    if (it != zhDictionary_.end()) {
        return QString::fromUtf8(it->second.c_str());
    }
    return sourceText;
}

void I18nManager::initDictionary() {
    auto add = [this](const char* en, const char* zh) {
        zhDictionary_[en] = zh;
    };

    // --- Main Menus ---
    add("&File", "文件(&F)");
    add("File", "文件");
    add("&Edit", "编辑(&E)");
    add("Edit", "编辑");
    add("&Layer", "图层(&L)");
    add("Layer", "图层");
    add("&Image", "图像(&I)");
    add("Image", "图像");
    add("&View", "视图(&V)");
    add("View", "视图");
    add("&Language", "语言(&L)");
    add("Language", "语言");
    add("&Help", "帮助(&H)");
    add("Help", "帮助");
    add("Tools", "工具栏");

    // --- File Menu Actions ---
    add("&New Canvas…", "新建画布(&N)…");
    add("New Canvas…", "新建画布…");
    add("&Open Project…", "打开项目(&O)…");
    add("Open Project…", "打开项目…");
    add("&Import Image…", "导入图像(&I)…");
    add("Import Image…", "导入图像…");
    add("&Export Image…", "导出图像(&E)…");
    add("Export Image…", "导出图像…");
    add("Export As…", "导出为…");
    add("Export PDF…", "导出 PDF…");
    add("Export PDF", "导出 PDF");
    add("&Save Project", "保存项目(&S)");
    add("Save Project", "保存项目");
    add("Save Project &As…", "项目另存为(&A)…");
    add("Save Project As…", "项目另存为…");
    add("Close Project", "关闭项目");
    add("Exit", "退出");

    // --- Edit Menu Actions ---
    add("Undo", "撤销");
    add("Redo", "重做");
    add("Cut", "剪切");
    add("Copy", "复制");
    add("Paste", "粘贴");
    add("Copy Merged", "合并复制");
    add("Clear", "清除");
    add("Fill…", "填充…");
    add("Invert", "反向");
    add("Trim", "裁切");
    add("Free Transform", "自由变换");
    add("Distort", "扭曲");
    add("Scale…", "缩放…");
    add("Flip H", "水平翻转");
    add("Flip V", "垂直翻转");
    add("Flip Layer Horizontally", "水平翻转图层");
    add("Flip Layer Vertically", "垂直翻转图层");
    add("Apply Transform", "应用变换");
    add("Cancel Transform", "取消变换");

    // --- Select Actions ---
    add("Select", "选择");
    add("All", "全部");
    add("Deselect", "取消选择");
    add("Inverse", "反向选择");
    add("Expand…", "扩展…");
    add("Contract…", "收缩…");
    add("Feather…", "羽化…");
    add("Color Range…", "色彩范围…");

    // --- Layer Menu Actions ---
    add("New Layer", "新建图层");
    add("Duplicate Layer", "复制图层");
    add("Rename Layer…", "重命名图层…");
    add("Rename Layer", "重命名图层");
    add("Delete Layer", "删除图层");
    add("Raise Layer", "上移图层");
    add("Lower Layer", "下移图层");
    add("New Group", "新建图层组");
    add("Group Selected", "编组图层");
    add("Ungroup", "取消编组");
    add("Move Out of Group", "移出图层组");
    add("Create / Release Clipping Mask", "创建/释放剪贴蒙版");
    add("Merge Layers", "向下合并图层");
    add("Merge Visible", "合并可见图层");
    add("Layer via Copy", "通过拷贝建立图层");
    add("Copy Layer to Project…", "复制图层到项目…");
    add("Text Layer…", "文字图层…");

    // --- Mask Actions ---
    add("Mask", "蒙版");
    add("Add Layer Mask", "添加图层蒙版");
    add("Add White Mask (Reveal Selection)", "添加白色蒙版 (显示选区)");
    add("Add Black Mask (Hide Selection)", "添加黑色蒙版 (隐藏选区)");
    add("Reveal All", "全部显示");
    add("Hide All", "全部隐藏");
    add("Enable / Disable Mask", "启用/停用蒙版");
    add("Link / Unlink Mask", "链接/取消链接蒙版");
    add("Delete Mask", "删除蒙版");
    add("Edit Image", "编辑图像");
    add("Edit Mask", "编辑蒙版");
    add("Select Image Alpha", "载入图像Alpha选区");
    add("Select Mask Black Areas", "载入蒙版黑区选区");

    // --- Canvas Menu Actions ---
    add("Canvas Size…", "画布大小…");
    add("Image Size…", "图像大小…");
    add("Flip Canvas Horizontally", "水平翻转画布");
    add("Flip Canvas Vertically", "垂直翻转画布");
    add("Rotate Canvas 90° CW", "顺时针旋转画布 90°");
    add("Rotate Canvas 90° CCW", "逆时针旋转画布 90°");
    add("Crop Tool", "裁剪工具");
    add("Apply Crop", "应用裁剪");
    add("Cancel Crop", "取消裁剪");

    // --- View Menu Actions ---
    add("Fit Canvas", "缩放至适合画布");
    add("Actual Pixels", "实际像素 (100%)");
    add("Pixel Grid", "像素网格");
    add("Guides", "参考线");
    add("Snap", "吸附");
    add("Canvas Only", "纯黑全屏模式 (Canvas Only)");
    add("Mac-style title bar", "Mac 风格标题栏");
    add("Windows title bar", "Windows 原生标题栏");

    // --- Tools ---
    add("Move (V)", "移动 (V)");
    add("Marquee (M)", "选框 (M)");
    add("Lasso (L)", "套索 (L)");
    add("Polygon", "多边形套索");
    add("Wand (W)", "魔棒 (W)");
    add("Crop (C)", "裁剪 (C)");
    add("Brush (B)", "画笔 (B)");
    add("Eraser (E)", "橡皮擦 (E)");
    add("Heal (J)", "修复画笔 (J)");
    add("Clone (S)", "仿制图章 (S)");
    add("Retouch (R)", "修图 (R)");
    add("Gradient (G)", "渐变 (G)");
    add("Shape (U)", "形状 (U)");
    add("Type (T)", "文字 (T)");
    add("Eyedropper (I)", "吸管 (I)");
    add("Hand (H)", "抓手 (H)");
    add("Zoom (Z)", "缩放 (Z)");
    add("Foreground", "前景色");
    add("Background", "背景色");
    add("Swap (X)", "切换前背景色 (X)");
    add("Default (D)", "默认黑白颜色 (D)");
    add("Apply Gradient", "应用渐变");
    add("Cancel Gradient", "取消渐变");
    add("Update Shape Style", "更新形状样式");

    // --- Adjustments ---
    add("Adjustments", "调整");
    add("Hue/Saturation…", "色相/饱和度…");
    add("Levels…", "色阶…");
    add("Curves…", "曲线…");
    add("Exposure…", "曝光度…");
    add("Gradient Map…", "渐变映射…");
    add("Grain…", "胶片颗粒…");
    add("Invert…", "反相…");
    add("Black and White…", "黑白…");
    add("Color Balance…", "色彩平衡…");
    add("New Adjustment Layer", "新建调整图层");
    add("Edit Adjustment Layer…", "编辑调整图层…");

    // --- Filters ---
    add("Filters", "滤镜");
    add("Gaussian Blur…", "高斯模糊…");
    add("Motion Blur…", "动态模糊…");
    add("Add Noise…", "添加杂色…");
    add("Lens Correction…", "镜头校正…");
    add("Content-Aware Fill…", "内容识别填充…");
    add("Vignette…", "晕影…");
    add("Bloom…", "泛光 (Bloom)…");
    add("Tonal Contrast…", "色调对比度…");
    add("Dither…", "抖动…");
    add("Scanlines…", "扫描线 (Scanlines)…");
    add("Camera Raw Filter…", "Camera Raw 滤镜…");
    add("Remove Background…", "移除背景…");
    add("Last Filter", "上次滤镜");

    // --- Blend Modes ---
    add("Normal", "正常");
    add("Dissolve", "溶解");
    add("Darken", "变暗");
    add("Multiply", "正片叠底");
    add("Color Burn", "颜色加深");
    add("Linear Burn", "线性加深");
    add("Darker Color", "深色");
    add("Lighten", "变亮");
    add("Screen", "滤色");
    add("Color Dodge", "颜色减淡");
    add("Linear Dodge (Add)", "线性减淡 (添加)");
    add("Lighter Color", "浅色");
    add("Overlay", "叠加");
    add("Soft Light", "柔光");
    add("Hard Light", "强光");
    add("Vivid Light", "亮光");
    add("Linear Light", "线性光");
    add("Pin Light", "点光");
    add("Hard Mix", "实色混合");
    add("Difference", "差值");
    add("Exclusion", "排除");
    add("Subtract", "减去");
    add("Divide", "划分");
    add("Hue", "色相");
    add("Saturation", "饱和度");
    add("Color", "颜色");
    add("Luminosity", "明度");

    // --- Layer Panel & Properties ---
    add("Layers", "图层");
    add("Opacity", "不透明度");
    add("Opacity:", "不透明度:");
    add("Blend Mode:", "混合模式:");
    add("Lock", "锁定");
    add("Visible", "可见");
    add("Hidden", "隐藏");
    add("Name", "名称");
    add("Properties", "属性");
    add("Width", "宽度");
    add("Height", "高度");
    add("Angle", "角度");

    // --- Dialogs & Common UI ---
    add("OK", "确定");
    add("Cancel", "取消");
    add("Apply", "应用");
    add("Save", "保存");
    add("Don't Save", "不保存");
    add("Discard", "放弃");
    add("Close", "关闭");
    add("Reset", "重置");
    add("Name:", "名称:");
    add("Width:", "宽度:");
    add("Height:", "高度:");
    add("Resolution:", "分辨率:");
    add("Units:", "单位:");
    add("Pixels", "像素");
    add("Inches", "英寸");
    add("Centimeters", "厘米");
    add("Millimeters", "毫米");
    add("Transparent", "透明");
    add("White", "白色");
    add("Black", "黑色");
    add("Background:", "背景:");

    // --- Help & About ---
    add("Keyboard Shortcuts…", "键盘快捷键…");
    add("Keyboard Shortcuts", "键盘快捷键");
    add("Check for Updates…", "检查更新…");
    add("About Compositor", "关于 Compositor");
    add("Command Palette", "命令搜索面板");

    // --- Language Switch Actions ---
    add("English", "English (英文)");
    add("Simplified Chinese", "简体中文 (Chinese)");
}

} // namespace compositor::ui
