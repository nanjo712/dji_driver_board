#include "VESC.h"
#include "../motor_cxx/motor_math.h"

#define re_ratio Reduction_Ratio_VESC

const int RM_RECV_BASE = 0x01; //收报基准ID
const int RM_SEND_BASE = 0x4D; //发报基准ID

typedef enum {
    CAN_PACKET_SET_DUTY = 0,
    CAN_PACKET_SET_CURRENT,
    CAN_PACKET_SET_CURRENT_BRAKE,
    CAN_PACKET_SET_RPM,
    CAN_PACKET_SET_POS,
    CAN_PACKET_SET_CURRENT_REL = 10,
    CAN_PACKET_SET_CURRENT_BRAKE_REL,
    CAN_PACKET_SET_CURRENT_HANDBRAKE,
    CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
    CAN_PACKET_MAKE_ENUM_32_BITS = 0xFFFFFFFF,
} CAN_PACKET_ID;

uint8_t change_ctrl_mode(MotorCtrlMode_Def CtrlType){
    switch (CtrlType) {
        case CUR_Mode:
            return CAN_PACKET_SET_CURRENT;
        case BRAKE_Mode:
            return CAN_PACKET_SET_CURRENT_BRAKE;
        case POS_Mode:
            return CAN_PACKET_SET_POS;
        case Multi_POS_Mode:
            return CAN_PACKET_SET_POS;
        case VEL_Mode:
            return CAN_PACKET_SET_RPM;
        default:
            return  CAN_PACKET_MAKE_ENUM_32_BITS;
    }
}

//VESC的函数
void Motor_VESC::Init(int num)
{
    MotorType = VESC;
    On = false;
    Ctrl_Reset();
    can_ID.get_ide = CAN_ID_EXT;
    can_ID.get_ide = CAN_ID_EXT;
    can_ID.send_Id = RM_SEND_BASE;
    can_ID.get_Id = RM_RECV_BASE + num;
}

void Motor_VESC::Write_CtrlMode(MotorCtrlMode_Def CtrlMode) {
    MotorCtrlMode = CtrlMode;
    Ctrl_Reset();
    if(CtrlMode == Multi_POS_Mode || CtrlMode == POS_Mode)
        PosUsed_Flag = 1;
}

void Motor_VESC::Ctrl_Reset() {
    MotorPID.Vel_PID = (PID_s){VEL_KP_VESC,VEL_KI_VESC,VEL_KD_VESC,0,2000,CURRENT_MAX_VESC,0,0,0,0,0};
    MotorPID.Pos_PID = (PID_s){POS_KP_VESC,POS_KI_VESC,POS_KD_VESC,0,0,MotorPID.Pos_PID.ctrl_max,0,0,0,0,0};
    MotorPID.Cur_PID = (PID_s){CUR_KP_VESC,CUR_KI_VESC,CUR_KD_VESC,0,0,CURRENT_MAX_VESC,0,0,0,0,0};
    MotorState = MotorState_Def{0,0,0,0,0};
    Final_OutPut = 0;
    PosUsed_Flag = 0;
}

void Motor_VESC::Motor_MessageCreate(int num) {
    int msg_num;
    msg_num=get_Id_address(can_ID.send_Id);
    if(msg_num== 9)
        msg_num = add_Id_address(can_ID.send_Id);

    send_Msg[msg_num].ide = can_ID.send_ide;
}

void Motor_VESC::Data_Receive(CAN_ConnMessage msg) {
    int32_t temp_content;
    float temp_pos;
    float del_pos;

    temp_content = (int16_t)((msg.payload.ui8[0] << 8) + msg.payload.ui8[1]);
    temp_pos = (float)temp_content * 360.0f / 8196.0f / re_ratio;

    temp_content = (int16_t)((msg.payload.ui8[2] << 8) + msg.payload.ui8[3]);
    MotorState.Vel_Now = (float)temp_content / re_ratio;

    temp_content = (int16_t)((msg.payload.ui8[4] << 8) + msg.payload.ui8[5]);
    MotorState.Cur_Now = (float)temp_content * 10.0f / CURRENT_MAX_VESC;


    if(PosUsed_Flag)
    {
        PosUsed_Flag = 0;
        MotorState.Pos_Begin = temp_pos;
        MotorState.Pos_Last = temp_pos;
    }

    del_pos = temp_pos - MotorState.Pos_Last;
    MotorState.Pos_Last = temp_pos;
    MotorState.Pos_Now += LimitPos_f(del_pos,re_ratio);
}

void buffer_append_int16(uint8_t* buffer, int16_t number, int32_t *index) {
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_int32(uint8_t* buffer, int32_t number, int32_t *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}

void buffer_append_float16(uint8_t* buffer, float number, float scale, int32_t *index) {
    buffer_append_int16(buffer, (int16_t)(number * scale), index);
}

void buffer_append_float32(uint8_t* buffer, float number, float scale, int32_t *index) {
    buffer_append_int32(buffer, (int32_t)(number * scale), index);
}

void comm_can_set_duty(uint8_t controller_id, float duty, uint8_t* buffer) {
    int32_t send_index = 0;
    buffer_append_int32(buffer, (int32_t)(duty * 100000.0), &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_DUTY << 8), buffer, send_index);
}

void comm_can_set_current(uint8_t controller_id, float current, uint8_t* buffer) {
    int32_t send_index = 0;
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index);
}

void comm_can_set_current_off_delay(uint8_t controller_id, float current, float off_delay) {
    int32_t send_index = 0;
    uint8_t buffer[6];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    buffer_append_float16(buffer, off_delay, 1e3, &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT << 8), buffer, send_index);
}

void comm_can_set_current_brake(uint8_t controller_id, float current) {
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0), &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8), buffer, send_index);
}

void comm_can_set_rpm(uint8_t controller_id, float rpm) {
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)rpm, &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_RPM << 8), buffer, send_index);
}

void comm_can_set_pos(uint8_t controller_id, float pos) {
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_int32(buffer, (int32_t)(pos * 1000000.0), &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_POS << 8), buffer, send_index);
}

void comm_can_set_current_rel(uint8_t controller_id, float current_rel) {
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT_REL << 8), buffer, send_index);
}

/**
 * Same as above, but also sets the off delay. Note that this command uses 6 bytes now. The off delay is useful to set to keep the current controller running for a while even after setting currents below the minimum current.
 */
void comm_can_set_current_rel_off_delay(uint8_t controller_id, float current_rel, float off_delay) {
    int32_t send_index = 0;
    uint8_t buffer[6];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    buffer_append_float16(buffer, off_delay, 1e3, &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT_REL << 8), buffer, send_index);
}

void comm_can_set_current_brake_rel(uint8_t controller_id, float current_rel) {
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE_REL << 8), buffer, send_index);
}

void comm_can_set_handbrake(uint8_t controller_id, float current) {
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current, 1e3, &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8), buffer, send_index);
}

void comm_can_set_handbrake_rel(uint8_t controller_id, float current_rel) {
    int32_t send_index = 0;
    uint8_t buffer[4];
    buffer_append_float32(buffer, current_rel, 1e5, &send_index);
    can_transmit_eid(controller_id |
                     ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE_REL << 8), buffer, send_index);
}