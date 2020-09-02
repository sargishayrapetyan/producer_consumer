#include "file_writer.hpp"

#include <cassert>
#include <iostream>


file_writer::file_writer(const std::string& file_name)
    : m_fileName(file_name)
{
    assert(!m_fileName.empty());
    std::ofstream m_of(m_fileName, std::ios_base::trunc);
}

void file_writer::append(const int s) {
    std::ofstream m_of(m_fileName, std::ios_base::app | std::ios_base::binary);
    if (m_fileName.empty() || !m_of.is_open()) {
        std::cerr << "Could not open the file for writing." << std::endl;
        exit(1);
    }
    assert(m_of.good());
    assert(m_of.is_open() && m_of.good());
    std::lock_guard<std::mutex> lg(m_mutex);
    std::string str = std::to_string(s);
    m_of << str << ",";
}
