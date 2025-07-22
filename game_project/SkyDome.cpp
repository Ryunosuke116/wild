#include "common.h"
#include "SkyDome.h"
#include "BaseObject.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SkyDome::SkyDome(const char* path)
{
	modelHandle = MV1LoadModel(path);
	MV1SetScale(modelHandle, VGet(2.0f, 2.0f, 2.0f));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SkyDome::~SkyDome()
{

}

/// <summary>
/// ������
/// </summary>
void SkyDome::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void SkyDome::Update(const VECTOR& playerPos)
{
	MV1SetPosition(modelHandle, playerPos);
}

void SkyDome::Update() {};

/// <summary>
/// �`��
/// </summary>
bool SkyDome::Draw()
{
	MV1DrawModel(modelHandle);
	return true;
}