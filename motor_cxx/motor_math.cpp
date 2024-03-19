#include "motor_math.h"

float LimitPos_f(float input, float re_ratio)
{
    while(input > PI / re_ratio)
        input -= 2 * PI / re_ratio;
    while(input < -PI / re_ratio)
        input += 2 * PI / re_ratio;
    return input;
}