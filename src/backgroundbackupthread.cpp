#include "backgroundbackupthread.h"
#include <QDir>
#include <QUuid>

using namespace cryptonotes;

BackgroundBackupThread::BackgroundBackupThread(QString dbPath, QStringList backupPaths, QObject* parent) : QThread(parent) {
    _dbPath = dbPath;
    _backupPaths = backupPaths;
}

void BackgroundBackupThread::run() {
    auto failedPaths = QStringList();

    auto dbFile = QFile(_dbPath);
    if (!dbFile.exists()) {
        emit resultReady(failedPaths, false);
        return;
    }

    for (int i = 0; i < _backupPaths.size(); i++) {
        auto path = _backupPaths.at(i);
        auto backupDir = QDir(path);

        if (!backupDir.exists()) {
            failedPaths.append(path);
            continue;
        }

        auto backupFilePath = backupDir.filePath(
            QString("%1_%2_%3.edb")
                .arg("notes_backup")
                .arg(QDateTime::currentMSecsSinceEpoch())
                .arg(QUuid::createUuid().toString(QUuid::WithoutBraces))
        );

        auto result = dbFile.copy(backupFilePath);
        if (!result) failedPaths.append(path);
    }

    QThread::sleep(std::chrono::milliseconds(200));

    emit resultReady(failedPaths, true);
}