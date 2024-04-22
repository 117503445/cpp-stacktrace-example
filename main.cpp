#include <execinfo.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <zconf.h>
#include "regex"
#include "vector"

std::string getexepath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    auto r = std::string(result, (count > 0) ? count : 0);
    std::cout << "" << r << std::endl;
    return r;
}

std::string sh(std::string cmd) {
    // std::array<char> buffer;
    std::vector<char> buffer(128);
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
    }
    return result;
}


void print_backtrace(void) {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    printf("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }

    free(strings);
    

    // void *bt[1024];
    // int bt_size;
    // char **bt_syms;
    // int i;

    // bt_size = backtrace(bt, 1024);
    // bt_syms = backtrace_symbols(bt, bt_size);
    // std::regex re("\\[(.+)\\]");
    // auto exec_path = getexepath();
    // std::string addrs = "";
    // for (i = 1; i < bt_size; i++) {
    //     std::string sym = bt_syms[i];
    //     std::smatch ms;
    //     if (std::regex_search(sym, ms, re)) {
    //         std::string m = ms[1];
    //         addrs += " " + m;
    //     }
    // }
    // std::cout << "addrs = " << addrs << std::endl;
    // auto r = sh("addr2line -e " + exec_path + " -f -C " + addrs);
    // std::cout << r << std::endl;
    // free(bt_syms);
}

void test_m() {
    print_backtrace();
}

int main() {
    test_m();
    return 0;
}   