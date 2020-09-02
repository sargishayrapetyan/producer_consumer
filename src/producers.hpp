#pragma once

#include <mutex>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>

//project sourcies
#include "number_generator.hpp"
#include "data_queue.hpp"
#include "file_writer.hpp"

class producers
{
    public: 
        producers(int, std::condition_variable&, data_queue&);

    public: 
        ~producers() = default;
    
    public:
        void push_element();
        void sleep_time(int);
        void stop();
        void worker_thread();
        void run(int);

        void set_stop_signal(bool s) {
            m_stop_signal = s;
        }

    private:
        int m_pn;
        std::atomic<bool> m_stop_signal;
        std::condition_variable& m_cv;
        data_queue& m_shared_data;
        number_generator m_gen;
        file_writer m_file;
        std::vector<std::thread> m_threads;
        std::mutex m_mutex;
};
