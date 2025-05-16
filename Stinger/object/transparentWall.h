#pragma once
#include "object/gameObject.h"

// �O���錾
class BoxCollisionComponent;
// �����蔻��̂��Ă��鋫�E���p�̓����ȕ�
class TransparentWall :public GameObject
{
private:
	BoxCollisionComponent* m_BoxCollCache = nullptr;		// �����ȕ� / �f�o�b�O��ʂł͕\�������
 public:
	 TransparentWall(const XMFLOAT3& pos,const XMFLOAT3& scale);
	 TransparentWall(const XMFLOAT3& pos,const XMFLOAT3& scale,const XMFLOAT3& rot);

	 virtual void Init()override;
};