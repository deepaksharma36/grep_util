# Grep Utility with Thread Pool

This project is a multi-threaded utility for performing efficient search operations in files using regular expressions. It leverages a custom thread pool to handle parallel file processing, allowing users to search within directories for specific patterns. The utility supports case-sensitive and exact-match search options.

## Features

- **Parallel File Processing**: Uses a thread pool to distribute file searches across multiple threads, improving performance.
- **Flexible Search Options**: Supports case-sensitive, case-insensitive, and exact match searches.
- **Configurable Regular Expressions**: Generates regex patterns based on user options.
- **Mocking Capabilities for Testing**: Includes mock classes and test cases for isolated testing without actual file operations.

## Project Structure
```
.
├── include
│   ├── grep_util.hpp       # Header for GrepUtil class
│   ├── thread_pool.hpp     # Header for ThreadPool class
├── src
│   ├── grep_util.cpp       # Implementation of GrepUtil class
│   ├── thread_pool.cpp     # Implementation of ThreadPool class
├── test
│   ├── grep_util_test.cpp  # Unit tests for GrepUtil
│   └── (todo)thread_pool_test.cpp# Unit tests for ThreadPool
└── README.md               # Project documentation

```

## Installation
Prerequisites
C++20 or higher
CMake for building
Google Test for running unit tests (the libs will be downloaded directly from gtest repo)
Building the Project
Clone the repository:


### Build with CMake:

```bash
# download and extract the code
cd grep_util
mkdir build
cd build
cmake ..
make
```
### Running tests:

```
cd build
ctest
```

# Design Document

## System Architecture
```
The system is divided into the following key components:

Main Program (Executable):

This is the entry point of the program where the user provides the command-line arguments, and based on these options, different methods are invoked.
GrepUtil Class:

The core class of the project that is responsible for pattern searching. It generates regex based on the search options and iterates through files to find matches.
This class also handles user-specified options like case-sensitivity and exact match.
ThreadPool Class:

A worker thread pool used for handling file search operations concurrently. This class manages the threads and ensures that files are processed in parallel, improving performance when searching large datasets or directories.
SearchOptions Struct:

A structure that holds configuration settings for the search operation, such as whether to perform case-sensitive searches and whether to search for an exact match.
Regex Handling:

Regular expressions are used to match the pattern within the files. The regex is generated dynamically based on user input, considering options like exact match and case sensitivity.
```

## Data Flow and Interaction

```
Command-Line Interface (CLI)
The user provides a search pattern and an optional directory to search.
Options like --ignore-case or --exact-match can be passed to modify the behavior of the search.


GrepUtil::search()
The search() function in the GrepUtil class initiates the search operation.
It parses the command-line arguments and constructs the search pattern based on the options provided.
This method utilizes the ThreadPool to distribute the search tasks across multiple threads.


ThreadPool
The ThreadPool class manages a pool of worker threads, each tasked with processing a file or a set of files.
The worker threads fetch tasks (file paths) from a queue and perform the search operation.
Upon completion, the thread pool ensures that all threads are joined, and no thread remains running after the search operation ends.


File Processing and Regex Matching
The files are read line by line, and each line is checked against the generated regular expression.
If a match is found, the program prints the file path, line number, and matching line to the standard output.
The regex is dynamically built depending on the search options like exact match (\bpattern\b) or case-insensitive (std::regex::icase).
```
## Design Decisions

```
ThreadPool Implementation
Rationale: The decision to use a thread pool is made to handle large directories and multiple files efficiently. Without a thread pool, the program would run sequentially, causing performance bottlenecks when dealing with large datasets.

Thread Pool Size: The thread pool scales based on the number of available CPU cores X Scaling Factor(3) and can be manually adjusted for different workloads.
Since IO operations are involed therefore it make sense to have number of thread greater than number of CPU cores

Regex Handling

Exact Match: When the exact_match option is enabled, the tool uses word boundaries (\b) to ensure that only the exact pattern is matched.
Case Sensitivity: The tool allows users to toggle case sensitivity via command-line options, using std::regex::icase for case-insensitive matching.

Error Handling Strategy
The program does not terminate abruptly when encountering errors such as permission denials or file access issues. Instead, it prints an error message and continues with other files.
```


## Modules

```
Module 1: ThreadPool
Purpose: Manages concurrent tasks (file search operations).
Methods:
submit(): Adds a file to the queue for processing.
worker_thread_function(): Runs the worker threads.
stop(): Stops the thread pool and joins all threads.

Module 2: GrepUtil
Purpose: Core functionality to search for patterns in files.
Methods:
generate_regex(): Creates the regex pattern based on the search options.
search(): Initiates the search process across directories and files.
search_in_file(): Searches a single file for the specified pattern.

Module 3: SearchOptions
Purpose: Holds the configuration for the search.
Members:
case_sensitive: Determines if the search is case-sensitive.
exact_match: Determines if the search matches the exact word.
```

## Testing

```
Testing
The project uses Google Test for unit testing.

Test Cases
Regex Generation: Tests to ensure that regex is generated correctly for different options like case-sensitivity and exact match.
(TODO) File Processing: Tests to check if the files are correctly opened and searched.
(TODO) ThreadPool Behavior: Tests to ensure threads are correctly created and joined.

```