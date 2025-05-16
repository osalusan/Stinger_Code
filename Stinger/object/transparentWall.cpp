#include "transparentWall.h"
#include "component/boxCollisionComponent.h"
#include "manager/objModelManager.h"

TransparentWall::TransparentWall(const XMFLOAT3& pos, const XMFLOAT3& scale)
{
	m_Position = pos;
	m_Scale = scale;
	m_BoxCollCache = AddComponent<BoxCollisionComponent>(this, COLLISION_TAG::OBJECT);
}

TransparentWall::TransparentWall(const XMFLOAT3& pos, const XMFLOAT3& scale, const XMFLOAT3& rot)
	:TransparentWall(pos,scale)
{
	m_Rotation = rot;
}

void TransparentWall::Init()
{
	// ���f���������̂ŁA�ˋ�̃��f�����g�p
	// GameScene�̍ő�s���͈͂ɍ��킹��ׁA�X�P�[����2�{�ɂ��đΉ�
	float customScale = DEFAULT_MODELSCALE * 2.0f;
	m_BoxCollCache->SetCollisionInfo(m_Position, m_Scale, { 0.0f,0.0f,0.0f }, { customScale,customScale,customScale }, GetRotationMatrix());
}
