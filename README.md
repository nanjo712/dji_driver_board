# DJI-Driver-Board0-Cxx_feature 大疆驱动板

## 1. 项目简介
使用了C with Class 进行编写，构造了电机类

预先实例化了RM3508、M2006、GM6020三种电机，并编写了相应控制代码，可完全兼容旧版

已经迁移到新版队库，后续会转变为子模块模式引入，方便后续维护

**如果PID有问题请自行调节，不要上传到仓库里！**

**电机工况不同，PID不同，不要随意上传！**

## 2. 更新情况

1.不用手动配置BAORDID以及GM6020_USED，BOARDID可通过CAN或者串口修改

2.对于使用


## 3. CAN通讯报文格式

<table>
        <tr>
            <th>功能</th>
            <th>CanID</th>
            <th>D0</th>
            <th>D1</th>
            <th>D2</th>
            <th>D3</th>
            <th>D4</th>
            <th>D5</th>
            <th>D6</th>
            <th>D7</th>
        </tr>
        <tr>
            <td>MotorOn</td>
            <td  rowspan="9">boardID + 0x200</td>
            <td colspan="2" rowspan="9">motorID</td>
            <td colspan="2">1</td>
            <td> </td>
            <td> </td>
            <td> </td>
            <td> </td>
        </tr>
        <tr>
            <td>MotorOff</td>
            <td colspan="2">2</td>
            <td> </td>
            <td> </td>
            <td> </td>
            <td> </td>
        </tr>
        <tr>
            <td>velCfg</td>
            <td colspan="2">3</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>posCfg maxPosVel</td>
            <td colspan="2">4</td>    
            <td colspan="4">maxPosVel</td>
        </tr>
        <tr>
            <td>curCfg</td>
            <td colspan="2">5</td>    
            <td>&nbsp;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>velCtrl vel</td>    
            <td colspan="2">6</td>    
            <td colspan="4">vel</td>
        </tr>
        <tr>
            <td>posCtrl pos</td>    
            <td colspan="2">7</td>    
            <td colspan="4">pos</td>    
        </tr>
        <tr>
            <td>curCtrl cur</td>    
            <td colspan="2">8</td>    
            <td colspan="4">cur</td>    
        </tr>
        <tr>
            <td>velCtrlAll</td>
            <td>boardID + 0x204</td>
            <td colspan="2">vel1</td>    
            <td colspan="2">vel2</td>    
            <td colspan="2">vel3</td>    
            <td colspan="2">vel4</td>    
        </tr>
        <tr>
            <td>posCtrlAll</td>
            <td>boardID + 0x208</td>
            <td colspan="2">pos1</td>
            <td colspan="2">pos2</td>
            <td colspan="2">pos3</td>
            <td colspan="2">pos4</td>
        </tr>
</table>

## 4.反馈报文格式 （新版暂未测试）

<table>
        <tr>
            <th>功能</th>
            <th>CanID</th>
            <th>D0</th>
            <th>D1</th>
            <th>D2</th>
            <th>D3</th>
            <th>D4</th>
            <th>D5</th>
            <th>D6</th>
            <th>D7</th>
        </tr>
        <tr>
            <td>returnInfo</td>
            <td>boardID + 0x280</td>
            <td >高两位为电机id,低六位为vel高六位</td>
            <td >vel低8位(vel采用14位移码存储)</td>
            <td colspan="2">cur(int16)</td>
            <td colspan="4">pos(float)</td>
        </tr>
        <tr>
            <td>homingOver</td>
            <td>boardID + 0x285</td>
            <td colspan="2">boardId</td>
            <td colspan="2">motorId</td>
            <td colspan="4"> </td>
        </tr>
    	<tr>
            <td>PosArrival</td>
            <td>boardID + 0x286</td>
            <td colspan="2">boardId</td>
            <td colspan="2">motorId</td>
            <td colspan="4"> </td>
        </tr>
</table>

