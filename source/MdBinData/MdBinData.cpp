//================================================================================
//!	@file	 MdBinData.cpp
//!	@brief	 バイナリーモデルデータClass
//! @details 
//!	@author  Kai Araki									@date 2018/12/29
//================================================================================


//****************************************
// インクルード文
//****************************************
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "MdBinData.h"



//****************************************
// static関数定義
//****************************************
bool MdBinData::InportData(MdBinData* md_bin_data_container,
						   std::string file_path)
{
	// ファイルオープン
	std::ifstream ifstream(file_path, std::ios::binary);
	if (!ifstream) return false;
	boost::archive::binary_iarchive iarchive(ifstream);

	// 読み込み
	iarchive >> *md_bin_data_container;

	ifstream.close();

	return true;
}



bool MdBinData::ExportData(MdBinData* md_bin_data_container,
						   std::string file_path)
{
	// ファイルオープン
	std::ofstream ofstream(file_path, std::ios::binary);
	boost::archive::binary_oarchive oarchive(ofstream);

	// 書き込み
	oarchive << *md_bin_data_container;

	ofstream.close();

	return true;
}



//****************************************
// プロパティ定義
//****************************************
float* MdBinData::Vector2::getpX()
{
	return &x_;
}



float* MdBinData::Vector2::getpY()
{
	return &y_;
}



float* MdBinData::Vector3::getpX()
{
	return vector2_.getpX();
}



float* MdBinData::Vector3::getpY()
{
	return vector2_.getpY();
}



float* MdBinData::Vector3::getpZ()
{
	return &z_;
}



float* MdBinData::Color::getpR()
{
	return &r_;
}



float* MdBinData::Color::getpG()
{
	return &g_;
}



float* MdBinData::Color::getpB()
{
	return &b_;
}



float MdBinData::Matrix::getMatrixElement(unsigned index0, unsigned index1)
{
	return array_[index0][index1];
}



void MdBinData::Matrix::setMatrixElement(float value, unsigned index0, unsigned index1)
{
	array_[index0][index1] = value;
}



std::string* MdBinData::Material::Texture::getpFilePath()
{
	return &file_path_;
}



MdBinData::Material::Texture::Type* MdBinData::Material::Texture::getpType()
{
	return &type_;
}



std::string* MdBinData::Material::getpName()
{
	return &name_;
}



MdBinData::Color* MdBinData::Material::getpAmbient()
{
	return &ambient_;
}



MdBinData::Color* MdBinData::Material::getpDiffuse()
{
	return &diffuse_;
}



MdBinData::Color* MdBinData::Material::getpEmissive()
{
	return &emissive_;
}



MdBinData::Color* MdBinData::Material::getpBump()
{
	return &bump_;
}



float* MdBinData::Material::getpTransparent()
{
	return &transparent_;
}



MdBinData::Color* MdBinData::Material::getpSpecular()
{
	return &specular_;
}



float* MdBinData::Material::getpPower()
{
	return &power_;
}



float* MdBinData::Material::getpReflection()
{
	return &reflection_;
}



int MdBinData::Material::getTextureArraySize()
{
	return texture_array_.size();
}



MdBinData::Material::Texture* MdBinData::Material::getpTexture(int index)
{
	return texture_array_[index];
}



int MdBinData::Mesh::UVSet::getUVArraySize()
{
	return uv_array_.size();
}



void MdBinData::Mesh::UVSet::setUVArraySize(int value)
{
	uv_array_.resize(value);
}



float* MdBinData::Mesh::UVSet::getpU(int index)
{
	return uv_array_[index].getpX();
}



float* MdBinData::Mesh::UVSet::getpV(int index)
{
	return uv_array_[index].getpY();
}



int MdBinData::Mesh::UVSet::getTextureArraySize()
{
	return texture_.size();
}



MdBinData::Material::Texture* MdBinData::Mesh::UVSet::getpTexture(int index)
{
	return texture_[index];
}



std::string* MdBinData::Mesh::Bone::getpName()
{
	return &name_;
}



MdBinData::Matrix* MdBinData::Mesh::Bone::getpOffsetMatrix()
{
	return &offset_matrix_;
}



int MdBinData::Mesh::Bone::getAnimationMatrixArraySize()
{
	return animation_matrix_array_.size();
}



void MdBinData::Mesh::Bone::setAnimationMatrixArraySize(int value)
{
	animation_matrix_array_.resize(value);
}



MdBinData::Matrix* MdBinData::Mesh::Bone::getpAnimationMatrix(int index)
{
	return &animation_matrix_array_[index];
}



int MdBinData::Mesh::BoneWeight::getBoneIndex(int index)
{
	return bone_index_[index];
}



