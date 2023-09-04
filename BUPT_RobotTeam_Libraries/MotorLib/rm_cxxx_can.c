/**
 * @file rm_cxxx_can.c
 * @author simon
 * @brief ��C610/620���CANͨ��Э�����
 * @version 0.1
 * @date 2021-07-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "rm_cxxx_can.h"
#ifdef USE_MTR_DRIVER_RM_CXXX

DJIMotor_t DJIMotor[4];                            // �󽮵���Ĳ����ṹ�壬����ת�ǡ��ٶȵȲ�������can_robomaster_rcv_1�и�����ֵ
int Robomaster_RPMValue[4] = {50, 0, 0, 0};        // 4���󽮵�����ٶ��趨ֵ����
int Robomaster_PositionValue[4] = {1000, 0, 0, 0}; // 4���󽮵����λ�ý��趨ֵ����
int Robomaster_PositionOffset = 0;
// �󽮵�����ٶȻ���PID����
PID_s RM_Speed_PID[4] = {1.4, 0.9, 0.6, 0, 0, 5000, 0, 0.005}; // = {1.4,0.9,0.6,0,0,5000,0,0.005};
// �󽮵����λ�û���PID����
PID_s RM_Position_PID[4] = {0.13, 0, 0.082, 0, 0, 5000, 0, 0.005}; // = {0.13,0,0.082,0,0,5000,0,0.005};
int robomaster_flag = 0;
int ver_slide_error = 0;
int RM_PrintInfo_Flag = 0;
int RM_RPMControl_Flag = 0;
int RM_PosControl_Flag = 0;

/*����1�ŵ�����͵ķ���CAN��Ϣ*/
void can_robomaster_rcv_1(CAN_Message_u *pRxMsg)
{
  //pRxMsg->StdId
  static int first_flag = 1;
  if (first_flag == 1)
  {
    DJIMotor[0].now_position = (uint16_t)(pRxMsg->ui8[0] << 8 | pRxMsg->ui8[1]);
    DJIMotor[0].offset_position = DJIMotor[0].now_position;
    first_flag = 0;
    DJIMotor[0].round_cnt = 0;
    return;
  }
  DJIMotor[0].last_position = DJIMotor[0].now_position;
  DJIMotor[0].now_position = (uint16_t)(pRxMsg->ui8[0] << 8 | pRxMsg->ui8[1]);
  DJIMotor[0].now_rpm = (int16_t)(pRxMsg->ui8[2] << 8 | pRxMsg->ui8[3]);
  DJIMotor[0].now_current = (pRxMsg->ui8[4] << 8 | pRxMsg->ui8[5]) * 5.f / 16384.f;

  if (DJIMotor[0].now_position - DJIMotor[0].last_position > 4096)
    DJIMotor[0].round_cnt--;
  else if (DJIMotor[0].now_position - DJIMotor[0].last_position < -4096)
    DJIMotor[0].round_cnt++;
  DJIMotor[0].total_angle = DJIMotor[0].round_cnt * 8192 + DJIMotor[0].now_position - DJIMotor[0].offset_position;
  // if (Robomaster_PrintInfo_Flag == 1)
  // {
  //   uprintf("--robomaster[0] info:\r\n");
  //   uprintf("  angle:%d speed_rpm:%d current:%d\r\n",
  //           robomaster[0].now_position, robomaster[0].now_rpm, robomaster[0].now_current);
  // }
}

