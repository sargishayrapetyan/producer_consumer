#include "producers.hpp"

#include <cassert>


producers::producers(int pn, std::condition_variable& cv, data_queue& shared_data) 
    : m_pn(pn)
    , m_stop_signal(false)
    , m_cv(cv)
    , m_shared_data(shared_data)
    , m_gen(1, 100)
    , m_file("test_log.txt")
{
    m_threads.reserve(m_pn);
}


void producers::worker_thread() {
    while(!m_stop_signal) {
        const int sleep_time = m_gen.generate_number();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        if(m_stop_signal) {
            break;
        }
        const int number = m_gen.generate_number();
        if(m_shared_data.ready_to_push()) {
            m_shared_data.push(number);
            m_file.append(number);
        } else {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]{return (m_stop_signal || m_shared_data.ready_to_push());});
            if (m_stop_signal) {
                break;
            }
            if (m_shared_data.ready_to_push()) {
                m_shared_data.push(number);
                m_file.append(number);
            }
        }
    }
}

void producers::run(int m_pn) {
    for(int i = 0; i < m_pn; ++i) {
        m_threads.push_back(std::move(std::thread(&producers::worker_thread, this)));
    }
}

void producers::stop() {
    m_stop_signal.store(true);
    //m_cv.notify_all();
    for(auto& t : m_threads) {
        if(t.joinable()){
            t.join();
        }
    }
}
