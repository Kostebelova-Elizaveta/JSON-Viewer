#include "jsonmodel.h"

JsonModel::JsonModel(QObject *parent) : QAbstractItemModel(parent) {}

void JsonModel::addItem(const QString &text, const QModelIndex &parent) {
    auto node = new Node{text, {}};
    if (parent.isValid()) {
        auto parentNode = getNode(parent);
        parentNode->children.append(node);
        node->parent = parentNode;
    }
    else {
        m_rootNodes.append(node);
        node->parent = nullptr;
    }

    beginInsertRows(parent, rowCount(parent), rowCount(parent));
    endInsertRows();
    Logger::log(INFO, "Added item to the tree with text: " + text);

    if (parent.isValid()) {
        emit dataChanged(parent, parent);
    }
    else {
        emit dataChanged(index(0, 0), index(rowCount(), 0));
    }
}

QModelIndex JsonModel::index(int row, int column, const QModelIndex &parent) const {
    if (column != 0) return QModelIndex();

    if (!parent.isValid()) {
        if (row >= 0 && row < m_rootNodes.size()) {
            return createIndex(row, 0, m_rootNodes.at(row));
        }
    }
    else {
        auto parentNode = getNode(parent);
        if (row >= 0 && row < parentNode->children.size()) {
            return createIndex(row, 0, parentNode->children.at(row));
        }
    }

    return QModelIndex();
}

QModelIndex JsonModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) return QModelIndex();

    auto node = getNode(child);
    if (!node) return QModelIndex();

    if (node->parent) {
        return createIndex(node->parent->children.indexOf(node), 0, node->parent);
    }

    return QModelIndex();
}



int JsonModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()) {
        return m_rootNodes.size();
    }

    auto parentNode = getNode(parent);
    if (parentNode) {
        return parentNode->children.size();
    }

    return 0;
}

int JsonModel::columnCount(const QModelIndex &) const {
    return 1;
}

QVariant JsonModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    auto node = getNode(index);
    if (node) {
        switch (role) {
        case Qt::DisplayRole:
            return node->text;
        case RoleData:
            return QVariant::fromValue(node);
        default:
            return QVariant();
        }
    }

    return QVariant();
}


Qt::ItemFlags JsonModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QModelIndex JsonModel::rootIndex() const {
    return index(0, 0);
}


Node* JsonModel::getNode(const QModelIndex &index) const {
    return static_cast<Node*>(index.internalPointer());
}


void JsonModel::createTree(const QJsonValue &value, const QJsonValue &key, const QModelIndex &parent) {
    Logger::log(DEBUG, "Creating tree, Input key: " + key.toString() + " and value: " + value.toString());
    if (value.isObject()) {
        QString itemText;
        if (key.isObject()) {
            itemText = "object {" + QString::number(value.toObject().count()) + "}";
        }
        else {
            if (value.isArray()) {
                itemText = key.toString() + " [" + QString::number(value.toArray().count()) + "]";
            }
            else {
                itemText = key.toString() + " {" + QString::number(value.toObject().count()) + "}";
            }
        }
        this->addItem(itemText, parent);
        QModelIndex childIndex = this->index(this->rowCount(parent) - 1, 0, parent);

        const QJsonObject &json = value.toObject();
        for (const auto &key : json.keys()) {
            const auto &itemValue = json.value(key);
            createTree(itemValue, key, childIndex);
        }
    }
    else {
        if (value.isArray()) {
            QString itemText;
            if (key.isArray()) {
                itemText = "array [" + QString::number(value.toArray().count()) + "]";
            }
            else {
                itemText = key.toString() + " [" + QString::number(value.toArray().count()) + "]";
            }
            this->addItem(itemText, parent);
            QModelIndex childIndex = this->index(this->rowCount(parent) - 1, 0, parent);

            const QJsonArray &array = value.toArray();
            for (int i = 0; i < array.size(); ++i) {
                const auto &element = array.at(i);
                createTree(element, QString::number(i), childIndex);
            }
        }
        else {
            QString itemText;
            if (value.isDouble()) {
                itemText = key.toString() + ": " + QString::number(value.toDouble());
            }
            else {
                if (value.isBool()) {
                    itemText = key.toString() + ": " + (value.toBool() ? "true" : "false");
                }
                else {
                    itemText = key.toString() + ": " + value.toString();
                }
            }
            this->addItem(itemText, parent);
        }
    }
}

bool JsonModel::hasElement(const QModelIndex &parentIndex, const QString &elementText) {
    if (parentIndex.isValid() and this->data(parentIndex, Qt::DisplayRole).toString() == elementText) {
        return true;
    }
    if (!parentIndex.isValid()) {
        for (int i = 0; i < this->rowCount(); ++i) {
            QModelIndex index = this->index(i, 0);
            if (this->data(index, Qt::DisplayRole).toString() == elementText) {
                return true;
            }
        }
    } else {
        for (int i = 0; i < this->rowCount(parentIndex); ++i) {
            QModelIndex index = this->index(i, 0, parentIndex);
            if (this->data(index, Qt::DisplayRole).toString() == elementText) {
                return true;
            }
        }
    }
    for (int i = 0; i < this->rowCount(parentIndex); ++i) {
        QModelIndex childIndex = this->index(i, 0, parentIndex);
        if (hasElement(childIndex, elementText)) {
            return true;
        }
    }

    return false;

}


