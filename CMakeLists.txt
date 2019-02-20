cmake_minimum_required (VERSION 3.1)
project(safe)

set(CMAKE_CXX_STANDARD 11)

find_package(Doxygen)
if (${DOXYGEN_FOUND})
	set(DOXYFILE_TO_CONFIGURE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(DOXYFILE ${CMAKE_BINARY_DIR}/Doxyfile)

	configure_file(${DOXYFILE_TO_CONFIGURE} ${DOXYFILE} @ONLY)
	
	add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE})
endif(${DOXYGEN_FOUND})

find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

include_directories(include)

add_executable(safe_tests tests/tests.cpp tests/test_safe.cpp)
target_link_libraries(safe_tests GTest::GTest ${CMAKE_THREAD_LIBS_INIT})