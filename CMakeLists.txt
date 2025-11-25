cmake_minimum_required(VERSION 3.14)
project(BlackJack 
    VERSION 1.0.0
    DESCRIPTION "Blackjack simulation with genetic algorithm AI"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Header-only library for the core components
add_library(blackjack_core INTERFACE)
target_include_directories(blackjack_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Main executable
add_executable(blackjack main.cpp)
target_link_libraries(blackjack PRIVATE blackjack_core)

# Tests executable
add_executable(blackjack_tests tests.cpp)
target_link_libraries(blackjack_tests PRIVATE blackjack_core)

# Enable testing
enable_testing()
add_test(NAME blackjack_tests COMMAND blackjack_tests)

# Installation rules
install(TARGETS blackjack DESTINATION bin)
install(DIRECTORY core/ DESTINATION include/blackjack FILES_MATCHING PATTERN "*.h")
