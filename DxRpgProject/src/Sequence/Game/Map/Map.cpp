#include "Sequence/Game/Map/Map.h"
#include "Sequence/Game/Parent.h"
#include "Map/AbstractMapCharacter.h"
#include "Util/ResourceLoader.h"
#include "Util/CsvMapReader.h"


//#define FIXED_      // ��l���̍��W���Œ肳��Ȃ����[�h

namespace Sequence
{
	namespace Game
	{
		namespace Map
		{
			// �}�b�v��ʂ̏���������
			Map::Map(Sequence::Game::Parent *parent, int stage)
				: hasChanged_(true), mapStage_(stage), nextSequence_(NextMap)
			{
				for (int i = 0; i < YBlock * XBlock; i++)
				{
					isPassable_[i] = Through;
				}

				parent_ = parent;
				cmr_ = new Util::CsvMapReader();
				rl_ = Util::ResourceLoader::getInstance();
			}

			Map::~Map()
			{
				delete cmr_;
			}

			void Map::update(Sequence::Game::Parent *parent)
			{
				UNREFERENCED_PARAMETER(parent);
				// ���y�Đ�
				if (DxLib::CheckSoundMem(rl_->getHdlMapSound(0)) == 0)
				{
					DxLib::PlaySoundMem(rl_->getHdlMapSound(0), DX_PLAYTYPE_LOOP);
				}
				if (hasChanged_)
				{
					initMapState();
					initCharState();
				}
				else
				{
					// �}�b�v�̕`��
					drawMap();
					// �L�����N�^�̕`��
					drawCharacter();
				}
				// �}�b�v�ʒu�̌v�Z�y�ѕ��s��Ԃ̌���
				moveCharacter();
				// �}�b�v�ύX����
				changeMap();
			}

			// �}�b�v��ʂ̏���������
			void Map::initMapState()
			{
				cmr_->load(MapFile[mapStage_]);
				// �}�b�v�̏�Ԃ���ʉ߃t���O���쐬
				for (int y = 0; y < YBlock; y++)            // �c��15���
				{
					for (int x = 0; x < XBlock; x++)        // ����20���
					{
						if (cmr_->getMapData(x, y) > (0 | 1))    // �ʂ�Ȃ��ꏊ�Ȃ�
						{
							isPassable_[y * XBlock + x] = NoThrough;    // �ʂ�Ȃ��t���O�𗧂Ă�
						}
						// �ʂ��ꏊ�Ȃ�
						else
						{
							// �l�� + ���̈�����Ȃ�(���ꂪ�ړ����ɖ�����d�Ȃ�o�O����������i)
							// ��肷���������邪�܂�������
							if (y == 0 || y == 1 || y == YBlock - 2 || y == YBlock - 1
								|| x == 0 || x == 1 || x == XBlock - 2 || x == XBlock - 1)
							{   // ���L��������ʑJ�ڂ��Ȃ��悤�ɂ���
								isPassable_[y * XBlock + x] = MainCharOnly;
							}
							else
							{
								isPassable_[y * XBlock + x] = Through;
							}
						}
					}
				}
			}

			void Map::initCharState()
			{
				int ty;     // �v�Z�pY���W
				int tx;     // �v�Z�pX���W
				int dir;    // ����
				int kind;   // �L�����N�^�̎��
				// �L�����N�^�[��z�u����B
				for (int i = 0; i < MainCharMax + CompCharMax;)
				{
					// ��l��
					if (i == MainCharIndex)
					{
						tx = parent_->getCharacter(MainCharIndex)->getX();
						ty = parent_->getCharacter(MainCharIndex)->getY();
						dir = parent_->getCharacter(MainCharIndex)->getDir();
						kind = Down;
					}
					// ���̐l
					else
					{
						// �l���Ƃ��̂Q����(-4���炳���-1���Ă��邱�Ƃɒ���[GetRand�̓���])�������
						int rx = DxLib::GetRand(XBlock - 5) + 2;
						int ry = DxLib::GetRand(YBlock - 5) + 2;
						// ���̐l�ŗ����Ō��߂��ꏊ���ʂ�Ȃ��ꏊ�Ȃ�
						if (isPassable_[ry * XBlock + rx] >= MainCharOnly)
						{
							continue;    // ��蒼��
						}
						tx = rx * BlockLen;
						ty = ry * BlockLen;
						dir = DxLib::GetRand(3);        // �����������_���Ɍ��� 0-3
						kind = i % (CharKindMax - 1) + 1;        // �l�̎�ނ����� ��0�͎�l��  1-3
					}

					parent_->getCharacter(i)->initMapState(tx, ty, dir, kind);
					// ��l�������̐l������Ƃ���͒ʂ�Ȃ��ꏊ�Ƃ���
					isPassable_[ty / BlockLen * XBlock + tx / BlockLen] = NoThrough;
					i++;    // �ʒu���߂����������Ƃ��̂݃J�E���^����
				}
				hasChanged_ = false;
			}


