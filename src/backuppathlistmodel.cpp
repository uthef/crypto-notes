#include "backuppathlistmodel.h"

using namespace cryptonotes;

int BackupPathListModel::rowCount(const QModelIndex& parent) const {
    return _pathList->size();
}

QVariant BackupPathListModel::data(const QModelIndex& index, int role) const {
    switch (role) {
        case Roles::PATH:
            return QString(_pathList->at(index.row()));
        case Roles::INDEX:
            return QVariant(index.row());
        default:
            return "null";
    }
}

QHash<int, QByteArray> BackupPathListModel::roleNames() const {
    return {
        { Roles::PATH, QByteArray("path") },
        { Roles::INDEX, QByteArray("index") }
    };
}

BackupPathListModel::BackupPathListModel(QStringList* pathList) {
    _pathList = pathList;
}

void BackupPathListModel::remove(size_t index) {
    beginRemoveRows(QModelIndex(), index, index);
    _pathList->remove(index);
    endRemoveRows();
}

void BackupPathListModel::remove(QString path) {
    qsizetype idx = _pathList->indexOf(path);

    if (idx < 0) {
        return;
    }

    remove(idx);
}

void BackupPathListModel::update() {
    beginResetModel();
    endResetModel();
}

void BackupPathListModel::update(QString oldValue, QString newValue) {
    qsizetype oldValIdx = _pathList->indexOf(oldValue);
    
    if (oldValIdx < 0) {
        return;
    }

    _pathList->replace(oldValIdx, newValue);

    beginResetModel();
    endResetModel();
}

void BackupPathListModel::push(QString path) {
    beginInsertRows(QModelIndex(), _pathList->size(), _pathList->size());
    _pathList->push_back(path);
    endInsertRows();
}