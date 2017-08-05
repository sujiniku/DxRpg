#include "Battle/BattleAnimation.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"
#include "Util/ResourceLoader.h"
#include "StdCommon.h"

namespace Battle
{

	BattleAnimation::BattleAnimation()
	{
		rl_ = Util:: ResourceLoader::getInstance();
	}

	BattleAnimation::~BattleAnimation()
	{
	}

	// �U���A�j���[�V�����̊J�n
	void BattleAnimation::startAnimation(int selected, int target, int actor)
	{
		// �g���Ă��Ȃ��A�j���[�V������T��
		int i = 0;
		for (i = 0; i < AnimationMax - 1; i++)   // 1���炷
		{
			// �t���O�������Ă��Ȃ�
			if (!animation_[i].flag)
			{
				break;
			}
		}
		animation_[i].flag = true;
		animation_[i].target = target;   // ���� 100:����,  0:�G
		animation_[i].kind = selected;   // menuSelect()�̒l
		animation_[i].cnt = 0;
		animation_[i].actor = actor;     // ���� 100:����,  0:�G

	}

	void BattleAnimation::graphAnimation(BattleCharacter *chr, BattleMonster *mon)
	{
		for (int i = 0; i < AnimationMax; i++)
		{
			if (animation_[i].flag)
			{
				int k = animation_[i].kind / 10000;
				switch (k) // �A�j���[�V�����̎�ނɂ���ď����𕪊�
				{
					case 1:    // �U��
						// �G
						if (animation_[i].target == 0)    // �^�[�Q�b�g���Gno.0��������
						{
							animation0(i, mon, chr, mon->getX() + 5, mon->getY() + 5);
						}
						// ����
						if (animation_[i].target == 100)  // �^�[�Q�b�g������no.0�Ȃ�
						{
							animation0(i, chr, mon, chr->getX() - 32, chr->getY() - 16);
						}
						break;
					case 2:    // ���@
						// �񕜖��@
						if (animation_[i].kind == 20000)
						{
							// �G
							if (animation_[i].actor == 0)
							{
								animation1(mon, i, 0, 0);
							}
							// ����
							else if (animation_[i].actor == 100)
							{
								animation1(chr, i, -30, 20);
							}
						}
						// �U�����@
						else
						{
							// �G
							if (animation_[i].target == 0)    // �^�[�Q�b�g���Gno.0��������
							{
								animation0(i, mon, chr, mon->getX() + 5, mon->getY() + 5);
							}
							// ����
							if (animation_[i].target == 100)  // �^�[�Q�b�g������no.0�Ȃ�
							{
								animation0(i, chr, mon, chr->getX() - 32, chr->getY() - 16);
							}
						}
						break;
					case 3:    // ����\�� Ability
						// �G
						if (animation_[i].actor == 0)
						{
							animation2(mon, i, 0, 0);
						}
						// ����
						else if (animation_[i].actor == 100)
						{
							animation2(chr, i, -30, 20);
						}
						break;
					default:
						exit(9999);
						break;
				}
			}
		}
		graphDamage();   // Animation�̌��Damage
	}

	void BattleAnimation::graphDamage()
	{
		// �r�������ł��Ă��悢 break���Ȃ��B
		for (int i = 0; i < PopDamMax; i++)
		{
			if (popDamage_[i].flag)
			{           
				calcDamage(i);
            
				popDamage_[i].cnt++;
				// �����Ă���120(2�b)��������t���O�𗎂Ƃ�
				if (popDamage_[i].cnt == 120)
				{
					popDamage_[i].flag = false;
				}
			}
		}

	}

