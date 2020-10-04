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

#include "GraphLayer.h"

#include <cmath>

GraphLayer::GraphLayer
(
	const FunctionModel& functionModel
):
	_isVisible(true),
	_pen(QColor(0, 0, 0), 2.),
	_layerType(LayerType::Function),
	_functionModel(functionModel)
{

}

GraphLayer::GraphLayer
(
	IPointSetModel* pointSetModel
):
	_isVisible(true),
	_layerType(LayerType::Points),
	_pointsModel(pointSetModel),
	_pen(QColor(0, 0, 0), 3)
{

}

GraphLayer::GraphLayer
(
	GraphLayer&& other
):
	_isVisible(true),
	_pointsModel(std::move(other._pointsModel)),
	_functionModel(other._functionModel),
	_layerType(other._layerType),
	_pen(QColor(0, 0, 0), 3)
{

}

void GraphLayer::setPen
(
	const QPen& pen
)
{
	_pen = pen;
}

const QPen& GraphLayer::pen() const
{
	return _pen;
}

QPen& GraphLayer::pen()
{
	return _pen;
}

void GraphLayer::setColor
(
	const QColor& color
)
{
	_pen.setColor(color);
}

QColor GraphLayer::color() const
{
	return _pen.color();
}

bool GraphLayer::isVisible() const
{
	return _isVisible;
}
void GraphLayer::setVisible
(
	bool isVisible
)
{
	_isVisible = isVisible;
}

void GraphLayer::draw
(
	QPainter& painter,
	const QRectF& logicalBounds,
	const QTransform& viewportTransform
)	const
{
	if (_isVisible)
	{
		painter.setPen(_pen);
		switch (_layerType)
		{
		case Points:
			for (const auto& point: *_pointsModel)
			{
				if (logicalBounds.contains(point))
				{
					painter.drawPoint(viewportTransform.map(point));
				}
			}
			break;
		case Function:
			QVector<QPointF> pointsToDraw;
			drawFunction(painter, logicalBounds, viewportTransform);
			break;
		}
	}
}

void GraphLayer::drawFunction
(
	QPainter& painter,
	const QRectF& logicalBounds,
	const QTransform& viewportTransform
)	const
{
	const auto points = _functionModel.buildImage(logicalBounds, viewportTransform, painter.pen());
	for (const auto point: points)
	{
		painter.drawPoint(point);
	}
}

qreal GraphLayer::getClosestSquaredDistance
(
	const QVector<QPointF>& points,
	const QPointF& pointOfInterest
)	const
{
	auto result = std::numeric_limits<qreal>::max();

	for (const auto& point : points)
	{
		const auto difference = point - pointOfInterest;
		const auto currentDistanceSquared = QPointF::dotProduct(difference, difference);
		if (currentDistanceSquared < result)
			result = currentDistanceSquared;
	}

	return result;
}
