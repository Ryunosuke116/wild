#include "common.h"
#include "EffekseerForDXLib.h"
#include "SceneManager.h"
#include "Game.h"
#include "nlohmann/json.hpp"

//��{�ݒ�
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{

    // ��ʃ��[�h�̃Z�b�g
    ChangeWindowMode(TRUE);
    SetGraphMode(1600, 900, 16);

    // DirectX11���g�p����悤�ɂ���B(DirectX9���A�ꕔ�@�\�s��)
    // Effekseer���g�p����ɂ͕K���ݒ肷��B
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    // �c�w���C�u��������������
    if (DxLib_Init() == -1)
    {
        return -1;	// �G���[���N�����璼���ɏI��
    }

    // Effekseer������������B
    // �����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��B
    if (Effekseer_Init(8000) == -1)
    {
        DxLib_End();
        return -1;
    }

    // �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
    // Effekseer���g�p����ꍇ�͕K���ݒ肷��B
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    // DX���C�u�����̃f�o�C�X���X�g�������̃R�[���o�b�N��ݒ肷��B
    // �E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����B
    // �������ADirectX11���g�p����ꍇ�͎��s����K�v�͂Ȃ��B
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();

    // �y�o�b�t�@��L���ɂ���
    SetUseZBuffer3D(TRUE);

    // �y�o�b�t�@�ւ̏������݂�L���ɂ���
    SetWriteZBuffer3D(TRUE);



    // �G�X�P�[�v�L�[��������邩�E�C���h�E��������܂Ń��[�v
    LONGLONG frameTime = 0;

    SceneManager* sceneManager = new SceneManager();

    sceneManager->Add<Game>("Game");

    while (!ProcessMessage() && !ClearDrawScreen() && !CheckHitKey(KEY_INPUT_ESCAPE))
    {
        frameTime = GetNowHiPerformanceCount();

        //�G�t�F�N�V�A��3D�ɓ���
        Effekseer_Sync3DSetting();

        //�X�V
        sceneManager->Update();

        //��ʂ̏�����
        ClearDrawScreen();

        //�`��
        sceneManager->Draw();

        // ����ʂ̓��e��\��ʂɔ��f������
        ScreenFlip();

        // �G��60fps�Œ�
        while ((GetNowHiPerformanceCount() - frameTime) < 16667) {}
    }

    // Effekseer���I������B
    Effkseer_End();

    // DX���C�u�������I������B
    DxLib_End();

    return 0;
}