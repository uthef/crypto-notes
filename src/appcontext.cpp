#define WIN _WIN

#include "appcontext.h"
#include "database.h"
#include "QDebug"
#include "qwindowdefs.h"
#include <QFileInfo>
#include <QDir>
#include <formatter.h>
#include <QClipboard>
#include <qwindowdefs.h>
#include <qapplicationstatic.h>
#include <qguiapplication.h>

using namespace cryptonotes;

AppContext::AppContext() : _listModel(&_noteList) {
    _searchTimer.setSingleShot(true);
    connect(&_searchTimer, &QTimer::timeout, this, &AppContext::onSearchDelayTimeout);
}

void AppContext::onPasswordValidated(QString password) {
    if (db.isOpen()) return;

    char* errMsg;

    QFileInfo pathInfo(_config.dbPath());

    if (!pathInfo.dir().exists()) {
        pathInfo.dir().mkpath(".");
    }

    QFile file(pathInfo.filePath());
    file.open(QFile::OpenModeFlag::ReadWrite);

    if (!file.isOpen() || !file.isWritable()) {
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
    // clipboard->setText(value2, QClipboard::Clipboard);
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

QString AppContext::dbFolder() {
    auto dir = QDir(_config.dbPath());
    return dir.path();
}

void AppContext::onNewDbPathSelected(QString folder) {
    folder = folder.replace("file:///", "");
    _config.setDbPath(folder + (folder.endsWith("/") ? "" : "/") + "notes.edb");
    emit dbFolderUpdated();
}

size_t AppContext::rowCount() {
    return _noteList.size();
}
