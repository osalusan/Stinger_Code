#pragma once
#include "object/gameObject.h"
#define PARTICLE_MAX (800)

// �O���錾
enum class TEXTURE;
class Camera;
// �p�[�e�B�N���p�̊��N���X
class ParticleEmiter :public GameObject
{
protected:
	XMFLOAT3 m_Position = {};
	XMFLOAT3 m_Rotation = {};
	XMFLOAT3 m_Scale = { 1.0f, 1.0f, 1.0f };

	// �`��֘A
	ID3D11Buffer* m_VertexBuffer = NULL;

	TEXTURE m_Texture;						// �g�p�e�N�X�`�� / �R���X�g���N�^�ŏ�����

	Camera* m_CameraCache = nullptr;

	// �p�[�e�B�N�����̃f�[�^
	struct PARTICLE {
		bool enable;
		float lifetime;
		float age;
		XMFLOAT4 color;
		XMFLOAT3 position;
		XMFLOAT3 velocity;
		XMFLOAT3 scale;
		XMFLOAT3 maxScale;
		XMFLOAT3 upScale;
		XMFLOAT3 num;
	};

	PARTICLE m_Particle[PARTICLE_MAX] = {};

	bool m_Mix = false;							// ���Z�����̃I���I�t
	bool m_Enable = true;						// �p�[�e�B�N���S�̂̔���
	bool m_ReservEnable = false;				// �w��̕b����ɗL������
	float m_ParticleLifeTime = 0.0f;			// �p�[�e�B�N���������ł��Ȃ��Ȃ�
	float m_CurrentLifeTime = 0.0f;				// �����̌o�ߎ���

	virtual void CreateParticleEffect(const float& deltaTime) = 0;		// �������̍��W�Ȃǂ�ݒ�
	virtual void UpdateParticleEffect(const float& deltaTime) = 0;		// �ړ��Ȃǂ�ݒ�

	virtual void ReservTexture() = 0;
public:
	ParticleEmiter();
	// �p�[�e�B�N���̍��W�̋N�_�ƂȂ鏊��ݒ�
	ParticleEmiter(const XMFLOAT3& pos);
	ParticleEmiter(const XMFLOAT3& pos,const bool& disable);

	ParticleEmiter(const bool& disable);
	~ParticleEmiter();
	void Init();
	void Update(const float& deltaTime);
	void Draw();

	virtual void End();
	virtual void Start();

	void SetEnable(const bool& flag)
	{
		m_Enable = flag;
	}
	const bool& GetEnable()const
	{
		return m_Enable;
	}

	void SetPos(const XMFLOAT3& pos)
	{
		m_Position = pos;
	}
};