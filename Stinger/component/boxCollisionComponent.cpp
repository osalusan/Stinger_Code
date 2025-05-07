#include "boxCollisionComponent.h"
#include "object/gameObject.h"
#include "renderer/renderer.h"
#include "manager/objModelManager.h"

void BoxCollisionComponent::GetMyObb(OBB& myObb)
{
	// �␳���ꂽ���_���v�Z
	XMVECTOR correctedOrigin = XMVectorSet(
		m_ModelCenter.x * m_Scale.x,
		m_ModelCenter.y * m_Scale.y,
		m_ModelCenter.z * m_Scale.z,
		0.0f);

	// �I�u�W�F�N�g�̉�]��␳���ꂽ���_�ɓK�p
	correctedOrigin = XMVector3TransformCoord(correctedOrigin, m_RotationMatrix);

	// OBB�̒��S���v�Z
	const XMVECTOR& myCenter = XMVectorSet(
		m_Position.x,
		m_Position.y,
		m_Position.z,
		0.0f) + correctedOrigin;

	// OBB�̃T�C�Y�i�n�[�t�T�C�Y�j���v�Z
	const XMVECTOR& mySize = XMVectorSet(
		(m_ModelScale.x * m_Scale.x) * 0.5f,
		(m_ModelScale.y * m_Scale.y) * 0.5f,
		(m_ModelScale.z * m_Scale.z) * 0.5f,
		0.0f);

	// OBB�̎���ݒ�
	XMFLOAT3 axisX;
	XMFLOAT3 axisY;
	XMFLOAT3 axisZ;
	XMStoreFloat3(&axisX, m_RotationMatrix.r[0]);
	XMStoreFloat3(&axisY, m_RotationMatrix.r[1]);
	XMStoreFloat3(&axisZ, m_RotationMatrix.r[2]);

	myObb.Axis[0] = XMLoadFloat3(&axisX);
	myObb.Axis[1] = XMLoadFloat3(&axisY);
	myObb.Axis[2] = XMLoadFloat3(&axisZ);

	// OBB�ɒl��ݒ�
	myObb.Center = myCenter;
	myObb.Size = mySize;
}

void BoxCollisionComponent::Init()
{
	CollisionComponent::Init();
#if _DEBUG
	m_Model = STATICMESH_MODEL::COLLISION_BOX;
	ObjModelManager::ReservModel(m_Model, "asset\\model\\object\\boxCollision.obj");
#endif
}

void BoxCollisionComponent::Update()
{
	// �������z�֐��̈׋����
}

bool BoxCollisionComponent::CheckHitObject(const OBJECT& object)
{
	if (!CollisionComponent::CheckHitObject(object)) return false;

	if (m_GameObject == nullptr) return false;

	OBB myObb = {};

	GetMyObb(myObb);

	for (GameObject* object : m_GameObjectsCache[static_cast<int>(object)])
	{
		if (object == nullptr) continue;

		std::vector<BoxCollisionComponent*> boxCollisions = {};
		if (!object->GetComponents(boxCollisions)) continue;

		for (BoxCollisionComponent* boxCollision : boxCollisions)
		{
			if (boxCollision == nullptr) continue;
			if (m_GameObject == boxCollision->GetGameObject()) continue;

			OBB boxObb = {};


			const XMFLOAT3& boxPosition = boxCollision->GetPos();
			const XMFLOAT3& boxScale = boxCollision->GetScale();
			const XMMATRIX& boxRotationMatrix = boxCollision->GetRotationMatrix();

			const XMFLOAT3& boxModelCenter = boxCollision->GetModelCenter();
			const XMFLOAT3& boxModelScale = boxCollision->GetModelScale();

			// �␳���ꂽ���_���v�Z
			XMVECTOR boxCorrectedOrigin = XMVectorSet(
				boxModelCenter.x * boxScale.x,
				boxModelCenter.y * boxScale.y,
				boxModelCenter.z * boxScale.z,
				0.0f);

			// �{�b�N�X�̉�]��␳���ꂽ���_�ɓK�p
			boxCorrectedOrigin = XMVector3TransformCoord(boxCorrectedOrigin, boxRotationMatrix);

			// OBB�̒��S���v�Z
			const XMVECTOR& boxCenter = XMVectorSet(
				boxPosition.x,
				boxPosition.y,
				boxPosition.z,
				0.0f) + boxCorrectedOrigin;

			// OBB�̃T�C�Y�i�n�[�t�T�C�Y�j���v�Z
			const XMVECTOR& boxSize = XMVectorSet(
				(boxModelScale.x * boxScale.x) * 0.5f,
				(boxModelScale.y * boxScale.y) * 0.5f,
				(boxModelScale.z * boxScale.z) * 0.5f,
				0.0f);

			// OBB�̎���ݒ�
			XMFLOAT3 boxAxisX;
			XMFLOAT3 boxAxisY;
			XMFLOAT3 boxAxisZ;
			XMStoreFloat3(&boxAxisX, boxRotationMatrix.r[0]);
			XMStoreFloat3(&boxAxisY, boxRotationMatrix.r[1]);
			XMStoreFloat3(&boxAxisZ, boxRotationMatrix.r[2]);

			boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
			boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
			boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

			// OBB�ɒl��ݒ�
			boxObb.Center = boxCenter;
			boxObb.Size = boxSize;

			if (HitOBB(myObb, boxObb))
			{
				// MTV���v�Z
				m_Mtv = XMVectorScale(m_MtvAxis, m_MinOverlap);

				// ���������I�u�W�F�N�g���i�[
				SetHitObject(object);
			}
		}	
	}

	if (!m_HitGameObjectsCache.empty())
	{
		return true;
	}
	return false;
}

