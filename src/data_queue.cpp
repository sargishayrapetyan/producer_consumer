#include <cassert>
#include "data_queue.hpp"

data_queue::data_queue(std::condition_variable& p_cv, std::condition_variable& c_cv)
    : m_p_cv(p_cv)
    , m_c_cv(c_cv)
{
}

bool data_queue::ready_to_push() const {
    std::lock_guard<std::mutex> lg{ m_mutex };
    return m_container.size() < max_range;
}

bool data_queue::ready_to_pop() const {
    std::lock_guard<std::mutex> lg{ m_mutex };
    return !m_container.empty();
}

void data_queue::push(const int n) {
    std::lock_guard<std::mutex> lg{ m_mutex };
    m_container.push(n);
    m_c_cv.notify_all();
}

int data_queue::pop() {
    std::lock_guard<std::mutex> lg{ m_mutex };
    //assert(!m_container.empty());
    int n = -1;
    if (!m_container.empty()) {
        m_container.pop();
        n = m_container.front();
    }
    if (m_container.size() <= min_range) {
        m_p_cv.notify_all();
    }
    return n;
}

bool data_queue::empty() const {
    std::lock_guard<std::mutex> lg{ m_mutex };
    return m_container.empty();
}
