#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"

// �O���錾
enum class TEXTURE;
class Camera;
// �r���{�[�h�p�̊��N���X
class BillBoard :public GameObject
{
protected:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	Camera* m_CameraCache = nullptr;
	TEXTURE m_Texture;						// �g�p�e�N�X�`�� / �R���X�g���N�^�ŏ�����

	VERTEX_3D m_Vertex[4] = {};

	// �X�v���C�g�V�[�g�̉摜����
	XMINT2 m_Sprite = {};
	float m_CurrentTime = 0;
	int m_AnimCount = 0;
	int m_MaxCount = 0;
	float m_NextAnimTime = 0.0f;
	float m_MaxTime = 0.0f;
	bool m_Loop = false;
	int m_LoopStart = 0;
	int m_LoopEnd = 0;
	XMFLOAT2 m_Offset = {};
	bool m_BillboardY = false;				// Y���̃r���{�[�h�̂ݗL��
public:
	BillBoard() = delete;
	~BillBoard();
																																		// �X�v���C�g�V�[�g�ɗ]��������ꍇ�͍ő喇�����w��
	BillBoard(const XMFLOAT3& position, const XMFLOAT3& size, const TEXTURE& texture, const wchar_t* fileName,const float& nextAnimTime ,const XMINT2& sprite,const int& maxTexture = 0);

	virtual void Init()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;

	virtual void End();
	void UseBillboard();
};