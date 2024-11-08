#include <grep_util.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace GrepUtility {

    GrepUtil::GrepUtil() : pool_() {
        pool_.register_task([this](const std::string& file_path) {
        this->search_in_file(file_path);
    });
    }
    GrepUtil::~GrepUtil() {
        // Explicitly call stop to gracefully shut down threads before the pool is destroyed
        pool_.stop();  
    }

    void GrepUtil::search(const std::string& directory, const std::string& query, const SearchOptions& options){

        if(!generate_regex(query, options)) {
            std::cerr<<"Unable to generate regex for provided pattern";
        }
        for (const auto& entry : fs::recursive_directory_iterator(directory, fs::directory_options::skip_permission_denied)) {
            try {
                if (entry.is_regular_file()) {
                    pool_.submit(entry.path().string());
                }
            }
            catch(const std::exception& e) {
                std::cerr << "Unexpected error in directory " << directory << ": " << e.what() << std::endl;
            }
        }
    }

    void GrepUtil::search_in_file(const std::string& file_path) const{
        try {
            std::ifstream file(file_path);
            if (!file.is_open()) {
                std::cerr<< "Could not open file " << file_path << std::endl;
                return;
            }

            std::string line;
            int line_number = 1;
            while (std::getline(file, line)) {
                if (std::regex_search(line, query_regex_)) {
                    std::cout << file_path << ":" << line_number << ": " << line << std::endl;
                }
                line_number++;
            }
        }
        catch (const std::ifstream::failure& e) {
            std::cerr << "File I/O error in file " << file_path << ": " << e.what() << std::endl;}
        catch (const std::exception& e) {
            std::cerr << "Unexpected error in file " << file_path << ": " << e.what() << std::endl;
        }
    }
    

    bool GrepUtil::generate_regex(const std::string& query, const SearchOptions& options ) {
        std::string pattern(query); 
        try {
            if (options.exact_match) {
                pattern = "\\b" + pattern + "\\b";
            }

            if (!options.case_sensitive) {
                query_regex_ = std::regex(pattern, std::regex::icase);
            }
            else {
                query_regex_ = std::regex(pattern); //case-sensitive
            }
            return true;
        }
        catch (const std::regex_error& e) {
            // Catch and handle the regex error
            std::cerr << "Regex error occurred: " << e.what() << std::endl;
            return false;
        }
    }


} //namespace GrepUtility



