cmake_minimum_required(VERSION 3.24)


set(
        Motor_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/ctrl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/motor_class.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/motor_class.h
        ${CMAKE_CURRENT_SOURCE_DIR}/message.h
        ${CMAKE_CURRENT_SOURCE_DIR}/motor_cxx.h
        ${CMAKE_CURRENT_SOURCE_DIR}/message.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/motor_math.h
        ${CMAKE_CURRENT_SOURCE_DIR}/motor_math.cpp
)
set_source_files_properties(${Motor_SRC} PROPERTIES LANGUAGE CXX)
set(Motor_CXX ${Motor_SRC} PARENT_SCOPE)
set(
        Motor_Inc PARENT_SCOPE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(
        ${Motor_Inc}
)