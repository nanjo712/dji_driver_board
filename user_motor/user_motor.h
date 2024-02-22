#ifndef DJI_DRIVER_BOARD_USER_MOTOR_H
#define DJI_DRIVER_BOARD_USER_MOTOR_H


#include "RM_3508.h"
#include "M_2006.h"

template<typename First, typename ... Rest>
constexpr std::common_type_t<First, Rest...> mymax(const First& first, const Rest...rest) {
    if constexpr (sizeof...(rest) == 0) {
        return first;
    }
    else {
        std::common_type_t<First, Rest...> x = mymax(rest...);
        return first > x ? first : x;
    }
}

template<typename ...MotorTypes>
constexpr size_t Get_Size() {
    return mymax(sizeof(MotorTypes)...);
}

const size_t Max_Size = Get_Size<Motor_M_2006,Motor_RM_3508>();

static char c_buffer[Max_Size * Motor::COUNT] = { 0 };				// 对象的存储空间
static char c_map[Motor::COUNT] = { 0 };					// 每个存储对象位置是否空间，0表示空间可用

#endif