/*����2�ŵ�����͵ķ���CAN��Ϣ*/
void can_robomaster_rcv_2(CAN_Message_u *pRxMsg)
{
  //pRxMsg->StdId
  //����id��û��
  static int first_flag = 1;
  if (first_flag == 1)
  {
    DJIMotor[1].now_position = (uint16_t)(pRxMsg->ui8[0] << 8 | pRxMsg->ui8[1]);
    DJIMotor[1].offset_position = DJIMotor[1].now_position;
    first_flag = 0;
    DJIMotor[1].round_cnt = 0;
    return;
  }
  DJIMotor[1].last_position = DJIMotor[0].now_position;
  DJIMotor[1].now_position = (uint16_t)(pRxMsg->ui8[0] << 8 | pRxMsg->ui8[1]);
  DJIMotor[1].now_rpm = (int16_t)(pRxMsg->ui8[2] << 8 | pRxMsg->ui8[3]);
  DJIMotor[1].now_current = (pRxMsg->ui8[4] << 8 | pRxMsg->ui8[5]) * 5.f / 16384.f;

  if (DJIMotor[1].now_position - DJIMotor[1].last_position > 4096)
    DJIMotor[1].round_cnt--;
  else if (DJIMotor[1].now_position - DJIMotor[1].last_position < -4096)
    DJIMotor[1].round_cnt++;
  DJIMotor[1].total_angle = DJIMotor[1].round_cnt * 8192 + DJIMotor[1].now_position - DJIMotor[1].offset_position;
}

/*���ٶȻ�*/
void Robomaster_RPMControl()
{
  if (RM_RPMControl_Flag == 0)
  {
    return;
  }
  float speed_out[4];
  for (int i = 0; i < 4; i++)
  {
    speed_out[i] = robomaster_speed_pid_control(i); // �����趨�ٶȵó�Ӧ�÷��͵ĵ���ֵ
  }
  robomaster_set_current((int16_t)speed_out[0], (int16_t)speed_out[1], (int16_t)speed_out[2], (int16_t)speed_out[3]);
}

/*��λ�û�*/
void Robomaster_PositionControl()
{
  if (RM_PosControl_Flag == 0)
  {
    return;
  }
  float pos_out[4];
  for (int i = 0; i < 4; i++)
  {
    pos_out[i] = robomaster_position_pid_control(i); // �����趨�ٶȵó�Ӧ�÷��͵ĵ���ֵ
    if (pos_out[i] > 1000)                           // ���Ƶ�����С
    {
    }
  }
}

/**
 * @brief �󽮵�����ٶȻ���pid���㺯��
 * @param id Ҫ���õĵ��can id
 * @return ��ǰӦ���͸�������ٶ�ֵ
 **/
float robomaster_speed_pid_control(int id)
{
  float speed_out = 0;
  speed_out = PID_GetOutput(&RM_Speed_PID[id], Robomaster_RPMValue[id], (float)DJIMotor[id].now_rpm);
  //speed_out=PID_GetOutput(&Robomaster_Speed_PID[id],(float)robomaster[id].target_speed,(float)robomaster[id].speed_rpm);
  return speed_out;
}

/**
 * @brief �󽮵����λ�û���pid���㺯��
 * @param id Ҫ���õĵ��can id
 * @return ��ǰӦ���͸�������ٶ�ֵ
 **/
float robomaster_position_pid_control(int id)
{
  float speed_out = 0;
  float position_out = 0;
  position_out = PID_GetOutput(&RM_Position_PID[id],
                             (float)DJIMotor[id].target_position + ver_slide_error, (float)DJIMotor[id].total_angle);
  speed_out = PID_GetOutput(&RM_Speed_PID[id], position_out, (float)DJIMotor[id].now_rpm);
  return speed_out;
}

/**
 * @brief ���ô󽮵������
 * @param iq1 ����ֵ/mA
 **/
void robomaster_set_current(int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
  uint8_t Data[8];
  Data[0] = (iq1 >> 8);
  Data[1] = iq1;
  Data[2] = (iq2 >> 8);
  Data[3] = iq2;
  Data[4] = iq3 >> 8;
  Data[5] = iq3;
  Data[6] = iq4 >> 8;
  Data[7] = iq4;
  CAN_Message_u dji_data;
  for (int i = 0; i <= 7; i++)
  {
    dji_data.ui8[i] = Data[i];
  }
  CAN_SendMsg(0x200, &dji_data); // ǰ4�������can_idΪ0x200
}

