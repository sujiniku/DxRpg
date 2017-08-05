#pragma once

namespace Util
{
	class GameController;
	class ResourceLoader;
}
namespace Battle
{
	const int  BattleStringLenMax = 32;
	const char Main[5][BattleStringLenMax] =
		{ "�U��", "���@", "�A�r���e�B", "�A�C�e��", "������" };
	const char Sub[5][5][BattleStringLenMax] =
	{
		{ "�ʏ�U��", "�e�N�j�J���A�^�b�N", "", "", "" },
		{ "�L���A", "�G�A�[", "�t�@�C�A", "�t���[�U", "�T���_�[" },
		{ "����", "�f�B�t�F���_�[", "�o�[�T�N", "���͂̐�", "���͂̐�" },
		{ "�L���A�h�����N", "�n�C�L���A�h�����N", "��b�̓�", "�T�̉���", "���|�r�^��D" },
		{ "������", "�Ƃ񂸂�", "", "", "" }
	};

	const int  MessageLenMax = 128;
	const int  MenuNum = 5;
	const int  SubMenuKind = 5;

	enum CharacterEnemyKind
	{
		Character,
		Enemy,
	};

	struct Board
	{
		bool     flag;
		int      cnt;
		char     message[MessageLenMax];
		Board()
			: flag(false), cnt(0)
		{
			for (int i = 0; i < sizeof(message); i++)
			{
				message[i] = 0;
			}
		}
	};

	struct Sakura
	{
		bool     flag;
		int      cnt;   // ��]�̂Ƃ��̃��W�A���̎��̃p�����[�^�ɂȂ��Ă���
		double   x;
		double   y;
		double   enlarge; // �g�嗦
		double   speed;
		double   rotation;
		Sakura()
			: flag(false), cnt(0), x(0.0), y(0.0),
			enlarge(0.0), speed(0.0), rotation(0.0) {}
	};

	struct Line
	{
		bool     flag;
		int      cnt;
		int      target;
		Line()
			: flag(false), cnt(0), target(0) { }
	};

	const int    BoardMax = 100;
	const int    SakuraMax = 100;

	const double PI = 3.14159265358979;

	const int    AbilityTime0 = 30;
	const int    AbilityTime1 = 120;
	const int    AbilityTime2 = 120;
	const int    AbilityTime3 = 30;
	const int    AbilityTime4 = 30;

	class AbstractBattleCharacter;
	class BattleCharacter;
	class BattleMonster;
	class BattleAnimation;

	class BattleProcess
	{
	public:
		BattleProcess(BattleCharacter *chr, BattleMonster *mon);
		~BattleProcess();

		void   main();
		bool   getWinMoveNext() const;
		bool   getGameoverMoveNext() const;

	private:
		void   initSakura();
		void   calculateAtb();
		void   selectMenu();
		void   doAction(AbstractBattleCharacter *self,
			AbstractBattleCharacter *opponent, CharacterEnemyKind k);
    
		int    attackAction(AbstractBattleCharacter *my, AbstractBattleCharacter *you, CharacterEnemyKind k);
		int    cureAction(AbstractBattleCharacter *c);
		int    abilityAction(AbstractBattleCharacter *c);

		int    decideDamage(AbstractBattleCharacter *c, int to, int actKind);
		void   addBoard(char *message);

		void   decideGameover();
		void   decideWin();
		void   graphBattle();
    
		void   displayMeter();
		void   decreaseHpAction(AbstractBattleCharacter *c);

		void   countupAbility();
		void   stopAbi(AbstractBattleCharacter *c);

		void   calcSakura();
		void   graphSakura();
		void   graphBoard();
		void   graphLine();
		int    calcLine(int ix1, int iy1, int ix2, int iy2, int iy3, int ix);

		void   graphBar(int x, int y, int hp, int hpMax,
			int mp, int mpMax, int atbCnt, int dp);
		void   decorateMeter(int x, int y, int h, int len);

		void   graphGameover();
		void   graphWin();
		void   graphIcon();

		char   *decideBoardString(char *message);
		double genRandom(double sigma, double average);

		Util::GameController  *gc_;
		Util::ResourceLoader  *rl_;
		BattleMonster   *mon_;
		BattleCharacter *chr_;
		BattleAnimation *ba_;

		Board   board_[BoardMax];
		Sakura  sakura_[SakuraMax];
		Line    line_;

		bool   isGameover_;
		bool   isWin_;
		bool   isGameoverMoveNext_;
		bool   isWinMoveNext_;

	};
}

