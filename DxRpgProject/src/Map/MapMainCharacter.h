#pragma once
#include "AbstractMapCharacter.h"
#include "SharedCharacterStatus.h"  // �s���S�N���X�^�ւ̃|�C���^�[���Q�Ƃł��Ȃ������Ȃ̂�

namespace Map
{
	const int    EncRand  =  4;                // �o�g���ɓ˓�����m��
	const int    EncMinWalkCnt = 4;            // �o�g���ɓ˓�����Œ����

	class MapMainCharacter : public AbstractMapCharacter
	{
	public:
		MapMainCharacter();
		~MapMainCharacter() override;
		void    initMapState(int tx, int ty, int dir, int kind) override;
		bool    move(int *isPassable) override;
		// ����������SharedCharacterStatus���Q�Ƃł���悤�ɂ���

	private:
		int     prevX_;         // �O�񂢂�X���W
		int     prevY_;         // �O�񂢂�Y���W
		int     encCnt_;        // �G�����J�E���g
		bool    encountersEnemy();

		::SharedCharacterStatus *scs_;
	};
}