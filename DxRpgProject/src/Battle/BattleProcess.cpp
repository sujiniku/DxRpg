#include "Battle/BattleProcess.h"
#include "StdCommon.h"
#include "Util/ResourceLoader.h"
#include "Util/GameController.h"
#include "Battle/AbstractBattleCharacter.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"
#include "Battle/BattleAnimation.h"

namespace Battle
{
	BattleProcess::BattleProcess(BattleCharacter *chr, BattleMonster *mon)
		:isWin_(false), isGameover_(false),
		 isWinMoveNext_(false), isGameoverMoveNext_(false)
	{
		gc_ = Util::GameController::getInstance();
		rl_ = Util::ResourceLoader::getInstance();

		chr_ = chr;
		mon_ = mon;
		ba_ = new BattleAnimation();

		initSakura();
	}

	BattleProcess::~BattleProcess()
	{
		delete ba_;
	}

	void BattleProcess::initSakura()
	{
		int r;
		double rand;
		for (int i = 0; i < SakuraMax; i++)
		{
			r = DxLib::GetRand(static_cast<int>(WindHeight * sqrt(2.0)));
			if (i < 60)
			{
				sakura_[i].x = DxLib::GetRand(WindWidth) + r;
				sakura_[i].y = 0.0 - r;
			}
			else
			{
				sakura_[i].x = WindWidth + r;
				sakura_[i].y = DxLib::GetRand(WindHeight) - r;
			}
			sakura_[i].cnt = 0;
			sakura_[i].flag = true;
			if (i < 30 || (60 <= i && i < 80))
			{
				rand = DxLib::GetRand(80) / 100.0 + 0.1;
			}
			else
			{
				rand = DxLib::GetRand(40) / 100.0 + 0.1;
			}
			sakura_[i].speed = rand * 2;
			sakura_[i].enlarge = rand;
			sakura_[i].rotation = DxLib::GetRand(60) / 100.0 + 0.3;
		}
	}

	void BattleProcess::main()
	{
		calculateAtb();  // �o�g���p�̃J�E���^(=ATB)���グ��
 
		if (!isGameover_ && !isWin_)
		{
			// ����
			if (!chr_->getMotionFlag() && chr_->getAtbCnt() == AtbCntMax)
			{
				selectMenu();
			}
			// �G
			if (!mon_->getMotionFlag() && mon_->getAtbCnt() == AtbCntMax)
			{
				mon_->setMotionFlag(true);
			}
		}
		// �ȉ��̏����͐퓬�I����(Win or Gameover)�ł��ꕔ�s��
		// �U���̃��[�V����
		if (chr_->getMotionFlag())
		{
			doAction(chr_, mon_, Character);
		}
		if (mon_->getMotionFlag())
		{
			doAction(mon_, chr_, Enemy);
		}

		decideGameover();
		decideWin();

		graphBattle();
		displayMeter();

		ba_->graphAnimation(chr_, mon_);

		countupAbility();

		calcSakura();
		graphSakura();
		graphBoard();
		graphLine();
		// ���[�V�������ĂȂ���ATB�J�E���^��MAX�Ȃ�I�����ڂ�`��
		if (!chr_->getMotionFlag() && chr_->getAtbCnt() == AtbCntMax)
		{
			graphIcon();
		}

		if (isGameover_)
		{
			// �Q�[���I�[�o�[����
			graphGameover();
		}
		else if (isWin_)
		{
			// �������Ƃ��̏���
			graphWin();
		}

	}

	bool BattleProcess::getWinMoveNext() const
	{
		return isWinMoveNext_;
	}

	bool BattleProcess::getGameoverMoveNext() const
	{
		return isGameoverMoveNext_;
	}

	void BattleProcess::calculateAtb()
	{
		if (mon_->getAtbCntFlag() && !isGameover_ && !isWin_)
		{
			mon_->increaseAtbCnt();
		}
		if (chr_->getAtbCntFlag() && !isGameover_ && !isWin_)
		{
			chr_->increaseAtbCnt();
		}
	}

