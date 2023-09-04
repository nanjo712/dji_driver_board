#include "motorTask.h"
#include "command.h"
#include "ctrl.h"
#include "oslib.h"
#include "config.h"

/**
 * @brief can1接收消息线程
 *        用来接受大疆电机报文消息
 * @param argument
 */

extern osMutexId_t motorsMutexHandle;

int firstFlag[4];

void can1ReceiveFunc(void *argument)
{
    for (int i = 0; i < 4; i++)
    {
        firstFlag[i] = 1;
    }
    DriverInit();
    for (;;)
    {
        static CAN_ConnMessage msg;
        osMessageQueueGet(can1ReceiveQueueHandle, &msg, NULL, osWaitForever);
        /* user functional code start */

        // uprintf("CAN1: Received [%x]\r\n", msg.id);
        uint8_t n = msg.id - RM_RECV_BASE;
        if (n >= 4)
            return;
        osMutexAcquire(motorsMutexHandle, osWaitForever);
        motors[n].pos = (msg.payload.ui8[0] << 8) + msg.payload.ui8[1];
        motors[n].vel = (msg.payload.ui8[2] << 8) + msg.payload.ui8[3];
        motors[n].cur = (int16_t)(msg.payload.ui8[4] << 8) + msg.payload.ui8[5];
        motors[n].temp = msg.payload.ui8[6];
        osMutexRelease(motorsMutexHandle);
        if (firstFlag[n])
        {
            firstFlag[n] = 0;
            drivers[n].posCtrl.actulPos = 0;
            motors[n].posLast = motors[n].pos;
        }
        if (n == 0)
            MotorCtrl();
        /* user functional code end */
    }
}

/**
 * @brief can2接受消息线程
 *        用来接受主控控制消息
 * @param argument
 */
void can2ReceiveFunc(void *argument)
{
    for (;;)
    {
        static CAN_ConnMessage msg;
        osMessageQueueGet(can2ReceiveQueueHandle, &msg, NULL, osWaitForever);
        /* user functional code start */
        // uprintf("CAN2: Received [%x]\r\n", msg.id);
        switch (msg.id)
        {
        case 0x200 + BOARDID:
            can2Handle(&(msg.payload));
            break;
        case 0x204 + BOARDID: // 控制四个电机速度环
            VelCtrlAll(&(msg.payload));
            break;
        case 0x208 + BOARDID: // 控制四个电机位置环
            PosCtrlAll(&(msg.payload));
            break;
        default:
            break;
        }

        /* user functional code end */
    }
}

/**
 * @brief can1发送函数线程
 *        用来发送电机控制信息
 * @param argument
 */
void can1SendFunc(void *argument)
{
    osThreadExit();
    for (;;)
    {

        osSemaphoreAcquire(can1sendSemaHandle, osWaitForever);
        /* user functional code start */

        /* user functional code end */
    }
}

/**
 * @brief 发送控制电流报文
 *
 * @param cur
 */
void SetCur(float *cur)
{

    can_msg msg;
    int16_t data[4] = {0};

    int canId = RM_SEND_BASE;
    for (int i = 0; i < 4; i++)
        data[i] = (int16_t)cur[i];

    msg.ui8[0] = (data[0] >> 8) & 0xff;
    msg.ui8[1] = data[0] & 0xff;
    msg.ui8[2] = (data[1] >> 8) & 0xff;
    msg.ui8[3] = data[1] & 0xff;
    msg.ui8[4] = (data[2] >> 8) & 0xff;
    msg.ui8[5] = data[2] & 0xff;
    msg.ui8[6] = (data[3] >> 8) & 0xff;
    msg.ui8[7] = data[3] & 0xff;
    OSLIB_CAN_SendMessage(&hcan1, CAN_ID_STD, canId, &msg);
}

void can2Handle(CAN_Message *msg)
{
    uint16_t motorId = msg->ui16[0];
    uint16_t option = msg->ui16[1];
    int32_t num = msg->in[1];
    int16_t vel = msg->i16[2];
    float cur = ((float)msg->i16[3]) / 1000.0;
    if (motorId < 1 || motorId > 4)
        return;
    motorId -= 1;
    switch (option)
    {
    case MOTORON:
        MotorOn(motorId);
        break;
    case MOTOROFF:
        MotorOff(motorId);
        break;
    case VELCFG:
        VelLoopCfg(motorId);
        break;
    case POSCFG:
        PosLoopCfg(motorId, num);
        break;
    case CURCFG:
        CurLoopCfg(motorId);
        break;
    case VELCTRL:
        SetVelCtrl(motorId, num);
        break;
    case POSCTRL:
        SetPosCtrl(motorId, num);
        break;
    case CURCTRL:
        SetCurCtrl(motorId, num);
        break;
    case READINFO:
        returnInfo(motorId);
        break;
    case HOMING:
        HomingModeCfg(motorId, vel, cur);
        break;
    case LIMITVELCFG:
        LimitVelCfg(motorId);
        break;
    case NOINITPOSCFG:
        NoInitPosCfg(motorId, num);
        break;
    default:
        break;
    }
}

/**
 * @brief 用一个报文实现3个信息的读取
 *        vel的范围是-8192~8192
 * |第1字节高两位|第1字节低6位和第2字节 |第3、4字节 |第5、6、7、8字节|
 * |motorId     |      vel           |  cur     |     pos       |
 * @param n must be 1~4
 */
void returnInfo(int n)
{
    int canId = 0x280 + BOARDID;

    int16_t vel = GetVel(n);
    int16_t vel_shift = vel + (1 << 13);
    float pos = GetPos(n);
    int16_t cur = GetCur(n);
    uprintf("%d\r\n%f\r\n%d\r\n", vel, pos, cur);
    can_msg msg;
    msg.ui8[0] = (n << 6) & 0xff;
    // msg.ui8[0] |= (vel >> 10) & 0x20;//置入符号位
    // msg.ui8[0] |= (vel >> 8) & 0x1f;//置有效数据位
    msg.ui8[0] |= (vel_shift >> 8) & 0x3f;
    msg.ui8[1] = vel & 0xff;
    msg.i16[1] = cur;
    msg.fl[1] = pos;

    OSLIB_CAN_SendMessage(&hcan2, CAN_ID_STD, canId, &msg);
}

void VelCtrlAll(CAN_Message *msg)
{
    for (int i = 0; i < 4; i++)
    {
        uprintf("%d ", msg->i16[i]);
        SetVelCtrl(i, msg->i16[i]);
    }
    uprintf("\r\n");
}
/**
 * @brief 一个函数控制所有电机
 * 通常在舵轮舵向电机场景下使用，因此不通知到达。
 * 收报的数据大小不够，所以*819.1发给电机，作为角度数据
 * @param msg
 * @note 由于该指令使用一条can消息控制4个电机，因此每个电机被分配到的数值大小只有两字节16位，值为0~65536。\n
 * 	     而2006电机的转子位置使用线数，一圈8191，减速比36：1，因此想要让输出端转一圈需要294912线，显然是
 * 	     不够用的\n2020/11/06改为从主控板发送输出轴的目标角度值，在驱动板里解析换算为线数
 */
void PosCtrlAll(CAN_Message *msg)
{
    for (int i = 0; i < 4; i++)
    {
        uprintf("%d ", msg->i16[i]);
        SetPosCtrl(i, 819.1 * msg->i16[i]);   ///@todo 修改当前版本代码
        drivers[i].posCtrl.arrivalInform = 0; /* 不通知到达 */
    }
    uprintf("\r\n");
}