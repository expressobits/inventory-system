#ifndef TEST_MINIMAL_H
#define TEST_MINIMAL_H

#include "tests.h"

// Minimal test to verify the test infrastructure works
class TestMinimal : public InventoryTestBase {
public:
    const char* get_test_name() const override {
        return "Minimal Tests";
    }

    bool run_tests() override {
        log_test_start("Minimal Tests");
        
        bool result = true;
        
        // Call test methods and aggregate results
        result &= test_basic_arithmetic();
        result &= test_basic_string();
        
        log_test_end("Minimal Tests", result);
        return result;
    }

private:
    // Simple arithmetic test
    TEST_CASE(test_basic_arithmetic)
        CHECK(1 + 1 == 2);
        CHECK(5 * 6 == 30);
        CHECK(10 / 2 == 5);
    TEST_CASE_END
    
    // Simple string test using Godot String
    TEST_CASE(test_basic_string)
        String test_str = "Hello World";
        CHECK(test_str.length() == 11);
        CHECK(test_str.begins_with("Hello"));
        CHECK(test_str.ends_with("World"));
    TEST_CASE_END
};

#endif // TEST_MINIMAL_H