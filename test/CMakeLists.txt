cmake_minimum_required(VERSION 3.16)
project(test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include (CTest)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(TEST ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)
# link_directories(${CMAKE_SOURCE_DIR}/lib)

add_library(config 				SHARED ${SRC}/config.cpp)
add_library(message 			SHARED ${SRC}/formats/message.cpp)
add_library(edge	 			SHARED ${SRC}/fsm/edge.cpp)
add_library(node	 			SHARED ${SRC}/fsm/node.cpp)
add_library(fsm	 				SHARED ${SRC}/fsm/fsm.cpp)
add_library(TCP_fsm	 			SHARED ${SRC}/fsm/TCP_fsm.cpp)
add_library(IO_handler	 		SHARED ${SRC}/io_handler/io_handler.cpp)

add_library(TCP_encoder SHARED ${SRC}/encoders/TCP_encoder.cpp)
add_library(TCP_decoder SHARED ${SRC}/decoders/TCP_decoder.cpp)
add_library(UDP_encoder SHARED ${SRC}/encoders/UDP_encoder.cpp)
add_library(UDP_decoder SHARED ${SRC}/decoders/UDP_decoder.cpp)

add_library(Sender SHARED ${SRC}/senders/sender.cpp)
add_library(Receiver SHARED ${SRC}/receivers/receiver.cpp)
add_library(TCP_receiver SHARED ${SRC}/receivers/TCP_receiver.cpp)
add_library(TCP_sender SHARED ${SRC}/senders/TCP_sender.cpp)

add_library(transceiver SHARED ${SRC}/transceivers/transceiver.cpp)
add_library(TCP_transceiver SHARED ${SRC}/transceivers/TCP_transceiver.cpp)

enable_testing()

add_executable(message_test_0 message/test0.cpp)
add_executable(tcp_encode_test_0 encoders/test0.cpp)
add_executable(udp_encode_test_0 decoders/test0.cpp)
add_executable(tcp_sender_test_0 senders/test0.cpp)
add_executable(tcp_trans_test_0 transceivers/test0.cpp)
add_executable(argparse_test_0 argparse/test0.cpp)
add_executable(iohandler_test_0 misc/test0_io_handler.cpp)
add_executable(polling_test_0 misc/test1_polling.cpp)
add_executable(polling_test_1 misc/test2_fsmpoll.cpp)


target_link_libraries(Sender config)
target_link_libraries(Receiver config)
target_link_libraries(IO_handler message)

target_link_libraries(TCP_sender Sender config)
target_link_libraries(TCP_receiver Receiver config)

# target_link_libraries(TCP_receiver Receiver)
target_link_libraries(TCP_transceiver TCP_sender TCP_receiver transceiver config)
target_link_libraries(TCP_fsm LINK_PUBLIC node edge TCP_transceiver fsm TCP_decoder TCP_encoder IO_handler TCP_sender TCP_receiver transceiver config)

target_link_libraries(message_test_0 message config)
target_link_libraries(tcp_encode_test_0 message TCP_encoder config)
target_link_libraries(tcp_sender_test_0 message TCP_sender config)
target_link_libraries(udp_encode_test_0 message UDP_decoder config)
target_link_libraries(tcp_trans_test_0 message TCP_sender transceiver TCP_transceiver)
target_link_libraries(argparse_test_0 config)
target_link_libraries(iohandler_test_0 config IO_handler message TCP_encoder)
target_link_libraries(polling_test_1 config IO_handler message TCP_encoder TCP_fsm)

add_test(NAME test_message_basic_0 COMMAND message_test_0 "Jakub" "Hello world")
set_property(TEST test_message_basic_0 PROPERTY PASS_REGULAR_EXPRESSION "Jakub\\[4\\]: Hello world")

add_test(NAME test_message_basic_1 COMMAND message_test_0 "verylongusername" "Hello world")
set_property(TEST test_message_basic_1 PROPERTY PASS_REGULAR_EXPRESSION "verylongusername\\[4\\]: Hello world")

add_test(NAME test_message_basic_2 COMMAND message_test_0 "even longer username!" "Hello world")
set_tests_properties(test_message_basic_2 PROPERTIES PASS_REGULAR_EXPRESSION "Author too long.*")

add_test(NAME test_tcp_enc_basic_0 COMMAND tcp_encode_test_0 4 "Jakub" "Hello world")
set_tests_properties(test_tcp_enc_basic_0 PROPERTIES PASS_REGULAR_EXPRESSION "MSG FROM Jakub IS Hello world")