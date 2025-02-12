cmake_minimum_required(VERSION 3.12)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_executable(BlockDemo 
    ${SOURCES}
    ${HEADERS}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


target_link_libraries(BlockDemo PRIVATE FCT)

target_include_directories(BlockDemo PRIVATE 
    ${CMAKE_SOURCE_DIR}/FCT 
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET BlockDemo PROPERTY CXX_STANDARD 14)
endif()