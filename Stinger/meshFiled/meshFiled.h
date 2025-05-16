#pragma once
#include "object/gameObject.h"
#include "renderer/renderer.h"
#define FILED_MAX (51)

enum class TEXTURE;
// ���b�V���t�B�[���h
class MeshFiled :public GameObject
{
protected:
	TEXTURE m_Texture;						// �g�p�e�N�X�`�� / �R���X�g���N�^�ŏ�����
	TEXTURE m_Normal;						// �g�p�@�� / �R���X�g���N�^�ŏ�����

	// ���_�Ǘ�
	ID3D11Buffer* m_VertexBuffer = NULL;
	ID3D11Buffer* m_IndexBuffer = NULL;

	VERTEX_3D m_Vertex[FILED_MAX][FILED_MAX] = {};

	LIGHT m_Light;
	float m_FiledHeight[FILED_MAX][FILED_MAX] ={};

	// CSV�̐������t�B�[���h��Y���W�ɔ��f������
	void FiledDataLoadCSV(const std::string& filePath);
public:
	MeshFiled() = delete;
	// �t�@�C����DDS�`���̂ݓǂݍ��߂�
	MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture,const wchar_t* textureName, std::string filePath = "");
	MeshFiled(const XMFLOAT3& pos,const TEXTURE& texture, const wchar_t* textureName,const TEXTURE& normal, const wchar_t* normalName, std::string filePath = "");
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Draw()override;

	// XZ���W����Y���W�̍�����������
	float GetHeight(const XMFLOAT3& position);
};