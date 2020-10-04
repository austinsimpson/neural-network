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

#ifndef FUNCTIONMODEL_H
#define FUNCTIONMODEL_H

#include "IPointSetModel.h"

#include <functional>
#include <QLinkedList>
#include <QRectF>

typedef std::function<qreal(qreal)> RealValuedFunction;

class FunctionModel
{
public:
	FunctionModel();
	FunctionModel(RealValuedFunction function);

	qreal operator()(qreal input) const;
	qreal value(qreal input) const;

	QLinkedList<QPointF> buildImage(const QRectF& logicalBounds, const QTransform& logicalToPixelTransform, const QPen& activePen) const;

private:
	RealValuedFunction _function;
};

#endif // FUNCTIONMODEL_H
