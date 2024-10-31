#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "logger.h"

struct Node {
    QString text;
    QList<Node*> children;
    Node* parent;
};


Q_DECLARE_METATYPE(Node*)

class JsonModel : public QAbstractItemModel {
    Q_OBJECT

public:
    enum Roles {
        RoleData = Qt::UserRole + 1
    };

    explicit JsonModel(QObject *parent = nullptr);

    void addItem(const QString &text, const QModelIndex &parent = QModelIndex());

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex rootIndex() const;
    void createTree(const QJsonValue &value, const QJsonValue &key, const QModelIndex &parent = QModelIndex());
    bool hasElement(const QModelIndex &parentIndex, const QString &elementText);

private:
    QList<Node*> m_rootNodes;
    Node* getNode(const QModelIndex &index) const;
};

#endif // JSONMODEL_H
