#include <gtest/gtest.h>
#include <regex>
#include <sstream>
#include <iostream>

#include "grep_util.hpp"

namespace GrepUtility {

class MockGrepUtil : public GrepUtil {
public:
    using GrepUtil::query_regex_; 
    using GrepUtil::pool_; 
    using GrepUtil::generate_regex; // Access protected method for testing
};

// Fixture for GrepUtil tests
class GrepUtilTest : public ::testing::Test {
protected:
    MockGrepUtil grep_util;
};

// Test case for case-sensitive regex creation
TEST_F(GrepUtilTest, RegexCreation_CaseSensitive) {
    SearchOptions options;
    options.case_sensitive = true;
    options.exact_match = false;

    grep_util.generate_regex("pattern", options);
    EXPECT_EQ(grep_util.query_regex_.flags(), std::regex::ECMAScript);

    // Test strings
    std::string lowercase_text = "This is a pattern in text.";
    std::string uppercase_text = "This is a PATTERN in text.";

    // Check if regex matches both lowercase and uppercase "pattern"
    EXPECT_TRUE(std::regex_search(lowercase_text, grep_util.query_regex_));
    EXPECT_FALSE(std::regex_search(uppercase_text, grep_util.query_regex_));
}

// Test case for case-insensitive regex creation
TEST_F(GrepUtilTest, RegexCreation_CaseInsensitive) {
    SearchOptions options;
    options.case_sensitive = false;
    options.exact_match = false;

    grep_util.generate_regex("pattern", options);
    EXPECT_EQ(grep_util.query_regex_.flags(), std::regex::icase);

    // Test strings
    std::string lowercase_text = "This is a pattern in text.";
    std::string uppercase_text = "This is a PATTERN in text.";

    // Check if regex matches both lowercase and uppercase "pattern"
    EXPECT_TRUE(std::regex_search(lowercase_text, grep_util.query_regex_));
    EXPECT_TRUE(std::regex_search(uppercase_text, grep_util.query_regex_));
}

// Test case for exact match regex creation
TEST_F(GrepUtilTest, ExactMatchRegex) {
    SearchOptions options;
    options.case_sensitive = true;
    options.exact_match = true;

    grep_util.generate_regex("pattern", options);
    std::string matching_text = "This is a pattern in text";
    std::string non_matching_text = "This is a patterned text";
      // Check if "pattern" is matched as a standalone word
    EXPECT_TRUE(std::regex_search(matching_text, grep_util.query_regex_));
    
    // Ensure "patterned" does not match as it is not a standalone occurrence
    EXPECT_FALSE(std::regex_search(non_matching_text, grep_util.query_regex_));
    
}


}  // namespace GrepUtility

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
