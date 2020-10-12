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

#include "GraphWidget.h"
#include "PointSetModel.h"
#include "Range.h"

#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>

#include <cmath>
#include <memory>


constexpr auto kAxisTextMargins = 5.;
constexpr auto kGridUpdateThresholdLower = 100.;
constexpr auto kGridUpdateThresholdUpper = 200.;
constexpr auto kDefaultPixelScale = 50.;

//This tells us how many minor lines we want to draw before each major lines
const QMap<int, int> kMajorIndexForIncrementBase = {
    {1, 5}, //This means every fifth line should be a major line
    {2, 4}, //This means every fourth line should be a major line
    {5, 5} //Every fifth line should be a major line
};

GraphWidget::GraphWidget
(
    QWidget* parent
):
    QWidget(parent),
	_currentIncrementBase(2),
	_currentIncrementExponent(0),
	_pointRadius(3),
	_mouseLocation(-1, -1),
    _zoomLevel(0),
    _translationInPixelSpace(0.0, 0.0),
	_zoomCenter(0, 0),
    _shouldRenderGridlines{true}
{
    setMouseTracking(true);

    computeTransform();

    _majorGridlinePen.setColor(QColor(25, 25, 25));
    _majorGridlinePen.setWidth(1);

    _minorGridlinePen.setColor(Qt::lightGray);
    _minorGridlinePen.setWidth(1);

	update();
}


void GraphWidget::paintEvent
(
    QPaintEvent* paintEvent
)
{
    Q_UNUSED(paintEvent);

    QPainter painter;
    painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
    
	painter.setBrush(Qt::lightGray);
    painter.fillRect(0, 0, width(), height(), Qt::white);

	QPen pen = painter.pen();
	pen.setWidthF(3.0);
	pen.setColor(QColor(0, 0, 0));
	painter.setPen(pen);

    if (_shouldRenderGridlines)
    {
        drawGrid(painter);
    }

	for (const auto& layer : _layers)
	{
        layer->draw(painter, _logicalBounds, _viewportTransform);
    }

    painter.end();
}

void GraphWidget::drawGrid
(
	QPainter& painter
)
{
    if (!painter.isActive())
    {
		painter.begin(this);
    }
    
    drawAxisLines(painter, Qt::Horizontal);
    drawAxisLines(painter, Qt::Vertical);

    auto pen = painter.pen();
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    painter.drawLine(_viewportTransform.map(QPointF(0, _logicalBounds.bottom())), _viewportTransform.map(QPointF(0, _logicalBounds.top())));
    painter.drawLine(_viewportTransform.map(QPointF(_logicalBounds.left(), 0)), _viewportTransform.map(QPointF(_logicalBounds.right(), 0)));
}

void GraphWidget::drawAxisLines
(
    QPainter& painter,
    Qt::Orientation orientation
)
{
    const auto logicalLineMajorIncrement = _currentIncrementBase * pow(10, _currentIncrementExponent);
    const auto logicalLineMinorIncrement = static_cast<double>(logicalLineMajorIncrement) / kMajorIndexForIncrementBase[_currentIncrementBase];

    const auto firstLineCoord = orientation == Qt::Vertical ? _logicalBounds.left() - fmod(_logicalBounds.left(), logicalLineMinorIncrement) : _logicalBounds.top() - fmod(_logicalBounds.top(), logicalLineMinorIncrement);
    const auto numberOfLines = orientation == Qt::Vertical ? static_cast<int>(_logicalBounds.width() / logicalLineMinorIncrement) : static_cast<int>(_logicalBounds.height() / logicalLineMinorIncrement);
    const auto firstLineIndex = orientation == Qt::Vertical ? static_cast<int>(round(fmod(firstLineCoord, logicalLineMajorIncrement) / logicalLineMinorIncrement)) : static_cast<int>(round(fmod(firstLineCoord, logicalLineMajorIncrement) / logicalLineMinorIncrement));

    for (auto i = firstLineIndex; i < numberOfLines + firstLineIndex + 1; ++i)
    {
        const auto logicalLinePosition = firstLineCoord + ((i - firstLineIndex) * logicalLineMinorIncrement);

        if (i % kMajorIndexForIncrementBase[_currentIncrementBase] == 0)
        {
            painter.setPen(_majorGridlinePen);
            drawMajorGridline(painter, logicalLinePosition, orientation);
        }
        else
        {
            painter.setPen(_minorGridlinePen);
            drawMinorGridline(painter, logicalLinePosition, orientation);
        }
    }
}


