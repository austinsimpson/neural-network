#ifndef MNISTRESULTSVIEWMODEL_H
#define MNISTRESULTSVIEWMODEL_H

#include <QAbstractTableModel>

#include "VectorND.h"

struct MnistResultItem
{
   VectorND<784> imageData;
   VectorND<10> actualClass;
   VectorND<10> prediction;
};

class MnistResultsViewModel : public QAbstractTableModel
{
public:
    MnistResultsViewModel();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    void setItems(const QVector<MnistResultItem>& items);

private:
    QVector<MnistResultItem> _items;
};

#endif // MNISTRESULTSVIEWMODEL_H
