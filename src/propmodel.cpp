#include "propmodel.h"

#include "Prop.h"

struct ModelItem
{
    uint8_t monsterId;
    uint8_t partId;
    uint8_t propId;
    Prop::ePropType propType;
    Prop::ePropMode propMode;
    QVariant value;
};

QString propTypeToString(const Prop::ePropType &t)
{
    switch (t)
    {
    case Prop::NONE:        return "NONE";
    case Prop::LIGHT_A:     return "LIGHT ANALOG";
    case Prop::LIGHT_D:     return "LIGHT DIGITAL";
    case Prop::TEMP:        return "TEMPERATURE";
    case Prop::HUM:         return "HUMIDITY";
    default:                return "undefined";
    }
}

QString propModeToString(const Prop::ePropMode &m)
{
    switch (m)
    {
    case Prop::READ:        return "R";
    case Prop::WRITE:       return "W";
    case Prop::READ_WRITE:  return "R/W";
    default:                return "undefined";
    }
}

QVariant propValueToQVariant(const Message &msg)
{
    switch (msg.propType)
    {
    case Prop::NONE:        return QVariant();
    case Prop::LIGHT_A:     return *(int*)(&msg.value);
    case Prop::LIGHT_D:     return *(bool*)(&msg.value);
    case Prop::TEMP:
    case Prop::HUM:
    {
        float f;
        memcpy(&f,&msg.value,sizeof(float));
        return f;
    }
    default:                return QVariant();
    }
}

PropModel::PropModel(QObject *parent) : QAbstractTableModel(parent)
{
    itemList.clear();
}

void PropModel::update(const Message &msg)
{
    ModelItem * item = findModelItem(msg);
    if (item == 0)
    {
        item = new ModelItem;
        item->monsterId = msg.monsterId;
        item->partId = msg.partId;
        item->propId = msg.propId;
        item->propType = (Prop::ePropType) msg.propType;
        item->propMode = (Prop::ePropMode) msg.propMode;

        // add row to model
        int row = itemList.size();
        beginInsertRows(QModelIndex(),row,row);
        itemList.append(item);
        endInsertRows();
    }

    // set value in model
    item->value = propValueToQVariant(msg);

    // emit data changed
    int row = itemList.indexOf( item );
    QModelIndex idx = index(row,5);
    emit dataChanged(idx,idx);
}

int PropModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : itemList.size();
}

int PropModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 6;
}

QVariant PropModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column()<0 || index.row()<0) return QVariant();
    if (index.column()>=columnCount() || index.row()>=rowCount()) return QVariant();

    if (role == Qt::DisplayRole)
    {
        ModelItem * item = itemList[index.row()];
        switch (index.column())
        {
        case 0: return item->monsterId;
        case 1: return item->partId;
        case 2: return item->propId;
        case 3: return propTypeToString(item->propType);
        case 4: return propModeToString(item->propMode);
        case 5: return item->value;
        }
    }

    return QVariant();
}

QVariant PropModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0: return "MonsterId";
        case 1: return "PartId";
        case 2: return "PropId";
        case 3: return "Typ";
        case 4: return "Mód";
        case 5: return "Hodnota";
        }
    }

    return QAbstractTableModel::headerData(section,orientation,role);
}

ModelItem * PropModel::findModelItem(const Message &msg) const
{
    for(int i=0; i<itemList.size(); i++)
    {
        ModelItem * item = itemList[i];
        if (item->monsterId == msg.monsterId &&
                item->partId == msg.partId &&
                item->propId == msg.propId)
            return item;
    }
    return 0;
}