void GraphWidget::drawMinorGridline
(
    QPainter& painter, 
    qreal logicalPosition, 
    Qt::Orientation orientation
)
{
    switch (orientation)
    {
        case Qt::Horizontal:
        {
            QPointF left(_logicalBounds.left(), logicalPosition);
            QPointF right(_logicalBounds.right(), logicalPosition);
            painter.drawLine(_viewportTransform.map(left), _viewportTransform.map(right));
            break;
        }
        case Qt::Vertical:
        {
            QPointF top(logicalPosition, _logicalBounds.top());
            QPointF bottom(QPointF(logicalPosition, _logicalBounds.bottom()));
            painter.drawLine(_viewportTransform.map(top), _viewportTransform.map(bottom));
            break;
        }
    }
}

void GraphWidget::drawMajorGridline
(
    QPainter& painter, 
    qreal logicalPosition, 
    Qt::Orientation orientation
)
{
    //First we get some information about the text and what it looks like in the pixel space.
    const auto numberAsString = QString::number(logicalPosition);
    const auto textWidthInPixelSpace = QFontMetricsF(painter.font()).width(numberAsString);
    const auto textHeightInPixelSpace = orientation == Qt::Horizontal ? QFontMetricsF(painter.font()).capHeight() : QFontMetricsF(painter.font()).height();

    QPointF textCoords;
    switch (orientation)
    {
        case Qt::Orientation::Horizontal:
        {
            textCoords = _viewportTransform.map(QPointF(0.0, logicalPosition));
            textCoords.rx() -= textWidthInPixelSpace + kAxisTextMargins;
            textCoords.ry() += textHeightInPixelSpace / 2;

            const QPointF left(_logicalBounds.left(), logicalPosition);
            QPointF textLeft = textCoords;
            textLeft.rx() -= kAxisTextMargins;
            textLeft.ry() -= textHeightInPixelSpace / 2;
            painter.drawLine(_viewportTransform.map(left), textLeft);

            QPointF onAxis(0.0, logicalPosition);
            QPointF right(_logicalBounds.right(), logicalPosition);
            painter.drawLine(_viewportTransform.map(onAxis), _viewportTransform.map(right));
            break;
        }
      
        case Qt::Orientation::Vertical:
        {
            textCoords = _viewportTransform.map(QPointF(logicalPosition, 0.0));
            textCoords.rx() -= textWidthInPixelSpace / 2;
            textCoords.ry() += textHeightInPixelSpace + kAxisTextMargins;

            //I know this reads really weird, but Qt's Rectangle class's bottom is larger than the top. So the bottom is the effective top. Fun.

            const QPointF top(logicalPosition, _logicalBounds.bottom());
            const QPointF onAxis(logicalPosition, 0.0);

            QPointF textBottom = textCoords;
            textBottom.ry() += textHeightInPixelSpace;
            textBottom.rx() += textWidthInPixelSpace / 2;
            const QPointF bottom(QPointF(logicalPosition, _logicalBounds.top()));
            
            painter.drawLine(_viewportTransform.map(top), _viewportTransform.map(onAxis));
            painter.drawLine(textBottom, _viewportTransform.map(bottom)); 
            break;
        }
    }
    if (abs(logicalPosition) > 0.000000001)
        painter.drawText(textCoords, numberAsString);
}

void GraphWidget::adjustGridlineScale(bool didZoomOut)
{
    const QPointF origin(0.0, 0.0);
    const QPointF firstGridline(_currentIncrementBase  * pow(10, _currentIncrementExponent), 0.0);

    const auto mappedOrigin = _viewportTransform.map(origin);
    const auto mappedFirstGrid = _viewportTransform.map(firstGridline);

    const auto horizontalDistance = mappedFirstGrid.x() - mappedOrigin.x();
    if (didZoomOut) {
		if (horizontalDistance < kGridUpdateThresholdLower)
        {
            switch (_currentIncrementBase)
            {
            case 1:
                _currentIncrementBase = 2;
                break;
            case 2:
                _currentIncrementBase = 5;
                break;
            case 5:
                _currentIncrementBase = 1;
                _currentIncrementExponent++;
                break;
            default:
                throw;
            }
        }
    }
	else if (horizontalDistance > kGridUpdateThresholdUpper)
    {
        switch (_currentIncrementBase)
        {
        case 1:
            _currentIncrementBase = 5;
            _currentIncrementExponent--;
            break;
        case 2:
            _currentIncrementBase = 1;
            break;
        case 5:
            _currentIncrementBase = 2;
            break;
        default:
            throw;
        }
    }
}

