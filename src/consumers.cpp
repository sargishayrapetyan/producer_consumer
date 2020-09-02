#include <thread>
#include <cassert>
#include <iostream>

#include "consumers.hpp"


consumers::consumers(int cn, std::condition_variable& cv, 
        data_queue& shared_data, file_writer& file)
    : m_cn(cn)
    , m_stop_signal(false)
    , m_cv(cv)
    , m_shared_data(shared_data)
    , m_file(file)
    , m_gen(1, 100)
{   
    m_threads.reserve(m_cn);
}

void consumers::worker_thread() {
    while(true) {
        const int sleep_time = m_gen.generate_number();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        if(m_stop_signal && m_shared_data.empty()) {
            break;
        }
        if(!m_shared_data.empty()) {
            if (m_shared_data.ready_to_pop()) {
                m_file.append(m_shared_data.pop());
            }
        } else {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]{return (m_stop_signal || m_shared_data.ready_to_pop());});
            if (m_stop_signal) {
                while (m_shared_data.ready_to_pop()) {
                    const int n = m_shared_data.pop();
                    if (n == -1) {
                        std::cout << "----Error-----" << std::endl;
                        std::terminate();
                    }
                    m_file.append(n);
                }
                break;
            }
            if (m_shared_data.ready_to_pop()) {
                //m_file.append(m_shared_data.pop());
                const int n = m_shared_data.pop();
                if (n == -1) {
                    std::cout << "----Error-----" << std::endl;
                    std::terminate();
                }
                m_file.append(n);
            }
        }
    } 
}

void consumers::run(int m_cn) {
    for(int i = 0; i < m_cn; ++i) {
        m_threads.push_back(std::move(std::thread(&consumers::worker_thread, this)));    
    } 
}

void consumers::stop() {
    m_stop_signal.store(true);
    //m_cv.notify_all();
    for(auto& t : m_threads) {
        if(t.joinable()){
            t.join();
        }
    }
}
