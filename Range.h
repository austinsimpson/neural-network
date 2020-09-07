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

#ifndef RANGE_H
#define RANGE_H

class range
{
public:
    range(int begin, int end) : _begin{begin}, _end{end}, _currentValue{begin}
    {
    }

    range(const range& other) : _begin(other._begin), _end(other._end), _currentValue(other._currentValue) {}

    range& operator=(const range& other)
    {
        _begin = other._begin;
        _end = other._end;
        _currentValue = other._currentValue;
        return *this;
    }

    bool operator==(const range& other)
    {
        return _currentValue >= other._end;
    }

    bool operator!=(const range& other)
    {
        return !(*this == other);
    }

    range& begin()
    {
        return *this;
    }

    range& end()
    {
        return *this;
    }

    int operator*()
    {
        return _currentValue;
    }

    range operator++()
    {
        ++_currentValue;
        return *this;
    }


private:
    int _begin;
    int _end;
    int _currentValue;
};

#endif // RANGE_H
