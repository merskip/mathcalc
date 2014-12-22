cmake_minimum_required(VERSION 2.8.4)
project(MathCalc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Exception.cpp RationalNumber.cpp Operator.cpp ParserRPN.cpp CalculatorRPN.cpp)
add_executable(MathCalc ${SOURCE_FILES})
