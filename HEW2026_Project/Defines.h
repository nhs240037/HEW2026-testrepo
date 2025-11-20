#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include"CsvData.h"

static const int FPS = 60;
static const float fFPS = static_cast<float>(FPS);


// ���\�[�X�p�X
#define ASSET(path)	"Assets/"path

// 3D��Ԓ�`
#define CMETER(value) (value * 0.01f)
#define METER(value) (value * 1.0f)
#define MSEC(value) (value / fFPS)
#define CMSEC(value) MSEC(CMETER(value))

#define DEFAULT_CAMERA_ANGLE (3.7f)

#define IIKANJINOTEISU (0.01f)
#define IIKANJINOTEISU2 (1.0f)



//CSV�t�@�C���ǂݍ��݌�
#define CSV_ACCEPT

#ifndef CSV_ACCEPT
//----- csv�t�@�C�����ǂݍ��܂�Ă��Ȃ��ꍇ -----//
#define PLAYER_MOVE_SPEED (.1f)
#define FIELD_COLUMN (0xc)
#define FIELD_ROW    (0x6)
#define MAX_FIELD_WIDTH  (FIELD_COLUMN / PLAYER_WIDTH * PLAYER_WIDTH)
#define MAX_FIELD_HEIGHT (FIELD_ROW / PLAYER_HEIGHT * PLAYER_HEIGHT)
#endif

#ifdef CSV_ACCEPT

//----- csv�t�@�C���ǂݍ��ݐ��� -----//
#define PLAYER_MOVE_SPEED(set)	set.GetSpeed()
#define FIELD_COLUMN(set)		set.GetWidth()
#define FIELD_ROW(set)			set.GetHeight()
#define MAX_FIELD_WIDTH(set)  (FIELD_COLUMN(set) / PLAYER_WIDTH * PLAYER_WIDTH)
#define MAX_FIELD_HEIGHT(set) (FIELD_ROW(set) / PLAYER_HEIGHT * PLAYER_HEIGHT)
#endif

#define MAX_BLOCK	(0x1e)

#define PLAYER_WIDTH	(2.0f)
#define PLAYER_HEIGHT	(2.0f)


#define DEBUG_DISTANCE (5.0f)



#define HALF(in) (in / 2.0f)
#define FIELD_WIDTH (2.0f)
#define FIELD_HEIGHT (2.0f)

//#define FIELD_WIDTH (MAX_FIELD_WIDTH / FIELD_COLUMN)
//#define FIELD_HEIGHT (MAX_FIELD_HEIGHT / FIELD_ROW)

static const float GRAVITY = 0.98f;

// �^�C�g��
static const char* APP_TITLE = "DX22_Prototype";

// ��ʃT�C�Y
static const int SCREEN_WIDTH	= 1280;
static const int SCREEN_HEIGHT	= 720;

// �O���b�h�T�C�Y(�f�o�b�O�p
static const int	DEBUG_GRID_NUM		= 20;			// �O���b�h���S����[�܂ł̐��̖{��
static const float	DEBUG_GRID_MARGIN	= METER(1.0f);	// �O���b�h�z�u��


#endif // __DEFINES_H__