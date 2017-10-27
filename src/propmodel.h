#ifndef PROPMODEL_H
#define PROPMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "Message.h"

class ModelItem;

class PropModel : public QAbstractTableModel
{
public:
    PropModel(QObject * parent = 0);

    void update(const Message &msg);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<ModelItem*> itemList;
    ModelItem * findModelItem(const Message &msg) const;
};

#endif // PROPMODEL_H
