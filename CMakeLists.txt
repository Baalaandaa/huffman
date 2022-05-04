enable_testing()
find_package(GTest REQUIRED)

SET(SRC BitInput.cpp BitOutput.cpp HuffmanTree.cpp Archiver.cpp)

file(GLOB TESTS_SRC
        "tests/*.cpp")

message(STATUS src ${SRC})
message(STATUS tests ${TESTS_SRC})

add_executable(archiver main.cpp ${SRC})

add_executable(archiver_tests ${SRC} tests/test.cpp)
target_link_libraries(archiver_tests GTest::GTest GTest::Main)

add_test(AllArchiverTests archiver_tests)
