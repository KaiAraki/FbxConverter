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



MdBinDataContainer::~MdBinDataContainer()
{
	for (int i = 0; i < getMaterialArraySize(); i++)
	{
		getpMaterial(i)->ReleaseTextureArray();
	}
}