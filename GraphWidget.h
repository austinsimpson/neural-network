#ifndef POINTSWIDGET_H
#define POINTSWIDGET_H

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

#include <QWidget>

#include <QVector>
#include <QPen>
#include <QPoint>

#include "GraphLayer.h"

class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* paintEvent);

    void mousePressEvent(QMouseEvent* mouseEvent);
    void mouseMoveEvent(QMouseEvent* mouseEvent);
    void mouseReleaseEvent(QMouseEvent* mouseEvent);

    void wheelEvent(QWheelEvent* wheelEvent);
    void resizeEvent(QResizeEvent* resizeEvent);

    void centerOnLogicalCoordinate(const QPointF& logicalCoord);

	void addLayer(const FunctionModel& model);
	void addLayer(const QVector<QPointF>& points);
    void addLayer(std::unique_ptr<GraphLayer>&& layer);

    void setShouldRenderGridlines(bool shouldRender);

public slots:
    void reset();
signals:


private:
    QRectF visibleBounds() const;

    void drawGrid(QPainter& painter);
    void drawAxisLines(QPainter& painter, Qt::Orientation orientation);
    void drawMinorGridline(QPainter& painter, qreal logicalPosition, Qt::Orientation orientation);
	void drawMajorGridline(QPainter& painter, qreal logicalPosition, Qt::Orientation orientation);
    void adjustGridlineScale(bool didZoomOut);

    void computeTransform();

    QPointF _zoomCenterInPixelSpace;

    uint8_t _currentIncrementBase;
    int32_t _currentIncrementExponent;

    int _pointRadius;
    QPointF _mouseLocation;

    QTransform _viewportTransform;
    QTransform _inverseViewportTransform;

    qreal _zoomLevel;
    QPointF _translationInPixelSpace;
    QPointF _zoomCenter;

    QRectF _logicalBounds;

    QPen _majorGridlinePen;
    QPen _minorGridlinePen;

    bool _shouldRenderGridlines;

    std::vector<std::unique_ptr<GraphLayer>> _layers;
};

#endif // POINTSWIDGET_H
