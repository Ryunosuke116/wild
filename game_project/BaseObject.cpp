#include "common.h"
#include "BaseObject.h"

/// <summary>
/// �C���X�^���X
/// </summary>
BaseObject::BaseObject()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BaseObject::~BaseObject()
{

}

/// <summary>
/// ������
/// </summary>
void BaseObject::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void BaseObject::Update()
{

}

/// <summary>
/// �`��
/// </summary>
bool BaseObject::Draw()
{
	//MV1SetWireFrameDrawFlag(modelHandle,TRUE);
	MV1DrawModel(modelHandle);
	return true;
}
