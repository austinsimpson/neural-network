#include "MnistResultsViewModel.h"

#include "VectorNDUtility.h"

static QVector<QString> gHeaderNames{ "Image", "Predicted", "Actual" };

MnistResultsViewModel::MnistResultsViewModel()
{

}


QVariant MnistResultsViewModel::data
(
    const QModelIndex& index,
    int role
)   const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case 1:
                return std::max_element(_items[index.row()].prediction.begin(), _items[index.row()].prediction.end()) - _items[index.row()].prediction.begin();
            case 2:
                for (int oneIndex = 0; oneIndex < 10; ++oneIndex)
                {
                    if (_items[index.row()].actualClass[oneIndex] == 1.0)
                    {
                        return oneIndex;
                    }
                }
        default:
            break;
        }
    }
    else if (role == Qt::DecorationRole && index.column() == 0)
    {
        return vectorToImage(_items[index.row()].imageData, QSize(28, 28));
    }
    return QVariant();
}

QVariant MnistResultsViewModel::headerData
(
    int section,
    Qt::Orientation orientation,
    int role
)   const
{
    QVariant result;

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        result = gHeaderNames[section];
    }

    return result;
}

int MnistResultsViewModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED (parent)
    return _items.count();
}

int MnistResultsViewModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED (parent)
    return 3;
}

void MnistResultsViewModel::setItems(const QVector<MnistResultItem>& items)
{
    beginResetModel();
    _items = items;
    endResetModel();
}
