#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>


class data_queue 
{
    public:
        data_queue(std::condition_variable&, std::condition_variable&);

    public:
        bool ready_to_push() const;
        bool ready_to_pop() const;
        void push(const int n);
        int pop();
        bool empty() const;

    public:
        ~data_queue() = default;

        data_queue(const data_queue&) = delete;
        data_queue(data_queue&&) = delete;
        data_queue& operator=(const data_queue&) = delete;
        data_queue& operator=(data_queue&&) = delete;

    private:
        const static unsigned short min_range = 80;
        const static unsigned short max_range = 100;
        std::condition_variable& m_p_cv;
        std::condition_variable& m_c_cv;
        mutable std::mutex m_mutex;
        std::queue<int> m_container;
};
