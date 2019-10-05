/*
 * Schroedinger(c) 2019 Licensed under the GNU LGPL 2.1
 * See the included LICENSE for details
 */

#pragma once
#include <QAbstractListModel>
#include <QListView>

template <class T>
class InstanceListModel : public QAbstractListModel {
    Q_OBJECT
  public:
    explicit InstanceListModel(const std::vector<T> &data, QObject *obj = nullptr);
    ~InstanceListModel() = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  private:
    const std::vector<T> &data;
};

class InstanceList : public QListView {
    Q_OBJECT
  public:
    explicit InstanceList(QWidget *parent = nullptr);
    ~InstanceList() = default;

  private:
    InstanceListModel *m_model = nullptr;
};