	void BattleProcess::selectMenu()
	{
		if (gc_->getKey(KEY_INPUT_UP) == Util::GameController::PressedNow)
		{
			chr_->pressedKeyUp();
			PlaySoundMem(rl_->getHdlSoundEffect(2), DX_PLAYTYPE_BACK);
		}
		else if (gc_->getKey(KEY_INPUT_DOWN) == Util::GameController::PressedNow)
		{
			chr_->pressedKeyDown();
			PlaySoundMem(rl_->getHdlSoundEffect(2), DX_PLAYTYPE_BACK);
		}

		if (gc_->getKey(KEY_INPUT_X) == Util::GameController::PressedNow)
		{
			chr_->pressedKeyX();
			if (chr_->getMenuSelect() < 10000)   // �I�������ׂĊ������Ă��Ȃ����
			{
				PlaySoundMem(rl_->getHdlSoundEffect(1), DX_PLAYTYPE_BACK);
			}
			else
			{
				chr_->resetSelectState();
				PlaySoundMem(rl_->getHdlSoundEffect(3), DX_PLAYTYPE_BACK);
				chr_->setMotionFlag(true);
			}
		}
		else if (gc_->getKey(KEY_INPUT_Z) == Util::GameController::PressedNow)
		{
			chr_->pressedKeyZ();
			PlaySoundMem(rl_->getHdlSoundEffect(4), DX_PLAYTYPE_BACK);
		}
	}

	void BattleProcess::doAction(AbstractBattleCharacter *self,
		AbstractBattleCharacter *opponent,
		CharacterEnemyKind k)
	{
		if (isGameover_ || isWin_)
		{
			self->setActionCnt(0);
			return;
		}
		// �O�ɂ����A�N�V����
		if (20 < self->getActionCnt() && self->getActionCnt() <= 40)
		{
			//�@����
			if (k == Character)
			{
				self->decreaseX();
			}
			// �G
			else
			{
				self->increaseX();
			}
		}
		// �U�����[�V�������J�n
		if (self->getActionCnt() == 50)
		{
			// ����
			if (k == Character)
			{
				ba_->startAnimation(self->getMenuSelect(), 0, 100);
			}
			// �G
			else
			{
				// 10000�͒ʏ�U��
				ba_->startAnimation(10000, 100, 0);
			}
		}

		// ���̏u�ԃ_���[�W�A�N�V����������t���O�������Ă����� START
		int damage = 0;
		if (self->getActionFlag())    // BattleAnimation�N���X��ON�ɂȂ�
		{
			// �G�̏ꍇ
			if (k == Enemy)
			{
				// �ʏ�U���I��
				self->setMenuSelect(10000);
			}
			char message[MessageLenMax];
			switch (self->getMenuSelect())
			{
				case 10000:
				case 10100:
				case 20100:
				case 20200:
				case 20300:
				case 20400:
					damage = attackAction(self, opponent, k);
					ba_->setPopDamage(damage, opponent->getCenterX() - 20, opponent->getCenterY() - 20, 0);
					// ����
					if (k == Character)
					{
						sprintf(message, "�G��%d�̃_���[�W��^����", damage);
					}
					// �G
					else
					{
						sprintf(message, "%d�̃_���[�W���󂯂�", damage);
					}             
					addBoard(message);
					break;

				case 20000:
					damage = cureAction(self);
					ba_->setPopDamage(damage, self->getCenterX() - 20, self->getCenterY() - 20, 1);
					// ����
					if (k == Character)
					{
						sprintf(message, "HP��%d�񕜂���", damage);
					}
					// �G
					else
					{
						sprintf(message, "�G��HP��%d�񕜂���", damage);
					}       
					addBoard(message);
					break;
            
				case 30000:
				case 30100:
				case 30200:
				case 30300:
				case 30400:
					// �����̂�
					damage = abilityAction(self);
					// 30300�̂Ƃ��̂�
					if (self->getMenuSelect() == 30300) {
						ba_->setPopDamage(damage, self->getCenterX() - 20, self->getCenterY() - 20, 0);
					}
					break;

				default:
					break;
			}
			self->setActionFlag(false);
			self->increaseDamageActionCnt();
		}

		// ���̏u�ԃ_���[�W�A�N�V����������t���O�������Ă����� END

		// ���ɉ�����
		if (0 < self->getDamageActionCnt() && self->getDamageActionCnt() <= 20)
		{
			// ���ɓ���
			if (k == Character)
			{
				self->increaseX();
			}
			else
			{
				self->decreaseX();
			}
		}

		self->increaseActionCnt();    // �����͕K��������s
		if (self->getDamageActionCnt() > 0)
		{
			self->increaseDamageActionCnt();
		}
		if (self->getDamageActionCnt() == 30)
		{
			// �e���ڂ�������
			self->setActionCnt(0);
			self->setDamageActionCnt(0);

			self->setMenuSelect(1);
			self->setMenuSelectY(0);
			self->setMotionFlag(false);
			self->setAtbCntFlag(true);
			self->setAtbCnt(0);
		}

		// �Q�[���I�[�o�[������
		if (isGameover_ || isWin_)
		{
			self->setActionCnt(0);
			self->setDamageActionCnt(0);
		}
	}

