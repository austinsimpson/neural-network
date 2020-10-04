#ifndef TRAININGMETRICS_H
#define TRAININGMETRICS_H

#include "VectorND.h"

#include <vector>

class TrainingMetrics
{
public:
    TrainingMetrics(size_t batchSize): _batchSize(batchSize) {}

    void addErrorPoint(size_t epoch, size_t iteration, double error)
    {
        _errors.emplace_back((static_cast<double>(epoch) * _batchSize + iteration) / 10000, error);
    }

    const std::vector<VectorND<2>> getErrors() const
    {
        return _errors;
    }

private:
    std::vector<VectorND<2>> _errors;
    size_t _batchSize;
};

#endif // TRAININGMETRICS_H
