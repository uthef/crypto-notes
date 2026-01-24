#include "models/backuppathlistmodel.h"

using namespace cryptonotes;

int BackupPathListModel::rowCount(const QModelIndex& parent) const {
    return _pathList->size();
}

QVariant BackupPathListModel::data(const QModelIndex& index, int role) const {
    switch (role) {
        case Roles::PATH:
            return QString(_pathList->at(index.row()));
        default:
            return "null";
    }
}

QHash<int, QByteArray> BackupPathListModel::roleNames() const {
    return {
        { Roles::PATH, QByteArray("path") }
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

void BackupPathListModel::reset() {
    beginResetModel();
    endResetModel();
}

void BackupPathListModel::update(size_t idx, QString newValue) {
    _pathList->replace(idx, newValue);
    QModelIndex modelIdx = createIndex(idx, 0);
    emit dataChanged(modelIdx, modelIdx, { Roles::PATH });
}

void BackupPathListModel::push(QString path) {
    beginInsertRows(QModelIndex(), _pathList->size(), _pathList->size());
    _pathList->push_back(path);
    endInsertRows();
}