#include "grep_util.hpp"
#include <iostream>


int main(int argc, char* argv[]) {
    std::cout<< argc << "1\n";
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "<directory> <query>" << std::endl;
        return 0;
    }

    GrepUtility::SearchOptions options;
    for (int i = 3; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--exact") {
            options.exact_match = true;
        }
        else if (arg == "--case-sensitive") {
            options.case_sensitive = true;
        }
    }
    auto grp = GrepUtility::GrepUtil();
    grp.search(argv[1], argv[2], options);
    return 0;
}