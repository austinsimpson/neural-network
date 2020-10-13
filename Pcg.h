/*This bit of code was written by Jason Turner, without license as far as I can tell. You can find the video that this code is sourced from here: https://www.youtube.com/watch?v=rpn_5Mrrxf8*/

#ifndef PCG_H
#define PCG_H

#include <cstdint>
#include <numeric>

constexpr uint64_t seedPcg()
{
    uint64_t shifted = 0;

    for (const auto c: __TIME__)
    {
        shifted <<= 8;
        shifted |= c;
    }

    return shifted;
}

struct PCG
{
    struct pcg32_random_t { std::uint64_t state = 0; std::uint64_t inc = seedPcg(); };
    pcg32_random_t rng;
    typedef std::uint32_t result_type;

    constexpr result_type operator()()
    {
        return pcg32_random_r();
    }

    static constexpr result_type min()
    {
        return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max()
    {
        return std::numeric_limits<result_type>::max();
    }
private:
    constexpr std::uint32_t pcg32_random_r()
    {
        std::uint64_t oldState = rng.state;

        rng.state = oldState * 6364136223846793005ULL + (rng.inc|1);

        std::uint32_t xorshifted  = ((oldState >> 18u) ^ oldState) >> 27u;
        std::uint32_t rot = oldState >> 59u;
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }
};

#endif // PCG_H
