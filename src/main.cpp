#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include <cstdlib>
#include <cassert>
#include "application.hpp"

application* app_ptr = nullptr;
bool is_stop_handled = false;

void handle_stop_signal(int signal) {
    std::cout << "Terminate signal received.\n";
    if (is_stop_handled) {
        std::cout << "Waiting for consumers to finish." << std::endl;
        return;
    }
    if(signal == SIGINT) {
        is_stop_handled = true;
        assert(app_ptr != nullptr);
        app_ptr->stop();
        //app_ptr->wait();
        //exit(1);
    }    
}

void usage() {
    std::cout << "Number of consumers and producers can not be more then 10, and less then 0" << std::endl;    
}

int main(int argc, char** argv) {
    if(argc != 3) {
        std::cout << "ERROR: wrong number of arguments." << std::endl;
        std::cout << "producers and consumers count should be specified." << std::endl;
        return 1;
    }

    const unsigned short cons_count = std::stoi(argv[1]);
    const unsigned short prod_count = std::stoi(argv[2]);
    
    if(0 >= cons_count || 10 < cons_count || 0 >= prod_count || 10 < prod_count ) {
        usage();    
        return 1;
    }
    const std::string file_name("numbers.txt");
    application app(file_name, prod_count,cons_count);
    std::cout << "before run" << std::endl;
    
    app_ptr = &app;
    signal(SIGINT, handle_stop_signal);
    
    app.run();
    app.wait();
    return 0;
}
