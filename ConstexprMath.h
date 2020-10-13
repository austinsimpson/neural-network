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

#ifndef CONSTEXPRMATH_H
#define CONSTEXPRMATH_H

constexpr double constexprPow(double x, int y)
{
    return y == 0 ? 1.0 : x * constexprPow(x, y - 1);
}

constexpr double factorial(int n)
{
    if (n == 0)
    {
        return 1;
    }
    else
    {
        return n * factorial(n - 1);
    }
}

constexpr double constexprExp(double x)
{
    return 1.0 + x + constexprPow(x, 2) / factorial(2) + constexprPow(x, 3) / factorial(3)
                   + constexprPow(x, 4) / factorial(4) + constexprPow(x, 5) / factorial(5)
                   + constexprPow(x, 6) / factorial(6) + constexprPow(x, 7) / factorial(7)
                   + constexprPow(x, 8) / factorial(8) + constexprPow(x, 9) / factorial(9);
}

constexpr double constexprSin(double x)
{
    return x - constexprPow(x, 3) / factorial(3) + constexprPow(x, 5) / factorial(5)
             - constexprPow(x, 7) / factorial(7) + constexprPow(x, 9) / factorial(9)
             - constexprPow(x, 11) / factorial(11) + constexprPow(x, 13) / factorial(13)
             - constexprPow(x, 15) / factorial(15) + constexprPow(x, 17) / factorial(17);
}

//You really only want to use this for x <= 4
constexpr double constexprLog(double x)
{
    const auto xSquared = constexprPow(x, 2);
    return (3 * xSquared - 3.) / (xSquared + (4 * x) + 1);
}


#endif // CONSTEXPRMATH_H
