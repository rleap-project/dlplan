#include "logging.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace dlplan::utils {

Log g_log;

Log::Log() {
    // Silence the logger if the environment variable is set to 1.
    const char *env_silent = getenv("DLPLAN_SILENT_LOG");
    if (env_silent && string(env_silent) == "1") {
        silent = true;
    }
}

}
