#pragma once
#include <climits>
namespace
{
    const int    MetricTimes = 60;          // FPS�v����
    const int    OneFrameMillsec = 16;      // 1�t���[���̃~���b(16ms)
    const int    GCountMax = INT_MAX;       // �O���[�o���J�E���^�l�`�w
    const int    KeyKindNum = 256;
}

namespace Util
{
	class GameController
	{
	public:
		// ������R�s�[���֎~
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
		int     frameSpdAvg;          // ���ς�1�t���[������
		int     waitTime_;            // �e�o�r�Ŏg�p����ϐ�
		int     prevTime_;
		int     fps_[MetricTimes];
		char    key_[KeyKindNum];
		char    prevKey_[KeyKindNum];
	};
}

