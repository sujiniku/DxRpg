#pragma once
#include "StdCommon.h"
#include "Sequence/Game/Child.h"

namespace Util
{
	class CsvMapReader;
	class ResourceLoader;
}
namespace Map
{
	class AbstractMapCharacter;
}
// 定数関連の情報
namespace Sequence
{
	class Parent;
	namespace Game
	{
		namespace Map
		{
			const int   SlidePicVal = 16;            // キャラ縦描画位置ずらし分
			const int   MapNum = 2;
			const char  MapFile[MapNum][MAX_PATH + 1] =
			{
				"resources/csv/map/20x16/0.txt",
				"resources/csv/map/20x16/1.txt"
			};

			// ワールドマップ制御のクラス
			class Map : public Sequence::Game::Child
			{
			public:
				enum NextSequence
				{
					NextMap,
					NextBattle,
				};
				Map(Sequence::Game::Parent *parent, int stage);
				~Map();
				void update(Sequence::Game::Parent *parent) override;

			private:
				Parent *parent_;
				Util::ResourceLoader *rl_;
				Util::CsvMapReader  *cmr_;
				int    isPassable_[YBlock * XBlock];    // 通行可能かどうかの情報(重要！)
				int    mapStage_;
				bool   hasChanged_;
				NextSequence nextSequence_;
				void    initMapState();
				void    initCharState();
				void    drawMap();
				void    moveCharacter();
				void    changeMap();
				void    drawCharacter();
			};
		}
	}
}