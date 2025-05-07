#include "main/main.h"
#include "renderer/fbxModelRenderer.h"

void FbxModelRenderer::Draw()
{
	// GPU�X�L�j���O
	std::vector<XMFLOAT4X4> boneMatrix;
	for (const BONE* bone : m_BoneIndex)
	{
		const aiMatrix4x4& aiMat = bone->Matrix;
		XMFLOAT4X4 mat = {};

		mat._11 = aiMat.a1; mat._12 = aiMat.b1; mat._13 = aiMat.c1; mat._14 = aiMat.d1;
		mat._21 = aiMat.a2; mat._22 = aiMat.b2; mat._23 = aiMat.c2; mat._24 = aiMat.d2;
		mat._31 = aiMat.a3; mat._32 = aiMat.b3; mat._33 = aiMat.c3; mat._34 = aiMat.d3;
		mat._41 = aiMat.a4; mat._42 = aiMat.b4; mat._43 = aiMat.c4; mat._44 = aiMat.d4;

		// �]�u
		XMMATRIX matTranspose = XMMatrixTranspose(XMLoadFloat4x4(&mat));
		XMStoreFloat4x4(&mat, matTranspose);

		boneMatrix.emplace_back(mat);
	}
	// �{�[�������V�F�[�_�[��
	Renderer::SetBoneMatrix(boneMatrix);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);


	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];


		// �}�e���A���ݒ�
		aiString texture;
		aiColor3D diffuse;
		float opacity;

		aiMaterial* aimaterial = m_AiScene->mMaterials[mesh->mMaterialIndex];
		aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aimaterial->Get(AI_MATKEY_OPACITY, opacity);

		if (texture == aiString(""))
		{
			material.TextureEnable = false;
		}
		else
		{
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[texture.data]);
			material.TextureEnable = true;
		}

		material.Diffuse = XMFLOAT4(diffuse.r, diffuse.g, diffuse.b, opacity);
		material.Ambient = material.Diffuse;
		Renderer::SetMaterial(material);

		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		// �C���f�b�N�X�o�b�t�@�ݒ�
		Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void FbxModelRenderer::LoadAnimation(const char* FileName, const char* Name)
{
	// TODO :Importer���g�p���Ă̓ǂݍ��݂ɕύX
	m_Animation[Name] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[Name]);
}

void FbxModelRenderer::CreateBone(aiNode* node, std::map<std::string, int>& boneIndexMap, int& boneCount)
{
	// �{�[�������擾
	std::string boneName = node->mName.C_Str();

	BONE bone;
	m_Bone[boneName] = bone;

	// �{�[�����܂��}�b�v�ɑ��݂��Ȃ��ꍇ�A�C���f�b�N�X�����蓖�Ă�
	if (boneIndexMap.count(boneName) <= 0)
	{
		boneIndexMap[boneName] = boneCount;
		boneCount++;
	}

	// �q�m�[�h���ċA�I�ɏ���
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		CreateBone(node->mChildren[i], boneIndexMap, boneCount);
	}
}