/**
 * @brief ��ӡ�󽮵����Ϣ
 * @param index ������
 *        @arg 0 1 2 3
 **/
void Robomaster_PrintInfo(int index)
{
  if (RM_PrintInfo_Flag == 1)
  {
    // uprintf("--robomaster[%d] info:\r\n", index);
    uprintf("  now_angle:%d total_angle:%d speed_rpm:%d current:%d\r\n",
            (uint32_t)DJIMotor[index].now_position * 360 / 8192, (uint32_t)DJIMotor[index].total_angle * 360 / 8192,
            DJIMotor[index].now_rpm, DJIMotor[index].now_current);
  }
}

uint32_t Robomaster_OpenAngleControl_Flag = 0; // CMD�����Ƿ�����ת��ֹͣ����1����
uint32_t Robomaster_TargetOffsetAngle = 0;     // CMDָ��Ŀ��ת�Ǳ仯������λ�ȣ����ǻ��ȣ�
static uint32_t now_angle = 0;
static uint32_t robomaster_origin_angle = 0;
static int first_flag = 0;
/**
 * @brief ���ƴ󽮵��ת��һ���ǶȺ�ֹͣ,��Ҫ��m2006_exm2006_Exe
 *        Ŀ��ת�Ǳ仯����ͨ��CMDָ����ȫ�ֱ���
 * @param index �󽮵�����
 *    @arg 0 1 2 3
 * @return none
 **/
void Robomaster_StopByAngle(int index)
{
  if (Robomaster_OpenAngleControl_Flag == 0)
  {
    return;
  }

  if (first_flag == 0) // ��¼��ʼ�Ƕ�ֵ
  {
    robomaster_origin_angle = (uint32_t)DJIMotor[index].total_angle * 360 / 8192;
    first_flag = 1;
  }
  else
  {
    now_angle = (uint32_t)DJIMotor[index].total_angle * 360 / 8192;
    int offset_position = now_angle - robomaster_origin_angle; // ����������ٶ��Ƕ�ֵ�������������
    if (offset_position >= Robomaster_TargetOffsetAngle)
    {
      robomaster_set_current(0, 0, 0, 0); // ����ִ��
      DJIMotor[index].target_current = 0; // ��mm2006_Exe)�е�ִ�е�����0
      /*TODO:���ڿ��԰��������иĳɷ������������ֹͣ�ź�*/
      robomaster_origin_angle = 0;
      offset_position = 0;
      first_flag = 0;
      Robomaster_OpenAngleControl_Flag = 0; // �������ʹ�ã���ͨ��CMD�ٴ���1
    }
  }
}

void M2006_init(int id)
{
  PID_Reset(&RM_Speed_PID[id]);
  PID_Reset(&RM_Position_PID[id]);
  RM_Speed_PID[id].Kp = 1.4;
  RM_Speed_PID[id].Ki = 0.9;
  RM_Speed_PID[id].Kd = 0.6;
  RM_Speed_PID[id].int_max = 5000;
  RM_Speed_PID[id].int_duty = 0.005;

  RM_Position_PID[id].Kp = 0.13;
  RM_Position_PID[id].Ki = 0;
  RM_Position_PID[id].Kd = 0.082;
  RM_Position_PID[id].int_max = 5000;
  RM_Position_PID[id].int_duty = 0.005;
}

void M3508_init(int id)
{
  PID_Reset(&RM_Speed_PID[id]);
  PID_Reset(&RM_Position_PID[id]);
  RM_Speed_PID[id].Kp = 1.6;
  RM_Speed_PID[id].Ki = 0.52;
  RM_Speed_PID[id].Kd = 0.6;
  RM_Speed_PID[id].int_max = 5000;
  RM_Speed_PID[id].int_duty = 0.005;

  RM_Position_PID[id].Kp = 0.1;
  RM_Position_PID[id].Ki = 0;
  RM_Position_PID[id].Kd = 0.8;
  RM_Position_PID[id].int_max = 5000;
  RM_Position_PID[id].int_duty = 0.005;
}

#endif