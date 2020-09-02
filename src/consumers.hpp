#pragma once

#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <condition_variable>
#include <atomic>

#include "file_writer.hpp"
#include "data_queue.hpp"
#include "number_generator.hpp"

class consumers 
{
    public: 
        consumers(int, std::condition_variable&, data_queue&, file_writer&);
    public:
        void worker_thread();
        void run(int m_cn);
        void stop();

        void set_stop_signal(bool s) {
            m_stop_signal = s;
        }

    private:
        int m_cn;
        std::atomic<bool> m_stop_signal;
        std::condition_variable& m_cv;
        data_queue& m_shared_data;
        file_writer&  m_file;
        number_generator m_gen;
        std::vector<std::thread> m_threads;
        std::mutex m_mutex;
};
