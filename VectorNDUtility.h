#ifndef UTILITY_H
#define UTILITY_H

#include <QImage>

#include "VectorND.h"

template <size_t N>
QImage vectorToImage(const VectorND<N>& input, const QSize& size)
{
    assert(input.dimension() == size.width() * size.height());

    QImage result(size.width(), size.height(), QImage::Format::Format_Grayscale8);
    for (int index = 0; index < N; ++index)
    {
        int row = index / size.width();
        int column = index % size.width();
        int grayValue = input[index] * 255;
        result.setPixel(column, row, qRgb(grayValue, grayValue, grayValue));
    }
    return result;
}

#endif // UTILITY_H
