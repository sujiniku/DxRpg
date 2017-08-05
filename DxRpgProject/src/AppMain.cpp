#include "StdCommon.h"
#include "Util/GameController.h"
#include "Util/ResourceLoader.h"
#include "Sequence/Parent.h"
#include "Resource.h"

// ---------------------------------------------------------------
// �G���g���|�C���g
// ---------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#ifdef _DEBUG
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); // ���������[�N���o
#endif   
    DxLib::SetWindowTextA("DX RPG");    // �^�C�g�����Z�b�g
    SetWindowIconID(ID_ICON);           // �A�C�R�����Z�b�g
    // �E�B���h�E���[�h�ɃZ�b�g�A�}�E�X��\��
    if ( DxLib::ChangeWindowMode(TRUE) != DX_CHANGESCREEN_OK
        || DxLib::DxLib_Init() == -1
        || DxLib::SetWindowSizeExtendRate(1.0) == -1
        || DxLib::SetMouseDispFlag(FALSE) == -1)
    {
        return -1;
    }
    // ����ʕ`��ɂ���
    if ( DxLib::SetDrawScreen(DX_SCREEN_BACK) == -1 )
    {
        return -1;
    }
    // ���[�e�B���e�B�N���X�̃C���X�^���X���擾
    Util::ResourceLoader   *rl = Util::ResourceLoader::getInstance();
	Util::GameController   *gc = Util::GameController::getInstance();
    Sequence::Parent *parent = Sequence::Parent::getInstance();
    if ( rl->load() == -1 )            // �摜�Ǎ���
    {
        return -1;
    }
    
    // ���b�Z�[�W���[�v
    while( DxLib::ProcessMessage() == 0
        && DxLib::ClearDrawScreen() == 0
        && gc->getAllKeyPressed() == 0
        && gc->getKey(KEY_INPUT_ESCAPE) == 0 )
    {
        gc->adjustKeyState();
        parent->update();
        gc->control();      
        DxLib::ScreenFlip();     // ����ʂ𔽉f
    }
    DxLib::DxLib_End();
    
    return 0;
}
