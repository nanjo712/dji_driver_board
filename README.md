# DJI-Driver-Board 大疆驱动板

## 1. 项目简介

支持RM3508、M2006、GM6020等多种大疆电机控制，重新设计了can控制报文

已经迁移到新版队库，后续会转变为子模块模式引入，方便后续维护

## 2. 使用说明

在`Core/Inc/config.h`中配置两项

`BOARDID`这块板子的ID

`GM6020_USED`是否需要驱动GM6020电机

**注意：若使用GM6020电机，需要将GM6020电机ID设置在1~4之间。将C620、C610电调ID设置在5~8之间。C620和C610在板子中实际ID为电调显示的ID减去4**

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
            <td>Homing vel cur</td>    
            <td colspan="2">9</td>    
            <td colspan="2">vel</td>    
            <td colspan="2">cur</td>    
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

