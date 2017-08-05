#pragma once
#include "StdCommon.h"

namespace
{
    const int    MapchipNum = 4;     // �}�b�v�`�b�v��
    const int    BackGroundNum = 20;    // �w�i�̐�
    const int    MonsterNum = 10;
    const int    BattleSoundNum = 2;
    const int    MapSoundNum = 2;
    const int    SoundEffectNum = 400;   // �Ƃ肠�������ꂾ������Α��v
    const int    ImgMainIconNum = 5;
    const int    ImgIconKind = 5;
    const int    ImgIconDivNum = 5;
    const int    ImgNumberKind = 5;
    const int    ImgNumberDivNum = 10;
    const int    ImgAnimationKind = 5;
    const int    ImgAnimationDivNum = 40;
    const int    ImgEtcNum = 5;
}

namespace Util
{
	class ResourceLoader
	{
	public:
		// ������R�s�[���֎~
		ResourceLoader(const ResourceLoader&) = delete;
		ResourceLoader& operator=(const ResourceLoader&) = delete;

		static ResourceLoader *getInstance();

		int    load();
		void   captureImgMap();
		int    getHdlImgChar(int kind, int pos) const;
		int    getHdlImgMapchip(int kind) const;
		int    getHdlImgBackGround(int kind) const;
		int    getHdlImgMonster(int kind) const;
		int    getHdlMapSound(int kind) const;
		int    getHdlBattleSound(int kind) const;
		int    getHdlSoundEffect(int kind) const;
		int    getHdlImgStartBattle(int kind) const;
		void   setImageStartBattle(int idx, int width, int height);

		int    getHdlImgMainIcon(int kind) const;
		int    getHdlImgIcon(int kind, int pos) const;
		int    getHdlImgNumber(int kind, int pos) const;
		int    getHdlImgAnimation(int kind, int pos) const;
		int    getHdlImgEtc(int kind) const;
		int    getHdlImgCursor() const;
		int    getHdlFont(int kind) const;
		int    getHdlNullImg() const;

	private:
		ResourceLoader() {};
		~ResourceLoader() {};
		void dispProgress();

		int  hdlImgMapchip_[MapchipNum];            // �}�b�v�`�b�v(32x32pixel)�i�[�p
		int  hdlImgChar_[CharKindMax][ImgCharDivNum];   // �L�����摜
		int  hdlImgBackGround_[BackGroundNum];
		int  hdlImgMonster_[MonsterNum];
		int  hdlMapSound_[MapSoundNum];
		int  hdlBattleSound_[BattleSoundNum];
		int  hdlSoundEffect_[SoundEffectNum];
		int  hdlImgStartBattle_[ImgCharDivNum];

		int  hdlImgMainIcon_[ImgMainIconNum];
		int  hdlImgIcon_[ImgIconKind][ImgIconDivNum];
		int  hdlImgNumber_[ImgNumberKind][ImgNumberDivNum];
		int  hdlImgAnimation_[ImgAnimationKind][ImgAnimationDivNum];
		int  hdlImgEtc_[ImgEtcNum];
		int  hdlImgCursor_;

		int  hdlFont_[2];

	};
}