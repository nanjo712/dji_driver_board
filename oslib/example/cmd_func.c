#include "oslib_config.h"

#ifdef OSLIB_UART_MODULE_ENABLED

#include "uart/oslib_uart.h"
#include "uart/oslib_uart_cmd.h"
#ifdef OSLIB_CAN_MODULE_ENABLED
    #include "can/oslib_can.h"
#endif

#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os2.h"
#include "ctrl.h"
#include "motorflash.h"
#include "motorTask.h"
#include "../BUPT_RobotTeam_Libraries/SimpleLib/utils/utils.h"
#include "command.h"

/* 在这里添加命令回调函数的定义或外部声明 */
static void Command_SetPosPID(OSLIB_UART_Handle_t *uartHandle,int argc,char *argv[])
{
    int num=atoi(argv[1]);num--;
    float kp,ki,kd;
    kp=atof(argv[2]);ki=atof(argv[3]);kd=atof(argv[4]);
    drivers[num].posCtrl.pos_pid.Kp=kp;
    drivers[num].posCtrl.pos_pid.Ki=ki;
    drivers[num].posCtrl.pos_pid.Kd=kd;
    uprintf("set motor:%d pospid \r\n  kp:%.4f\r\n  ki:%.4f\r\n  kd:%.4f\r\n"
    ,num,kp,ki,kd);
}
static void Command_SetVelPID(OSLIB_UART_Handle_t *uartHandle,int argc,char *argv[])
{
    int num=atoi(argv[1]);num--;
    float kp,ki,kd;
    kp=atof(argv[2]);ki=atof(argv[3]);kd=atof(argv[4]);
    drivers[num].velCtrl.speed_pid.Kp=kp;
    drivers[num].velCtrl.speed_pid.Ki=ki;
    drivers[num].velCtrl.speed_pid.Kd=kd;
    uprintf("set motor:%d velpid \r\n  kp:%.4f\r\n  ki:%.4f\r\n  kd:%.4f\r\n"
    ,num,kp,ki,kd);
}
static void Command_Hello(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    uprintf("Hello SimpleLib!\r\n");
}

static void Command_Echo(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
        uprintf("Echo: %s\r\n", argv[i]);
}

/* 串口调试代码 */
static void Command_Motor_On(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        int num = atoi(argv[i]);
        if (num < 0 || num > 4)
        {
            uprintf("illegal motorId [%d]\r\n", num);
            continue;
        }
        uprintf("motor%d is on\r\n", num);
        MotorOn(num - 1);
    }
}

static void Command_Motor_Off(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        int num = atoi(argv[i]);
        if (num < 0 || num > 4)
        {
            uprintf("illegal motorId [%d]\r\n", num);
            continue;
        }
        uprintf("motor%d is off\r\n", num);
        MotorOff(num - 1);
    }
}