	int BattleProcess::attackAction(
		AbstractBattleCharacter *self,
		AbstractBattleCharacter *opponent,
		CharacterEnemyKind k)
	{
		int damage = decideDamage(self, ((k == Character) ? 0 : 1), self->getMenuSelect());
		if (self->getAbility(3).flag)
		{
			damage *= 2;
		}
		opponent->setDamage(damage);  // �_���[�W���Z�b�g

		// ����̃_���[�W�|�C���g���Z�b�g
		opponent->setDp();

		// ���@�Ȃ�
		if (20000 <= self->getMenuSelect() && self->getMenuSelect() < 30000)
		{
			self->useMp();
		}

		self->changeAbility();
		return damage;
	}


	int BattleProcess::cureAction(AbstractBattleCharacter *c)
	{
		int damage = decideDamage(c, 0, c->getMenuSelect());
		c->decreaseMp(9);
		c->increaseHp(damage);
    
		// �񕜂��������Ԃ�߂�
		if (c->getScs()->hp_ > c->getScs()->hpMax_)
		{
			c->getScs()->hp_ = c->getScs()->hpMax_;
		}
		c->increaseHp(0);   // �񕜗ʂ�߂�

		c->changeAbility();
		return damage;
	}

	int BattleProcess::abilityAction(AbstractBattleCharacter *c)
	{
		char message[MessageLenMax];
		
		int damage = 0;

		switch (c->getMenuSelect())
		{
			case 30000:
				c->getAbility(0).flag = true;
				break;
			case 30100:
				c->getStatus().defense = c->getScs()->defense_ / 3;
				c->getStatus().power = - c->getScs()->power_ / 3;
				c->getAbility(1).flag = true;
				c->getAbility(1).cnt = 0;
				addBoard("�h��͂��オ����");
				addBoard("�U���͂���������");
				break;
			case 30200:
				c->getStatus().defense = -c->getScs()->defense_ / 3;
				c->getStatus().power = c->getScs()->power_ / 3;
				c->getAbility(2).flag = true;
				c->getAbility(2).cnt = 0;
				addBoard("�U���͂��オ����");
				addBoard("�h��͂���������");
				break;
			case 30300:
				damage = decideDamage(c, 2, c->getMenuSelect());
				c->getAbility(3).flag = true;
				c->getAbility(3).cnt = 3;
				addBoard("����̍U�����Q�{�̃_���[�W");
				sprintf(message, "���͂̐�̔�����%d�_���[�W���󂯂�", damage);
				addBoard(message);
				break;
			case 30400:
				c->getAbility(4).flag = true;
				c->getAbility(4).cnt = 1;
				addBoard("����r�����Ԃ��S���̂P");
				break;
			default:
				break;
		}
		return damage;
	}

	// to: 0:����->�G�A1:�G->����
	int  BattleProcess::decideDamage(AbstractBattleCharacter *c, int to, int actKind)
	{
		int power = 0;
		int strength = 0;
		int weapon = 0;
		int level = 0;
		int defense = 0;
		int equip = 0;
		switch (to)
		{
			case 0:     // �����́u�ʏ�U���v
				chr_->getOffenseParam(&power, &strength, &weapon, &level);
				mon_->getDefenseParam(&defense, &equip);
				if (actKind % 10000 == 2)      // ���͂̐�̎�(��1�P�^��2)
				{
					power += 100 * (actKind % 10000) / 100;
				}
				if (actKind % 10000 == 100)   // ����U��
				{
					power *= 4;
				}
				break;
			case 1:     // �G�́u�ʏ�U���v
				mon_->getOffenseParam(&power, &strength, &weapon, &level);
				chr_->getDefenseParam(&defense, &equip);
				break;
			case 2:
				break;
			case 3:
				break;
		}
		if (actKind < 40000 && actKind != 20000)
		{
			return static_cast<int>(level * 2 + (power + weapon - (defense + equip))
				* strength * genRandom(0.01, 0.05) + strength * genRandom(0.2, 1.0));
		}
		if (actKind == 20000)    // ��
		{
			return static_cast<int>(c->getScs()->magic_ * 10.0 * genRandom(0.2, 1.0));
		}
		return -1;
	}