	void BattleAnimation::calcDamage(int t)
	{
		// �ʒu���� �h��Đ������o��l�q��\��
		const int ArraySize = 20;
		int y[ArraySize] =
		{
			-1, -3, -4, -5, -4, -3, -1, 1, 3, 7,
			11, 15, 22, 16, 13, 12, 13, 16, 22, 21
		};
		char damage[6];
		sprintf(damage, "%d", popDamage_[t].damage);
		int cy;
		// �_���[�W�̌����������[�v
		for (int i = 0; i < static_cast<int>(strlen(damage)); i++)
		{
			if (popDamage_[t].cnt - 4 * i < 0)
			{
				return;
			}
			if (popDamage_[t].cnt - 4 * i < ArraySize)
			{
				cy = y[popDamage_[t].cnt - 4 * i];
			}
			else
			{
				cy = 22;
			}
			// �������G�Ɩ����Ŕ����Ɉʒu���Ⴄ�ς���ׂ�
			// �|�b�v�_���[�W�`��
			DxLib::DrawGraph(popDamage_[t].x + 15 * i,
				popDamage_[t].y + cy - popDamage_[t].type * 14,
				rl_->getHdlImgNumber(2 + popDamage_[t].type, damage[i] - '0'),
				TRUE);
		}
	}

	void  BattleAnimation::animation0(int idx, AbstractBattleCharacter *c1,
		AbstractBattleCharacter *c2, int x, int y)
	{
		const int ComBlinkTime = 10;  // ����

		int animationTime[5] = { -15, 40, 60, 80, 100 };
		int eishoTime[5]     = {  40, 40, 60, 80, 100 };
		int blinkTime[5]     = { -20, 40, 60, 80, 100 };

		// �A�r���e�B4�̎����� ���ꏈ��
		if (c2->getAbility(4).flag)
		{
			for (int i = 1; i < 5; i++)
			{
				animationTime[i] /= 4;
				eishoTime[i] /= 4;
				blinkTime[i] /= 4;
			}
		}
		int kind = animation_[idx].kind % 10000 / 100;

		int t1 = animationTime[kind];
		int t2 = eishoTime[kind];
		int t3 = blinkTime[kind];
		animation_[idx].cnt++;

		bool cond_normal_attack = animation_[idx].kind == 10000;
		bool cond_magical_attack = 20000 <= animation_[idx].kind && animation_[idx].kind < 30000;


		if (animation_[idx].cnt == 5 && cond_normal_attack)
		{
			// �V���b
			DxLib::PlaySoundMem(rl_->getHdlSoundEffect(6), DX_PLAYTYPE_BACK);
		}
		// ���ׂĂ̎�ʂ̍U���Ŕ���
		if (animation_[idx].cnt == 5 + t2)
		{
			// ���ꂼ��̌��ʉ� ���� 100 �` 304�܂�
			DxLib::PlaySoundMem(
				rl_->getHdlSoundEffect(animation_[idx].kind / 100), DX_PLAYTYPE_BACK);
#ifdef _DEBUG
			DxLib::printfDx("sound idx: %d\n", animation_[idx].kind / 100);
#endif
			// �o�b�N�O���E���h�Đ� �炵�n�߂�Ƃ������̏����Ɉڂ�
		}
		// �r�����W�b�N
		if (animation_[idx].cnt == 5 && cond_magical_attack)
		{
			// �r����
			DxLib::PlaySoundMem(rl_->getHdlSoundEffect(5), DX_PLAYTYPE_LOOP);
		}
		// �r�������Đ����̏ꍇ
		if (DxLib::CheckSoundMem(rl_->getHdlSoundEffect(5)) == SEPlaying
			&& cond_magical_attack)
		{
			DxLib::DrawGraph(c2->getX() - 60 + DxLib::GetRand(2),
				c2->getY() - 35 + DxLib::GetRand(2),
				rl_->getHdlImgEtc(0), TRUE);
			DxLib::DrawBox(c2->getX() + 20,
				c2->getY() - 25,
				c2->getX() + 70,
				c2->getY() - 20,
				DxLib::GetColor(255, 255, 0), FALSE);
			DxLib::DrawBox(c2->getX() + 20,
				c2->getY() - 25,
				static_cast<int>(
				c2->getX() + 20
				+ (static_cast<double>(animation_[idx].cnt - 5) / (t3 + 20))
				* 50.0
				),
				c2->getY() - 20,
				DxLib::GetColor(255, 255, 0), TRUE);
		}
		if (animation_[idx].cnt == 25 + t2 && cond_magical_attack)
		{
			// �r���̏ꍇ stop
			DxLib::StopSoundMem(rl_->getHdlSoundEffect(5));
		}

		// �_�ŏ���
		for (int i = 0; i < 4; i++)
		{
			if (animation_[idx].cnt == 70 + t3 + ComBlinkTime * i)
			{
				c1->setGraphFlag(false);
			}
			else
			{
				c1->setGraphFlag(true);
			}
		}
		// ����A�j���[�V�����`��
		if (30 + t1 <= animation_[idx].cnt && animation_[idx].cnt < 70 + t1)
		{
			int kind2 = (animation_[idx].cnt - (30 + t1))
				/ 5 + (animation_[idx].kind % 10000) / 100 * 8;
			DxLib::DrawGraph(x, y, rl_->getHdlImgAnimation(1, kind2), TRUE);
		}

		if (animation_[idx].cnt == 70 + t1)  // ���� -> �����
		{
			animation_[idx].flag = false;
			c2->setActionFlag(true);    // �d�v
		}
		// �A�r���e�B4�̎�����
		if (c2->getAbility(4).flag)
		{
			for (int i = 1; i < 5; i++)
			{
				animationTime[i] *= 4;
				eishoTime[i] *= 4;
				blinkTime[i] *= 4;
			}
		}
	}

