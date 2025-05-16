#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"

enum class PIVOT
{
	NONE = 0,
	CENTER,
	LEFT_TOP,
	RIGHT_TOP,
	LEFT_BOTTOM,
	RIGHT_BOTTOM,
	MAX
};

// �O���錾
enum class TEXTURE;
class Camera;
// �e�N�X�`���̊���N���X
class Polygon2D final :public GameObject
{
protected:

	ID3D11Buffer* m_VertexBuffer = nullptr;
	TEXTURE m_Texture;							// �g�p�e�N�X�`�� / �R���X�g���N�^�ŏ�����
	PIVOT m_PivotPoint = PIVOT::NONE;			// �摜�̌��_

	VERTEX_3D m_Vertex[4] = {};
	XMFLOAT4 m_Color = { 1.0f,1.0f,1.0f,1.0f };
	bool m_UseStencil = false;					// �A�p�Ɏ���
	bool m_UseUI = false;						// UI�Ƃ��Ďg�� / �摜�̃T�C�Y��UV��ύX�ł���悤��
	Camera* m_CameraCache = nullptr;
public:
	Polygon2D() = delete;
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const wchar_t* fileName);
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const wchar_t* fileName,const bool& ui);
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const bool& useStencil, const wchar_t* fileName);
	Polygon2D(const XMFLOAT2& position, const XMFLOAT2& size, const PIVOT& pivot, const TEXTURE& texture, const bool& useStencil, const wchar_t* fileName,const bool& enable);

	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update(const float& deltaTime)override;
	virtual void Draw()override;
	// ���W�A�傫���AUV�l��ύX
	void SetPolygon(const XMFLOAT2& position, const XMFLOAT2& size, const XMFLOAT2& uvValue = { 1.0f,1.0f });
	// �摜�̑傫����ς�����UV�����ύX / m_UseUI��true����Ȃ��ƕύX�s��
	void ChangeUVScaling(const XMFLOAT2& uvValue);		

	void SetAlfa(const float& alfa)
	{
		m_Color.w = alfa;
	}
	const TEXTURE& GetTexture()const
	{
		return m_Texture;
	}
};