	void BattleProcess::addBoard(char *message)
	{
		int i = 0;
		for (i = 0; i < BoardMax - 1; i++)
		{
			if (!board_[i].flag)
			{
				break;
			}
		} 
		board_[i].flag = true;
		board_[i].cnt = 0;
		strcpy(board_[i].message, message);
	}

	void BattleProcess::decideGameover()
	{
		if (chr_->getScs()->hp_ <= 0)
		{
			isGameover_ = true; // �Q�[���I�[�o�[
		}
		else
		{
			return;
		}
		if (DxLib::CheckSoundMem(rl_->getHdlBattleSound(0) == 1))
		{
			StopSoundMem(rl_->getHdlBattleSound(0));
		}
	}

	void BattleProcess::decideWin()
	{

		if (mon_->getScs()->hp_ <= 0)
		{
			isWin_ = true; // ����
		}
		else
		{
			return;
		}
		if (DxLib::CheckSoundMem(rl_->getHdlBattleSound(0)) == 1)
		{
			StopSoundMem(rl_->getHdlBattleSound(0));
		}

	}

	void BattleProcess::graphBattle()
	{
		DxLib::DrawGraph(0, 0, rl_->getHdlImgBackGround(0), FALSE);
		DxLib::DrawGraph(0, WindHeight - 134, rl_->getHdlImgBackGround(10), FALSE);

		if (mon_->getGraphFlag())
		{
			DxLib::DrawGraph(
				mon_->getX(),
				mon_->getY(),
				rl_->getHdlImgMonster(mon_->getScs()->kind_),
				TRUE);
		}

		if (chr_->getGraphFlag())
		{
			DxLib::DrawGraph(
				chr_->getX(),
				chr_->getY(),
				rl_->getHdlImgChar(chr_->getScs()->kind_, 5),
				TRUE);
		}
	}

	void BattleProcess::displayMeter()
	{
		// ���[�^�̊J�n�ʒu�̍��W
		const int MonMtX = 15;
		const int MonMtY = 370;
		const int ChrMtX = 250;
		const int ChrMtY = 370;

		decreaseHpAction(mon_);
		graphBar(MonMtX, MonMtY,
			mon_->getScs()->hp_, mon_->getScs()->hpMax_,
			mon_->getScs()->mp_, mon_->getScs()->mpMax_,
			mon_->getAtbCnt(), -1
			);
		decreaseHpAction(chr_);
		graphBar(ChrMtX, ChrMtY,
			chr_->getScs()->hp_, chr_->getScs()->hpMax_,
			chr_->getScs()->mp_, chr_->getScs()->mpMax_,
			chr_->getAtbCnt(), chr_->getDp()
			);
	}

	void BattleProcess::countupAbility()
	{
		stopAbi(chr_);
		stopAbi(mon_);
	}

	void BattleProcess::stopAbi(AbstractBattleCharacter *c)
	{
		int time[AbilityMax] = {
			AbilityTime0,
			AbilityTime1,
			AbilityTime2,
			AbilityTime3,
			AbilityTime4
		};
    
		for (int i = 0; i < AbilityMax; i++)
		{
			if (c->getAbility(i).flag)
			{
				c->getAbility(i).cnt++;
				if (c->getAbility(i).cnt == time[i] * 60)
				{
					if (i == 1)
					{
						c->getStatus().defense = 0;
						c->getStatus().power = 0;
						addBoard("�f�B�t�F���_�[�̌��ʂ��؂ꂽ");
					}
					else if (i == 2)
					{
						c->getStatus().defense = 0;
						c->getStatus().power = 0;
						addBoard("�o�[�T�N�̌��ʂ��؂ꂽ");
					}
					else if (i == 3)
					{
						if (c->getAbility(i).flag)
						{
							addBoard("���͂̐�̌��ʂ��؂ꂽ");
						}

					}
					else if (i == 4)
					{
						if (c->getAbility(i).flag)
						{
							addBoard("���͂̐�̌��ʂ��؂ꂽ");
						}
					}
					c->getAbility(i).flag = false;
				}
			}
		}
	}

