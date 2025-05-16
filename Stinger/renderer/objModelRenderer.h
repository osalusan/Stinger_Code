#pragma once
#include "renderer/renderer.h"
#include <string>
#include <unordered_map>

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256] = { 0 };
	MATERIAL					Material;
	char						TextureName[256] = { 0 };
	ID3D11ShaderResourceView*	Texture = nullptr;
};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex = 0;
	unsigned int	IndexNum = 0;
	MODEL_MATERIAL	Material;
};


// ���f���\����
struct MODEL_OBJ
{
	VERTEX_3D*		VertexArray = nullptr;
	unsigned int	VertexNum = 0;

	unsigned int*	IndexArray = 0;
	unsigned int	IndexNum = 0;

	SUBSET*			SubsetArray = nullptr;
	unsigned int	SubsetNum = 0;

	XMFLOAT3     Origin = {}; // ���_��ǉ�
	XMFLOAT3     Size = {};   // ���f���̃T�C�Y
};

struct MODEL
{
	ID3D11Buffer*	VertexBuffer = nullptr;
	ID3D11Buffer*	IndexBuffer = nullptr;

	SUBSET*			SubsetArray = nullptr;
	unsigned int	SubsetNum = 0;

	XMFLOAT3     Center = {}; // ���_��ǉ�
	XMFLOAT3     Scale = {};   // ���f���̃T�C�Y
};
// OBJ�̓ǂݍ��ݏ������s���N���X
class ObjModelRenderer final
{	
private:
	MODEL* m_Model = {};
public:
	ObjModelRenderer(MODEL* model);
	void Draw();
	void DrawCollision();
	void Uninit();

	void SetModel(MODEL* model);

	MODEL* GetModel()
	{
		return m_Model;
	}
};