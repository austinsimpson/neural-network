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

#ifndef VECTORARRAYMODEL_H
#define VECTORARRAYMODEL_H

#include "IPointSetModel.h"
#include "VectorND.h"

class PointSetModel : public IPointSetModel
{
public:
	PointSetModel(const QVector<VectorND<2>>& vectors);
	PointSetModel(const QVector<QPointF>& points);
	PointSetModel(PointSetModel&& other);

	virtual  QVector<QPointF>::iterator begin() override;
	virtual QVector<QPointF>::const_iterator cbegin() override;
	virtual QVector<QPointF>::iterator end() override;
	virtual QVector<QPointF>::const_iterator cend() override;

	virtual size_t count() const  override;
	virtual const QPointF& get(size_t atIndex) override;
	virtual void clear() override;

private:
	QVector<QPointF> _points;

};

#endif // VECTORARRAYMODEL_H
