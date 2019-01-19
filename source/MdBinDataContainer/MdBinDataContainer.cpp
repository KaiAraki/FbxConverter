//================================================================================
//!	@file	 MdBinDataContainer.h
//!	@brief	 バイナリーモデルClass
//! @details 
//!	@author  Kai Araki									@date 2018/12/29
//================================================================================


//****************************************
// インクルード文
//****************************************
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include "MdBinDataContainer.h"



//****************************************
// static関数定義
//****************************************
bool MdBinDataContainer::InportData(MdBinDataContainer* md_bin_data_container,
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



bool MdBinDataContainer::ExportData(MdBinDataContainer* md_bin_data_container,
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
float* MdBinDataContainer::Vector2::getpX()
{
	return &x_;
}



float* MdBinDataContainer::Vector2::getpY()
{
	return &y_;
}



float* MdBinDataContainer::Vector3::getpX()
{
	return vector2_.getpX();
}



float* MdBinDataContainer::Vector3::getpY()
{
	return vector2_.getpY();
}



float* MdBinDataContainer::Vector3::getpZ()
{
	return &z_;
}



float* MdBinDataContainer::Color::getpR()
{
	return &r_;
}



float* MdBinDataContainer::Color::getpG()
{
	return &g_;
}



float* MdBinDataContainer::Color::getpB()
{
	return &b_;
}



float MdBinDataContainer::Matrix::getMatrixElement(unsigned index0, unsigned index1)
{
	return array_[index0][index1];
}



void MdBinDataContainer::Matrix::setMatrixElement(float value, unsigned index0, unsigned index1)
{
	array_[index0][index1] = value;
}



std::string* MdBinDataContainer::Material::Texture::getpFilePath()
{
	return &file_path_;
}



MdBinDataContainer::Material::Texture::Type* MdBinDataContainer::Material::Texture::getpType()
{
	return &type_;
}



std::string* MdBinDataContainer::Material::getpName()
{
	return &name_;
}



MdBinDataContainer::Color* MdBinDataContainer::Material::getpAmbient()
{
	return &ambient_;
}



MdBinDataContainer::Color* MdBinDataContainer::Material::getpDiffuse()
{
	return &diffuse_;
}



MdBinDataContainer::Color* MdBinDataContainer::Material::getpEmissive()
{
	return &emissive_;
}



MdBinDataContainer::Color* MdBinDataContainer::Material::getpBump()
{
	return &bump_;
}



float* MdBinDataContainer::Material::getpTransparent()
{
	return &transparent_;
}



MdBinDataContainer::Color* MdBinDataContainer::Material::getpSpecular()
{
	return &specular_;
}



float* MdBinDataContainer::Material::getpPower()
{
	return &power_;
}



float* MdBinDataContainer::Material::getpReflection()
{
	return &reflection_;
}



int MdBinDataContainer::Material::getTextureArraySize()
{
	return texture_.size();
}



MdBinDataContainer::Material::Texture* MdBinDataContainer::Material::getpTexture(int index)
{
	return texture_[index];
}



int MdBinDataContainer::Mesh::UVSet::getUVArraySize()
{
	return uv_array_.size();
}



void MdBinDataContainer::Mesh::UVSet::setUVArraySize(int value)
{
	uv_array_.resize(value);
}



float* MdBinDataContainer::Mesh::UVSet::getpU(int index)
{
	return uv_array_[index].getpX();
}



float* MdBinDataContainer::Mesh::UVSet::getpV(int index)
{
	return uv_array_[index].getpY();
}



int MdBinDataContainer::Mesh::UVSet::getTextureArraySize()
{
	return texture_.size();
}



MdBinDataContainer::Material::Texture* MdBinDataContainer::Mesh::UVSet::getpTexture(int index)
{
	return texture_[index];
}



std::string* MdBinDataContainer::Mesh::Bone::getpName()
{
	return &name_;
}



MdBinDataContainer::Matrix* MdBinDataContainer::Mesh::Bone::getpOffsetMatrix()
{
	return &offset_matrix_;
}



int MdBinDataContainer::Mesh::Bone::getAnimationMatrixArraySize()
{
	return animetion_matrix_array_.size();
}



void MdBinDataContainer::Mesh::Bone::setAnimationMatrixArraySize(int value)
{
	animetion_matrix_array_.resize(value);
}



MdBinDataContainer::Matrix* MdBinDataContainer::Mesh::Bone::getpAnimationMatrixArray(int index)
{
	return &animetion_matrix_array_[index];
}



int MdBinDataContainer::Mesh::BoneWeight::getBoneIndex(int index)
{
	return bone_index_[index];
}



float MdBinDataContainer::Mesh::BoneWeight::getBoneWeight(int index)
{
	return bone_weight_[index];
}



void MdBinDataContainer::Mesh::BoneWeight::setBoneIndexAndWeight(int bone_index, 
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



int MdBinDataContainer::Mesh::getVertexArraySize()
{
	return position_array_.size();
}



int MdBinDataContainer::Mesh::getPositionArraySize()
{
	return position_array_.size();
}



void MdBinDataContainer::Mesh::setPositionArraySize(int value)
{
	position_array_.resize(value);
}



MdBinDataContainer::Vector3* MdBinDataContainer::Mesh::getpPosition(int index)
{
	return &position_array_[index];
}



int MdBinDataContainer::Mesh::getNormalArraySize()
{
	return normal_array_.size();
}



void MdBinDataContainer::Mesh::setNormalArraySize(int value)
{
	normal_array_.resize(value);
}



MdBinDataContainer::Vector3* MdBinDataContainer::Mesh::getpNormal(int index)
{
	return &normal_array_[index];
}



int MdBinDataContainer::Mesh::getUVSetArraySize()
{
	return uv_set_array_.size();
}



void MdBinDataContainer::Mesh::setUVSetArraySize(int value)
{
	uv_set_array_.resize(value);
}



MdBinDataContainer::Mesh::UVSet* MdBinDataContainer::Mesh::getpUVSet(int index)
{
	return &uv_set_array_[index];
}



int MdBinDataContainer::Mesh::getIndexArraySize()
{
	return index_array_.size();
}



void MdBinDataContainer::Mesh::setIndexArraySize(int value)
{
	index_array_.resize(value);
}



int* MdBinDataContainer::Mesh::getpIndex(int index)
{
	return &index_array_[index];
}



int MdBinDataContainer::Mesh::getBoneArraySize()
{
	return bone_array_.size();
}



void MdBinDataContainer::Mesh::setBoneArraySize(int value)
{
	bone_array_.resize(value);
}



MdBinDataContainer::Mesh::Bone* MdBinDataContainer::Mesh::getpBone(int index)
{
	return &bone_array_[index];
}



int MdBinDataContainer::Mesh::getBoneArrayEndIndex()
{
	return bone_array_.size() - 1;
}



int MdBinDataContainer::Mesh::getBoneWeightArraySize()
{
	return bone_weight_array_.size();
}



void MdBinDataContainer::Mesh::setBoneWeightArraySize(int value)
{
	bone_weight_array_.resize(value);
}



MdBinDataContainer::Mesh::BoneWeight* MdBinDataContainer::Mesh::getpBoneWeight(int index)
{
	return &bone_weight_array_[index];
}



int* MdBinDataContainer::Mesh::getpMaterialIndex()
{
	return &material_index_;
}



int MdBinDataContainer::getMeshArraySize()
{
	return mesh_array_.size();
}



void MdBinDataContainer::setMeshArraySize(int value)
{
	mesh_array_.resize(value);
}



MdBinDataContainer::Mesh* MdBinDataContainer::getpMesh(int index)
{
	return &mesh_array_[index];
}



int MdBinDataContainer::getMaterialArraySize()
{
	return material_array_.size();
}



void MdBinDataContainer::setMaterialArraySize(int value)
{
	material_array_.resize(value);
}



MdBinDataContainer::Material* MdBinDataContainer::getpMaterial(int index)
{
	return &material_array_[index];
}



//****************************************
// 関数定義
//****************************************
void MdBinDataContainer::Material::ReleaseTextureArray()
{
	for (int i = 0; i < (int)texture_.size(); i++)
	{
		if (texture_[i] == nullptr) continue;
		delete texture_[i];
		texture_[i] = nullptr;
	}
}



void MdBinDataContainer::Material::AddTextureArray(MdBinDataContainer::Material::Texture* texture)
{
	texture_.emplace_back(texture);
}



void MdBinDataContainer::Mesh::UVSet::AddTextureArray(MdBinDataContainer::Material::Texture* texture)
{
	texture_.emplace_back(texture);
}



void MdBinDataContainer::Mesh::AddBoneArray(MdBinDataContainer::Mesh::Bone* bone)
{
	bone_array_.emplace_back(*bone);
}



void MdBinDataContainer::Mesh::BoneWeight::Init()
{
	for (int i = 0; i < MAX_BONE_NUM; i++)
	{
		bone_index_[i] = 0;
		bone_weight_[i] = 0.0f;
	}
}



MdBinDataContainer::~MdBinDataContainer()
{
	for (int i = 0; i < getMaterialArraySize(); i++)
	{
		getpMaterial(i)->ReleaseTextureArray();
	}
}