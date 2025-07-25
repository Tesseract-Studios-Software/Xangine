#include "./src/xgn_log/log.hpp"
#include "./src/xgn_core/core.hpp"
using namespace std;

int main() {
    int i;
    set_executable_dir(); // Set the working directory to the executable directory.
    log("99", 1, "The tester is now about to test all error codes.");
    log("95", 1);
    for (i = 100; i < 103; i++) {
        log(to_string(i), 1, "This is a test log message.");
    }
    log("99", 1, "The tester is now about to test all log levels.");
    log("95", 1);
    log("98", 0);
    log("98", 1);
    log("98", 2);
    log("98", 3);
    log("98", 4);
    log("98", 5);
    log("96", 1);
    log("99", 1, "The tester will now create an instance of Xangine to test all features.");
    // Create window.

    log("96", 1);
    log("3", 2);
    log("2", 3);
    log("1", 4);
    log("0", 5);
}