			// �L�����̈ړ�����
			void Map::moveCharacter()
			{
				for (int i = 0; i < MainCharMax + CompCharMax; i++)
				{
					// �G�ɑ���
					if (parent_->getCharacter(i)->move(isPassable_))
					{
						// �G�ɑ����������ʃL���v�`�������y��~�����[�h�J��
						// ���̃^�C�~���O�ŌĂ΂Ȃ��ƃL���v�`���ł��Ȃ�(ClearDrawScreen�̑O)
						rl_->captureImgMap();
						DxLib::StopSoundMem(rl_->getHdlMapSound(0));
						// �J�ڐ�ݒ�
						parent_->moveTo(Parent::NextBattle);
					}
					// �����C���[�W���Z�b�g
					parent_->getCharacter(i)->setImg(
						rl_->getHdlImgChar(
							parent_->getCharacter(i)->getKind(),
							parent_->getCharacter(i)->getAnimePicPos()
						)
					);
				}
			}

			// �}�b�v�؂�ւ�
			void Map::changeMap()
			{
				if (parent_->getCharacter(MainCharIndex)->getX() <= 0
					|| parent_->getCharacter(MainCharIndex)->getY() <= 0
					|| parent_->getCharacter(MainCharIndex)->getX() >= MapWidth - BlockLen
					|| parent_->getCharacter(MainCharIndex)->getY() >= MapHeight - BlockLen)
				{
					// �E�ɏo����
					if (parent_->getCharacter(MainCharIndex)->getX() >= MapWidth - BlockLen)
					{
						// y���̂܂܂ň�ԍ��̈�E��
						parent_->getCharacter(MainCharIndex)->setX(BlockLen);
						// �}�b�v�ύX
						mapStage_++;
					}
					// ���ɏo����
					if (parent_->getCharacter(MainCharIndex)->getX() <= 0)
					{
						// y���̂܂܂ň�Ԃ̉E�����
						parent_->getCharacter(MainCharIndex)->setX(MapWidth - BlockLen * 2);
						// �}�b�v�ύX
						mapStage_--;
					}
					// �K�v������Ύ���
					// ��A��
					// �}�b�v�ύX
					hasChanged_ = true;
					parent_->setMapStage(mapStage_);
				}
			}

			// �}�b�v�̕`��
#ifdef FIXED_
			void Map::drawMap()
			{
				for (int y = 0; y < YBlock; y++)        // �c�̋�Ԍ������[�v
				{
					for (int x = 0; x < XBlock; x++)    // ���̋�Ԍ�
					{
						// ��������Ő���`��
						DxLib::DrawGraph(
							x * BlockLen - BlockLen / 2,    // �[�����[�Ȃ��ߔ������炷
							y * BlockLen,
							rl_->getHdlImgMapchip(0), TRUE);
						// �{���A�Ő��łȂ����̉����̕`��ł����
						if (cmr_->getMapData(x, y) != 0)
						{                                // ���̉摜��`��
							DxLib::DrawGraph(
								x * BlockLen - BlockLen / 2,
								y * BlockLen,
								rl_->getHdlImgMapchip(cmr_->getMapData(x, y)), TRUE);
						}
					}
				}
			}
#else
			void Map::drawMap()
			{
				for (int y = 0; y < YBlock; y++)        // �c�̋�Ԍ������[�v
				{
					// ��l���𒆐S�Ƃ�����W�ŉ摜�̍��[/��[�J�n�_�����߂�B
					int ofsY = WindHeight / 2 - BlockLen / 2 - parent_->getCharacter(MainCharIndex)->getY();     // ��l�����΍��W
					for (int x = 0; x < XBlock; x++)    // ���̋�Ԍ�
					{
						int ofsX = WindWidth / 2 - BlockLen / 2 - parent_->getCharacter(MainCharIndex)->getX();  // ��l�����΍��W
						// ��������Ő���`��
						DxLib::DrawGraph(
							x * BlockLen + ofsX,    // �`�摊�Έʒu
							y * BlockLen + ofsY,                   // �`�摊�Έʒu
							rl_->getHdlImgMapchip(0), TRUE);
						// �{���A�Ő��łȂ����̉����̕`��ł����
						if (cmr_->getMapData(x, y) != 0)
						{                                // ���̉摜��`��
							DxLib::DrawGraph(
								x * BlockLen + ofsX,
								y * BlockLen + ofsY,
								rl_->getHdlImgMapchip(cmr_->getMapData(x, y)), TRUE);
						}
					}
				}
			}
#endif

