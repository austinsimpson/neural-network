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

#include "PointSetModel.h"

#include <exception>

PointSetModel::PointSetModel
(
	const QVector<VectorND<2>>& vectors
)
{
	for (auto vector : vectors)
	{
		_points.append(QPointF{ vector[0], vector[1] });
	}
}

PointSetModel::PointSetModel
(
	const QVector<QPointF>& points
):
	_points(points)
{

}
 
PointSetModel::PointSetModel
(
	PointSetModel&& other
):
	_points(other._points)
{
}

QVector<QPointF>::iterator PointSetModel::begin()
{
	return _points.begin();
}

QVector<QPointF>::const_iterator PointSetModel::cbegin()
{
	return _points.cbegin();
}

QVector<QPointF>::iterator PointSetModel::end()
{
	return _points.end();
}

QVector<QPointF>::const_iterator PointSetModel::cend()
{
	return _points.cend();
}


size_t PointSetModel::count() const
{
	return _points.count();
}

const QPointF& PointSetModel::get
(
	size_t index
)
{
	if (0 <= index && index < _points.count())
	{
        return _points[index];
	}
    else
    {
        throw std::exception("Index out of bounds, bro-ski.");
    }
}

void PointSetModel::clear()
{
	_points.clear();
}
