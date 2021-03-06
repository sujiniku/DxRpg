#pragma once
#include <climits>
namespace
{
    const int    MetricTimes = 60;          // FPS計測回数
    const int    OneFrameMillsec = 16;      // 1フレームのミリ秒(16ms)
    const int    GCountMax = INT_MAX;       // グローバルカウンタＭＡＸ
    const int    KeyKindNum = 256;
}

namespace Util
{
	class GameController
	{
	public:
		// 生成やコピーを禁止
		GameController(const GameController&) = delete;
		GameController& operator=(const GameController&) = delete;

		enum KeyState {
			NotPressed = 0,
			Pressed = 1,
			PressedNow = 2
		};
		static GameController* getInstance();

		char    getKey(int input) const;
		int     getAllKeyPressed();

		void    increaseGCount();
		int     getGCount() const;

		void    adjustKeyState();
		void    control();

	private:
		GameController() {};
		~GameController() {};
		void    calcFps();
		void    graphFps();
		void    controlFps();

		int     gCount_;
		int     frameSpdAvg;          // 平均の1フレーム時間
		int     waitTime_;            // ＦＰＳで使用する変数
		int     prevTime_;
		int     fps_[MetricTimes];
		char    key_[KeyKindNum];
		char    prevKey_[KeyKindNum];
	};
}

