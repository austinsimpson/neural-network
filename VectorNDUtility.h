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