void GraphWidget::mousePressEvent
(
    QMouseEvent* mouseEvent
)
{
    _mouseLocation = _inverseViewportTransform.map(mouseEvent->localPos());

    update();
}

void GraphWidget::mouseMoveEvent
(
    QMouseEvent* mouseEvent
)
{
    const auto newMouseLocation = mouseEvent->localPos();
    if (mouseEvent->buttons().testFlag(Qt::LeftButton))
    {
        const auto mousePosTemp = _mouseLocation;
        auto difference = newMouseLocation - _viewportTransform.map(mousePosTemp);
		_translationInPixelSpace += difference;
		computeTransform();
		update();
    }

    _mouseLocation = _inverseViewportTransform.map(newMouseLocation);
}

void GraphWidget::mouseReleaseEvent
(
    QMouseEvent* mouseEvent
)
{
    _mouseLocation = _inverseViewportTransform.map(mouseEvent->localPos());
    update();
}

void GraphWidget::wheelEvent
(
    QWheelEvent* wheelEvent
)
{
    _zoomLevel += (qreal)wheelEvent->delta() / 4000;

    _zoomCenter = _inverseViewportTransform.map(wheelEvent->posF());
    computeTransform();

    const auto newZoomLocation = _viewportTransform.map(_zoomCenter);
    _translationInPixelSpace += wheelEvent->posF() - newZoomLocation;
    computeTransform();

    _mouseLocation = _inverseViewportTransform.map(wheelEvent->position());

    adjustGridlineScale(wheelEvent->delta() < 0);
    update();
}

void GraphWidget::resizeEvent
(
    QResizeEvent* resizeEvent
)
{
    resizeEvent->accept();
    computeTransform();
    update();
}

void GraphWidget::computeTransform()
{
    //First thing we want to do is invert the y-axis to make it consistent with mathematics instead of computer graphics
    QTransform centerOriginTransform;
    centerOriginTransform.scale(1.0, -1.0);

    //Im using regular exp for my zooming function because it is always non-negative and has nice, nonlinear characteristics
    auto zoomScale = exp(_zoomLevel);

    //Maps the logical coordinates to some starting pixel coordinates. Default as of writing is 50, but it becomes irrelevant once we start zooming
    QTransform logicalToViewport;
    logicalToViewport.scale(kDefaultPixelScale, kDefaultPixelScale);

    //I wrote out the zoom transformation as two separate transformations, one for scaling and one for translation
    QTransform zoomScaleTransform;
    zoomScaleTransform.scale(zoomScale, zoomScale);

    QTransform zoomCenterTransform;
    zoomCenterTransform.translate(_zoomCenter.x(), _zoomCenter.y());
    
    //Stores data for our panning. Sometimes we also use this to store a fudge factor after doing a zoom and forcing pixel coordinates to stay the same
    QTransform translation;
    translation.translate(_translationInPixelSpace.x(), _translationInPixelSpace.y());

    _viewportTransform = ((zoomCenterTransform.inverted() * zoomScaleTransform * zoomCenterTransform.scale(zoomScale, zoomScale) * centerOriginTransform) * logicalToViewport) * translation;
    _inverseViewportTransform = _viewportTransform.inverted();

    _logicalBounds = visibleBounds();
}

void GraphWidget::reset()
{
    _layers.clear();
}


void GraphWidget::centerOnLogicalCoordinate
(
    const QPointF& logicalCoordinate
)
{
    _zoomLevel = 0;
    _translationInPixelSpace += QPointF(width() / 2, height() / 2) - _viewportTransform.map(logicalCoordinate);
    computeTransform();
}

QRectF GraphWidget::visibleBounds() const
{
    return _inverseViewportTransform.mapRect(QRectF(0.0, 0.0, width(), height())).normalized();
}

void GraphWidget::addLayer
(
	const FunctionModel& model
)
{
    _layers.push_back(std::make_unique<GraphLayer>(model));
    update();
}

void GraphWidget::addLayer
(
	const QVector<QPointF>& points
)
{
    _layers.push_back(std::make_unique<GraphLayer>(new PointSetModel(points)));
    update();
}

void GraphWidget::addLayer
(
    std::unique_ptr<GraphLayer>&& layer
)
{
    _layers.push_back(std::move(layer));
    update();
}

void GraphWidget::setShouldRenderGridlines
(
    bool shouldRender
)
{
    _shouldRenderGridlines = shouldRender;
}