#pragma once
#include "thread_pool.hpp"
#include <string>
#include <regex>

namespace GrepUtility {

/**
 * @brief Structure to specify search options for the GrepUtil class.
 * 
 * SearchOptions allows customization of search criteria, such as 
 * exact match and case sensitivity. 
 */
struct SearchOptions {
    bool exact_match{false};       ///< When true, search is performed for an exact word match.
    bool case_sensitive{false};    ///< When true, search is case-sensitive.
    // TODO: Additional options can be added for future features
};

/**
 * @brief A utility class for performing regex-based searches in files within a directory.
 * 
 * GrepUtil is designed to search for a specified query within a directory's files, 
 * supporting both case-sensitive and exact match searches. It uses a thread pool to 
 * parallelize file processing for efficiency.
 */
class GrepUtil {
public:
    /**
     * @brief Constructs a GrepUtil object with a default thread pool.
     */
    GrepUtil();
    ~GrepUtil();

    /**
     * @brief Searches for the given query within all files in a specified directory.
     * 
     * This method initiates a search across all files within the specified directory
     * that match the search criteria defined in `options`. It delegates individual file 
     * searches to worker threads from the thread pool.
     * 
     * @param directory The path to the directory to search in.
     * @param query The search query.
     * @param options The search options to control exact match and case sensitivity.
     */
    void search(const std::string& directory, const std::string& query, const SearchOptions& options);

    /**
     * @brief Searches for the specified query within a single file.
     * 
     * This function opens the file located at `file_path` and performs a regex search
     * line-by-line to locate occurrences of the query.
     * 
     * @param file_path The path to the file to search in.
     */
    void search_in_file(const std::string& file_path) const;

protected:
    /**
     * @brief Generates a regex pattern based on the query and search options.
     * 
     * This function creates a regex pattern based on the provided search options.
     * - If `case_sensitive` is false, a case-insensitive regex is generated.
     * - If `exact_match` is true, word boundaries are added to enforce whole-word matching.
     * 
     * @param query The search query string.
     * @param options The search options to control regex generation.
     * @return return true iff regex generation was successfull
     */
    bool generate_regex(const std::string& query, const SearchOptions& options);

    ThreadPool pool_;             ///< Thread pool for parallel file processing.
    std::regex query_regex_;      ///< Compiled regex pattern based on search query and options.
};

} // namespace GrepUtility
