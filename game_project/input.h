#pragma once
class Input
{
private:
	int		nowFrameInput;				// ���݂̃t���[���ŉ�����Ă���{�^��
	int		nowFrameNewInput;			// ���݂̃t���[���ŐV���ɉ����ꂽ�{�^��

	struct JoyPad
	{
		float input_x_left;
		float input_y_left;
		float input_x_Right;
		float input_y_Right;
	};

	JoyPad* joyPad;
public:

	Input();
	~Input();

	void Update();

	int GetNowFrameInput() const { return nowFrameInput; }
	int GetNowFrameNewInput() const { return nowFrameNewInput; }
	float GetJoyPad_x_left() const { return joyPad->input_x_left; }
	float GetJoyPad_y_left() const { return joyPad->input_y_left; }
	float GetJoyPad_x_right() const { return joyPad->input_x_Right; }
	float GetJoyPad_y_right() const { return joyPad->input_y_Right; }

};

