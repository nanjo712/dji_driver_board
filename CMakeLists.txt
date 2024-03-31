#THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.25)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(dji_driver_board C CXX ASM)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

#Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

include_directories(
        Core/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        Middlewares/Third_Party/FreeRTOS/Source/include
        Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
        Drivers/CMSIS/Include
)

add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx)

#set(
#        CXX_Build_C_Sources
#        Core/Src/freertos.c
#        Core/Src/main.c
#)
#set_source_files_properties(${CXX_Build_C_Sources} PROPERTIES LANGUAGE CXX)

set(
        Core_INC
        Core/Inc/can.h
        Core/Inc/config.h
        Core/Inc/dma.h
        Core/Inc/FreeRTOSConfig.h
        Core/Inc/gpio.h
        Core/Inc/main.h
        Core/Inc/stm32f4xx_hal_conf.h
        Core/Inc/stm32f4xx_it.h
        Core/Inc/tim.h
        Core/Inc/usart.h
        Core/Inc/toolBoxScope.h
        Core/Inc/motorflash.h
        Core/Inc/flashwear.h
        Core/Inc/heart.h
        BUPT_RobotTeam_Libraries/SimpleLib/utils/utils.h
)
set(
        Core_SRC
        Core/Src/can.c
        Core/Src/dma.c
        Core/Src/freertos.c
        Core/Src/gpio.c
        Core/Src/main.c
        Core/Src/stm32f4xx_hal_msp.c
        Core/Src/stm32f4xx_hal_timebase_tim.c
        Core/Src/stm32f4xx_it.c
        Core/Src/syscalls.c
        Core/Src/sysmem.c
        Core/Src/system_stm32f4xx.c
        Core/Src/tim.c
        Core/Src/toolBoxScope.c
        Core/Src/usart.c
        Core/Src/motorflash.c
        Core/Src/flashwear.c
        Core/Src/heart.c
        BUPT_RobotTeam_Libraries/SimpleLib/utils/utils.c
        Core/Startup/startup_stm32f405rgtx.s
)

file(GLOB_RECURSE RTOS "Middlewares/*.*")
file(GLOB_RECURSE HAL_Driver "Drivers/STM32F4xx_HAL_Driver/*.*")

add_definitions("-DUSE_OSLIB")
add_subdirectory(OSLib)
message(DEBUG "OSLib include these files: ${OSLib}")
if(OSLib)
    list(APPEND UserImport ${OSLib})
    include_directories(${OSLib_IncDir})
endif()

add_subdirectory(BUPT_RobotTeam_Libraries)
message(DEBUG "BUPT_RobotTeam_Libraries include these files: ${BUPTLib}")
if (BUPTLib)
    list(APPEND UserImport ${BUPTLib})
    include_directories(${BUPTLib_Inc})
endif ()

add_subdirectory(motor_cxx)
if (Motor_CXX)
    message(WARNING "motor_cxx include these files: ${Motor_CXX}")
    list(APPEND UserImport ${Motor_CXX})
    message(DEBUG "motor_cxx include these files: ${Motor_Inc}")
    include_directories(${Motor_Inc})
endif ()

add_subdirectory(user_motor)
if (User_Motor_CXX)
    message(WARNING "user_motor include these files: ${User_Motor_CXX}")
    list(APPEND UserImport ${User_Motor_CXX})
    message(DEBUG "motor_cxx include these files: ${User_Motor_Inc}")
    include_directories(${User_Motor_Inc})
endif ()

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F405RGTX_FLASH.ld )

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

#add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
#target_link_libraries(${PROJECT_NAME}.elf OSLIB)
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${Core_INC} ${Core_SRC} ${RTOS} ${HAL_Driver} ${LINKER_SCRIPT} ${UserImport} ${UserCode})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")