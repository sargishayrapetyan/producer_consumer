#pragma once

#include <string>
#include <fstream>
#include <mutex>

class file_writer {
    public:
        file_writer(const std::string&);

    public:
        void append(const int);
        
        ~file_writer() = default;

        file_writer() = delete;
        file_writer(const file_writer&) = delete;
        file_writer(file_writer&&) = delete;
        file_writer& operator=(const file_writer&) = delete;
        file_writer& operator=(file_writer&&) = delete;

    private:
        std::string m_fileName;
        std::mutex m_mutex;
};
