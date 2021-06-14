#include "system.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <ios>


namespace dlplan::utils {

int get_peak_memory_in_kb() {
    // On error, produces a warning on cerr and returns -1.
    int memory_in_kb = -1;

    std::ifstream procfile;
    procfile.open("/proc/self/status");
    std::string word;
    while (procfile.good()) {
        procfile >> word;
        if (word == "VmPeak:") {
            procfile >> memory_in_kb;
            break;
        }
        // Skip to end of line.
        procfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if (procfile.fail())
        memory_in_kb = -1;

    if (memory_in_kb == -1)
        std::cerr << "warning: could not determine peak memory" << std::endl;
    return memory_in_kb;
}

}
