/*
 * Schroedinger(c) 2019 Licensed under the GNU LGPL 2.1
 * See the included LICENSE for details
 */

#include "InstanceList.h"

template<typename T>
InstanceListModel::InstanceListModel(const std::vector<T>& data, QObject* obj) : QAbstractListModel(obj) {}

int InstanceListModel::rowCount(const QModelIndex& parent) const { return 0; }

QVariant InstanceListModel::data(const QModelIndex& index, int role) const {
    switch (role) {
        case Qt::DisplayRole: {
            return "Name";
        }

        case Qt::DecorationRole: {
            return {};
        }
    }
    return {};
}

InstanceList::InstanceList(QWidget* parent) : QListView(parent) {}
