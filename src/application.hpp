#include <condition_variable>

//headers for ctrl+c handler
#include "producers.hpp"
#include "consumers.hpp"
#include "data_queue.hpp"
#include "file_writer.hpp"

class application
{
    public:
        application(const std::string& fileName, const int pn, const int cn);

    public:
        void run();
        void stop();
        void wait();

    private:
        std::atomic<bool> stop_signal;
        int m_number_producers;
        int m_number_consumers;
        file_writer m_file;
        data_queue m_shared_data;
        producers m_producers;
        consumers m_consumers;
        std::condition_variable m_producers_cv;
        std::condition_variable m_consumers_cv;
};
