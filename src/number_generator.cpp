#include "number_generator.hpp"

number_generator::number_generator(const int b, const int e)
    : m_rd()
    , m_mt( m_rd() )
    , m_dist( b, static_cast<int>(std::nextafter(e, INT_MAX)) )
{
}

int number_generator::generate_number() {
    return m_dist(m_mt);
}
