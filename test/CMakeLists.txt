cmake_minimum_required(VERSION 3.16)
project(test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include (CTest)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(TEST ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(CMAKE_BUILD_TYPE Debug)

add_library(message 	SHARED ${SRC}/formats/message.cpp)
add_library(TCP_encoder SHARED ${SRC}/encoders/TCP_encoder.cpp)
add_library(TCP_decoder SHARED ${SRC}/decoders/TCP_decoder.cpp)
add_library(Sender SHARED ${SRC}/senders/sender.cpp)
add_library(TCP_sender SHARED ${SRC}/senders/TCP_sender.cpp)
enable_testing()

add_executable(message_test_0 message/test0.cpp)
add_executable(tcp_encode_test_0 encoders/test0.cpp)
add_executable(tcp_sender_test_0 senders/test0.cpp)

target_link_libraries(message_test_0 message)
target_link_libraries(tcp_encode_test_0 message TCP_encoder)
target_link_libraries(TCP_sender Sender)
target_link_libraries(tcp_sender_test_0 message TCP_sender)

add_test(NAME test_message_basic_0 COMMAND message_test_0 "Jakub" "Hello world")
set_property(TEST test_message_basic_0 PROPERTY PASS_REGULAR_EXPRESSION "Jakub\\[4\\]: Hello world")

add_test(NAME test_message_basic_1 COMMAND message_test_0 "verylongusername" "Hello world")
set_property(TEST test_message_basic_1 PROPERTY PASS_REGULAR_EXPRESSION "verylongusername\\[4\\]: Hello world")

add_test(NAME test_message_basic_2 COMMAND message_test_0 "even longer username!" "Hello world")
set_tests_properties(test_message_basic_2 PROPERTIES PASS_REGULAR_EXPRESSION "Author too long.*")

add_test(NAME test_tcp_enc_basic_0 COMMAND tcp_encode_test_0 4 "Jakub" "Hello world")
set_tests_properties(test_tcp_enc_basic_0 PROPERTIES PASS_REGULAR_EXPRESSION "MSG FROM Jakub IS Hello world")