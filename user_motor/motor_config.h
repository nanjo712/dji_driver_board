#ifndef DJI_DRIVER_BOARD_MOTOR_CONFIG_H
#define DJI_DRIVER_BOARD_MOTOR_CONFIG_H
#if GM6020_USED
    const int RM_RECV_BASE = 0x205; //收报基准ID
    const int RM_SEND_BASE = 0x1ff; //发报基准ID
#else
const int RM_RECV_BASE = 0x201; //收报基准ID
const int RM_SEND_BASE = 0x200; //发报基准ID
#endif

const float VOLTAGE_MAX_6020 = 30.0f;

//GM_6020常量
const float VEL_MAX_6020 = 10000.0f;
const float VEL_KP_6020 = 0.08f;
const float VEL_KI_6020 = 20.0f;
const float VEL_KD_6020 = 0.0f;
const float POS_KP_6020 = 0.80f;
const float POS_KI_6020 = 0.0f;
const float POS_KD_6020 = 0.00f;
const float CUR_KP_6020 = 0.0f;
const float CUR_KI_6020 = 0.0f;
const float CUR_KD_6020 = 0.0f;
#endif