	// �L���A
	void  BattleAnimation::animation1(AbstractBattleCharacter *c, int idx, int x, int y)
	{
		if (animation_[idx].cnt == 0)
		{
			DxLib::PlaySoundMem(rl_->getHdlSoundEffect(200), DX_PLAYTYPE_BACK);
		}
		if (animation_[idx].cnt < 50)
		{
			DxLib::DrawGraph(c->getX() + x,
				c->getY() + y - animation_[idx].cnt,
				rl_->getHdlImgAnimation(0, 10 + animation_[idx].cnt / 5), TRUE);
		}
		animation_[idx].cnt++;

		if (animation_[idx].cnt == 50)
		{
			animation_[idx].flag = false;
			c->setActionFlag(true);
		}
	}

	// �A�r���e�B
	void  BattleAnimation::animation2(AbstractBattleCharacter *c, int idx, int x, int y)
	{
		if (animation_[idx].cnt == 0)
		{
			DxLib::PlaySoundMem(rl_->getHdlSoundEffect(c->getMenuSelect() / 100),
				DX_PLAYTYPE_BACK);
		}
		if (animation_[idx].cnt < 50)
		{
			DxLib::DrawGraph(c->getX() + x,
				c->getY() + y - animation_[idx].cnt,
				rl_->getHdlImgAnimation(0, 20 + animation_[idx].cnt / 10), TRUE);
		}
		animation_[idx].cnt++;

		if (animation_[idx].cnt == 50)
		{
			animation_[idx].flag = false;
			c->setActionFlag(true);
		}
	}

	void BattleAnimation::setPopDamage(int damage, int x, int y, int type)
	{
		int i = 0;
		for (i = 0; i < PopDamMax - 1; i++)   // 1���炷
		{
			if (!popDamage_[i].flag) { break; }
		}
		popDamage_[i].flag = true;
		popDamage_[i].cnt = 0;
		popDamage_[i].damage = damage;
		popDamage_[i].x = x;
		popDamage_[i].y = y;
		popDamage_[i].type = type;  // 1: cure, 0: others

		// �_���[�W�A�j���[�V������Ƀt���O�������Ă��܂��B
		// �܂�animationFlag�������Ă��炽�͗l�B
#ifdef _DEBUG
		DxLib::printfDx("i: %d, damage: %d\n", i, popDamage_[i].damage);
#endif
	}

}