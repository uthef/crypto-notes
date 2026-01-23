#define WIN _WIN

#include "appcontext.h"
#include "storage/database.h"
#include "QDebug"
#include "qwindowdefs.h"
#include <QFileInfo>
#include <QDir>
#include <utils/formatter.h>
#include <QClipboard>
#include <qwindowdefs.h>
#include <qapplicationstatic.h>
#include <qguiapplication.h>
#include <utils/passwordgenerator.h>
#include <tasks/backgroundbackupthread.h>

using namespace cryptonotes;

AppContext::AppContext(bool isAnotherInstanceRunning) : _listModel(&_noteList), _backupPathListModel(&_pathList) {
    _isAnotherInstanceRunning = isAnotherInstanceRunning;

    _searchTimer.setSingleShot(true);
    connect(&_searchTimer, &QTimer::timeout, this, &AppContext::onSearchDelayTimeout);

    _windowMaximized = _config.windowMaximized();
    _windowWidth = _config.windowWidth();
    _windowHeight = _config.windowHeight();

    _pathList = _config.backupPaths();
    _pathList.removeDuplicates();
}

void AppContext::onPasswordValidated(QString password) {
    if (db.isOpen()) return;

    char* errMsg;

    QFileInfo pathInfo(_config.dbPath());

    if (!pathInfo.dir().exists()) {
        pathInfo.dir().mkpath(".");
    }

    QFile file(pathInfo.filePath());
    auto isFileOpen = file.open(QFile::OpenModeFlag::ReadWrite);

    if (!file.isOpen() || !file.isWritable() || !isFileOpen) {
        emit dbConnectionFail("The current database path cannot be used");
        file.close();
        return;
    }

    file.close();

    db.open(_config.dbPath().toStdString().c_str(), password.toStdString());
    db.makeSureTableNotesExists(&errMsg);

    if (errMsg) {
        emit dbConnectionFail("Wrong password or corrupted file");
        db.close();

        return;
    }

    int code;
    _noteList = *db.getRecentNotes(code);

    if (!db.isCodeSuccessful(code)) {
        emit dbConnectionFail("Fetching notes from the database resulted in an error");
        return;
    }

    emit dbConnectionSuccess();
}

void AppContext::onSearchRequest(QString query) {
    _searchTimer.stop();
    _searchQuery = Formatter::escapeQuery(query.trimmed(), '!');

    _searchTimer.setInterval(400);
    _searchTimer.start();
    emit searchProgress(false);
}

void AppContext::onSearchDelayTimeout() {
    if (!db.isOpen()) return;

    int code;

    if (_searchQuery.isEmpty()) {
        _noteList = *db.getRecentNotes(code);
    }
    else {
        _noteList = *db.find(_searchQuery.toStdString().c_str(), code);
    }

    if (!db.isCodeSuccessful(code)) {
        emit abort("Unable to perform search");
        return;
    }

    _listModel.update();
    emit rowCountUpdated();
    emit searchProgress(true);
}

void AppContext::onNoteRequested(size_t index, bool shortcut) {
    auto note = db.getNoteById(_noteList.at(index).id());

    if (!note.has_value()) {
        emit abort("Unable to fetch note");
        return;
    }

    emit noteDataAvailable(
        index,
        note->id(),
        QString::fromStdString(note->title()),
        QString::fromStdString(note->summary()),
        QString::fromStdString(note->content()),
        Formatter::timestampToReadableDateTime(note->timestamp()),
        shortcut
    );
}

void AppContext::onPasswordUpdateRequested(QString oldPassword, QString newPassword) {
    if (db.isOpen()) return;

    db.open(_config.dbPath().toStdString().c_str(), oldPassword.toStdString());

    char* errMsg;
    db.makeSureTableNotesExists(&errMsg);

    if (errMsg) {
        db.close();
        emit passwordUpdateResult("Password update failed. Double check your password", false);
        return;
    }

    db.rekey(newPassword.toStdString());
    db.close();

    emit passwordUpdateResult("Success", true);
}

void AppContext::onCopyToClipboardRequest(QString value) {
    auto clipboard = QGuiApplication::clipboard();
    clipboard->setText(value, QClipboard::Clipboard);
}

