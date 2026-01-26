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
        void setWindowSize(int width, int height);
        int windowWidth();
        int windowHeight();
        bool windowMaximized();
        void setWindowMaximized(bool value);
        QString language();

    private:
        QString _path;
        QSettings _settings;
        QStringList _backupPaths;
    };
}

#endif
