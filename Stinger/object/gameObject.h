#pragma once
#include "main/main.h"
#include <string>

// �O���錾
class BoxCollisionComponent;
class Component;
enum class COLLISION_TAG;
// �Q�[�����Ŏg�p����S�ẴI�u�W�F�N�g�̊��N���X
class GameObject
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };
	XMFLOAT3 m_Rotation = {};

	bool m_Enable = true;		// �L���A����

	std::vector<Component*> m_Components = {};

public:
	virtual ~GameObject();
	virtual void Init();
	virtual void Uninit();
	virtual void Update(const float& deltaTime);
	virtual void Draw();

	template <typename T, typename... Arg>
	T* AddComponent(Arg&&...args)
	{
		T* component = new T(std::forward<Arg>(args)...);
		if (component == nullptr) return nullptr;

		component->Init();
		m_Components.emplace_back(component);

		return component;
	}

	template <typename T>
	bool GetComponents(std::vector<T*>& components)
	{
		for (Component* component : m_Components)
		{
			if (component == nullptr) continue;

			if (T* tComponent = dynamic_cast<T*>(component))
			{
				components.emplace_back(tComponent);
			}
		}
		return !components.empty();
	}

	void SetEnable(const float& flag)
	{
		m_Enable = flag;
	}

	const bool& GetEnable()const
	{
		return m_Enable;
	}
	const XMFLOAT3& GetPos()const
	{
		return m_Position;
	}
	const XMFLOAT3& GetScale()const
	{
		return m_Scale;
	}
	const XMFLOAT3& GetRot()const
	{
		return m_Rotation;
	}

	// ---------------------------------------------------
	// m_Rotation�ɐ��l�������Ă��Ȃ��Ɛ���ɓ��삵�Ȃ�
	// ---------------------------------------------------
	XMFLOAT3 GetForward()const;			//�O���x�N�g���̎擾
	XMFLOAT3 GetRight()const;			//�E�����x�N�g���̎擾
	XMFLOAT3 GetUp()const;				//������x�N�g���̎擾
	XMMATRIX GetRotationMatrix()const;	// ��]�}�g���b�N�X���擾

	// �^�[�Q�b�g�ւ̃x�N�g�����擾
	XMFLOAT3 GetTargetDirection(const XMFLOAT3& targetpos)const;

};