# CMakeList.txt : CMake project for CanvasPainter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
# Add source to this project's executable.
add_executable (CanvasPainter "canvas_accessor.cpp" "canvas_accessor.hpp" "point.hpp" "shapes/shape.hpp" "canvas_descriptor.hpp" "counted_object.hpp"  "shapes/rectangle.hpp" "shapes/circle.hpp" "shapes/triangle.hpp" "canvas.hpp" "shapes/shape_factory.hpp" "exceptions/collision_exception.hpp" "point_collection.hpp")



enable_testing()

add_executable(
  CanvasPainterTests
  "Tests/CanvasPainterTests.cpp"
  "gtest/include/gtest/gtest.h"
)

target_include_directories(CanvasPainterTests PRIVATE "gtest/include")
find_library(gtest_main_lib gtest_maind HINTS gtest/lib)
find_library(gtest_lib gtestd HINTS gtest/lib)
find_library(gmock_main_lib gmock_maind HINTS gtest/lib)
find_library(gmock_lib gmockd HINTS gtest/lib)
target_link_libraries(
  CanvasPainterTests
  ${gtest_lib}
)
target_link_libraries(
  CanvasPainterTests
  ${gtest_main_lib}
)

target_link_libraries(
  CanvasPainterTests
  ${gmock_lib}
)
target_link_libraries(
  CanvasPainterTests
  ${gmock_main_lib}
)