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
set_property(SOURCE include/Version.hpp.in PROPERTY OBJECT_DEPENDS)
set_source_files_properties(Version.hpp PROPERTIES GENERATED TRUE)

set(SOURCE_FILES
    src/main.cpp
    src/Exception.cpp
    src/RationalNumber.cpp
    src/Operator.cpp
    src/ParserRPN.cpp
    src/CalculatorRPN.cpp
    src/LatexMath.cpp)

include_directories("${PROJECT_BINARY_DIR}/include" include)
add_executable(MathCalc ${SOURCE_FILES} app_version)
add_dependencies(MathCalc include/Version.hpp.in)

add_custom_target(clion_fix_headers SOURCES
    include/CalculatorRPN.hpp
    include/Exception.hpp
    include/LatexMath.hpp
    include/Operator.hpp
    include/ParserRPN.hpp
    include/RationalNumber.hpp
    include/Token.hpp
    include/Version.hpp
    include/Version.hpp.in
)
