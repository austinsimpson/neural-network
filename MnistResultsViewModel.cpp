//Copyright(c) 2020 Austin Simpson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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
