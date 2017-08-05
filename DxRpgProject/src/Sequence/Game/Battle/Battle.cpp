#include "Sequence/Game/Battle/Battle.h"
#include "Util/GameController.h"
#include "Util/ResourceLoader.h"
#include "Sequence/Parent.h"
#include "Battle/BattleProcess.h"
#include "Battle/BattleCharacter.h"
#include "Battle/BattleMonster.h"

namespace Sequence
{
	namespace Game
	{
		namespace Battle
		{

			Battle::Battle(SharedCharacterStatus *scs)
			{
				gc_ = Util::GameController::getInstance();
				rl_ = Util::ResourceLoader::getInstance();
				chr_ = new ::Battle::BattleCharacter(scs);
				mon_ = new ::Battle::BattleMonster();
				// �L������ʂ��O����ݒ�
				chr_->getScs()->kind_ = 0;
				mon_->getScs()->kind_ = 0;

				// ���S���O����ݒ�
				int axisChrX, axisChrY;
				DxLib::GetGraphSize(
					rl_->getHdlImgChar(chr_->getScs()->kind_, 5),
					&axisChrX, &axisChrY);
				chr_->setCenterX(chr_->getX() + axisChrX / 2);
				chr_->setCenterY(chr_->getY() + axisChrY / 2);

				int axisMonX, axisMonY;
				DxLib::GetGraphSize(
					rl_->getHdlImgMonster(mon_->getScs()->kind_),
					&axisMonX, &axisMonY);
				mon_->setCenterX(mon_->getX() + axisMonX / 2);
				mon_->setCenterY(mon_->getY() + axisMonY / 2);

				bp_ = new ::Battle::BattleProcess(chr_, mon_);

				for (int i = 0; i < ImgCharDivNum; i++)
				{
					startBattleX_[i] = 0;
					startBattleY_[i] = 0;
					randX_[i] = 0;
					randY_[i] = 0;
				}

				nextSequence_ = NextBattleShatter;   // �ŏ��̓R��
			}

			Battle::~Battle()
			{
				delete chr_;
				delete mon_;
				delete bp_;
			}

			// �o�g���ɂ����āA�ǂ̊֐��ɏ�����n�����𐧌�
			void Battle::update(Sequence::Game::Parent *parent)
			{
				switch (nextSequence_)
				{
				case NextBattleShatter:     // ��ʂ�����ė�����V�[��
					startBattleShatter();
					break;
				case NextBattleEnter:       // �퓬�˓��V�[��
					startBattleEnter();
					break;
				case NextBattleMain:        // �퓬�V�[��
					bp_->main();
					if (bp_->getWinMoveNext())
					{
						nextSequence_ = NextMap;
					}
					if (bp_->getGameoverMoveNext())
					{
						nextSequence_ = NextRestart;
					}
					break;
				case NextBattleDebug:       // DEBUG�p
					//debugBattleMode();
					break;
				case NextMap:
					parent->moveTo(Parent::NextMapMain);
					break;
				case NextRestart:
					parent->moveTo(Parent::NextRestart);
					break;
				default:
					exit(EXIT_FAILURE);
					break;
				}
			}

