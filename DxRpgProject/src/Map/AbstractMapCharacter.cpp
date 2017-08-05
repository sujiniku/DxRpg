#include "Map/AbstractMapCharacter.h"

namespace Map
{
	AbstractMapCharacter::AbstractMapCharacter()
		: x_(0), y_(0), walkPixel_(0), walkFlag_(false), dir_(0),
		flag_(0), img_(0), imgFlag_(0), kind_(0),
		animePicPos_(0), walkSpeed_(DefWalkSpeed)
	{
	}

	int AbstractMapCharacter::getX() const
	{
		return x_;
	}

	int AbstractMapCharacter::getY() const
	{
		return y_;
	}

	int AbstractMapCharacter::getImg() const
	{
		return img_;
	}

	bool AbstractMapCharacter::getImgFlag() const
	{
		return imgFlag_;
	}

	int AbstractMapCharacter::getDir() const
	{
		return dir_;
	}

	int AbstractMapCharacter::getKind() const
	{
		return kind_;
	}

	int AbstractMapCharacter::getAnimePicPos() const
	{
		return animePicPos_;
	}

	void AbstractMapCharacter::stop()
	{
		walkPixel_ = 0;
		walkFlag_ = false;
	}
	void AbstractMapCharacter::walk()
	{
		walkPixel_++;
	}

	void AbstractMapCharacter::setImg(int img)
	{
		img_ = img;
	}
	void AbstractMapCharacter::setImgFlag(bool imgFlag)
	{
		imgFlag_ = imgFlag;
	}

	void AbstractMapCharacter::setPosition()
	{
		if (walkFlag_)
		{
			// walkSpeed�����������ɑ΂��ăL�����̍��W�𑝌�����B
			x_ = x_ + OffsetX[dir_] * walkSpeed_;
			y_ = y_ + OffsetY[dir_] * walkSpeed_;
		}
	}

	int AbstractMapCharacter::decideAnimePic(int walkPixel, int dir)
	{
		int axisX;
		int axisY;
		// walkPixel��1�s�N�Z��������1����
		// 1���̂ǂ̂�����ɂ���̂��Ƃ����̂��A
		// 1����4���������l�ł���Ă��Ƃ܂�̂����̊G�ƂȂ�B
		// �A�j���[�V�����̂ǂꂩ�����i�摜��x����[1�����z��]�j
		// 0-31, /, 8 �W�ł���0-3�Ɏ��܂�
		axisX = (walkPixel % BlockLen) / 8;
		// �A�j���[�V�����̌��������(�摜��y����[1�����z��])
		axisY = dir * 4;
		// 0�`3�͉������摜�A4�`7�͍������摜�A
		// 8�`11�͉E�����摜�A12�`15�͏�����摜����Ɛ��l��ϊ�
		return axisX + axisY;
	}

	bool AbstractMapCharacter::canMove(int isPassable)
	{
		// ��l��
		if (kind_ == MainCharIndex)
		{
			return !(isPassable == NoThrough);
		}
		// ���L����
		else
		{
			return !(isPassable >= MainCharOnly);
		}
	}

	void AbstractMapCharacter::setX(const int x)
	{
		x_ = x;
	}
	void AbstractMapCharacter::setY(const int y)
	{
		y_ = y;
	}
}