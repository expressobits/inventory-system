# Inventory System Test Suite

This document describes the C++ test infrastructure for the Inventory System gdextension, based on the LimboAI testing pattern.

## Overview

The test suite provides comprehensive testing for the core inventory system functionality, converted from the original GDScript tests in the addon branch.

## Test Structure

### Base Framework (`src/tests.h`)

The test framework provides:
- **Test Macros**: `CHECK()`, `CHECK_MESSAGE()`, `TEST_CASE()`, `TEST_CASE_END`
- **Base Classes**: `InventoryTestBase` for test suites
- **Utilities**: `TestUtils` for common test operations

### Test Suites

1. **`test_inventory.h`**: Basic inventory functionality
   - Adding/removing items
   - Stack management
   - Serialization/deserialization
   - Signal testing (placeholders)

2. **`test_grid_inventory.h`**: Grid-based inventory testing
   - Position-based item placement
   - Spatial constraints
   - Stack splitting/merging
   - Grid-specific operations

3. **`test_quad_tree.h`**: Quad tree data structure testing
   - Subdivision logic
   - Spatial queries
   - Item insertion/removal
   - Collision detection

### Test Runner (`src/test_runner.h`, `src/test_main.cpp`)

Command-line test execution with options:
- Run all tests: `./inventory_tests`
- Run specific suite: `./inventory_tests "Inventory Tests"`
- List available suites: `./inventory_tests --list`
- Show help: `./inventory_tests --help`

## Building Tests

### Prerequisites
- SCons build system: `pip install scons`
- Initialized godot-cpp submodule: `git submodule update --init --recursive`

### Build Commands

```bash
# Build main library
scons target=template_debug

# Build test executable
scons tests=1 target=template_debug

# Run tests
./bin/linux/inventory_tests
```

## Test Development

### Creating New Tests

1. **Include the test header**:
```cpp
#include "tests.h"
#include "core/your_class.h"
```

2. **Create test class**:
```cpp
class TestYourClass : public InventoryTestBase {
public:
    virtual const char* get_test_name() const override {
        return "Your Class Tests";
    }
    
    virtual bool run_tests() override {
        log_test_start("Your Class Tests");
        
        bool all_passed = true;
        all_passed &= test_basic_functionality();
        // Add more tests...
        
        log_test_end("Your Class Tests", all_passed);
        return all_passed;
    }
};
```

3. **Implement test methods**:
```cpp
TEST_CASE(test_basic_functionality)
    YourClass* obj = memnew(YourClass);
    
    CHECK(obj->some_method() == expected_value);
    CHECK_MESSAGE(obj->another_method(), "Custom error message");
    
    memdelete(obj);
TEST_CASE_END
```

### Test Patterns

**Memory Management**:
```cpp
YourClass* obj = memnew(YourClass);
// ... test operations
memdelete(obj);
```

**Resource Testing**:
```cpp
Ref<YourResource> resource;
resource.instantiate();
// ... test operations
// Resources are automatically managed
```

**Error Conditions**:
```cpp
CHECK(invalid_operation() == ERROR_CODE);
CHECK(!should_fail_condition);
```

### API Validation

Use the validation script to check API compatibility:
```bash
python /path/to/validate_api.py
```

## Test Coverage

### Current Coverage

| Component | Test File | Status |
|-----------|-----------|--------|
| Inventory | `test_inventory.h` | ✅ Complete |
| GridInventory | `test_grid_inventory.h` | ✅ Complete |
| QuadTree | `test_quad_tree.h` | ✅ Complete |

### Converted from GDScript

The tests are direct conversions from these GDScript files:
- `addons/inventory-system-demos/tests/inventory_tests.gd`
- `addons/inventory-system-demos/tests/grid_inventory_test.gd`
- `addons/inventory-system-demos/tests/quad_tree_tests.gd`
- `addons/inventory-system-demos/tests/test_suite.gd`

## Future Enhancements

1. **Signal Testing**: Implement proper signal connection testing
2. **Mock Objects**: Create mock databases and resources for isolated testing
3. **Performance Tests**: Add benchmark tests for critical operations
4. **Integration Tests**: Test component interactions
5. **Memory Leak Detection**: Add memory usage validation

## Troubleshooting

### Common Issues

1. **Compilation Errors**: Ensure godot-cpp submodule is properly initialized
2. **Missing Methods**: Check API validation output for method availability
3. **Memory Issues**: Always pair `memnew()` with `memdelete()`
4. **Resource Issues**: Use `Ref<>` for Godot resources

### Debug Output

Enable verbose test output by modifying test macros or adding debug prints:
```cpp
std::cout << "Debug: Testing condition X" << std::endl;
```

---

For more information, see the main repository documentation and the LimboAI test patterns that inspired this framework.