static void Command_Motor_Velcfg(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 2)
    {
        uprintf("Param num is error: velcfg <motorid>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    VelLoopCfg(num - 1);
    uprintf("motor%d's mode changes to velloopmode\r\n", num);
}

static void Command_Motor_LimitVelcfg(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 2)
    {
        uprintf("Param num is error: limitvelcfg <motorid>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    LimitVelCfg(num - 1);
    uprintf("motor%d's mode changes to limitvelloopmode\r\n", num);
}

static void Command_Motor_Poscfg(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 3)
    {
        uprintf("Param num is error: poscfg <motorid> <posVel>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    int vel = atoi(argv[2]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    PosLoopCfg(num - 1, vel);
    uprintf("motor%d's mode changes to posloopmode,maxv is %d\r\n",
            num, vel);
}

static void Command_Motor_Curcfg(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 2)
    {
        uprintf("Param num is error: curcfg <motorid>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    CurLoopCfg(num - 1);
    uprintf("motor%d's mode changes to curloopmode\r\n", num);
}

static void Command_Homing(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 4)
    {
        uprintf("Param num is error");
        return;
    }
    int num = atoi(argv[1]);
    int vel = atoi(argv[2]);
    float cur = atof(argv[3]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    if(fabsf(cur) > 7){
        uprintf("cur is too huge!(< 3)\r\n");
        return ;
    }
    if(abs(vel) > 500){
        uprintf("vel is too huge!(< 500)\r\n");
        return ;
    }
    HomingModeCfg(num - 1, vel, cur);
    uprintf("motor%d's mode changes to homingmode\r\n", num);
}

static void Command_Motor_Velctrl(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 3)
    {
        uprintf("Param num is error: velctrl <motorid> <vel>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    int vel = atoi(argv[2]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    SetVelCtrl(num - 1, vel);
    uprintf("motor%d's speed is set to %d\r\n", num, vel);
}

static void Command_Motor_Posctrl(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 3)
    {
        uprintf("Param num is error: posctrl <motorid> <pos>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    int pos = atoi(argv[2]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    SetPosCtrl(num - 1, pos);
    uprintf("motor%d's pos is set to %d\r\n", num, pos);
}

static void Command_Motor_Curctrl(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 3)
    {
        uprintf("Param num is error: curctrl <motorid> <cur>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    int cur = atoi(argv[2]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    SetCurCtrl(num - 1, cur);
    uprintf("motor%d's cur is set to %d\r\n", num, cur);
}

static void Command_Motor_Sendwave(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 2)
    {
        uprintf("Param num is error: sendwave <motorid>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    waveNum = num;
    if (waveNum == 0)
        uprintf("stop sending wave\r\n");
    else
        uprintf("motor%d start to sendwave\r\n", num);
}

static void Command_Motor_PrintInfo(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
    if (argc != 2)
    {
        uprintf("Param num is error: info <motorid>\r\n");
        return;
    }
    int num = atoi(argv[1]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    float vel = GetVel(num -1);
    float pos = GetPos(num - 1);
    float cur = GetCur(num - 1);
    uprintf("motor Type = %d,motor Mode = %d\r\n", motorType[num - 1], motorControlMode[num - 1]);
    uprintf("vel=%f\r\n pos=%f\r\n cur=%f\r\n", vel, pos, cur);
}

static void Command_SetMotor(OSLIB_UART_Handle_t *uartHandle,int argc, char *argv[])
{
//    OSLIB_UART_Printf(&huart3, "%s\r\n %d\n %d\n %d\n",argv[0],atoi(argv[1]), atoi(argv[2]),atoi(argv[3]));
    if (argc < 4)
    {
        uprintf("Param num is error: setmotor <motorid> <motorType> <controlMode> (<maxPosVel>) \r\n \
        motorType: 1 is 3508,2 is 2006,3 is 6020 \r\n");
        uprintf(" \
        controlMode: 1 is CurLoop, 2 is SpeedLoop, 3 is PositionLoop, 4 is Homing)> \r\n \
        maxPosVel: maxvel in PositionLoop\r\n");
        return;
    }
    int num = atoi(argv[1]);
    if (num < 0 || num > 4)
    {
        uprintf("illegal motorId [%d]\r\n", num);
        return;
    }
    int motor = atoi(argv[2]);
    if (motor < 1 || motor > 3)
    {
        uprintf("illegal motorType [%d]\r\n", motor);
        return;
    }
    motorType[num - 1] = motor;

    int mode = atoi(argv[3]);
    if (mode < 1 || mode > 5)
    {
        uprintf("illegal controlMode [%d]\r\n", mode);
        return;
    }
    motorControlMode[num - 1] = mode;

    if (mode == POSITION_CONTROL_MODE)
    {
        maxPosVel[num - 1] = atoi(argv[4]);
    }
    motor_WriteParam();
    DriverInit();
}

#ifdef OSLIB_CAN_MODULE_ENABLED
static void Command_CanSend(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    if (argc >= 3) {
        can_msg msg;
        msg.in[0] = 0x40005850;
        msg.in[1] = 0x00000000;
        uint32_t id = strtol(argv[2], NULL, 16);
        if (argv[1][0] == 's') {
            can_send_msg(id, &msg);
            OSLIB_UART_Printf(uart_handle->huart, "CanSend:std[%x]\r\n", id);
        } else if (argv[1][0] == 'e') {
            can_ext_send_msg(id, &msg);
            OSLIB_UART_Printf(uart_handle->huart, "CanSend:ext[%x]\r\n", id);
        } else
            OSLIB_UART_Printf(uart_handle->huart, "Param Error!\r\n");
    } else {
        OSLIB_UART_Printf(uart_handle->huart, "Param Error!\r\n");
    }
}
static void Command_CanSend2(OSLIB_UART_Handle_t *uart_handle,int argc, char *argv[])
{
    if (argc >= 3)
    {
        can_msg msg;
        msg.in[0] = 0x40005850;
        msg.in[1] = 0x00000000;
        uint32_t id = strtol(argv[2], NULL, 16);
        if (argv[1][0] == 's')
        {
            OSLIB_CAN_SendMessage(&hcan2, CAN_ID_STD, id, &msg);
            uprintf("CanSend:std[%x]\r\n", id);
        }
        else if (argv[1][0] == 'e')
        {
            OSLIB_CAN_SendMessage(&hcan1, CAN_ID_EXT, id, &msg);
            uprintf("CanSend:ext[%x]\r\n", id);
        }
        else
            uprintf("Param Error!\r\n");
    }
    else
    {
        uprintf("Param Error!\r\n");
    }
}
#endif

static void Command_Task(OSLIB_UART_Handle_t *uart_handle, int argc, char *argv[])
{
    OSLIB_UART_Printf(uart_handle->huart, "Free Heap: %dB\r\n", xPortGetFreeHeapSize());
#if ((configUSE_TRACE_FACILITY == 1) && (configUSE_STATS_FORMATTING_FUNCTIONS > 0) && (configSUPPORT_DYNAMIC_ALLOCATION == 1))
    static char tasklist_buffer[256];
    OSLIB_UART_Printf(uart_handle->huart, "Name          \tState\tPrio\tStack\tRank\r\n");
    vTaskList(tasklist_buffer);
    OSLIB_UART_Printf(uart_handle->huart, tasklist_buffer);
#endif
    // uprintf("Name          \tCount\tUtilization\r\n");
    // vTaskGetRunTimeStats(tasklist_buffer);
    // uprintf(tasklist_buffer);
}

/* 在这里完成命令到回调函数的映射 */
UART_CLI_Command_t UART_CommandList[] = 
{
    { "hello",      "Print \"Hello SimpleLib!\"",   Command_Hello   },
    { "echo",       "Echo message",                 Command_Echo    },
    { "tasks",      "Show task list",               Command_Task    },
    {"motoron", "motoron <motorid1> <motorid2> <...>", Command_Motor_On},
    {"motoroff", "motoroff <motorid1> <motorid2> <...>", Command_Motor_Off},
    {"velcfg", "velcfg <motorid>", Command_Motor_Velcfg},
    {"limitvelcfg", "limitvelcfg <motorid>", Command_Motor_LimitVelcfg},
    {"poscfg", "poscfg <motorid> <posVel>", Command_Motor_Poscfg},
    {"curcfg", "curcfg <motorid>", Command_Motor_Curcfg},
    {"homing", "homing <motorid> <vel> <cur>", Command_Homing},
    {"velctrl", "velctrl <motorid> <vel>", Command_Motor_Velctrl},
    {"posctrl", "posctrl <motorid> <pos>", Command_Motor_Posctrl},
    {"curctrl", "curctrl <motorid> <cur>", Command_Motor_Curctrl},
    {"sendwave", "sendwave <motorid>", Command_Motor_Sendwave},
    {"info", "info <motorid>", Command_Motor_PrintInfo},
    {"setmotor", "setmotor <motorid> <motorType> <controlMode> (<maxPosVel>) \r\n \
        motorType: 1 is 3508,2 is 2006,3 is 6020, \r\n \
        controlMode: 1 is CurLoop, 2 is SpeedLoop, 3 is PosLoop, 4 is Homing, 5 is SPEEDLIMIT)> \r\n \
        maxPosVel: maxvel in PosLoop",
     Command_SetMotor},
    {"setpospid","setpospid <motorid> <kp> <ki> <kd>",Command_SetPosPID},
    {"setvelpid","setvelpid <motorid> <kp> <ki> <kd>",Command_SetVelPID},
#ifdef OSLIB_CAN_MODULE_ENABLED
    { "can1send", "CAN1 send message", Command_CanSend },
    {"cansend2", "CAN2 send message", Command_CanSend2},

#endif
};
size_t UART_CommandListSize = sizeof(UART_CommandList) / sizeof(UART_CLI_Command_t);

#endif // OSLIB_UART_MODULE_ENABLED
