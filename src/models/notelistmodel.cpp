#include "models/notelistmodel.h"
#include "QMap"

#include <QVariant>
#include <QTime>
#include <utils/formatter.h>

using namespace cryptonotes;

int NoteListModel::rowCount(const QModelIndex& parent) const {
    return _noteList->size();
}

QVariant NoteListModel::data(const QModelIndex& index, int role) const {
    switch (role) {
        case Roles::TITLE:
            return QString::fromStdString(_noteList->at(index.row()).title());
        case Roles::SUMMARY:
            return QString::fromStdString(_noteList->at(index.row()).summary());
        case Roles::TIME:
            return Formatter::timestampToReadableDateTime(_noteList->at(index.row()).timestamp());
        default:
            return "null";
    }
}

QHash<int, QByteArray> NoteListModel::roleNames() const {
    return {
        { Roles::TITLE, QByteArray("title") },
        { Roles::SUMMARY, QByteArray("summary") },
        { Roles::TIME, QByteArray("time") }
    };
}

NoteListModel::NoteListModel(NoteList* list) {
    _noteList = list;
}

void NoteListModel::remove(size_t index) {
    beginRemoveRows(QModelIndex(), index, index);
    _noteList->remove(index);
    endRemoveRows();
}

void NoteListModel::update() {
    beginResetModel();
    endResetModel();
}