bool BoxCollisionComponent::CheckHitObject(const COLLISION_TAG& tag)
{
	if (!CollisionComponent::CheckHitObject(tag)) return false;

	if (m_GameObject == nullptr) return false;

	OBB myObb = {};

	GetMyObb(myObb);

	// ��ԍŏ��̃I�u�W�F�N�g���w��
	for (int i = static_cast<int>(OBJECT::CAMERA_MAIN); i < static_cast<int>(OBJECT::MAX); i++)
	{
		for (GameObject* object : m_GameObjectsCache[i])
		{
			if (object == nullptr) continue;

			if (m_BoxCollisionCaches.size() <= 0)
			{
				if (!object->GetComponents(m_BoxCollisionCaches)) continue;
			}

			for (BoxCollisionComponent* boxCollision : m_BoxCollisionCaches)
			{
				if (boxCollision == nullptr) continue;
				if (m_GameObject == boxCollision->GetGameObject()) continue;
				if (boxCollision->GetCollisionTag() != tag) continue;

				OBB boxObb = {};

				const XMFLOAT3& boxPosition = boxCollision->GetPos();
				const XMFLOAT3& boxScale = boxCollision->GetScale();
				const XMMATRIX& boxRotationMatrix = boxCollision->GetRotationMatrix();

				const XMFLOAT3& boxModelCenter = boxCollision->GetModelCenter();
				const XMFLOAT3& boxModelScale = boxCollision->GetModelScale();

				// �␳���ꂽ���_���v�Z
				XMVECTOR boxCorrectedOrigin = XMVectorSet(
					boxModelCenter.x * boxScale.x,
					boxModelCenter.y * boxScale.y,
					boxModelCenter.z * boxScale.z,
					0.0f);

				// �{�b�N�X�̉�]��␳���ꂽ���_�ɓK�p
				boxCorrectedOrigin = XMVector3TransformCoord(boxCorrectedOrigin, boxRotationMatrix);

				// OBB�̒��S���v�Z
				const XMVECTOR& boxCenter = XMVectorSet(
					boxPosition.x,
					boxPosition.y,
					boxPosition.z,
					0.0f) + boxCorrectedOrigin;

				// OBB�̃T�C�Y�i�n�[�t�T�C�Y�j���v�Z
				const XMVECTOR& boxSize = XMVectorSet(
					(boxModelScale.x * boxScale.x) * 0.5f,
					(boxModelScale.y * boxScale.y) * 0.5f,
					(boxModelScale.z * boxScale.z) * 0.5f,
					0.0f);

				// OBB�̎���ݒ�
				XMFLOAT3 boxAxisX;
				XMFLOAT3 boxAxisY;
				XMFLOAT3 boxAxisZ;
				XMStoreFloat3(&boxAxisX, boxRotationMatrix.r[0]);
				XMStoreFloat3(&boxAxisY, boxRotationMatrix.r[1]);
				XMStoreFloat3(&boxAxisZ, boxRotationMatrix.r[2]);

				boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
				boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
				boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

				// OBB�ɒl��ݒ�
				boxObb.Center = boxCenter;
				boxObb.Size = boxSize;

				if (HitOBB(myObb, boxObb))
				{
					// MTV���v�Z
					m_Mtv = XMVectorScale(m_MtvAxis, m_MinOverlap);

					// ���������I�u�W�F�N�g���i�[
					SetHitObject(object);
				}
			}
		}
	}
	if (!m_HitGameObjectsCache.empty())
	{
		return true;
	}
	return false;
}