float MdBinData::Mesh::BoneWeight::getBoneWeight(int index)
{
	return bone_weight_[index];
}



void MdBinData::Mesh::BoneWeight::setBoneIndexAndWeight(int bone_index,
														float bone_weight)
{
	for (int i = 0; i < MAX_BONE_NUM; i++)
	{
		if (bone_weight_[i] != 0.0f) continue;
		bone_index_[i] = bone_index;
		bone_weight_[i] = bone_weight;
		break;
	}
}



int MdBinData::Mesh::getVertexArraySize()
{
	return position_array_.size();
}



int MdBinData::Mesh::getPositionArraySize()
{
	return position_array_.size();
}



void MdBinData::Mesh::setPositionArraySize(int value)
{
	position_array_.resize(value);
}



MdBinData::Vector3* MdBinData::Mesh::getpPosition(int index)
{
	return &position_array_[index];
}



int MdBinData::Mesh::getNormalArraySize()
{
	return normal_array_.size();
}



void MdBinData::Mesh::setNormalArraySize(int value)
{
	normal_array_.resize(value);
}



MdBinData::Vector3* MdBinData::Mesh::getpNormal(int index)
{
	return &normal_array_[index];
}



int MdBinData::Mesh::getUVSetArraySize()
{
	return uv_set_array_.size();
}



void MdBinData::Mesh::setUVSetArraySize(int value)
{
	uv_set_array_.resize(value);
}



MdBinData::Mesh::UVSet* MdBinData::Mesh::getpUVSet(int index)
{
	return &uv_set_array_[index];
}



int MdBinData::Mesh::getIndexArraySize()
{
	return index_array_.size();
}



void MdBinData::Mesh::setIndexArraySize(int value)
{
	index_array_.resize(value);
}



int* MdBinData::Mesh::getpIndex(int index)
{
	return &index_array_[index];
}



int MdBinData::Mesh::getBoneArraySize()
{
	return bone_array_.size();
}



void MdBinData::Mesh::setBoneArraySize(int value)
{
	bone_array_.resize(value);
}



MdBinData::Mesh::Bone* MdBinData::Mesh::getpBone(int index)
{
	return &bone_array_[index];
}



int MdBinData::Mesh::getBoneArrayEndIndex()
{
	return bone_array_.size() - 1;
}



int MdBinData::Mesh::getBoneWeightArraySize()
{
	return bone_weight_array_.size();
}



void MdBinData::Mesh::setBoneWeightArraySize(int value)
{
	bone_weight_array_.resize(value);
}



MdBinData::Mesh::BoneWeight* MdBinData::Mesh::getpBoneWeight(int index)
{
	return &bone_weight_array_[index];
}



int* MdBinData::Mesh::getpMaterialIndex()
{
	return &material_index_;
}



int MdBinData::getMeshArraySize()
{
	return mesh_array_.size();
}



void MdBinData::setMeshArraySize(int value)
{
	mesh_array_.resize(value);
}



MdBinData::Mesh* MdBinData::getpMesh(int index)
{
	return &mesh_array_[index];
}



int MdBinData::getMaterialArraySize()
{
	return material_array_.size();
}



void MdBinData::setMaterialArraySize(int value)
{
	material_array_.resize(value);
}



MdBinData::Material* MdBinData::getpMaterial(int index)
{
	return &material_array_[index];
}



int MdBinData::getAnimationFramNum()
{
	return animation_frame_num_;
}



void MdBinData::setAnimationFramNum(int value)
{
	animation_frame_num_ = value;
}



//****************************************
// 関数定義
//****************************************
void MdBinData::Material::ReleaseTextureArray()
{
	for (int i = 0; i < (int)texture_array_.size(); i++)
	{
		if (texture_array_[i] == nullptr) continue;
		delete texture_array_[i];
		texture_array_[i] = nullptr;
	}
}



void MdBinData::Material::AddTextureArray(MdBinData::Material::Texture* texture)
{
	texture_array_.emplace_back(texture);
}



void MdBinData::Mesh::UVSet::AddTextureArray(MdBinData::Material::Texture* texture)
{
	texture_.emplace_back(texture);
}



void MdBinData::Mesh::AddBoneArray(MdBinData::Mesh::Bone* bone)
{
	bone_array_.emplace_back(*bone);
}



void MdBinData::Mesh::BoneWeight::Init()
{
	for (int i = 0; i < MAX_BONE_NUM; i++)
	{
		bone_index_[i] = 0;
		bone_weight_[i] = 0.0f;
	}
}



MdBinData::~MdBinData()
{
	for (int i = 0; i < getMaterialArraySize(); i++)
	{
		getpMaterial(i)->ReleaseTextureArray();
	}
}