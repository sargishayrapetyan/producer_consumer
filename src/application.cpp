#include <chrono>
#include <thread>

#include "application.hpp"

//std::condition_variable application::m_producers_cv;
//std::condition_variable application::m_consumers_cv;

application::application(const std::string& fileName, const int pn, const int cn)
    : stop_signal(false)
    , m_number_producers(pn)
    , m_number_consumers(cn)
    , m_file(fileName)
    , m_shared_data(m_producers_cv, m_consumers_cv)
    , m_producers(m_number_producers, m_producers_cv, m_shared_data)
    , m_consumers(m_number_consumers, m_consumers_cv, m_shared_data, m_file)
{
}

void application::run() {
    m_producers.run(m_number_producers);
    m_consumers.run(m_number_consumers);
}

void application::stop() {
    stop_signal = true;
    m_producers.set_stop_signal(true);
    m_consumers.set_stop_signal(true);
    m_producers_cv.notify_all();
    m_consumers_cv.notify_all();
    m_producers.stop();
    m_consumers.stop();
}

void application::wait() {
    while(!stop_signal){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }    
}
