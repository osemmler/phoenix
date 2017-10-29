#ifndef PROPMODEL_H
#define PROPMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QDateTime>

#include "Message.h"
#include "Prop.h"

struct ModelItem
{
    uint8_t monsterId;
    uint8_t partId;
    uint8_t propId;
    Prop::ePropType propType;
    Prop::ePropMode propMode;
    QVariant value;
    QDateTime updateTime;
};

class PropModel : public QAbstractTableModel
{
public:
    PropModel(QObject * parent = 0);

    void update(const Message &msg);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    ModelItem * modelItemForIndex(const QModelIndex &index);

private:
    QList<ModelItem*> itemList;
    ModelItem * findModelItem(const Message &msg) const;
};

#endif // PROPMODEL_H
