#pragma once

enum //ScenceObjIdentify
{
	SOBJ_NONE,
	SOBJ_VEHICLE_4TYRE,
	SOBJ_TANK,
	SOBJ_BULLET
};

enum //change state
{
	CST_FB_STATIC,
	CST_LR_STATIC,
	CST_GRNP_STATIC,
	CST_FORWARD, //move
	CST_BACKWARD, //move
	CST_LEFT,  //tyre yaw
	CST_RIGHT, //tyre yaw
	CST_GR_POSITIVE, //gun rotation
	CST_GR_NEGATIVE, //gun rotation
	CST_UP,
	CST_FALL,
	CST_FB_SPEED_DOWN
};

enum //obj state
{
	ST_STATIC = 0x00000000,
	ST_FB_STATIC = 0xfffffffc,
	ST_LR_STATIC = 0xfffffff3,
	ST_GRPN_STATIC = 0xffffffcf,
	ST_UF_STATIC = 0xffffff3f,
	ST_FORWARD = 0x01,
	ST_BACKWARD = 0x02,
	ST_LEFT = 0x04,
	ST_RIGHT = 0x08,
	ST_GRP = 0x10,
	ST_GRN = 0x20,
	ST_UP = 0x40,
	ST_FALL = 0x80,
	ST_FB_SPEED_DOWN = 0x100,
};

#define IS_FB_STATIC(x)       (!((x)&(ST_FORWARD+ST_BACKWARD)))
#define IS_LR_STATIC(x)       (!((x)&(ST_LEFT+ST_RIGHT)))
#define IS_GRPN_STATIC(x)     (!((x)&(ST_GRP+ST_GRN)))
#define IS_UF_STATIC(x)       (!((x)&(ST_UP+ST_FALL)))

#define IS_FORWARD(x)         ((x)&ST_FORWARD)
#define IS_BACKWARD(x)        ((x)&ST_BACKWARD)
#define IS_LEFT(x)            ((x)&ST_LEFT)
#define IS_RIGHT(x)           ((x)&ST_RIGHT)
#define IS_GRP(x)             ((x)&ST_GRP)
#define IS_GRN(x)             ((x)&ST_GRN)
#define IS_UP(x)              ((x)&ST_UP)
#define IS_FALL(x)            ((x)&ST_FALL)
#define IS_FB_SPEED_DOWN(x)   ((x)&ST_FB_SPEED_DOWN)

#define SET_FORWARD(x)        ((x) |= ST_FORWARD)
#define SET_BAKCKWARD(x)      ((x) |= ST_BACKWARD)
#define SET_LEFT(x)           ((x) |= ST_LEFT)
#define SET_RIGHT(x)          ((x) |= ST_RIGHT)
#define SET_GRP(x)            ((x) |= ST_GRP)
#define SET_GRN(x)            ((x) |= ST_GRN)
#define SET_UP(x)             ((x) |= ST_UP)
#define SET_FALL(x)           ((x) |= ST_FALL)
#define SET_FB_SPEED_DOWN(x)  ((x) |= ST_FB_SPEED_DOWN)

#define RESET_FB_SPEED_DOWN(x)     ((x) &= (!ST_FB_SPEED_DOWN))
#define RESET_FORWARD_BACKWARD(x)  ((x) &= ST_FB_STATIC)
#define RESET_LEFT_RIGHT(x)        ((x) &= ST_LR_STATIC)
#define RESET_GRN_GRP(x)           ((x) &= ST_GRPN_STATIC)
#define RESET_UP_FALL(x)           ((x) &= ST_UF_STATIC)