bool BoxCollisionComponent::CheckHitAllObject()
{
	if (!CollisionComponent::CheckHitAllObject()) return false;

	if (m_GameObject == nullptr) return false;

	OBB myObb = {};

	GetMyObb(myObb);

	// ��ԍŏ��̃I�u�W�F�N�g���w��
	for (int i = static_cast<int>(OBJECT::CAMERA_MAIN); i < static_cast<int>(OBJECT::MAX); i++)
	{
		for (GameObject* object : m_GameObjectsCache[i])
		{
			if (object == nullptr) continue;

			if (m_BoxCollisionCaches.size() <= 0)
			{
				if (!object->GetComponents(m_BoxCollisionCaches)) continue;
			}

			for (BoxCollisionComponent* boxCollision : m_BoxCollisionCaches)
			{
				if (boxCollision == nullptr) continue;
				if (m_GameObject == boxCollision->GetGameObject()) continue;

				OBB boxObb = {};


				const XMFLOAT3& boxPosition = boxCollision->GetPos();
				const XMFLOAT3& boxScale = boxCollision->GetScale();
				const XMMATRIX& boxRotationMatrix = boxCollision->GetRotationMatrix();

				const XMFLOAT3& boxModelCenter = boxCollision->GetModelCenter();
				const XMFLOAT3& boxModelScale = boxCollision->GetModelScale();

				// �␳���ꂽ���_���v�Z
				XMVECTOR boxCorrectedOrigin = XMVectorSet(
					boxModelCenter.x * boxScale.x,
					boxModelCenter.y * boxScale.y,
					boxModelCenter.z * boxScale.z,
					0.0f);

				// �{�b�N�X�̉�]��␳���ꂽ���_�ɓK�p
				boxCorrectedOrigin = XMVector3TransformCoord(boxCorrectedOrigin, boxRotationMatrix);

				// OBB�̒��S���v�Z
				const XMVECTOR& boxCenter = XMVectorSet(
					boxPosition.x,
					boxPosition.y,
					boxPosition.z,
					0.0f) + boxCorrectedOrigin;

				// OBB�̃T�C�Y�i�n�[�t�T�C�Y�j���v�Z
				const XMVECTOR& boxSize = XMVectorSet(
					(boxModelScale.x * boxScale.x) * 0.5f,
					(boxModelScale.y * boxScale.y) * 0.5f,
					(boxModelScale.z * boxScale.z) * 0.5f,
					0.0f);

				// OBB�̎���ݒ�
				XMFLOAT3 boxAxisX;
				XMFLOAT3 boxAxisY;
				XMFLOAT3 boxAxisZ;
				XMStoreFloat3(&boxAxisX, boxRotationMatrix.r[0]);
				XMStoreFloat3(&boxAxisY, boxRotationMatrix.r[1]);
				XMStoreFloat3(&boxAxisZ, boxRotationMatrix.r[2]);

				boxObb.Axis[0] = XMLoadFloat3(&boxAxisX);
				boxObb.Axis[1] = XMLoadFloat3(&boxAxisY);
				boxObb.Axis[2] = XMLoadFloat3(&boxAxisZ);

				// OBB�ɒl��ݒ�
				boxObb.Center = boxCenter;
				boxObb.Size = boxSize;

				if (HitOBB(myObb, boxObb))
				{
					// MTV���v�Z
					m_Mtv = XMVectorScale(m_MtvAxis, m_MinOverlap);

					// ���������I�u�W�F�N�g���i�[
					SetHitObject(object);
				}
			}
		}
	}
	if (!m_HitGameObjectsCache.empty())
	{
		return true;
	}
	return false;
}