	// HP���ۂ̃��[�V���� ��u�Ō���悤�ɉ���
	void BattleProcess::decreaseHpAction(AbstractBattleCharacter *c)
	{
		if (isGameover_ && isWin_)
		{
			return;
		}

		if (c->getScs()->hp_ - c->getDamage() < 0) {
			c->getScs()->hp_ = 0;
		}
		else {
			c->getScs()->hp_ -= c->getDamage();
		}
		c->setDamage(0);
	}

	void BattleProcess::graphBar(int x, int y, int hp, int hpMax,
		int mp, int mpMax, int atbCnt, int dp)
	{
		const int MeterHeight = 5;
		const int OffsetY = 25;

		// HP���[�^�̘g
		DxLib::DrawBox(
			x,
			y,
			x + 150,
			y + MeterHeight,
			DxLib::GetColor(0, 255, 255), FALSE);
		// HP���[�^�̒��g
		DxLib::DrawBox(
			x,
			y,
			x + 150 * hp / hpMax,
			y + MeterHeight,
			DxLib::GetColor(0, 255, 255), TRUE);
		// HP���[�^�[�̒��g
		DxLib::DrawBox(
			x + 150 + hp / hpMax,
			y,
			x + 150 * hp / hpMax,
			y + MeterHeight,
			DxLib::GetColor(200, 0, 0), TRUE);
		decorateMeter(x, y, MeterHeight, 150 * hp / hpMax);

		// MP���[�^�̘g
		DxLib::DrawBox(
			x,
			y + OffsetY,
			x + 150,
			y + OffsetY + MeterHeight,
			DxLib::GetColor(200, 200, 0), FALSE);
		// MP���[�^�̒��g
		DxLib::DrawBox(
			x,
			y + OffsetY,
			x + 150 * mp / mpMax,
			y + OffsetY + MeterHeight,
			DxLib::GetColor(200, 0, 0), TRUE);
		decorateMeter(x, y + OffsetY, MeterHeight, 150 * mp / mpMax);

		// ATB���[�^�[
		int barColorAtb = DxLib::GetColor(50, 150, 150);  // �D�F
		// ATB���[�^�[�̘g��`��
		DxLib::DrawBox(
			x,
			y + (OffsetY * 2),
			x + 150,
			y + (OffsetY * 2) + MeterHeight,
			barColorAtb, FALSE);
		// ATB���[�^�[�̒��g�`��
		DxLib::DrawBox(
			x,
			y + (OffsetY * 2),
			x + 150 * atbCnt / AtbCntMax,
			y + (OffsetY * 2) + MeterHeight,
			barColorAtb, TRUE);

		// DP
		int barColorDp = DxLib::GetColor(100, 100, 100);
		if (dp == DpMax)
		{
			// ���C���{�[
			barColorDp = GetColor(
				(gc_->getGCount() * 3) % 256,
				(gc_->getGCount() * 3 + 85) % 256,
				(gc_->getGCount() * 3 + 170) % 256);
		}
		if (dp != -1) {
			// DP���[�^�[�̘g��`��
			DxLib::DrawBox(
				x,
				y + (OffsetY * 3),
				x + 150,
				y + (OffsetY * 3) + MeterHeight,
				DxLib::GetColor(100, 100, 100), FALSE);

			// DP���[�^�[�̒��g��`��
			if (dp < DpMax)
			{
				DxLib::DrawBox(
					x,
					y + (OffsetY * 3),
					x + 150 * dp / DpMax,
					y + (OffsetY * 3) + MeterHeight,
					barColorDp, TRUE);
			}
			else
			{
				DxLib::DrawBox(
					x,
					y + (OffsetY * 3),
					x + 150 * dp / DpMax,
					y + (OffsetY * 3) + MeterHeight,
					barColorDp, TRUE);
			}
		}

		DxLib::DrawFormatStringToHandle(x, y + 4, DxLib::GetColor(255, 255, 255),
			rl_->getHdlFont(0), "HP %4d/%4d", hp, hpMax);    // HP���l�\��
		DxLib::DrawFormatStringToHandle(x, y + OffsetY + 4, DxLib::GetColor(255, 255, 255),
			rl_->getHdlFont(0), "MP %4d/%4d", mp, mpMax);    // MP���l�\��
		DxLib::DrawFormatStringToHandle(x, y + (OffsetY * 2) + 4, DxLib::GetColor(255, 255, 255),
			rl_->getHdlFont(0), "ATB");
		if (dp == DpMax) {
			DxLib::DrawFormatStringToHandle(x, y + (OffsetY * 3) + 8, barColorDp,
				rl_->getHdlFont(1), "���~�b�g�u���C�N");    // DP
		}
	}

