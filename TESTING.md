# Inventory System Test Suite

This document describes the C++ test infrastructure for the Inventory System gdextension, based on the LimboAI testing pattern.

## Overview

The test suite provides comprehensive testing for the core inventory system functionality, converted from the original GDScript tests in the addon branch.

## Test Structure

### Base Framework (`tests/tests.h`)

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

### Test Runner (`tests/test_runner.h`, `tests/test_main.cpp`)

Command-line test execution with options:

#### Linux/macOS
- Run all tests: `./bin/linux/inventory_tests`
- Run specific suite: `./bin/linux/inventory_tests "Inventory Tests"`
- List available suites: `./bin/linux/inventory_tests --list`
- Show help: `./bin/linux/inventory_tests --help`

#### Windows
- Run all tests: `.\bin\windows\inventory_tests.exe`
- Run specific suite: `.\bin\windows\inventory_tests.exe "Inventory Tests"`
- List available suites: `.\bin\windows\inventory_tests.exe --list`
- Show help: `.\bin\windows\inventory_tests.exe --help`

## Building Tests

### Prerequisites
- SCons build system: `pip install scons`
- Initialized godot-cpp submodule: `git submodule update --init --recursive`

### Build Commands

#### Linux/macOS
```bash
# Build main library
scons target=template_debug

# Build test executable
scons tests=yes target=template_debug

# Run tests
./bin/linux/inventory_tests
```

#### Windows
```cmd
# Build main library
scons target=template_debug

# Build test executable  
scons tests=yes target=template_debug

# Run tests
.\bin\windows\inventory_tests.exe
```

#### Cross-platform build options
```bash
# Build for specific platform
scons tests=yes target=template_debug platform=linux
scons tests=yes target=template_debug platform=windows  
scons tests=yes target=template_debug platform=macos

# Build for specific architecture
scons tests=yes target=template_debug platform=windows arch=x86_64
scons tests=yes target=template_debug platform=windows arch=x86_32
```

## Platform Support

The test framework supports all platforms that the gdextension supports:

### Supported Platforms
- **Linux**: x86_64
- **Windows**: x86_64, x86_32  
- **macOS**: Universal (x86_64 + arm64)
- **Android**: arm64, arm32, x86_64, x86_32
- **iOS**: arm64
- **Web**: wasm32

### Platform-specific Notes

#### Windows
- Test executable has `.exe` extension: `inventory_tests.exe`
- Use backslashes in paths: `.\bin\windows\inventory_tests.exe`
- Supports both x86_64 and x86_32 architectures
- Can be built on Windows or cross-compiled from Linux

#### Linux
- Test executable has no extension: `inventory_tests`
- Use forward slashes in paths: `./bin/linux/inventory_tests`
- Requires executable permissions: `chmod +x bin/linux/inventory_tests`

#### macOS
- Test executable has no extension: `inventory_tests`
- Universal binary supports both Intel and Apple Silicon
- Use forward slashes in paths: `./bin/macos/inventory_tests`

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

### Platform-specific Issues

#### Windows
- **Path separators**: Use backslashes (`\`) in command line, forward slashes in SCons
- **Executable not found**: Ensure `.exe` extension is included when running tests
- **Permission denied**: Run command prompt as administrator if needed
- **Build failures**: Install Visual Studio Build Tools or MSVC compiler

#### Linux
- **Permission denied**: Run `chmod +x bin/linux/inventory_tests` to make executable
- **Library not found**: Ensure all dependencies are installed via package manager
- **Build failures**: Install build-essential package: `sudo apt install build-essential`

#### macOS
- **Code signing**: May need to allow unsigned binary in System Preferences
- **Architecture mismatch**: Ensure correct arch parameter (universal, x86_64, arm64)

### Debug Output

Enable verbose test output by modifying test macros or adding debug prints:
```cpp
std::cout << "Debug: Testing condition X" << std::endl;
```

---

For more information, see the main repository documentation and the LimboAI test patterns that inspired this framework.