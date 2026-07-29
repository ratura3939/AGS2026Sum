#include"../pch.h"
#include "../Application.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(
	std::wstring shaderFileNameVS, int constBufFloat4SizeVS,
	std::wstring shaderFileNamePS, int constBufFloat4SizePS,
	const int constBufMatrixSizeVS)
{

	// 頂点シェーダのロード
	shaderVS_ = LoadVertexShader(
		(Application::PATH_SHADER + shaderFileNameVS).c_str());

	// 頂点定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizeVS_ = constBufFloat4SizeVS;

	// 頂点シェーダー用の定数バッファを作成
	constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);



	// ピクセルシェーダのロード
	shaderPS_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderFileNamePS).c_str());

	// ピクセル定数バッファの確保サイズ(FLOAT4をいくつ作るか)
	constBufFloat4SizePS_ = constBufFloat4SizePS;

	// ピクセルシェーダー用の定数バッファを作成
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);

	// 頂点定数バッファマトリックスの確保サイズ(MATRIXをいくつ作るか)
	constBufMatrixSizeVs_ = constBufMatrixSizeVS;

	// 頂点シェーダー用のマトリックス定数バッファを作成
	constBufVSMatrix_ = CreateShaderConstantBuffer(sizeof(MATRIX) * constBufMatrixSizeVS);

	// テクスチャアドレス
	texAddress_ = TEXADDRESS::CLAMP;

}

void ModelMaterial::AddConstBufVS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(contBuf);
	}

}

void ModelMaterial::AddConstBufPS(const FLOAT4& contBuf)
{

	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(contBuf);
	}

}

void ModelMaterial::AddConstBufVSMatrix(const MATRIX& matrix)
{
	if (constBufMatrixSizeVs_ > constBufsVSMatrix_.size())
	{
		constBufsVSMatrix_.emplace_back(matrix);
	}
}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = contBuf;

}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& contBuf)
{

	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = contBuf;

}

void ModelMaterial::SetConstBufVSMatrix(int idx, const MATRIX& matrix)
{
	if (idx >= constBufsVSMatrix_.size())
	{
		return;
	}

	constBufsVSMatrix_[idx] = matrix;
}

void ModelMaterial::SetTextureBuf(int slot, int texDiffuse)
{

	if (textures_.count(slot) == 0)
	{
		textures_.emplace(slot, texDiffuse);
	}
	else
	{
		textures_[slot] = texDiffuse;
	}

}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsVS(void) const
{
	return constBufsVS_;
}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsPS(void) const
{
	return constBufsPS_;
}

const std::vector<MATRIX>& ModelMaterial::GetConstBufsVSMatrix(void) const
{
	return constBufsVSMatrix_;
}

const std::map<int, int>& ModelMaterial::GetTextures(void) const
{
	return textures_;
}

ModelMaterial::TEXADDRESS ModelMaterial::GetTextureAddress(void) const
{
	return texAddress_;
}

void ModelMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddress_ = texA;
}

int ModelMaterial::GetShaderVS(void) const
{
	return shaderVS_;
}

int ModelMaterial::GetShaderPS(void) const
{
	return shaderPS_;
}

int ModelMaterial::GetConstBufVS(void) const
{
	return constBufVS_;
}

int ModelMaterial::GetConstBufPS(void) const
{
	return constBufPS_;
}

int ModelMaterial::GetConstBufVSMatrix(void) const
{
	return constBufVSMatrix_;
}

ModelMaterial::~ModelMaterial(void)
{
	DeleteShader(shaderVS_);
	DeleteShaderConstantBuffer(constBufVS_);
	DeleteShader(shaderPS_);
	DeleteShaderConstantBuffer(constBufPS_);
	DeleteShaderConstantBuffer(constBufVSMatrix_);

}
