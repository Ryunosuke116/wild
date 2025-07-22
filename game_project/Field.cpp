#include "common.h"
#include "Field.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field(const char* path)
{
	modelHandle = MV1LoadModel(path);

	position = VGet(0, 0, 0);
	//���f���̑傫������
	MV1SetScale(modelHandle, VGet(0.9f, 0.9f, 0.9f));

	MV1SetPosition(modelHandle, position);


}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Field::~Field()
{

}

/// <summary>
/// ������
/// </summary>
void Field::Initialize()
{
	// ���f���̂O�Ԗڂ̃t���[���̃R���W���������\�z
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);

	isPoly = false;

}

/// <summary>
/// �X�V
/// </summary>
void Field::Update()
{

}

/// <summary>
/// �`��
/// </summary>
bool Field::Draw()
{
	if (isPoly)
	{
		MV1SetWireFrameDrawFlag(modelHandle, TRUE);
	}
	else
	{
		MV1SetWireFrameDrawFlag(modelHandle, FALSE);
	}

	MV1DrawModel(modelHandle);

	//MV1DrawModel(meshHandle);
	return true;
}