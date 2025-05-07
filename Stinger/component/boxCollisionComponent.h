#pragma once
#include "component/collisionComponent.h"

class BoxCollisionComponent final :public CollisionComponent
{
private:
	XMVECTOR m_Mtv = {};
	std::vector<BoxCollisionComponent*> m_BoxCollisionCaches = {};

	void GetMyObb(OBB& myObb);

public:
	using CollisionComponent::CollisionComponent;
	virtual void Init()override;
	virtual void Update()override;
	virtual bool CheckHitObject(const OBJECT& object)override;		// ��Ԍy��
	virtual bool CheckHitObject(const COLLISION_TAG& tag)override;
	virtual bool CheckHitAllObject()override;

	const XMVECTOR& GetMtv()
	{
		return m_Mtv;
	}
};