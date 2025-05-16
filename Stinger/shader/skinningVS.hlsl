#include "common.hlsl"

VS_OUTPUT main(VS_IN input)
{
    VS_OUTPUT output;

    // �X�L�j���O�v�Z����
    float4x4 skinMatrix;
    skinMatrix = mul(BoneMtx[input.BoneIndex.x], input.BoneWeight.x);
    skinMatrix += mul(BoneMtx[input.BoneIndex.y], input.BoneWeight.y);
    skinMatrix += mul(BoneMtx[input.BoneIndex.z], input.BoneWeight.z);
    skinMatrix += mul(BoneMtx[input.BoneIndex.w], input.BoneWeight.w);
    
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    // ���_�ʒu�̕ϊ�
    float4 pos = mul(input.Position, skinMatrix);
    output.Position = mul(pos, wvp);
    
    // �@���p�Ɉړ�������
    skinMatrix[0][3] = 0.0f;
    skinMatrix[1][3] = 0.0f;
    skinMatrix[2][3] = 0.0f;
    output.Normal = mul(input.Normal.xyz, (float3x3)skinMatrix);

    output.TexCoord = input.TexCoord;
    
    output.Diffuse = input.Diffuse * Material.Diffuse;

    return output;
}