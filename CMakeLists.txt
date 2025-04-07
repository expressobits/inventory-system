cmake_minimum_required(VERSION 3.27)
project(inventory_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GODOTCPP_USE_HOT_RELOAD ON CACHE BOOL "Enable hot reload for godot-cpp")
add_subdirectory(godot-cpp)

set(LIBRARY_TARGET "${PROJECT_NAME}")

file(GLOB_RECURSE SOURCES src/*.c** src/*.h**)

add_library(${LIBRARY_TARGET} SHARED ${SOURCES})

target_include_directories(${LIBRARY_TARGET} PRIVATE
	${CMAKE_BINARY_DIR}/godot-cpp/gen/include
	godot-cpp/include
	src
)

target_link_libraries(${LIBRARY_TARGET} PRIVATE godot::cpp)

target_compile_definitions(${LIBRARY_TARGET} PRIVATE TYPED_METHOD_BIND=1 HOT_RELOAD_ENABLED=1 $<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>>)

target_compile_options(${LIBRARY_TARGET} PRIVATE
	# added here to show what warnings have to be fixed!
	$<$<CXX_COMPILER_ID:MSVC>:
		/wd4244 # C4244: disable type conversion data loss warning
		/wd4305 # C4305: disable type truncation warning
		/wd4101 # C4101: disable unreferenced local variable warning
		/wd4018 # C4018: disable signed/unsigned mismatch warning
		/wd4267 # C4267: disable conversion from 'size_t' to 'int' warning
		/wd4458 # C4458: disable declaration of hides class member
		/wd4456 # C4456: disable declaration of hides function parameter
		/wd4701 # C4701: disable local variable may be used without having been initialized
		/wd4702 # C4702: disable unreachable code warning
		/wd4189 # C4189: disable local variable is initialized but not referenced
	>
	# added here to show what warnings have to be fixed!
	$<$<CXX_COMPILER_ID:GNU>:
		-Wno-sign-compare
		-Wno-unused-variable
		-Wno-unused-but-set-variable
		-Wno-unused-but-set-parameter
		-Wno-shadow=compatible-local
	>
)

target_link_options(${LIBRARY_TARGET} PRIVATE
	$<$<CXX_COMPILER_ID:GNU>:
		-fno-rtti
	>
)

# install

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/bin")

if(CMAKE_BUILD_TYPE STREQUAL Debug)
	set(TEMPLATE "template_debug")
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
	set(TEMPLATE "template_release")
else()
	message(FATAL_ERROR "Unsupported or invalid build type: ${CMAKE_BUILD_TYPE}")
endif()

get_target_property(LIBRARY_PLATFORM godot-cpp.${TEMPLATE} GODOTCPP_PLATFORM)
get_target_property(LIBRARY_SUFFIX godot-cpp.${TEMPLATE} GODOTCPP_SUFFIX)

set_target_properties(${LIBRARY_TARGET} PROPERTIES OUTPUT_NAME "${LIBRARY_TARGET}${LIBRARY_SUFFIX}")

install(
	TARGETS ${LIBRARY_TARGET}
	DESTINATION ${LIBRARY_PLATFORM}
)

# There is one exception where we provide the pdb file
if(MSVC)
	install(FILES $<TARGET_PDB_FILE:${LIBRARY_TARGET}>
		DESTINATION ${LIBRARY_PLATFORM}
		OPTIONAL
	)
endif ()
