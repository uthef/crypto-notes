#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QSettings>

namespace cryptonotes {
    class AppConfig {
    public:
        AppConfig();
        void setDbPath(QString path);
        QString dbPath() const;
        QString fileName() const;
        QString defaultDbPath() const;

    private:
        QString _path;
        QSettings _settings;
    };
}

#endif
