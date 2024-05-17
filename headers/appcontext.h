#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <notelistmodel.h>
#include <QAbstractItemModel>
#include <database.h>
#include <QTimer>
#include "appconfig.h"

namespace cryptonotes {
    class AppContext : public QObject {
        Q_OBJECT
        Q_PROPERTY(NoteListModel* listModel READ model CONSTANT)
        Q_PROPERTY(QString searchQuery READ searchQuery WRITE setSearchQuery NOTIFY searchQueryUpdated)
        Q_PROPERTY(size_t rowCount READ rowCount NOTIFY rowCountUpdated)
        Q_PROPERTY(QString dbFolder READ dbFolder NOTIFY dbFolderUpdated)
    public:
        AppContext();
        NoteListModel* model();
        void setSearchQuery(QString query);
        QString searchQuery();
        size_t rowCount();
        QString dbFolder();
    signals:
        void dbConnectionFail(QString message);
        void dbFolderUpdated();
        void passwordUpdateResult(QString msg, bool success);
        void dbConnectionSuccess();
        void rowCountUpdated();
        void noteDataAvailable(size_t index, long id, QString title, QString summary, QString content, bool shortcut);
        void searchProgress(bool finished);
        void searchQueryUpdated();
        void abort(QString msg);
    public slots:
        void onPasswordValidated(QString password);
        void onPasswordUpdateRequested(QString oldPassword, QString newPassword);
        void onNewDbPathSelected(QString folder);
        void copyToClipboard(QString value1, QString value2);
        void onNoteRemovalRequested(size_t index);
        void onSearchRequest(QString query);
        void onDbDisconnectionRequest();
        void onNoteRequested(size_t index, bool shortcut);
        void onNoteUpdateRequested(long id, QString title, QString summary, QString content, int flags, bool updateTimestamp);
    private slots:
        void onSearchDelayTimeout();
    private:
        Database db;
        NoteListModel _listModel;
        NoteList _noteList;
        QTimer _searchTimer;
        QString _searchQuery;
        AppConfig _config;
        void connectToDb();
    };
}

#endif // APPCONTEXT_H
