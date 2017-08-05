#include "Map/MapMainCharacter.h"
#include "DxLib.h"   // GetRand()
#include "Util/GameController.h"

namespace Map
{
	MapMainCharacter::MapMainCharacter()
		: prevX_(0), prevY_(0), encCnt_(0)
		//,x_(10 * BlockLen), y_(7 * BlockLen), dir_(Down), kind_(0)
		// note: �����I�Ȋ��̃����o�������͂ł��Ȃ�
	{
		// ��ꃁ���o�̈ꕔ������
		x_ = 10 * BlockLen;
		y_ = 7 * BlockLen;
		dir_ = Down;
		kind_ = 0;
	}

	MapMainCharacter::~MapMainCharacter()
	{
	}

	void MapMainCharacter::initMapState(int tx, int ty, int dir, int kind)
	{
		encCnt_ = 0;
		prevX_ = tx;
		prevY_ = ty;
		x_ = tx;
		y_ = ty;
		flag_ = true;
		dir_ = dir;  // �L�����̂ނ��Ă�����������ɃZ�b�g D:0,L:1,R:2,U:3
		imgFlag_ = true; kind_ = kind;
		walkPixel_ = 0; walkFlag_ = false; animePicPos_ = dir * 4;
		stop();            // �����O
	}

	bool MapMainCharacter::move(int *isPassable)
	{
		bool enc = false;
		// ���A���A�E�A��
		const int InputKeyDirection[4] =
		{
			KEY_INPUT_DOWN,
			KEY_INPUT_LEFT,
			KEY_INPUT_RIGHT,
			KEY_INPUT_UP,
		};
		Util::GameController* gc = Util::GameController::getInstance();

		setPosition();        // �L�����N�^�ʒu�̌���
		// x��y�����W���P���̃T�C�Y�̔{���̂Ƃ�
		if (x_%BlockLen == 0 && y_%BlockLen == 0)
		{
			// B�܂���F��������Ă���
			if (gc->getKey(KEY_INPUT_B) >= Util::GameController::Pressed
				|| gc->getKey(KEY_INPUT_F) >= Util::GameController::Pressed)
			{
				if (gc->getKey(KEY_INPUT_B) >= Util::GameController::Pressed)
				{
					walkSpeed_ = 2;
				}
				else
				{
					walkSpeed_ = 4;
				}
			}
			else
			{
				walkSpeed_ = 1;
			}

			// E��������Ă��Ȃ��Ƃ�
			if (gc->getKey(KEY_INPUT_E) == Util::GameController::NotPressed)
			{
				int cy = y_ / BlockLen;
				int cx = x_ / BlockLen;
				// �o�O����̂��߁A
				// �l���͈ړ��}�b�v�؂�ւ��𔺂��̂ł����ł̓G���J�E���g���Ȃ��悤�ɂ���
				const bool Condition
					= cy == 0 || cy == YBlock - 1 || cx == 0 || cx == XBlock - 1;
				if (!Condition) {
					// �G���J�E���g����
					enc = encountersEnemy();
				}
			}

			// �L�[��������ĂȂ��� (�L�[���͎�t����)
			if (gc->getKey(KEY_INPUT_LEFT) == Util::GameController::NotPressed
				&& gc->getKey(KEY_INPUT_UP) == Util::GameController::NotPressed
				&& gc->getKey(KEY_INPUT_RIGHT) == Util::GameController::NotPressed
				&& gc->getKey(KEY_INPUT_DOWN) == Util::GameController::NotPressed)
			{
				stop();                                // �����J�E���^��߂�
			}
			else                                    // �L�[������������Ă��鎞
			{
				for (int i = 0; i < DirNum; i++)                // �����̕����`�F�b�N
				{
					// ���̓L�[�̔���
					if (gc->getKey(InputKeyDirection[i]) >= Util::GameController::Pressed)
					{
						dir_ = i;    // ���͂���Ă���L�[�̕�����dir���Z�b�g����
						int cy = y_ / BlockLen;
						int cx = x_ / BlockLen;
						int ny = y_ / BlockLen + OffsetY[dir_];
						int nx = x_ / BlockLen + OffsetX[dir_];
						// �����Ă�������ʂ��ꏊ�Ȃ�
						if (canMove(isPassable[ny * XBlock + nx]))
						{
							walkFlag_ = true;
							// �������ꏊ�̏���
							// �l��+���̈�����Ȃ�(���ꂪ�ړ����ɖ�����d�Ȃ�o�O����������i) 
							const bool ConditionCur
								= cy == 0 || cy == 1 || cy == YBlock - 2 || cy == YBlock - 1
								|| cx == 0 || cx == 1 || cx == XBlock - 2 || cx == XBlock - 1;
							if (ConditionCur)
							{
								isPassable[cy * XBlock + cx] = MainCharOnly;    // ADD
							}
							else
							{
								isPassable[cy * XBlock + cx] = Through;
							}

							// ���̏ꏊ�̏���
							// ���̃}�X���l���Ȃ�
							const bool ConditionOuter
								= ny == 0 || ny == 1 || ny == YBlock - 2 || ny == YBlock - 1
								|| nx == 0 || nx == 1 || nx == XBlock - 2 || nx == XBlock - 1;
							if (ConditionOuter)
							{
								isPassable[ny * XBlock + nx] = MainCharOnly;
							}
							else
							{
								isPassable[ny * XBlock + nx] = NoThrough;
							}
						}
						else                    // �����Ă�������ʂ�Ȃ��ꏊ�Ȃ�
						{
							walkFlag_ = false;    // �����Ă��Ȃ���Ԃ�
						}
						break;    // �����ꂩ�̕����̏������I���΃��[�v�𔲂���
					}
				}
				walk();           // ���dir�̕����֕���
			}
		}
		else                      // x��y�����W���P���̃T�C�Y�̔{���łȂ��Ƃ�
		{
			walk();               // ���dir�̕����֕���
		}
		// ���֕`�悷��摜���Z�b�g
		animePicPos_ = decideAnimePic(walkPixel_, dir_);
		return enc;
	}

	bool MapMainCharacter::encountersEnemy()
	{
		bool enc = false;
		// �Œ���������Ȃ��Ɛ퓬�ɂȂ�Ȃ�
		if (encCnt_ >= EncMinWalkCnt)
		{    // ���m���őO�Ɠ������W�łȂ��Ȃ�퓬��
			if (DxLib::GetRand(EncRand) == 0
				&& (x_ != prevX_ || y_ != prevY_))
			{
				encCnt_ = 0;
				enc = true;
			}
		}
		// �O�Ɠ������W�ɂ��Ȃ��Ȃ�G�����J�E���g�𑝂₵�đO��ʒu���X�V����B
		if (x_ != prevX_ || y_ != prevY_)
		{
			encCnt_++;
			prevX_ = x_;
			prevY_ = y_;
		}
		return enc;
	}
}
