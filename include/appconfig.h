#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QSettings>
#include <QStringList>

namespace cryptonotes {
    class AppConfig {
    public:
        const int maxBackupPathAmount = 5;

        AppConfig();
        void setDbPath(QString path);
        void updateBackupPaths(QStringList paths);
        QString dbPath() const;
        QString fileName() const;
        QString defaultDbPath() const;
        QStringList backupPaths() const;

    private:
        QString _path;
        QSettings _settings;
        QStringList _backupPaths;
    };
}

#endif
