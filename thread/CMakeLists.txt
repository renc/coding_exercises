
cmake_minimum_required(VERSION 2.8.11)

project(thread_examples)


add_executable(ex1_hello ex1_hello.cpp)
add_executable(ex2_protectSharedData ex2_protectSharedData.cpp)
add_executable(ex3_cppRefConditionVaribale ex3_cppRefConditionVaribale.cpp)
add_executable(ex4_eventLoop ex4_eventLoop.cpp)
add_executable(ex5_stdqueue ex5_stdqueue.cpp)
add_executable(ex5_stdqueuemutex ex5_stdqueuemutex.cpp)
add_executable(ex5_spsc ex5_spsc.cpp)