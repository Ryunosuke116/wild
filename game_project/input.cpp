#include "common.h"
#include "Input.h"
#include "Calculation.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Input::Input()
	: nowFrameInput(0),
	nowFrameNewInput(0),
	joyPad(new JoyPad{ 0.0f, 0.0f, 0.0f, 0.0f }),
	joyPad_old(new JoyPad{ 0.0f, 0.0f, 0.0f, 0.0f })
{
	SetJoypadDeadZone(DX_INPUT_KEY_PAD1, 0.4f);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Input::~Input()
{
	// �����Ȃ�
}

/// <summary>
/// �X�V
/// </summary>
void Input::Update()
{
	// �ЂƂO�̃t���[���̓��͂�ϐ��ɂƂ��Ă���
	int Old = nowFrameInput;

	// ���݂̓��͏�Ԃ��擾
	nowFrameInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ���̃t���[���ŐV���ɉ����ꂽ�{�^���̃r�b�g���������Ă���l�� nowFrameNewInput �ɑ������
	nowFrameNewInput = nowFrameInput & ~Old;

	int input_x_left;
	int input_y_left;
	int input_x_Right;
	int input_y_Right;

	//���X�e�B�b�N
	GetJoypadAnalogInput(&input_x_left, &input_y_left, DX_INPUT_KEY_PAD1);
	//�E�X�e�B�b�N
	GetJoypadAnalogInputRight(&input_x_Right, &input_y_Right, DX_INPUT_KEY_PAD1);

	joyPad_old->input_x_left = joyPad->input_x_left;
	joyPad_old->input_y_left = joyPad->input_y_left;

	joyPad->input_x_left = static_cast<float>(input_x_left);
	joyPad->input_y_left = static_cast<float>(input_y_left);
	joyPad->input_x_Right = static_cast<float>(input_x_Right);
	joyPad->input_x_Right = static_cast<float>(input_x_Right);



}