			void Battle::startBattleShatter()
			{
				const    double Accel = 1.5;
				const    int    ShatterNum = 8;
				const    int    DownSt = 60;
				const    int    DownEd = 150;
				static   int    cnt = 0;

				if (cnt == 0)
				{
					DxLib::PlaySoundMem(rl_->getHdlSoundEffect(0), DX_PLAYTYPE_BACK);
					// �ŏ��ɏ������s��ꂽ���ADivNum*DivNum�̉摜�̏������W��ݒ�
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							startBattleX_[i * DivisionNum + j] = WindWidth / DivisionNum * j;   // �`�悷��e�����W
							startBattleY_[i * DivisionNum + j] = WindHeight / DivisionNum * i;  // �`�悷��e�����W
							randX_[i * DivisionNum + j] = DxLib::GetRand(30);     // �΂炯�����邽�߂̗���
							randY_[i * DivisionNum + j] = DxLib::GetRand(30);     // �΂炯�����邽�߂̗���
						}
					}
				}
				// �ӂ��U�郂�[�h(�O�֎U��΂���)
				if (cnt < ShatterNum)
				{   // ShatterNum�܂ł̊Ԓ��S����O�֎U��΂点��
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							// �l�������ɔ�юU�点��
							startBattleX_[i * DivisionNum + j]
								+= (randX_[i * DivisionNum + j] + (30 * j) - 45) / 8.0;
							startBattleY_[i * DivisionNum + j]
								+= (randY_[i * DivisionNum + j] + (30 * i) - 45) / 8.0;
						}
					}
				}
				// �����鎞�̗�������
				if (cnt == ShatterNum)
				{    // �����鎞�ɂ΂炯�����邽�߂̗�������
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							randX_[i * DivisionNum + j] = GetRand(40) / 10.0 - 2.0;
							randY_[i * DivisionNum + j] = GetRand(10) / 10.0 + 1.0;
						}
					}
				}

				// �����郂�[�h
				if (cnt >= DownSt && cnt <= DownEd)
				{
					// �����郂�[�V����
					double t = (cnt - DownSt) * 17 / 1000;
					for (int i = 0; i < DivisionNum; i++)
					{
						for (int j = 0; j < DivisionNum; j++)
						{
							startBattleX_[i * DivisionNum + j]
								+= randX_[i * DivisionNum + j];
							// �����鎞�����x�������Čv�Z�BAccel�̒l�ŕύX�\
							startBattleY_[i * DivisionNum + j]
								+= t * t * Accel * randY_[i * DivisionNum + j] / 2.0 * 10;
						}
					}
				}
				// 150�ɂȂ����玟�̃V�[����
				if (cnt == DownEd)
				{
					cnt = 0;
					nextSequence_ = NextBattleEnter;
					return;
				}
				// �����������ꂼ��`��
				for (int i = 0; i < ImgCharDivNum; i++)
				{
					DxLib::DrawGraph(static_cast<int>(startBattleX_[i]),
						static_cast<int>(startBattleY_[i]),
						rl_->getHdlImgStartBattle(i), FALSE);
				}
				cnt++;
			}

			void Battle::startBattleEnter()
			{
				const double StartBattleTime = 120.0;
				const int    Stage1 = 80;
				const int    Stage2 = 170;
				const int    Stage3 = 255;
				static int cnt = 0;
				//int   axisMonX = 0;
				//int   axisMonY = 0;

				if (cnt == 0)
				{   // �I�[�v�j���O�Q���n�܂�u�ԁA��U�S���`�悵�āA
					// ���̉�ʂ��L���v�`�����Ĕ��f������O�ɃN���A����B
					// ���̂P���̃L���v�`�������摜�ŃI�[�v�j���O�����
					DxLib::PlaySoundMem(rl_->getHdlBattleSound(0), DX_PLAYTYPE_LOOP);
					DxLib::DrawGraph(0, 0, rl_->getHdlImgBackGround(0), FALSE);

					// ��l���E�������摜�̓Ǎ� 0, 5
					DxLib::DrawGraph(::Battle::BattleCharPosX1, ::Battle::BattleCharPosY1,
						rl_->getHdlImgChar(chr_->getScs()->kind_, 5), TRUE);

					// �G�L�����Ǎ�
					DxLib::DrawGraph(::Battle::BattleMonPosX1, ::Battle::BattleMonPosY1,
						rl_->getHdlImgMonster(mon_->getScs()->kind_), TRUE);

					rl_->setImageStartBattle(0, WindWidth, WindHeight);

					// �摜�L���v�`��
					DxLib::GetDrawScreenGraph(0, 0, WindWidth, WindHeight,
						rl_->getHdlImgStartBattle(0));
					DxLib::ClearDrawScreen();
				}
				if (cnt > Stage1 && cnt <= Stage2)
				{
					DxLib::DrawRotaGraph(-50 + cnt, 100, 2.0, 0, rl_->getHdlImgStartBattle(0), FALSE);
				}
				if (cnt > Stage2 && cnt <= Stage3)
				{
					DxLib::DrawRotaGraph(500 - cnt, 400, 2.0, 0, rl_->getHdlImgStartBattle(0), FALSE);
				}
				if (cnt > Stage3 && cnt <= Stage3 + StartBattleTime)
				{
					int t = cnt - Stage3;
					DxLib::DrawExtendGraph(
						static_cast<int>((WindWidth / 2 + WindWidth / DivisionNum)
							- (WindWidth / 2 + WindWidth / DivisionNum)
							* t / StartBattleTime),
						0,
						static_cast<int>(WindWidth *t / StartBattleTime),
						static_cast<int>(WindHeight * t / StartBattleTime),
						rl_->getHdlImgStartBattle(0),
						TRUE
					);
				}
				if (cnt == Stage3 + StartBattleTime)
				{    // ���ԂɂȂ�����I�[�v�j���O���I�����Đ퓬��
					cnt = 0;
					// DEBUG
					//nextSequence_ = NextBattleDebug;
					nextSequence_ = NextBattleMain;
					return;
				}
				cnt++;
			}

			/*
			void Battle::debugBattleMode()
			{
				int        axisMonX = 0;
				int        axisMonY = 0;

				// �摜�`��̂��� START(���ꂪ�Ȃ��Ɛ^���Â�)
				DxLib::DrawGraph(0, 0, rl_->getHdlImgBackGround(0), FALSE);
				DxLib::GetGraphSize(rl_->getHdlImgMonster(monsterKind_), &axisMonX, &axisMonY);
				// �����Z���^�����O
				DxLib::DrawGraph(WindWidth / 3 - axisMonX / 2,
					WindHeight / 2 - axisMonY / 2,
					rl_->getHdlImgMonster(monsterKind_), TRUE);
				// ��l���E�������摜�̓Ǎ� 0, 5
				DxLib::DrawGraph(BattleCharPosX, BattleCharPosY, rl_->getHdlImgChar(0, 5), TRUE);
				// �摜�`��̂��� END  (���ꂪ�Ȃ��Ɛ^���Â�)

				if (gc_->getKey(KEY_INPUT_RETURN) >= 1)
				{
					//�o�g���̉��y������Ă�����~�߂�
					if (DxLib::CheckSoundMem(rl_->getHdlBattleSound(0)) == 1)
					{
						DxLib::StopSoundMem(rl_->getHdlBattleSound(0));
					}
					nextSequence_ = NextMap;
					return;
				}
				nextSequence_ = NextBattleDebug;
			}
			*/

		}
	}
    
}
