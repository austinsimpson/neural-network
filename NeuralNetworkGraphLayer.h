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

#pragma once

#include "GraphLayer.h"
#include "NeuralNetwork.h"

#include <QPainter>

template <typename... Layers>
class NeuralNetworkGraphLayer : public GraphLayer
{
public:
    NeuralNetworkGraphLayer(NeuralNetwork<Layers...>& net) : _net{ net } {}
	~NeuralNetworkGraphLayer() {}
	virtual void draw(QPainter& painter, const QRectF& logicalBounds = QRectF(0, 0, 0, 0), const QTransform& viewportTransform = QTransform()) const override
	{
		auto neuronCountIterator = _net.neuronCounts().cbegin();
		size_t leftLayerIndex = 0;

		while (neuronCountIterator != _net.neuronCounts().cend() - 1)
		{
			const size_t leftNeuronCount = *neuronCountIterator;
			const size_t rightNeuronCount = *(neuronCountIterator + 1);

			const double leftHalfHeight = leftNeuronCount % 2 == 1 ? static_cast<double>(leftNeuronCount - 1) / 2 : static_cast<double>(leftNeuronCount) / 2;
			const double rightHalfHeight = rightNeuronCount % 2 == 1 ? static_cast<double>(rightNeuronCount - 1) / 2 : static_cast<double>(rightNeuronCount) / 2;

			for (size_t leftNeuronIndex = 0; leftNeuronIndex < leftNeuronCount; ++leftNeuronIndex)
			{
				const size_t leftAdjustedDrawingIndex = leftNeuronCount % 2 == 0 ? (leftNeuronIndex >= leftNeuronCount / 2 ? leftNeuronIndex + 1 : leftNeuronIndex) : leftNeuronIndex;
				const double leftNeuronYIncrement = leftHalfHeight * (static_cast<double>(leftAdjustedDrawingIndex) / (leftHalfHeight > 0 ? leftHalfHeight : 1));
				
				const QPointF leftNeuronCenter = QPointF(2 * leftLayerIndex, leftHalfHeight - leftNeuronYIncrement);
				const auto mappedLeftNeuronCenter = viewportTransform.map(leftNeuronCenter);

				for (size_t rightNeuronIndex = 0; rightNeuronIndex < rightNeuronCount; ++rightNeuronIndex)
				{
					const size_t rightAdjustedDrawingIndex = rightNeuronCount % 2 == 0 ? (rightNeuronIndex >= rightNeuronCount / 2 ? rightNeuronIndex + 1 : rightNeuronIndex) : rightNeuronIndex;
					const double rightNeuronYIncrement = rightHalfHeight * (static_cast<double>(rightAdjustedDrawingIndex) / (rightHalfHeight > 0 ? rightHalfHeight : 1));
					const QPointF rightNeuronCenter = QPointF(2 * (leftLayerIndex + 1), rightHalfHeight - rightNeuronYIncrement);

					double weight = _net.getWeight(leftLayerIndex, leftNeuronIndex, rightNeuronIndex);
					QPen painterPen = painter.pen();
					painterPen.setWidthF(3.);
					if (weight < 0.)
					{
						painterPen.setColor(QColor(255, 0, 0).darker());
					}
					else
					{
						painterPen.setColor(QColor(0, 255, 0).darker());
					}
					painter.setPen(painterPen);
					
					const auto mappedRightNeuronCenter = viewportTransform.map(rightNeuronCenter);
					painter.drawLine(mappedLeftNeuronCenter, mappedRightNeuronCenter);

					painter.setBrush(QColor(255, 255, 255));
					painter.setPen(QColor(0, 0, 0));
					painter.drawEllipse(mappedRightNeuronCenter, 10., 10.);
				}

				painter.setPen(QColor(0, 0, 0));
				painter.setBrush(QColor(255, 255, 255));
				painter.drawEllipse(mappedLeftNeuronCenter, 10., 10.);
			}
			++neuronCountIterator;
			++leftLayerIndex;
		}
	}

private:
    NeuralNetwork<Layers...> _net;
};

