#include "tasks/backgroundbackupthread.h"
#include <QDir>
#include <QUuid>

using namespace cryptonotes;


BackgroundBackupThread::BackgroundBackupThread(QString dbPath, QStringList backupPaths, QString fileNameHint, QObject* parent) : QThread(parent) {
    _dbPath = dbPath;
    _backupPaths = backupPaths;
    _fileNameHint = fileNameHint;
    _task = Task::BACKUP;
}

BackgroundBackupThread::BackgroundBackupThread(QString dbPath, QString replacementPath, QObject* parent) : QThread(parent) {
    _dbPath = dbPath;
    _restorationPath = replacementPath;
    _task = Task::RESTORE;
}

BackgroundBackupThread* BackgroundBackupThread::createBackupTask(QString dbPath, QStringList backupPaths, QString fileNameHint) {
    return new BackgroundBackupThread(dbPath, backupPaths, fileNameHint);
}

BackgroundBackupThread* BackgroundBackupThread::createRestorationTask(QString dbPath, QString replacementPath) {
    return new BackgroundBackupThread(dbPath, replacementPath);
}

void BackgroundBackupThread::run() {
    switch (_task) {
        case Task::BACKUP:
            backup();
            break;
        case Task::RESTORE:
            restore();
            break;
    }
}

void BackgroundBackupThread::backup() {
    auto failedPaths = QStringList();

    auto dbFile = QFile(_dbPath);
    if (!dbFile.exists()) {
        emit backupResultReady(failedPaths, false);
        return;
    }

    auto uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    for (int i = 0; i < _backupPaths.size(); i++) {
        auto path = _backupPaths.at(i);
        auto backupDir = QDir(path);

        if (!backupDir.exists()) {
            failedPaths.append(path);
            continue;
        }

        auto backupFilePath = backupDir.filePath(
            QString("%1_%2%3_%4.edb")
                .arg("notes_backup")
                .arg(_fileNameHint.trimmed().size() > 0 ? _fileNameHint.trimmed().append("_") : "")
                .arg(QDateTime::currentMSecsSinceEpoch())
                .arg(uuid)
        );

        auto result = dbFile.copy(backupFilePath);
        if (!result) failedPaths.append(path);
    }

    QThread::sleep(std::chrono::milliseconds(200));
    emit backupResultReady(failedPaths, true);
}

void BackgroundBackupThread::restore() {
    auto dbFile = QFile(_dbPath);
    auto dbFileInfo = QFileInfo(_dbPath);
    auto fixedRestorationPath = _restorationPath.replace("file:///", "");

#ifdef UNIX
    fixedRestorationPath.prepend("/");
#endif

    auto backupFile = QFile(fixedRestorationPath);

    if (!backupFile.exists()) {
        emit restorationResultReady(false);
        return;
    }

    if (dbFile.exists() && !dbFile.remove()) {
        emit restorationResultReady(false);
        return;
    }

    if (!backupFile.copy(_dbPath)) {
        emit restorationResultReady(false);
        return;
    }

    QThread::sleep(std::chrono::milliseconds(200));

    emit restorationResultReady(true);
}
