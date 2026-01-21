#ifndef NOTELISTMODEL_H
#define NOTELISTMODEL_H

#include "QAbstractListModel"

#include <storage/notelist.h>

namespace cryptonotes {
    class NoteListModel : public QAbstractListModel {
    public:
        NoteListModel(NoteList* list);
        enum Roles {
            TITLE,
            SUMMARY,
            TIME
        };

        int rowCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;

        QHash<int, QByteArray> roleNames() const override;
        void remove(size_t index);
        void update();
    private:
        QString readableTime(long timestamp) const;
        NoteList* _noteList;
    };
}
#endif
