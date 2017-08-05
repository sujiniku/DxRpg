#include "Map/MapCityCharacter.h"

namespace Map
{
	void MapCityCharacter::initMapState(int tx, int ty, int dir, int kind)
	{
		x_ = tx;
		y_ = ty;
		flag_ = true;        // �L��������
		dir_ = dir;          // �L�����̂ނ��Ă�����������ɃZ�b�g D:0,L:1,R:2,U:3
		imgFlag_ = true;     // �`�悷��
		kind_ = kind;        // ���̐l�̎��
		walkPixel_ = 0;
		walkFlag_ = false;
		img_ = 0;
		walkSpeed_ = DefWalkSpeed;
		animePicPos_ = dir * 4;
		stop();                      // �����O
	}

	bool MapCityCharacter::move(int *isPassable)
	{
		setPosition();                // �L�����N�^�ʒu�̌���
		// x��y�����W���P���̔{���̂Ƃ�
		if (x_ % BlockLen == 0 && y_ % BlockLen == 0)
		{
			stop();                    // �����J�E���^��߂�
			if (GetRand(WalkRand) == 0)        // �����n�߂�
			{
				dir_ = GetRand(3);    // �����_���ɕ��������� 0-3
				int cy = y_ / BlockLen;
				int cx = x_ / BlockLen;
				int ny = y_ / BlockLen + OffsetY[dir_];
				int nx = x_ / BlockLen + OffsetX[dir_];

				if (canMove(isPassable[ny * XBlock + nx]))
				{
					// ���݈ʒu��ʂ����̂ɂ��āA�������ʒu��ʂ��Ȃ����̂Ƃ���
					walkFlag_ = true;
					isPassable[cy * XBlock + cx] = Through;
					isPassable[ny * XBlock + nx] = NoThrough;
				}
				else    // �����Ă�������ʂ�Ȃ��ꏊ�Ȃ�
				{
					walkFlag_ = false;
				}
			}
		}
		else    // ��y�����W���P���̔{���łȂ��Ƃ�
		{                        // ���dir�̕����֕���
			walk();
		}
		animePicPos_ = decideAnimePic(walkPixel_, dir_);
		return false;   // ���false��Ԃ�(�G���J�E���g���Ȃ�)
	}
}
