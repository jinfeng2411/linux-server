cmake_minimum_required(VERSION 3.5)

project(jinfeng)

set(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(src)
include_directories(src/http)

aux_source_directory(src SOURCE)

find_library(PTHREAD pthread)

add_executable(test_log test/test_log.cc  ${SOURCE})
add_executable(test_thread test/test_thread.cc ${SOURCE})
add_executable(test_fiber test/test_fiber.cc ${SOURCE})
add_executable(test_schedule test/test_schedule.cc ${SOURCE})
add_executable(test_timer test/test_timer.cc ${SOURCE})
add_executable(test_iomanager test/test_iomanager.cc ${SOURCE})
add_executable(test_address test/test_address.cc ${SOURCE})
add_executable(test_socket_serv test/test_socket_serv.cc ${SOURCE})
add_executable(test_socket_cli test/test_socket_cli.cc ${SOURCE})
add_executable(test_tcp_server test/test_tcp_server.cc ${SOURCE})
#add_executable(test_http test/test_http.cc src/http/http.cc ${SOURCE})
add_executable(test_bytearray test/test_bytearray.cc ${SOURCE})

target_link_libraries(test_log ${PTHREAD})
target_link_libraries(test_fiber ${PTHREAD})
target_link_libraries(test_thread ${PTHREAD})
target_link_libraries(test_schedule ${PTHREAD})
target_link_libraries(test_timer ${PTHREAD})
target_link_libraries(test_iomanager ${PTHREAD})
target_link_libraries(test_address ${PTHREAD})
target_link_libraries(test_socket_serv ${PTHREAD})
target_link_libraries(test_socket_cli ${PTHREAD})
target_link_libraries(test_tcp_server ${PTHREAD})
#target_link_libraries(test_http ${PTHREAD})
target_link_libraries(test_bytearray ${PTHREAD})
