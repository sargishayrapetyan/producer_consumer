#pragma once

#include <random>
#include <climits>

class number_generator
{
    public:
        number_generator(const int, const int);

    public:	
        int generate_number();
    
    public:
        ~number_generator() = default;

        number_generator(const number_generator&) = delete;
        number_generator(number_generator&&) = delete;
        number_generator& operator=(const number_generator&) = delete;
        number_generator& operator=(number_generator&&) = delete;

    private:
        std::random_device m_rd;
        std::mt19937 m_mt;
        std::uniform_int_distribution<> m_dist;
};
