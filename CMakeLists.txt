cmake_minimum_required(VERSION 2.8.4)
project(MathCalc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

configure_file(version.cmake.in version.cmake @ONLY)
add_custom_command(
    OUTPUT app_version
    COMMAND ${CMAKE_COMMAND}
    ARGS    -P version.cmake
            -D GIT_VERSION="${GIT_VERSION}"
            -D BUILD_DATE="${BUILD_DATE}"
    COMMENT "Generating version"
)
set_property(SOURCE Version.hpp.in PROPERTY OBJECT_DEPENDS)
set_source_files_properties(Version.hpp PROPERTIES GENERATED TRUE)

set(SOURCE_FILES
    main.cpp
    Exception.cpp
    RationalNumber.cpp
    Operator.cpp
    ParserRPN.cpp
    CalculatorRPN.cpp
    LatexMath.cpp
    Version.hpp.in)

include_directories("${PROJECT_BINARY_DIR}")
add_executable(MathCalc ${SOURCE_FILES} app_version)
add_dependencies(MathCalc Version.hpp.in)