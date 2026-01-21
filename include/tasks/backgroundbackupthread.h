#ifndef BACKGROUND_BACKUP_TASK_H
#define BACKGROUND_BACKUP_TASK_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QThread>

namespace cryptonotes {
    class BackgroundBackupThread : public QThread {
        Q_OBJECT
    public:
        BackgroundBackupThread(QString dbPath, QStringList backupPaths, QObject* parent = nullptr);
    protected:
        void run() override;
    signals:
        void resultReady(const QStringList failedPaths, bool dbFound);
    private:
        QString _dbPath;
        QStringList _backupPaths;
    };
}

#endif