	// ������E�ɗ���锒�������X�[���ƃ��[�^�[�Ƀf�R���[�g
	void BattleProcess::decorateMeter(int x, int y, int h, int len)
	{
		const int MdLen = 50;
		// �[�����֎~
		if (len == 0)
		{
			return;
		}
		int x2 = x + gc_->getGCount() % len;
		for (int i = 0; i < MdLen; i++)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200 / MdLen * i);
			DrawLine(x2, y, x2, y + h, DxLib::GetColor(255, 255, 255));
			x2++;
			if (x2 >= x + len)
			{
				x2 = x;
			}
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void  BattleProcess::calcSakura()
	{
		for (int i = 0; i < SakuraMax; i++)
		{
			if (sakura_[i].x < -24)
			{
				sakura_[i].x = WindWidth + 24;
			}
			if (sakura_[i].y > WindHeight + 24)
			{
				sakura_[i].y = -24;
			}
			sakura_[i].x -= sakura_[i].speed;
			sakura_[i].y += sakura_[i].speed;
		}
	}

	void  BattleProcess::graphSakura()
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		for (int i = 0; i < SakuraMax; i++)
		{
			if (sakura_[i].flag)
			{
				DxLib::DrawRotaGraph(
					static_cast<int>(sakura_[i].x),
					static_cast<int>(sakura_[i].y),
					sakura_[i].enlarge,    // �g�嗦
					PI * 2.0 / 180.0
						* (static_cast<int>(sakura_[i].cnt
							* sakura_[i].rotation) % 180),   // �A���O�� ���W�A��
					rl_->getHdlImgEtc(2),
					TRUE);
			}

			sakura_[i].cnt++;
		}
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}

	void  BattleProcess::graphBoard()
	{
		for (int i = 0; i < BoardMax; i++)
		{
			if (board_[i].flag)
			{
				if (board_[i].cnt == 120)
				{
					board_[i].flag = false;
					continue; 
				}
				// ���b�Z�[�W��
				DxLib::DrawGraph(10, 26, rl_->getHdlImgEtc(1), TRUE);
				DxLib::DrawFormatString(14, 28,
					DxLib::GetColor(255, 255, 255), "%s", board_[i].message);
				board_[i].cnt++;
				break;
			}
		}
	}

	void  BattleProcess::graphLine()
	{
		int x1 = mon_->getX() + 85;
		int y1 = mon_->getY();
		int x2 = chr_->getX() + 10;
		int y2 = chr_->getY();
		int y3 = 70;
		for (int x = x1; x <= x2; x++)
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 72);
			if (x > x1 + line_.cnt)
			{
				break;
			}
			DxLib::DrawLine(x, calcLine(x1, y1, x2, y2, y3, x),
				x + 1, calcLine(x1, y1, x2, y2, y3, x + 1),
				DxLib::GetColor(0, 255, 255));
			DxLib::DrawLine(x, calcLine(x1, y1, x2, y2, y3, x) + 1,
				x + 1, calcLine(x1, y1, x2, y2, y3, x + 1) + 1,
				DxLib::GetColor(255, 255, 255));
			DxLib::DrawLine(x, calcLine(x1, y1, x2, y2, y3, x) + 2,
				x + 1, calcLine(x1, y1, x2, y2, y3, x + 1) + 2,
				DxLib::GetColor(0, 255, 255));
		}
		for (int i = x1 - 128 + (gc_->getGCount() * 5 % (x2 - x1 + 128)),
			j = 0;
			i <= x1 + (gc_->getGCount() * 5 % (x2 - x1 + 128));
			i++, j++)
		{
			if (i < x1)
			{
				continue;
			}
			if (i > x2)
			{
				break;
			}
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, j * 3);
			DxLib::DrawLine(i, calcLine(x1, y1, x2, y2, y3, i),
				i + 1, calcLine(x1, y1, x2, y2, y3, i + 1),
				DxLib::GetColor(0, 255, 255));
			DxLib::DrawLine(i, calcLine(x1, y1, x2, y2, y3, i) + 1,
				i + 1, calcLine(x1, y1, x2, y2, y3, i + 1) + 1,
				DxLib::GetColor(0, 255, 255));
			DxLib::DrawLine(i, calcLine(x1, y1, x2, y2, y3, i) + 2,
				i + 1, calcLine(x1, y1, x2, y2, y3, i + 1) + 2,
				DxLib::GetColor(0, 255, 255));
		}

		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		if (line_.cnt < 480)
		{
			line_.cnt += 4;
		}
	}

	int BattleProcess::calcLine(int ix1, int iy1, int ix2, int iy2, int iy3, int ix)
	{
		double x1 = static_cast<int>(ix1);
		double y1 = static_cast<int>(iy1);
		double x2 = static_cast<int>(ix2);
		double y2 = static_cast<int>(iy2);
		double y3 = static_cast<int>(iy3);
		double x = static_cast<int>(ix);

		double y = ((sqrt(y1 - y3) + sqrt(y2 - y3)) 
			/ (x2 - x1))*((sqrt(y1 - y3) + sqrt(y2 - y3))
			/ (x2 - x1))*((x - x1) - sqrt(y1 - y3)*(x2 - x1)
			/ (sqrt(y1 - y3) + sqrt(y2 - y3)))*((x - x1) - sqrt(y1 - y3)*(x2 - x1)
			/ (sqrt(y1 - y3) + sqrt(y2 - y3))) + y3;
		return static_cast<int>(y);
	}

	/**
	* @param  sigma
	* @param  average
	* @return �U��΂�x�����Ƃ��Đ��K���z����������Ԃ�
	*/
	double BattleProcess::genRandom(double sigma, double average)
	{
		static double r1, r2, s, r;
		static int valid = 0;
		double c;
		if (!valid)
		{
			r1 = DxLib::GetRand(RAND_MAX) / static_cast<double>(RAND_MAX + 1);
			r2 = DxLib::GetRand(RAND_MAX) / static_cast<double>(RAND_MAX + 1);
			s = sqrt(-2.0 * log(1.0 - r2));
		}
		valid = !valid;
		c = 2.0 * PI * r1;
		r = valid ? s * sin(c) : s + cos(c);
		return r * sigma + average;
	}

	void BattleProcess::graphGameover()
	{
		static int cnt = 0;

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DxLib::DrawBox(0, 0, 640, 480, DxLib::GetColor(255, 0, 0), TRUE);
		DxLib::DrawString(300, 230, "GAME OVER", DxLib::GetColor(255, 255, 255));
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		cnt++;
		if (cnt == 120)
		{
			cnt = 0;
			isGameoverMoveNext_ = true;
		}
	}

	void BattleProcess::graphWin()
	{
		static int cnt = 0;

		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		DxLib::DrawBox(0, 0, 640, 480, DxLib::GetColor(0, 255, 255), TRUE);
		DxLib::DrawString(300, 230, "WON!!", DxLib::GetColor(255, 255, 255));
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		cnt++;
		if (cnt == 120)
		{
			cnt = 0;
			isWinMoveNext_ = true;
		}
	}

	// �o�g���̃��j���[
	void BattleProcess::graphIcon()
	{
		const int OffsetX = 480;
		const int OffsetY = 370;

		// �I��g
		DxLib::DrawGraph(WindWidth - 180, WindHeight - 134, rl_->getHdlImgBackGround(11), FALSE);
		// ���C�����j���[
		// �I����Ԃ��ŏ��̏��
		if (chr_->getMenuSelect() < 100)
		{
			int y;
			y = chr_->getMenuSelectY(); // �`�悙�ʒu�����ɃZ�b�g
			for (int i = 0; i < MenuNum; i++)
			{
				int x = 0;
				//���ݑI������Ă��鍀�ڂ͎኱���ɂ���
				if (i == y)
				{
					x = -5;
				}
				// ������\��
				DxLib::DrawFormatStringToHandle(OffsetX + x, OffsetY + 18 * i,
					DxLib::GetColor(255, 255, 255),
					rl_->getHdlFont(1), "%s", Main[i]);
			}
		}
		
		// �T�u���j���[
		for (int i = 0; i < SubMenuKind; i++)
		{
			// �키 ���@ �A�r���e�B��100���Ƃɂ킩��Ă���
			if (chr_->getMenuSelect() >= 100 + 100 * i
				&& chr_->getMenuSelect() < 200 + 100 * i)
			{
				int y = chr_->getMenuSelectY();
				for (int j = 0; j < MenuNum; j++)
				{
					int x = 0;
					// �I�����Ă�����͍��ɓ�����
					if (y == j)
					{
						x = -5;
					}
					// ������\��
					DxLib::DrawFormatStringToHandle(OffsetX + x, OffsetY + 18 * j,
						DxLib::GetColor(255, 255, 255),
						rl_->getHdlFont(1), "%s", Sub[i][j]);
				}
				// �U����
				if (i == 0) {
					if (chr_->getDp() == DpMax) {
						int x = 0;
						int y = chr_->getMenuSelectY();
						const int InsertMenuPositionY = 2;
						if (y == InsertMenuPositionY)
						{
							x = -5;
						}
						DxLib::DrawFormatStringToHandle(OffsetX + x, OffsetY + 18 * InsertMenuPositionY,
							DxLib::GetColor(255, 255, 255),
							rl_->getHdlFont(1), "%s", "���~�b�g�u���C�N");
					}
				}
			}
		}

		// ���J�[�\��
		DxLib::DrawGraph(OffsetX - 20 + (gc_->getGCount() % 20) / 4 - 5,
			OffsetY + 18 * chr_->getMenuSelectY() + 1,
			rl_->getHdlImgCursor(), TRUE);
		// ���b�Z�[�W��
		DxLib::DrawGraph(10, 5, rl_->getHdlImgEtc(1), TRUE);
		// ���b�Z�[�W��������
		char str[MessageLenMax] = { 0 };
		DxLib::DrawFormatString(14, 7, DxLib::GetColor(255, 255, 255),
			"%s", decideBoardString(str));
	}

	char *BattleProcess::decideBoardString(char *message)
	{
		if (chr_->getAtbCnt() == AtbCntMax
			&& chr_->getMenuSelect() < 10000)
		{
			switch (chr_->getMenuSelect())
			{
				case 1:
					strcpy(message, "�U�����s���܂��B");
					break;
				case 2:
					strcpy(message, "���@�U�����s���܂��B");
					break;
				case 3:
					strcpy(message, "�A�r���e�B���g�p���܂��B");
					break;
				case 4:
					strcpy(message, "�A�C�e�����g�p���܂��B");
					break;
				case 5:
					strcpy(message, "�퓬���瓦���܂��B");
					break;
				case 100:
					strcpy(message, "�ʏ�U�����s���܂��B");
					break;
				case 101:
					strcpy(message, "����U�����s���܂��B");
					break;
				case 200:
					strcpy(message, "MP9����ĉ񕜖��@���s���܂��B");
					break;
				case 201:
					strcpy(message, "MP8����ĕ��̖��@�U�����s���܂��B");
					break;
				case 202:
					strcpy(message, "MP16����ĉ΂̖��@�U�����s���܂��B");
					break;
				case 203:
					strcpy(message, "MP24����ĕX�̖��@�U�����s���܂��B");
					break;
				case 204:
					strcpy(message, "MP36����ė��̖��@�U�����s���܂��B");
					break;
				case 300:
					strcpy(message, "�������đ���������Ɉ����t���܂��B(30�b��Ďg�p��)");
					break;
				case 301:
					strcpy(message, "2���ԍU���͂������Ėh��͂��グ�܂��B(2����Ďg�p��)");
					break;
				case 302:
					strcpy(message, "2���ԍU���͂��グ�Ėh��͂������܂��B(2����Ďg�p��)");
					break;
				case 303:
					strcpy(message, "�����̗̑͂�����ēG��2�{�̃_���[�W��^���܂��B(30�b��Ďg�p��)");
					break;
				case 304:
					strcpy(message, "MP��2�{����ĉr�����Ԃ�1/4�ɂ��܂��B(30�b��Ďg�p��)");
					break;
				case 400:
					strcpy(message, "HP�������񕜂��܂��B");
					break;
				case 401:
					strcpy(message, "HP�������񕜂��܂��B");
					break;
				case 402:
					strcpy(message, "�U���͂��������܂��B");
					break;
				case 403:
					strcpy(message, "�h��͂��������܂��B");
					break;
				case 404:
					strcpy(message, "MP�������񕜂��܂��B");
					break;
				case 500:
					strcpy(message, "�퓬���瓦���܂��B");
					break;
				case 501:
					strcpy(message, "�}�l�[�𗎂Ƃ��č��m���Ő퓬���瓦���܂��B");
					break;
				default:
					strcpy(message, "");
					break;
			}
		}
		return message;
	}

}
