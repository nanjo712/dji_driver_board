#ifndef CONFIG_H
#define CONFIG_H

extern int BOARDID;

struct BoardState{
    uint16_t motorType[4];
    uint16_t motorCtrlMode[4];
    uint16_t motorCurMaxVel[4];
    uint16_t boardId;
};

extern struct BoardState boardState;
#endif