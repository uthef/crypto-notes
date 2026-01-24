#ifndef BACKUP_PATH_LIST_MODEL_H
#define BACKUP_PATH_LIST_MODEL_H

#include "QAbstractListModel"

namespace cryptonotes {
    class BackupPathListModel : public QAbstractListModel {
    public:
        BackupPathListModel(QStringList* list);
        enum Roles {
            PATH
        };

        int rowCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;

        QHash<int, QByteArray> roleNames() const override;
        void remove(size_t index);
        void remove(QString path);
        void reset();
        void update(size_t idx, QString newValue);
        void push(QString path);

    private:
        QStringList* _pathList;
    };
}

#endif