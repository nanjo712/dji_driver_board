cmake_minimum_required(VERSION 3.24)


set(
        User_Motor_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/M_2006.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/RM_3508.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/M_2006.h
        ${CMAKE_CURRENT_SOURCE_DIR}/RM_3508.h
        ${CMAKE_CURRENT_SOURCE_DIR}/user_motor.h
        ${CMAKE_CURRENT_SOURCE_DIR}/command.h
        ${CMAKE_CURRENT_SOURCE_DIR}/command.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/motor_config.h
)

set_source_files_properties(${User_Motor_SRC} PROPERTIES LANGUAGE CXX)
set(User_Motor_CXX ${User_Motor_SRC} PARENT_SCOPE)
set(
        User_Motor_Inc PARENT_SCOPE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(
        ${User_Motor_Inc}
)