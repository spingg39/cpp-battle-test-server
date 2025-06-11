file(GLOB_RECURSE SOURCES_IO ../src/IO/*.cpp ../src/IO/*.hpp)
file(GLOB_RECURSE SOURCES_GAME ../src/Game/*.cpp ../src/Game/*.hpp)
file(GLOB_RECURSE SOURCES_TESTS ./*.cpp)

add_executable(unit_tests
    ${SOURCES_IO}   
    ${SOURCES_GAME}   
    ${SOURCES_TESTS}
)

target_include_directories(unit_tests PUBLIC ../src/)

target_link_libraries(unit_tests PRIVATE
    GTest::gtest_main
    GTest::gmock_main
)

include(GoogleTest)

gtest_discover_tests(unit_tests)