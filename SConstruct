#!/usr/bin/env python
import os
import sys

from methods import print_error


libname = "inventory_system"
projectdir = "inventory-system"

localEnv = Environment(tools=["default"], PLATFORM="")

# Build profiles can be used to decrease compile times.
# You can either specify "disabled_classes", OR
# explicitly specify "enabled_classes" which disables all other classes.
# Modify the example file as needed and uncomment the line below or
# manually specify the build_profile parameter when running SCons.

# localEnv["build_profile"] = "build_profile.json"

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

# Filter out 'tests' parameter from ARGUMENTS for godot-cpp compatibility
filtered_args = {k: v for k, v in ARGUMENTS.items() if k != 'tests'}

opts = Variables(customs, ARGUMENTS)
opts.Add(BoolVariable("tests", "Build test executable (cross-platform)", False))
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()
opts.Update(env)

submodule_initialized = False
dir_name = 'godot-cpp'
if os.path.isdir(dir_name):
    if os.listdir(dir_name):
        submodule_initialized = True

if not submodule_initialized:
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

# Store the tests parameter before calling godot-cpp SConstruct  
tests_requested = env.get('tests', False)

# Call godot-cpp SConstruct with filtered arguments to avoid warnings
original_args = ARGUMENTS.copy()
try:
    # Temporarily remove tests from ARGUMENTS
    if 'tests' in ARGUMENTS:
        del ARGUMENTS['tests']
    
    env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})
finally:
    # Restore original ARGUMENTS
    ARGUMENTS.clear()
    ARGUMENTS.update(original_args)

# Restore the tests parameter after godot-cpp SConstruct
env['tests'] = tests_requested

env.Append(CPPPATH=["src/"])

# Main library sources
sources = [
    Glob('src/*.cpp'),
    Glob('src/base/*.cpp'),
    Glob('src/constraints/*.cpp'),
    Glob('src/core/*.cpp'),
    Glob('src/craft/*.cpp'),
    ]

if env["target"] in ["editor"]:
    sources.append(Glob('src/editor/*.cpp'))
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

file = "{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"])

# Build main library
libraryfile = "bin/{}/{}".format(env["platform"], file)
library = env.SharedLibrary(
    libraryfile,
    source=sources,
)

copy = env.InstallAs("{}/addons/{}/bin/{}/{}".format(projectdir, projectdir, env["platform"], file), library)

# Build test executable if requested
test_sources = [
    'tests/test_main.cpp',
    # Include needed source files for tests (excluding register_types.cpp)
    Glob('src/base/*.cpp'),
    Glob('src/constraints/*.cpp'),
    Glob('src/core/*.cpp'),
    Glob('src/craft/*.cpp'),
]

test_env = env.Clone()
test_env.Append(CPPPATH=["src/", "tests/"])

# Check if tests target is requested
# Usage: scons tests=yes target=template_debug
# Cross-platform: works on Linux, Windows, macOS
# Output: bin/{platform}/inventory_tests{extension}
#   Linux/macOS: bin/linux/inventory_tests
#   Windows: bin/windows/inventory_tests.exe
# Check if tests target is requested
# Usage: scons tests=yes target=template_debug
# Cross-platform: works on Linux, Windows, macOS
# Output: bin/{platform}/inventory_tests{extension}
#   Linux/macOS: bin/linux/inventory_tests
#   Windows: bin/windows/inventory_tests.exe
if env.get('tests', False):
    test_executable = test_env.Program(
        target="bin/{}/inventory_tests{}".format(env["platform"], env["PROGSUFFIX"]),
        source=test_sources
    )
    Alias('tests', test_executable)
    # Add tests to default build when tests=yes
    default_args = [library, copy, test_executable]
else:
    default_args = [library, copy]

Default(*default_args)
