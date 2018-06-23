#!/bin/bash

# Test helper script for AnyOption
#
# Requires: cppcheck valgrind lcov 
#
# Kishan Thomas <kishan.thomas@gmail.com>

cppcheck --error-exitcode=1 --enable=warning,performance,information,style *.cpp *.h
wget -q https://github.com/catchorg/Catch2/releases/download/v2.2.3/catch.hpp
g++ -g -o0 -coverage -std=c++11 -Wall -Wextra -Werror test.cpp anyoption.cpp -o test
valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all ./test
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info 'catch.hpp' '/usr/*' --output-file coverage.info
lcov --list coverage.info
# clean
rm test
rm test.options
rm catch.hpp
rm coverage.info
rm *.gcda *.gcno
