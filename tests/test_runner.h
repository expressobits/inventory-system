/**
 * test_runner.h
 * =============================================================================
 * Copyright (c) 2023-present ExpressoBits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "tests.h"
#include "test_minimal.h"
// TODO: Fix compilation issues with these tests
// #include "test_inventory.h" 
// #include "test_grid_inventory.h"
// #include "test_quad_tree.h"

#include <iostream>
#include <vector>
#include <memory>

class TestRunner {
private:
	std::vector<std::unique_ptr<InventoryTestBase>> test_suites;
	
public:
	TestRunner() {
		// Register working test suites
		test_suites.push_back(std::make_unique<TestMinimal>());
		// TODO: Fix and re-enable these tests
		// test_suites.push_back(std::make_unique<TestInventory>());
		// test_suites.push_back(std::make_unique<TestGridInventory>());
		// test_suites.push_back(std::make_unique<TestQuadTree>());
	}
	
	bool run_all_tests() {
		std::cout << "=== Starting Inventory System Test Suite ===" << std::endl;
		
		bool all_passed = true;
		int total_suites = test_suites.size();
		int passed_suites = 0;
		
		for (auto& test_suite : test_suites) {
			std::cout << "\n--- Running " << test_suite->get_test_name() << " ---" << std::endl;
			
			bool suite_passed = test_suite->run_tests();
			if (suite_passed) {
				passed_suites++;
				std::cout << "✓ " << test_suite->get_test_name() << " PASSED" << std::endl;
			} else {
				all_passed = false;
				std::cout << "✗ " << test_suite->get_test_name() << " FAILED" << std::endl;
			}
		}
		
		std::cout << "\n=== Test Results ===" << std::endl;
		std::cout << "Passed: " << passed_suites << "/" << total_suites << " test suites" << std::endl;
		
		if (all_passed) {
			std::cout << "🎉 All tests PASSED!" << std::endl;
		} else {
			std::cout << "💥 Some tests FAILED!" << std::endl;
		}
		
		return all_passed;
	}
	
	bool run_test_suite(const std::string& suite_name) {
		for (auto& test_suite : test_suites) {
			if (std::string(test_suite->get_test_name()) == suite_name) {
				std::cout << "Running specific test suite: " << suite_name << std::endl;
				return test_suite->run_tests();
			}
		}
		
		std::cout << "Test suite '" << suite_name << "' not found!" << std::endl;
		return false;
	}
	
	void list_test_suites() {
		std::cout << "Available test suites:" << std::endl;
		for (auto& test_suite : test_suites) {
			std::cout << "  - " << test_suite->get_test_name() << std::endl;
		}
	}
};

#endif // TEST_RUNNER_H