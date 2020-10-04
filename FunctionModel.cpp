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

#include "FunctionModel.h"

#include <QLinkedList>
#include <QRectF>
#include <QPen>
#include <QTransform>

#include <QDebug>

constexpr auto kXNeighborhoodTriggerDistance = 0.005;
constexpr auto kMaxNeighborhoodDistance = 200.0;

FunctionModel::FunctionModel() : _function([](qreal input){ return input; })
{

}

FunctionModel::FunctionModel
(
	RealValuedFunction function
):
	_function(function)
{

}

qreal FunctionModel::value
(
	qreal input
)	const
{
	return _function(input);
}

qreal FunctionModel::operator()(qreal input) const
{
	return _function(input);
}

QLinkedList<QPointF> FunctionModel::buildImage
(
	const QRectF& logicalBounds,
	const QTransform& logicalToPixelTransform,
	const QPen& activePen
)   const
{
	QLinkedList<QPointF> result;

	result.push_back(logicalToPixelTransform.map(QPointF{ logicalBounds.left(), _function(logicalBounds.left())}));
	result.push_back(logicalToPixelTransform.map(QPointF{ logicalBounds.right(), _function(logicalBounds.right())}));

	const auto newPointThreshold = (activePen.widthF() * activePen.widthF()) / 4;
	const auto inverseTransform = logicalToPixelTransform.inverted();

	auto point = result.begin();
	while (point != result.end() - 1)
	{
		auto nextPoint = point + 1;

		if (logicalBounds.contains(*nextPoint) == false && logicalBounds.contains(*point))
		{
			point++;
			continue;
		}

		bool shouldAddNewPoint = true;
		const auto xDifference = nextPoint->x() - point->x();
		while (xDifference < activePen.width() && nextPoint != result.end())
		{
			const auto difference = *nextPoint - *point;
			const auto distanceSquared = QPointF::dotProduct(difference, difference);
			if (distanceSquared < newPointThreshold || (xDifference < kXNeighborhoodTriggerDistance && distanceSquared > kMaxNeighborhoodDistance))
			{
				++point;
				shouldAddNewPoint = false;
				break;
			}
			else
			{
				++nextPoint;
			}
		}

		if (shouldAddNewPoint)
		{
			nextPoint = point + 1;
			const qreal xMidPixel = (point->x() + nextPoint->x()) / 2;
			const qreal unused = 0.0;

			qreal xMidLogical;
			qreal unusedLogical;
			inverseTransform.map(xMidPixel, unused, &xMidLogical, &unusedLogical);

			const auto logicalPoint = QPointF{ xMidLogical, _function(xMidLogical) };
			result.insert(point + 1, logicalToPixelTransform.map(logicalPoint));
		}
	}


	return result;
}
