#pragma once

namespace Util
{
	class ResourceLoader;
}
namespace Battle
{
	class AbstractBattleCharacter;
	class BattleCharacter;
	class BattleMonster;
	const int    PopDamMax = 20;
	const int    AnimationMax = 30;
	const int    SEPlaying = 1;
	struct PopDamage
	{
		bool     flag;
		int      cnt;
		int      damage;
		int      x;
		int      y;
		int      type;
		PopDamage()
			: flag(false), cnt(0), damage(0), x(0), y(0), type(0) { }
	};
	struct Animation
	{
		bool     flag;
		int      target;
		int      kind;
		int      cnt;
		int      actor;
		Animation()
			: flag(false), target(0), kind(0), cnt(0), actor(0) { }
	};

	class BattleAnimation
	{
	public:
		BattleAnimation();
		~BattleAnimation();
		void   startAnimation(int selected, int target, int actor);
		void   graphAnimation(BattleCharacter *chr, BattleMonster *mon);
    
		void   setPopDamage(int damage, int x, int y, int type);

	private:
		void   animation0(int idx, AbstractBattleCharacter *c1,
			AbstractBattleCharacter *c2, int x, int y);
		void   animation1(AbstractBattleCharacter *c, int idx, int x, int y);
		void   animation2(AbstractBattleCharacter *c, int idx, int x, int y);
		void   graphDamage();
		void   calcDamage(int t);

		Animation animation_[AnimationMax];

		PopDamage  popDamage_[PopDamMax];

		Util::ResourceLoader *rl_;
	};
}