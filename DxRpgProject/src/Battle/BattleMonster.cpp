#include "Battle/BattleMonster.h"
#include "DxLib.h"
#include "StdCommon.h"
#include "SharedCharacterStatus.h"

namespace Battle
{
	BattleMonster::BattleMonster()
	{
		// �G�̏ꍇ�͖��xHP�������ō쐬����
		scs_ = new ::SharedCharacterStatus();
		initializeParameter();
	}

	BattleMonster::~BattleMonster()
	{
		delete scs_;
	}

	void BattleMonster::initializeParameter()
	{
		x_ = BattleMonPosX1;
		y_ = BattleMonPosY1;
#ifdef _DEBUG
		scs_->hp_ = 500;
#else
		scs_->hp_ = 600 + DxLib::GetRand(20);
#endif
		scs_->hpMax_ = scs_->hp_;
		scs_->mp_ = 136;
		scs_->mpMax_ = 136;
		scs_->power_ = 20;               // �U����
		scs_->defense_ = 20;             // �h���
		scs_->strength_ = 20;            // ��
		scs_->magic_ = 20;
		scs_->weapon_ = 20;              // ����̍U����
		scs_->equip_ = 20;               // �����̖h���
		scs_->level_ = 10;
    
		dp_ = 0;
		cntSpeed_ = 1.0;              // �J�E���^�[����������X�s�[�h(double)
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
		animationFlag_ = false;            // �A�j���[�V���������邩�̃t���O
		damageFlag_    = false;            // �_���[�W�\�����邩�̃t���O
	}
}