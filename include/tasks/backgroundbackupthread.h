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
        static BackgroundBackupThread* createBackupTask(QString dbPath, QStringList backupPaths);
        static BackgroundBackupThread* createRestorationTask(QString dbPath, QString replacementPath);
    protected:
        void run() override;
    signals:
        void backupResultReady(const QStringList failedPaths, bool dbFound);
        void restorationResultReady(bool success);
    private:
        enum Task {
            BACKUP,
            RESTORE
        };

        Task _task;
        QString _dbPath;
        QString _restorationPath;
        QStringList _backupPaths;

        BackgroundBackupThread(QString dbPath, QStringList backupPaths, QObject* parent = nullptr);
        BackgroundBackupThread(QString dbPath, QString replacementPath, QObject* parent = nullptr);
        void backup();
        void restore();
    };
}

#endif