#pragma once
#include "AbstractMapCharacter.h"
#include "SharedCharacterStatus.h"  // �s���S�N���X�^�ւ̃|�C���^�[���Q�Ƃł��Ȃ������Ȃ̂�

namespace Map
{
	// ���L�����N�^�̃N���X
	class MapCityCharacter : public AbstractMapCharacter
	{
	public:
		MapCityCharacter() { }
		~MapCityCharacter() override { }

		void initMapState(int tx, int ty, int dir, int kind) override;
		bool move(int *isPassable) override;
	};
}