#ifndef VECTORND_H
#define VECTORND_H

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

#include <array>
#include <numeric>

#include "Range.h"

template <size_t n>
class VectorND
{
public:
    constexpr VectorND() : _values{}
    {
        std::fill(_values.begin(), _values.end(), 0.);
    }

    template <typename... T>
    constexpr VectorND(T... ts): _values{ts...} {}

    constexpr VectorND(const VectorND<n>& other) : _values(other._values){}

    constexpr VectorND<n>& operator= (const VectorND<n>& other)
	{
        for (int index = 0; index < n; ++index)
		{
			_values[index] = other[index];
		}
		return *this;
	}

    constexpr VectorND<n>& operator= (std::initializer_list<double> list)
	{
#pragma omp parallel for
		for (auto value : list)
		{
			_values.push_back(value);
		}
	}

    constexpr int dimension() const
    {
        return n;
    }

    constexpr auto begin()
    {
        return _values.begin();
    }

    constexpr auto end()
    {
        return _values.end();
    }

    constexpr auto begin() const
    {
        return _values.cbegin();
    }

    constexpr auto end() const
    {
        return _values.cend();
    }

    constexpr double operator[](int i) const
    {
        if (0 <= i && i < n)
        {
            return _values[i];
        }
        return 0.0;
    }

    constexpr double& operator[](int i)
    {
        if (0 <= i && i < n)
        {
            return _values[i];
        }
        throw;
    }


    constexpr void setValue(const int index, const double value)
    {
        if (0 <= index && index < n)
        {
            _values[index] = value;
        }
    }

    constexpr VectorND<n> operator+(const VectorND<n>& vector) const
    {
        VectorND<n> result;
#pragma omp parallel for
        for (int index = 0; index < n; ++index)
        {
            result[index] = (*this)[index] + vector[index];
        }
        return result;
    }

    constexpr VectorND<n>& operator+=(const VectorND<n>& other)
    {
        for (int index = 0; index < n; ++index)
        {
            (*this)[index] += other[index];
        }
        return *this;
    }

    constexpr VectorND<n> operator-(const VectorND<n>& other) const
    {
        VectorND<n> result;
#pragma omp parallel for
        for (int index = 0; index < n; ++index)
        {
            result[index] = (*this)[index] - other[index];
        }
        return result;
    }

    constexpr VectorND<n>& operator-=(const VectorND<n>& other)
    {
        for (int index = 0; index < n; ++index)
        {
            (*this)[index] -= other[index];
        }
        return *this;
    }

    double operator*(const VectorND<n>& vector) const
    {
        double result = 0.0;

#pragma omp parallel for
        for (int index = 0; index < n; ++index)
        {
            result += (*this)[index] * vector[index];
        }

        return result;
    }

    VectorND<n> operator*(double scalar) const
    {
        VectorND<n> result;
        #pragma omp parallel for
        for (auto index : range(0, n))
        {
            result[index] = (*this)[index] * scalar;
        }
        return result;
    }

	VectorND<n> operator/(double scalar) const
	{
		return operator*(1.0 / scalar);
	}

    constexpr void pointwiseDivide(const VectorND<n>& other)
    {
        #pragma omp parallel for
        for (int i = 0; i < n; ++i)
        {
            (*this)[i] /= other[i];
        }
    }

    constexpr VectorND<n>& pointwiseMultiply(const VectorND<n>& other)
    {
        #pragma omp parallel for
        for (int i = 0; i < n; ++i)
        {
            (*this)[i] *= other[i];
        }
        return *this;
    }

    constexpr VectorND<n> pointwiseMultiply(const VectorND<n>& other) const
    {
        VectorND<n> result{};
        #pragma omp parallel for
        for (int i = 0; i < n; ++i)
        {
            result[i] = (*this)[i] * other[i];
        }
        return result;
    }


    VectorND<n + 1> asHomogenous() const
    {
        VectorND<n+1> result(1.0);
        for (int index : range(0, n))
        {
            result[index + 1] = (*this)[index];
        }
        return result;
    }

	VectorND<n-1> asNonhomogenous() const
	{
		VectorND<n-1> result(0.0);

		for (int index : range(1, n))
		{
			result[index - 1] = _values[index];
		}
		return result;
	}

    constexpr double lengthSquared() const
    {
        auto result = 0.0;
        for (auto index : range(0, n))
        {
            const auto element = (*this)[index];
            result += element * element;
        }

        return result;
    }

    double length() const
    {
        return sqrt(lengthSquared());
    }

    constexpr double maxElement() const
    {
        double maximum = std::numeric_limits<double>::min();
        for (const double value: *this) {
            if (value > maximum)
            {
                maximum = value;
            }
        }
        return maximum;
    }

    static constexpr VectorND<n> ones()
    {
        VectorND<n> result;
        for (int i = 0; i < n; ++i)
        {
            result[i] = 1.;
        }
        return result;
    }

private:
    std::array<double, n> _values;
};


template <size_t m>
VectorND<m> operator*(double scalar, const VectorND<m>& vector)
{
    //Since multiplication of vectors and scalars commute, I just define this function in terms of the class function.
    return vector * scalar;
}

#endif // VECTORND_H
