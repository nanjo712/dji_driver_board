#ifndef CAMMAND_H
#define CAMMAND_H

void VelLoopCfg(int num);

void LimitVelCfg(int num);

void NoInitPosCfg(int num, int vel);

void PosLoopCfg(int num, int vel);

void CurLoopCfg(int num);

void SetVelCtrl(int num, int vel);

void SetPosCtrl(int num, int Pos);

void SetCurCtrl(int num, int cur);

void MotorOn(int n);

void MotorOff(int n);

float GetVel(int n);

float GetCur(int n);

float GetPos(int n);

void HomingModeCfg(int num, int vel, float cur);
#endif