#ifndef DJI_DRIVER_BOARD_MOTOR_MATH_H
#define DJI_DRIVER_BOARD_MOTOR_MATH_H
#ifdef __cplusplus
extern "C"
{
#endif


#define PI 180.0f
#define UTILS_LP_FAST(value, sample, filter_constant)   (value -= (filter_constant) * ((value) - (sample)))

float LimitPos_f(float input, float re_ratio);


#ifdef __cplusplus
}
#endif
#endif
