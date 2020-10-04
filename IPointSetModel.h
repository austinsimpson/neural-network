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

#ifndef IPOINTMODEL_H
#define IPOINTMODEL_H

#include <QPointF>
#include <QVector>
class IPointSetModel
{
public:
	IPointSetModel(){};
	virtual ~IPointSetModel() {};
	virtual QVector<QPointF>::iterator begin() = 0;
	virtual QVector<QPointF>::const_iterator cbegin() = 0;
	virtual QVector<QPointF>::iterator end() = 0;
	virtual QVector<QPointF>::const_iterator cend() = 0;

	virtual size_t count() const = 0;
	virtual const QPointF& get(size_t atIndex) = 0;
	virtual void clear() = 0;
};

#endif // IPOINTMODEL_H
