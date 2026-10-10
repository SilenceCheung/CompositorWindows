#pragma once
#include <QString>
#include <QObject>
#include <QSettings>
#include <unordered_map>
#include <functional>
#include <vector>

namespace compositor::ui {

enum class Language {
    English,
    SimplifiedChinese
};

class I18nManager final : public QObject {
    Q_OBJECT
public:
    static I18nManager& instance();

    Language currentLanguage() const { return currentLanguage_; }
    QString languageCode() const;
    void setLanguage(Language lang);

    // Translate source English string to current language
    QString translate(const QString& sourceText) const;
    QString translate(const char* sourceText) const;

    // Macro/convenience wrapper
    static QString tr(const char* sourceText) {
        return instance().translate(sourceText);
    }
    static QString tr(const QString& sourceText) {
        return instance().translate(sourceText);
    }

    // Register a callback for retranslating UI
    void registerRetranslateCallback(void* owner, std::function<void()> callback);
    void unregisterRetranslateCallback(void* owner);

signals:
    void languageChanged(Language newLang);

private:
    I18nManager();
    ~I18nManager() override = default;

    void initDictionary();

    Language currentLanguage_{Language::English};
    std::unordered_map<std::string, std::string> zhDictionary_;
    struct CallbackEntry {
        void* owner;
        std::function<void()> callback;
    };
    std::vector<CallbackEntry> callbacks_;
};

// Global inline helper
inline QString tr(const char* text) {
    return I18nManager::instance().translate(text);
}
inline QString tr(const QString& text) {
    return I18nManager::instance().translate(text);
}

} // namespace compositor::ui
