#include "Battle/BattleCharacter.h"
#include "StdCommon.h"
#include "SharedCharacterStatus.h"
#include <cstdlib>

namespace Battle
{
	BattleCharacter::BattleCharacter(SharedCharacterStatus *scs)
	{
		scs_ = scs;
		initializeParameter();
	}

	BattleCharacter::~BattleCharacter()
	{
	}

	void BattleCharacter::initializeParameter()
	{
		x_ = BattleCharPosX1;
		y_ = BattleCharPosY1;

		dp_ = 0;
		cntSpeed_ = 1.0;
		for (int i = 0; i < sizeof(abiFlag_); i++)
		{
			abiFlag_[i] = 0;
		}
		atbCnt_ = DxLib::GetRand(AtbCntMax - 5);  // �J�E���^�[240�ōs����
		atbCntFlag_ = true;
		motionFlag_ = false;
		menuSelect_ = 1;
		menuSelectY_ = 0;
		actionCnt_ = 0;
		damageActionCnt_ = 0;
		actionFlag_ = false;
		graphFlag_ = true;

		animationFlag_ = false;
		damageFlag_ = false;
	}

	void BattleCharacter::pressedKeyUp()
	{
		menuSelect_--;
		menuSelectY_--;
		if (menuSelectY_ == -1) // �I���ʒu����ԏ�𒴂�����
		{
			menuSelect_ += 5;     // ��ԉ��� 5�͑I�����ڂ̍ő吔
			menuSelectY_ = 4;   // ��ԉ��� 4�͑I��\�����ڂ̍ő吔-1
		}
	}

	void BattleCharacter::pressedKeyDown()
	{
		menuSelect_++;
		menuSelectY_++;
		if (menuSelectY_ == 5) // �I���ʒu����ԏ�𒴂�����
		{
			menuSelect_ -= 5;    // ��ԏ�� 5�͑I�����ڂ̍ő吔
			menuSelectY_ = 0;   // ��ԏ�� 4�͑I��\�����ڂ̍ő吔-1
		}
	}

	void BattleCharacter::pressedKeyX()
	{
		menuSelect_ = menuSelect_ * 100;    // ����{�^����������邽�т�100�{��
		menuSelectY_ = 0;                    // �I���ʒu��������Ԃ�
	}

	void BattleCharacter::pressedKeyZ()
	{
		menuSelect_ = 1;                    // �I����Ԃ�߂� 
		menuSelectY_ = 0;                    // �I���ʒu��������Ԃ�
	}

	void BattleCharacter::resetSelectState()
	{
		switch (menuSelect_)
		{
			case 10200:
			case 10300:
			case 10400:
				menuSelect_ = 10000;
				break;
			case 30000:
				menuSelect_ = 10000;
				break;
			case 40000:
			case 40100:
			case 40200:
			case 40300:
			case 40400:
				menuSelect_ = 10000;
				break;
			case 50000:
			case 50100:
			case 50200:
			case 50300:
			case 50400:
				menuSelect_ = 10000;
				break;
		}
	}
}