void FbxModelRenderer::Update(const char* AnimationName1, const float& time)
{
	if (m_Animation.count(AnimationName1) == 0)return;
	if (!m_Animation[AnimationName1]->HasAnimations())return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];

	// �A�j���[�V�����̃e�B�b�N�P�ʂ̎��Ԃ��v�Z
	float ticksPerSecond;
	if (animation1->mTicksPerSecond != 0.0f)
	{
		ticksPerSecond = static_cast<float>(animation1->mTicksPerSecond);
	}
	else
	{
		ticksPerSecond = 60.0f; // �f�t�H���g�̃e�B�b�N���[�g
	}
	float TimeInTicks = time * ticksPerSecond;

	// �A�j���[�V�������Ԃ����[�v������
	float AnimationTime = static_cast<float>(fmod(TimeInTicks, animation1->mDuration));

	for (std::pair<const std::string, BONE>& pair : m_Bone)
	{
		BONE* bone = &m_Bone[pair.first];

		aiNodeAnim* nodeAnim1 = nullptr;

		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim1 = animation1->mChannels[c];
				break;
			}
		}

		int f;

		aiQuaternion rot1;
		aiVector3D pos1;

		if (nodeAnim1)
		{
			// �L�[�t���[���̑���
			unsigned int numRotKeys = nodeAnim1->mNumRotationKeys;
			unsigned int numPosKeys = nodeAnim1->mNumPositionKeys;

			// �L�[�t���[���̃C���f�b�N�X���v�Z�i��ԂȂ��j
			f = 0;
			for (unsigned int i = 0; i < numRotKeys; i++)
			{
				if (AnimationTime < nodeAnim1->mRotationKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = 0;
			for (unsigned int i = 0; i < numPosKeys; i++)
			{
				if (AnimationTime < nodeAnim1->mPositionKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			pos1 = nodeAnim1->mPositionKeys[f].mValue;
		}
		else
		{
			rot1 = aiQuaternion();
			pos1 = aiVector3D(0.0f, 0.0f, 0.0f); 
		}

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot1, pos1);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);
}

void FbxModelRenderer::Update(const char* AnimationName1, const float& time1, const char* AnimationName2, const float& time2, float BlendRatio)
{
	if (m_Animation.count(AnimationName1) == 0)return;
	if (m_Animation.count(AnimationName2) == 0)return;
	if (!m_Animation[AnimationName1]->HasAnimations())return;
	if (!m_Animation[AnimationName2]->HasAnimations())return;

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];

	// �A�j���[�V�����̃e�B�b�N�P�ʂ̎��Ԃ��v�Z
	float ticksPerSecond1;
	if (animation1->mTicksPerSecond != 0.0f)
	{
		ticksPerSecond1 = static_cast<float>(animation1->mTicksPerSecond);
	}
	else
	{
		ticksPerSecond1 = 60.0f; // �f�t�H���g�̃��[�g
	}
	float TimeInTicks1 = time1 * ticksPerSecond1;

	// �A�j���[�V�������Ԃ����[�v������
	float AnimationTime1 = static_cast<float>(fmod(TimeInTicks1, animation1->mDuration));

	//�A�j���[�V�����f�[�^����{�[���}�g���N�X�Z�o
	aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];

	// �A�j���[�V�����̃e�B�b�N�P�ʂ̎��Ԃ��v�Z
	float ticksPerSecond2;
	if (animation2->mTicksPerSecond != 0.0f)
	{
		ticksPerSecond2 = static_cast<float>(animation2->mTicksPerSecond);
	}
	else
	{
		ticksPerSecond2 = 60.0f; // �f�t�H���g�̃��[�g
	}
	float TimeInTicks2 = time2 * ticksPerSecond2;

	// �A�j���[�V�������Ԃ����[�v������
	float AnimationTime2 = static_cast<float>(fmod(TimeInTicks2, animation2->mDuration));

	for (std::pair<const std::string, BONE>& pair : m_Bone)
	{
		BONE* bone = &m_Bone[pair.first];

		aiNodeAnim* nodeAnim1 = nullptr;

		for (unsigned int c = 0; c < animation1->mNumChannels; c++)
		{
			if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim1 = animation1->mChannels[c];
				break;
			}
		}

		aiNodeAnim* nodeAnim2 = nullptr;

		for (unsigned int c = 0; c < animation2->mNumChannels; c++)
		{
			if (animation2->mChannels[c]->mNodeName == aiString(pair.first))
			{
				nodeAnim2 = animation2->mChannels[c];
				break;
			}
		}

		int f;

		aiQuaternion rot1;
		aiVector3D pos1;

		if (nodeAnim1)
		{
			// �L�[�t���[���̑���
			unsigned int numRotKeys = nodeAnim1->mNumRotationKeys;
			unsigned int numPosKeys = nodeAnim1->mNumPositionKeys;

			// �L�[�t���[���̃C���f�b�N�X���v�Z�i��ԂȂ��j
			f = 0;
			for (unsigned int i = 0; i < numRotKeys; i++)
			{
				if (AnimationTime1 < nodeAnim1->mRotationKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			rot1 = nodeAnim1->mRotationKeys[f].mValue;

			f = 0;
			for (unsigned int i = 0; i < numPosKeys; i++)
			{
				if (AnimationTime1 < nodeAnim1->mPositionKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			pos1 = nodeAnim1->mPositionKeys[f].mValue;
		}

		aiQuaternion rot2;
		aiVector3D pos2;

		if (nodeAnim2)
		{
			// �L�[�t���[���̑���
			unsigned int numRotKeys = nodeAnim2->mNumRotationKeys;
			unsigned int numPosKeys = nodeAnim2->mNumPositionKeys;

			// �L�[�t���[���̃C���f�b�N�X���v�Z�i��ԂȂ��j
			f = 0;
			for (unsigned int i = 0; i < numRotKeys; i++)
			{
				if (AnimationTime2 < nodeAnim2->mRotationKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			rot2 = nodeAnim2->mRotationKeys[f].mValue;

			f = 0;
			for (unsigned int i = 0; i < numPosKeys; i++)
			{
				if (AnimationTime2 < nodeAnim2->mPositionKeys[i].mTime)
				{
					break;
				}
				f = i;
			}
			pos2 = nodeAnim2->mPositionKeys[f].mValue;
		}

		aiVector3D pos;
		pos = pos1 * (1.0f - BlendRatio) + pos2 * BlendRatio;// ���`���
		aiQuaternion rot;
		aiQuaternion::Interpolate(rot, rot1, rot2, BlendRatio);// ���ʐ��`���

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
	}

	//�ċA�I�Ƀ{�[���}�g���N�X���X�V
	aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion((float)AI_MATH_PI, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
	UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);
}
void FbxModelRenderer::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

	//�}�g���N�X�̏�Z���Ԃɒ���
	aiMatrix4x4 worldMatrix = matrix;
	worldMatrix *= bone->AnimationMatrix;

	bone->LocalMatrix = worldMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;//������|���Ȃ��Ƃ����Ȃ�

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}

float FbxModelRenderer::GetMaxAnimeTime(const std::string& name)const
{
	if (m_Animation.count(name.c_str()) <= 0) return 0.0f;

	aiAnimation* animation = m_Animation.at(name.c_str())->mAnimations[0];
	const float& ticksPerSecond = static_cast<float>(animation->mTicksPerSecond);
	return static_cast<float>(animation->mDuration / ticksPerSecond);
}

void FbxModelRenderer::Load(const char* FileName)
{
	const std::string modelPath(FileName);

	m_Importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	m_AiScene = m_Importer.ReadFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];

	// �{�[�����ƃC���f�b�N�X�̃}�b�v���쐬
	std::map<std::string, int> boneNameIndex;
	int boneCount = 0;

	// �{�[���̍쐬�ƃC���f�b�N�X�̊��蓖��
	CreateBone(m_AiScene->mRootNode, boneNameIndex, boneCount);

	// �{�[���C���f�b�N�X���̃{�[�����X�g��������
	m_BoneIndex.resize(boneCount, nullptr);

	// �{�[�����X�g���\�z
	for (const std::pair<std::string,int>& bonePair : boneNameIndex)
	{
		const std::string& boneName = bonePair.first;
		const int& boneIndex = bonePair.second;

		// m_Bone����{�[���ւ̃|�C���^���擾
		BONE* bone = &m_Bone[boneName];

		// �{�[�����X�g�Ɋi�[
		m_BoneIndex[boneIndex] = bone;
	}

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		// ���_�o�b�t�@����
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			// ���_�f�[�^�̏�����
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				// �{�[���C���f�b�N�X�ƃE�F�C�g�̏�����
				for (int i = 0; i < 4; i++)
				{
					vertex[v].BoneIndices[i] = 0;
					vertex[v].BoneWeights[i] = 0.0f;
				}
			}

			// �{�[���f�[�^�̐ݒ�
			for (unsigned int b = 0; b < mesh->mNumBones; b++)
			{
				aiBone* bone = mesh->mBones[b];
				const std::string& boneName = bone->mName.C_Str();

				// �{�[���C���f�b�N�X�̎擾
				const int& boneIndex = boneNameIndex[boneName];

				m_Bone[boneName].OffsetMatrix = bone->mOffsetMatrix;

				
				//�ό`�㒸�_�Ƀ{�[���f�[�^�i�[
				for (unsigned int w = 0; w < bone->mNumWeights; w++)
				{
					aiVertexWeight weight = bone->mWeights[w];
					int vertexId = weight.mVertexId;
					float boneWeight = weight.mWeight;

					VERTEX_3D& v = vertex[vertexId];

					for (int i = 0; i < 4; i++)
					{
						if (v.BoneWeights[i] == 0.0f)
						{
							v.BoneIndices[i] = boneIndex;
							v.BoneWeights[i] = boneWeight;
							break;
						}
					}
				}
			}

			// �E�F�C�g�̐��K��
			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				VERTEX_3D& vert = vertex[v];

				// �E�F�C�g�̍��v���v�Z
				const float& totalWeight = vert.BoneWeights[0] + vert.BoneWeights[1] + vert.BoneWeights[2] + vert.BoneWeights[3];

				// �E�F�C�g�̍��v��0�łȂ��ꍇ�̂ݐ��K��
				if (totalWeight > 0.0f && totalWeight != 1.0f)
				{
					vert.BoneWeights[0] /= totalWeight;
					vert.BoneWeights[1] /= totalWeight;
					vert.BoneWeights[2] /= totalWeight;
					vert.BoneWeights[3] /= totalWeight;
				}
			}



			// ���_�o�b�t�@�̍쐬
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = vertex;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;
		}


		// �C���f�b�N�X�o�b�t�@����
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

			delete[] index;
		}

		// �ŏ��l�ƍő�l�̏�����
		XMFLOAT3 min = {};
		XMFLOAT3 max = {};

		// ���b�V���ƒ��_�𑖍�
		for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
		{
			aiMesh* mesh = m_AiScene->mMeshes[m];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				aiVector3D vertex = mesh->mVertices[v];

				// �ŏ��l�̍X�V
				min.x = std::min(min.x, vertex.x);
				min.y = std::min(min.y, vertex.y);
				min.z = std::min(min.z, vertex.z);

				// �ő�l�̍X�V
				max.x = std::max(max.x, vertex.x);
				max.y = std::max(max.y, vertex.y);
				max.z = std::max(max.z, vertex.z);
			}
		}

		// ���S���W�̌v�Z
		const XMFLOAT3& center = { (min.x + max.x) / 2.0f ,(min.y+ max.y) / 2.0f ,(min.z + max.z) / 2.0f };
		m_Center = center;

		// �T�C�Y�̌v�Z
		const XMFLOAT3& size = { max.x - min.x ,max.y - min.y ,max.z - min.z };

		m_Scale = size;
	}



	//�e�N�X�`���ǂݍ���
	for (unsigned int i = 0; i < m_AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = m_AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;

		// �e�N�X�`���ǂݍ���
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		m_Texture[aitexture->mFilename.data] = texture;
	}
}

void FbxModelRenderer::Uninit()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	m_BoneIndex.clear();

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;
}