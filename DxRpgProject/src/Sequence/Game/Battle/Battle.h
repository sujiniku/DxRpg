#pragma once
#include "StdCommon.h"
#include "Sequence/Game/Child.h"
#include "SharedCharacterStatus.h"  // 不完全クラス型へのポインターが参照できないそうなので

namespace Util
{
	class GameController;
	class ResourceLoader;
}
namespace Battle
{
	class BattleProcess;
	class BattleCharacter;
	class BattleMonster;
}

namespace Sequence
{
	namespace Game
	{
		namespace Battle
		{
			enum NextSequence
			{
				NextMap,
				NextBattleShatter,
				NextBattleEnter,
				NextBattleMain,
				NextBattleDebug,
				NextRestart,
			};

			class Battle : public Sequence::Game::Child
			{
			public:
				Battle(SharedCharacterStatus *scs);
				~Battle();
				void   update(Sequence::Game::Parent *parent);

			private:
				void   startBattleShatter();
				void   startBattleEnter();

				NextSequence nextSequence_;
				Util::GameController  *gc_;
				Util::ResourceLoader  *rl_;
				::Battle::BattleProcess *bp_;

				double startBattleX_[ImgCharDivNum];
				double startBattleY_[ImgCharDivNum];
				double randX_[ImgCharDivNum];
				double randY_[ImgCharDivNum];

				::Battle::BattleCharacter *chr_;
				::Battle::BattleMonster   *mon_;
			};

		}

	}
}
