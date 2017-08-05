#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
    #include <crtdbg.h>
#endif
#include <cassert>  // DEBUG
#include "DxLib.h"
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>

// ���ʒ萔
const int    WindWidth     = 640;                 // ���E�B���h�E�T�C�Y
const int    WindHeight    = 480;                 // �c�E�B���h�E�T�C�Y
const int    BlockLen      = 32;                  // ���Ԃ̑傫���B��(32x32)
const int    MapWidth      = 640 + 32;            // �}�b�v�E�B���h�E�T�C�Y
const int    MapHeight     = 480;                 // �}�b�v�E�B���h�E�T�C�Y
const int    XBlock = MapWidth / BlockLen;        // ����20���
const int    YBlock = MapHeight / BlockLen;       // �c��15���

const int    CharKindMax    = 4;            // ��l�����܂߂��L�����̎��(��l���{���̐l�̎��)
const int    MainCharMax    = 1;            // ��l���̐l��
const int    CompCharMax    = 8;            // ���L�����̐l��
const int    ImgCharDivNum  = 16;           // �C���[�W������
const int    DivisionNum    = 4;
const int    AtbCntMax      = 240;

const int    MainCharIndex  = 0;

enum PassFlag {
    Through = 0,
    MainCharOnly = 1,
    NoThrough = 2    
};

enum Direction
{
    Down = 0,
    Left = 1,
    Right = 2,
    Up = 3
};

#ifdef _DEBUG
    #define DBGPRINTNUM(VALUE) \
    { \
        DxLib::printfDx("[DEBUG][Line %05d][Func %s]\n\t>> %s [%d]\n", __LINE__, __FUNCTION__, #VALUE, VALUE); \
    }
    #define DBGPRINTSTR(VALUE) \
    { \
        DxLib::printfDx("[DEBUG][Line %05d][Func %s]\n\t>>%s [%s]\n", __LINE__, __FUNCTION__, #VALUE, VALUE); \
    }
#else
    #define DBGPRINTNUM(VALUE)
    #define DBGPRINTSTR(VALUE)
#endif