void AppContext::onNoteUpdateRequested(long id, QString title, QString summary, QString content, int flags, bool updateTimestamp) {
    if (flags == 0) return;

    auto note = Note(id, title.toStdString(), summary.toStdString(), content.toStdString(), 0);
    int code;

    if (id == -1) {
        code = db.addNote(note);
    }
    else {
        code = db.updateNote(note, flags, updateTimestamp);
    }

    if (!db.isCodeSuccessful(code)) {
        emit abort("Unable to add or update note");
        return;
    }

    onSearchRequest(_searchQuery);
}

void AppContext::onBackupPathRemovalRequested(QString path) {
    _backupPathListModel.remove(path);
    _config.updateBackupPaths(_pathList);
}

bool AppContext::onBackupPathAdditionRequested(QString path) {
    path = Formatter::removePathPrefix(path);

    if (_pathList.indexOf(path) != -1) {
        return false;
    }

    _backupPathListModel.push(path);
    _config.updateBackupPaths(_pathList);
    return true;
}

void AppContext::onBackupPathChangeRequested(QString oldPath, QString newPath) {
    newPath = Formatter::removePathPrefix(newPath);
    
    if (_pathList.indexOf(newPath) != -1) {
        return;
    }

    _backupPathListModel.update(oldPath, newPath);
    _config.updateBackupPaths(_pathList);

    
}

void AppContext::onDatabaseFileRestorationRequested(QString filePath) {
    auto thread = BackgroundBackupThread::createRestorationTask(dbPath(), filePath);
    connect(thread, &BackgroundBackupThread::restorationResultReady, this, &AppContext::finishRestoration);
    connect(thread, &BackgroundBackupThread::finished, &QObject::deleteLater);
    thread->start();
}

void AppContext::onAppAboutToQuit() {
    if (_isAnotherInstanceRunning) return;
    _config.setWindowSize(_windowWidth, _windowHeight);
    _config.setWindowMaximized(_windowMaximized);
}

void AppContext::onWindowHeightChanged(int value) {
    _windowHeight = value;
}

void AppContext::onWindowWidthChanged(int value) {
    _windowWidth = value;
}

void AppContext::onWindowVisibilityChanged(bool maximized) {
    _windowMaximized = maximized;
}

QString AppContext::searchQuery() {
    return _searchQuery;
}

void AppContext::setSearchQuery(QString query) {
    _searchQuery = query;
    emit searchQueryUpdated();
}

void AppContext::onDbDisconnectionRequest() {
    db.close();
}

NoteListModel* AppContext::model() {
    return &_listModel;
}

BackupPathListModel* AppContext::backupPathListModel() {
    return &_backupPathListModel;
}

void AppContext::onNoteRemovalRequested(size_t index) {
    auto note = _noteList.at(index);
    int code = db.deleteNote(note.id());

    if (!db.isCodeSuccessful(code)) {
        emit abort("Unable to delete note");
        return;
    }

    _listModel.remove(index);
    emit rowCountUpdated();
}

QString AppContext::dbPath() {
    auto dir = QDir(_config.dbPath());
    return dir.path();
}

bool AppContext::isWindowMaximized() {
    return _windowMaximized;
}

int AppContext::windowWidth() {
    return _windowWidth;
}

int AppContext::windowHeight() {
    return _windowHeight;
}

QString AppContext::dbDir() {
    QFileInfo info(dbPath().prepend("file:///"));
    return info.dir().path();
}

QString AppContext::generatePassword() {
    auto password = PasswordGenerator::generate();
    return password;
}

QString AppContext::appVersion() {
    return QString(APP_VERSION);
}

void AppContext::onNewDbPathSelected(QString folder) {
    folder = Formatter::removePathPrefix(folder);
    _config.setDbPath(folder + (folder.endsWith("/") ? "" : "/") + "notes.edb");
    emit dbPathUpdated();
}

size_t AppContext::rowCount() {
    return _noteList.size();
}

void AppContext::initiateBackup(QString fileNameHint) {
    auto thread = BackgroundBackupThread::createBackupTask(dbPath(), _pathList, fileNameHint);
    connect(thread, &BackgroundBackupThread::backupResultReady, this, &AppContext::finishBackup);
    connect(thread, &BackgroundBackupThread::finished, &QObject::deleteLater);
    thread->start();
}

bool AppContext::isAnotherInstanceRunning() {
    return _isAnotherInstanceRunning;
}

void AppContext::finishBackup(QStringList failedPaths, bool dbFound) {
    if (!dbFound) {
        emit error("Database file is not found");
        emit backupCompleted(_pathList);
        return;
    }

    emit backupCompleted(failedPaths);
}

void AppContext::finishRestoration(bool success) {
    emit restorationCompleted(success);
}   