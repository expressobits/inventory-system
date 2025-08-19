/**
 * test_main.cpp
 * =============================================================================
 * Copyright (c) 2023-present ExpressoBits and the Inventory System contributors.
 *
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT.
 * =============================================================================
 */

#include "test_runner.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	TestRunner runner;
	
	if (argc == 1) {
		// No arguments - run all tests
		bool success = runner.run_all_tests();
		return success ? 0 : 1;
	}
	
	std::string command = argv[1];
	
	if (command == "--list" || command == "-l") {
		runner.list_test_suites();
		return 0;
	}
	
	if (command == "--help" || command == "-h") {
		std::cout << "Inventory System Test Runner" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << "  " << argv[0] << "                    Run all tests" << std::endl;
		std::cout << "  " << argv[0] << " --list             List available test suites" << std::endl;
		std::cout << "  " << argv[0] << " --help             Show this help" << std::endl;
		std::cout << "  " << argv[0] << " <suite_name>       Run specific test suite" << std::endl;
		return 0;
	}
	
	// Try to run specific test suite
	bool success = runner.run_test_suite(command);
	return success ? 0 : 1;
}