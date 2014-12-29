cmake_minimum_required(VERSION 2.8.4)
project(MathCalc)

exec_program(
    "git"
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS "describe --tags"
    OUTPUT_VARIABLE APP_VERSION )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(-DAPP_VERSION="${APP_VERSION}")

set(SOURCE_FILES main.cpp Exception.cpp RationalNumber.cpp Operator.cpp ParserRPN.cpp CalculatorRPN.cpp)
add_executable(MathCalc ${SOURCE_FILES})
