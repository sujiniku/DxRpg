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

// 共通定数
const int    WindWidth     = 640;                 // 横ウィンドウサイズ
const int    WindHeight    = 480;                 // 縦ウィンドウサイズ
const int    BlockLen      = 32;                  // 一区間の大きさ。例(32x32)
const int    MapWidth      = 640 + 32;            // マップウィンドウサイズ
const int    MapHeight     = 480;                 // マップウィンドウサイズ
const int    XBlock = MapWidth / BlockLen;        // 横の20区間
const int    YBlock = MapHeight / BlockLen;       // 縦の15区間

const int    CharKindMax    = 4;            // 主人公を含めたキャラの種類(主人公＋町の人の種類)
const int    MainCharMax    = 1;            // 主人公の人数
const int    CompCharMax    = 8;            // 町キャラの人数
const int    ImgCharDivNum  = 16;           // イメージ分割数
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