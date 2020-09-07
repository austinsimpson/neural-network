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

//You really only want to use this for x <= 4
constexpr double constexprLog(double x)
{
    const auto xSquared = constexprPow(x, 2);
    return (3 * xSquared - 3.) / (xSquared + (4 * x) + 1);
}


#endif // CONSTEXPRMATH_H