			// �L�����N�^�̕`��
#ifdef FIXED_
			void Map::drawCharacter()
			{
				// �L�������X���C�h���Ă��镪�A�d�Ȃ���l�����ďォ��`��
				for (int y = 0; y < YBlock; y++)
				{
					for (int i = 0; i < MainCharMax + CompCharMax; i++)
					{
						int ty = parent_->getCharacter(i)->getY();
						// �������Ă���Ƃ���Ɠ����͈͂̍��W�Ȃ�`��
						// �������Ȃ��Ǝ�O�̃L�������O�ɕ`�悳��Ȃ��i�d�Ȃ肪���������Ȃ�j
						// ��ʉ����珇�ɕ`�悷�邱�̕��@��Z�\�[�g�Ƃ���
						if (y == ty / BlockLen)
						{
							int tx = parent_->getCharacter(i)->getX();
							// x,y�͍��㒸�_�̍��W�ŁA
							// x:�}�b�v��1��x���W�ɒǉ����Ă��邽�߂̂��炵
							DxLib::DrawGraph(
								tx - BlockLen / 2,
								// �L�������c����48pixcel�̂��߁A
								// 32pixel����͂ݏo��16pixel����ɂ��炵�Ă���B
								ty - SlidePicVal, parent_->getCharacter(i)->getImg(), TRUE);
						}
					}
				}
			}
#else
			void Map::drawCharacter()
			{
				// �L�������X���C�h���Ă��镪�A�d�Ȃ���l�����ďォ��`��
				for (int y = 0; y < YBlock; y++)
				{
					for (int i = 0; i < MainCharMax + CompCharMax; i++)
					{
						int ty = parent_->getCharacter(i)->getY();
						// �������Ă���Ƃ���Ɠ����͈͂̍��W�Ȃ�`��
						// �������Ȃ��Ǝ�O�̃L�������O�ɕ`�悳��Ȃ��i�d�Ȃ肪���������Ȃ�j
						// ��ʉ����珇�ɕ`�悷�邱�̕��@��Z�\�[�g�Ƃ���
						if (y == ty / BlockLen)
						{
							if (i == 0)
							{
								// ��l��
								// �������Ă���Ƃ���Ɠ����͈͂̍��W�Ȃ�`��
								// �^�񒆂���̑��Έʒu
								DxLib::DrawGraph(
									WindWidth / 2 - BlockLen / 2,    // ���S
									// �L�������c����48pixcel�̂��߁A
									// 32pixel����͂ݏo��16pixel����ɂ��炵�Ă���B
									WindHeight / 2 - BlockLen / 2 - SlidePicVal,  // ���S + �X���C�h��
									parent_->getCharacter(MainCharIndex)->getImg(), TRUE);
							}
							else
							{
								// ��l���ȊO
								// �^�񒆂���̑��Έʒu
								int tx = parent_->getCharacter(i)->getX();
								int ofsY = WindHeight / 2 - BlockLen / 2 - parent_->getCharacter(MainCharIndex)->getY();  // ��l�����΍��W
								int ofsX = WindWidth / 2 - BlockLen / 2 - parent_->getCharacter(MainCharIndex)->getX();  // ��l�����΍��W
								// x,y�͍��㒸�_�̍��W�ŁA
								// x:�}�b�v��1��x���W�ɒǉ����Ă��邽�߂̂��炵
								// ��l�����������������炷
								DxLib::DrawGraph(
									tx + ofsX,
									// �L�������c����48pixcel�̂��߁A
									// 32pixel����͂ݏo��16pixel����ɂ��炵�Ă���B
									ty + ofsY - SlidePicVal,     // �u���b�N���̂��炵�̓L�����ɂ���
									parent_->getCharacter(i)->getImg(), TRUE);
							}
						}
					}
				}
			}
#endif
		}
	}
}
