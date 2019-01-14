//================================================================================
//!	@file	 FbxConverter.cpp
//!	@brief	 FBX変換器Class
//! @details 
//!	@author  Kai Araki									@date 2018/12/29
//================================================================================


//****************************************
// インクルード文
//****************************************
#include <iostream>
#include <fstream>
#include <windows.h>
#include <imagehlp.h>
#include <stdio.h>
#include <shlwapi.h>

#include "FbxConverter.h"



//****************************************
// 関数定義
//****************************************
bool FbxConverter::Init()
{
	// マネージャの生成
	manager_ = FbxManager::Create();
	if (!manager_)
	{
		std::cout << "※マネージャの作成に失敗" << std::endl;
		return false;
	}

	// IOSettingsオブジェクト(全Inport/Exportの設定を保持する)の作成
	io_settings_ = FbxIOSettings::Create(manager_, IOSROOT);
	if (!io_settings_)
	{
		std::cout << "※FbxIOSettingsの作成に失敗" << std::endl;
		return false;
	}
	manager_->SetIOSettings(io_settings_);

	// 実行可能なディレクトリ(オプション)からプラグインをロードする
	std::string temp = FbxGetApplicationDirectory();
	ConvertToFullPathOfUTF8(&temp);
	FbxString plugins_directory_path = (FbxString)temp.c_str();
	manager_->LoadPluginsDirectory(plugins_directory_path.Buffer());

	// シーンの作成
	scene_ = FbxScene::Create(manager_, "scene");
	if (!scene_)
	{
		std::cout << "※シーンの作成に失敗" << std::endl;
		return false;
	}

	return true;
}



void FbxConverter::Uninit()
{
	// マネージャ解放(関連する全オブジェクトが解放される)
	manager_->Destroy();

	std::cout << "\n★FBXの変換終了...";
}



bool FbxConverter::ConvertToMdBin(std::string* file_path)
{
	// FBXの読み込み
	if (!Load(file_path))
	{
		std::cout << "※FBXの読み込み失敗" << std::endl;
		return false;
	}

	// FBXからデーターを抽出
	if (!ExtractData())
	{
		std::cout << "※データの抽出に失敗" << std::endl;
		return false;
	}

	// MdBinファイルの出力
	if (!ExportOfMdBinFile(file_path))
	{
		std::cout << "※MdBinファイルの出力に失敗" << std::endl;
		return false;
	}

	return true;
}



bool FbxConverter::Load(std::string* file_path)
{
	std::cout << "[Load]\n" << std::endl;

	// インポータ―の作成
	FbxImporter* importer = FbxImporter::Create(manager_, "");
	if (!importer)
	{
		std::cout << "※インポータ―の作成に失敗" << std::endl;
		return false;
	}

	// インポータ―の初期化
	std::string temp_file_path = *file_path;
	ConvertToFullPathOfUTF8(&temp_file_path);
	if (!importer->Initialize(temp_file_path.c_str(), -1, manager_->GetIOSettings()))
	{
		std::cout << "※インポータ―の初期化に失敗" << std::endl;
		return false;
	}

	// シーンの流し込みとインポーターの解放
	importer->Import(scene_);
	importer->Destroy();

	// 座標系の変換
	std::string temp;
	std::cout << "★どちらの座標系にしますか？ DirectX[y], OpenGL[n]" << std::endl;
	do
	{
		std::cout << "⇒";
		std::cin >> temp;
	} while (temp != "y" && temp != "n");

	axis_system_ = temp == "y" ? FbxAxisSystem::DirectX : FbxAxisSystem::OpenGL;

	if (scene_->GetGlobalSettings().GetAxisSystem() != axis_system_)
	{
		std::cout << "★指定座標系と異なる為変換 ⇒ ";

		if (axis_system_ == FbxAxisSystem::DirectX)
		{
			FbxAxisSystem::DirectX.ConvertScene(scene_);
			std::cout << "左手系座標へ変換" << std::endl;
		}
		else
		{
			FbxAxisSystem::OpenGL.ConvertScene(scene_);
			std::cout << "右手系座標へ変換" << std::endl;
		}
	}

	// スケールを㎝単位へ変換
	FbxSystemUnit system_unit = scene_->GetGlobalSettings().GetSystemUnit();
	if (system_unit.GetScaleFactor() != 1.0f)
	{
		FbxSystemUnit::cm.ConvertScene(scene_);
		std::cout << "★スケールを㎝単位へ変換" << std::endl;
	}

	// 全ポリゴンを三角形ポリゴンへ変換
	FbxGeometryConverter geometry_converter(manager_);
	if (geometry_converter.Triangulate(scene_, true))
	{
		std::cout << "★全ポリゴンを三角形ポリゴンへ変換" << std::endl;
		geometry_converter.RemoveBadPolygonsFromMeshes(scene_);
	}

	// マテリアルごとにメッシュを分割
	if (geometry_converter.SplitMeshesPerMaterial(scene_, true))
	{
		std::cout << "★マテリアルごとにメッシュを分割" << std::endl;
	}

	// ルートノードの取得
	root_node_ = scene_->GetRootNode();
	if (!root_node_)
	{
		std::cout << "※ルートノードの取得に失敗" << std::endl;
		return false;
	}

	std::cout << "\n************************************************************" << std::endl;

	return true;
}



bool FbxConverter::ExtractData()
{
	// マテリアルデータ抽出
	ExtractMaterialData();

	// メッシュデータ抽出
	ExtractMeshData();

	return true;
}



void FbxConverter::ExtractMaterialData()
{
	// マテリアル数取得
	int material_num = scene_->GetMaterialCount();
	md_bin_data_container_.setMaterialArraySize(material_num);

	// マテリアルごとの処理
	for (int i = 0; i < material_num; i++)
	{
		// マテリアル取得
		//FbxSurfaceMaterial* material = scene_->GetMaterial(i);
	
		// 各種マテリアル情報抽出
		//ExtractMaterialName(i, material);
		//ExtractAmbientData(i, material);
		//ExtractDiffuseData(i, material);
		//ExtractEmissiveData(i, material);
		//ExtractBumpData(i, material);
		//ExtractTransparentData(i, material);
		//ExtractSpecularData(i, material);
		//ExtractPowerData(i, material);
		//ExtractReflectionData(i, material);
	}
}



void FbxConverter::ExtractMeshData()
{
	// メッシュ数取得
	int mesh_num = scene_->GetSrcObjectCount<FbxMesh>();
	md_bin_data_container_.setMeshArraySize(mesh_num);

	// インデックスの並びで各データを抽出(複数UVの関係上インデックスの最適化は行わない)
	for (int i = 0; i < mesh_num; i++)
	{
		// メッシュ取得
		FbxMesh* mesh = scene_->GetSrcObject<FbxMesh>(i);

		// 各種メッシュ情報抽出
		ExtractIndexData(i, mesh);
		ExtractVertexData(i, mesh);
		//ExtractNormalData(i, mesh);
		//ExtractUVSetData(i, mesh);
		//
		//// マテリアルデータとの関連付け
		//AssociateWithMaterialData(i, mesh);
		//
		//// UVセットデータとテクスチャの関連付け
		//AssociatingUVSetDataAndTexture(i);
	}
}



void FbxConverter::ExtractMaterialName(int material_index, FbxSurfaceMaterial* material)
{
	*md_bin_data_container_.getpMaterial(material_index)->getpName() = material->GetName();
}



void FbxConverter::ExtractAmbientData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sAmbient);
	if (property.IsValid())
	{
		FbxDouble3 ambient = property.Get<FbxDouble3>();
		*md_bin_data_container_.getpMaterial(material_index)->getpAmbient()->getpR() = (float)ambient[0];
		*md_bin_data_container_.getpMaterial(material_index)->getpAmbient()->getpG() = (float)ambient[1];
		*md_bin_data_container_.getpMaterial(material_index)->getpAmbient()->getpB() = (float)ambient[2];

		ExtractTextureData(material_index, property,
						   MdBinDataContainer::Material::Texture::Type::AMBIENT);
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpAmbient()->getpR() = 1.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpAmbient()->getpG() = 1.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpAmbient()->getpB() = 1.0f;
	}
}



void FbxConverter::ExtractDiffuseData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (property.IsValid())
	{
		FbxDouble3 diffuse = property.Get<FbxDouble3>();
		*md_bin_data_container_.getpMaterial(material_index)->getpDiffuse()->getpR() = (float)diffuse[0];
		*md_bin_data_container_.getpMaterial(material_index)->getpDiffuse()->getpG() = (float)diffuse[1];
		*md_bin_data_container_.getpMaterial(material_index)->getpDiffuse()->getpB() = (float)diffuse[2];

		ExtractTextureData(material_index, property,
						   MdBinDataContainer::Material::Texture::Type::DIFFUSE);
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpDiffuse()->getpR() = 1.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpDiffuse()->getpG() = 1.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpDiffuse()->getpB() = 1.0f;
	}
}



void FbxConverter::ExtractEmissiveData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sEmissive);
	if (property.IsValid())
	{
		FbxDouble3 emissive = property.Get<FbxDouble3>();
		*md_bin_data_container_.getpMaterial(material_index)->getpEmissive()->getpR() = (float)emissive[0];
		*md_bin_data_container_.getpMaterial(material_index)->getpEmissive()->getpG() = (float)emissive[1];
		*md_bin_data_container_.getpMaterial(material_index)->getpEmissive()->getpB() = (float)emissive[2];

		ExtractTextureData(material_index, property,
						   MdBinDataContainer::Material::Texture::Type::EMISSIVE);
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpEmissive()->getpR() = 0.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpEmissive()->getpG() = 0.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpEmissive()->getpB() = 0.0f;
	}
}



void FbxConverter::ExtractBumpData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sBump);
	if (property.IsValid())
	{
		FbxDouble3 bump = property.Get<FbxDouble3>();
		*md_bin_data_container_.getpMaterial(material_index)->getpBump()->getpR() = (float)bump[0];
		*md_bin_data_container_.getpMaterial(material_index)->getpBump()->getpG() = (float)bump[1];
		*md_bin_data_container_.getpMaterial(material_index)->getpBump()->getpB() = (float)bump[2];

		ExtractTextureData(material_index, property,
						   MdBinDataContainer::Material::Texture::Type::NORMAL);
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpBump()->getpR() = 0.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpBump()->getpG() = 0.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpBump()->getpB() = 0.0f;
	}
}



void FbxConverter::ExtractTransparentData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (property.IsValid())
	{
		// Blenderで確認すると何故か透明値が逆になる為
		FbxDouble transparent = property.Get<FbxDouble>();
		*md_bin_data_container_.getpMaterial(material_index)->getpTransparent() = 1.0f - (float)transparent;
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpTransparent() = 1.0f;
	}
}



void FbxConverter::ExtractSpecularData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sSpecular);
	if (property.IsValid())
	{
		FbxDouble3 specular = property.Get<FbxDouble3>();
		*md_bin_data_container_.getpMaterial(material_index)->getpSpecular()->getpR() = (float)specular[0];
		*md_bin_data_container_.getpMaterial(material_index)->getpSpecular()->getpG() = (float)specular[1];
		*md_bin_data_container_.getpMaterial(material_index)->getpSpecular()->getpB() = (float)specular[2];

		ExtractTextureData(material_index, property,
						   MdBinDataContainer::Material::Texture::Type::SPECULAR);
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpSpecular()->getpR() = 0.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpSpecular()->getpG() = 0.0f;
		*md_bin_data_container_.getpMaterial(material_index)->getpSpecular()->getpB() = 0.0f;
	}
}



void FbxConverter::ExtractPowerData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sShininess);
	if (property.IsValid())
	{
		FbxDouble3 specular = property.Get<FbxDouble3>();
		*md_bin_data_container_.getpMaterial(material_index)->getpPower() = (float)specular[0];
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpPower() = 0.0f;
	}
}



void FbxConverter::ExtractReflectionData(int material_index, FbxSurfaceMaterial* material)
{
	FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
	if (property.IsValid())
	{
		FbxDouble reflection = property.Get<FbxDouble>();
		*md_bin_data_container_.getpMaterial(material_index)->getpReflection() = (float)reflection;
	}
	else
	{
		*md_bin_data_container_.getpMaterial(material_index)->getpReflection() = 0.0f;
	}
}



void FbxConverter::ExtractTextureData(int material_index, FbxProperty property,
									  MdBinDataContainer::Material::Texture::Type texture_type)
{
	// テクスチャ数取得
	int texture_num = property.GetSrcObjectCount<FbxFileTexture>();
	if (texture_num > 0)
	{
		// テクスチャを作成
		MdBinDataContainer::Material::Texture* texture = new MdBinDataContainer::Material::Texture();
		FbxFileTexture* fbx_texture = property.GetSrcObject<FbxFileTexture>(0);
		*texture->getpFilePath() = fbx_texture->GetRelativeFileName();
		ExtractFileName(texture->getpFilePath(), texture->getpFilePath());
		*texture->getpType() = texture_type;

		// テクスチャを追加
		material_index = material_index;
		md_bin_data_container_.getpMaterial(material_index)->AddTextureArray(texture);
	}
}



void FbxConverter::ExtractIndexData(int mesh_index, FbxMesh* mesh)
{
	// インデックス数取得
	int index_num = mesh->GetPolygonVertexCount();	// インデックス数
	md_bin_data_container_.getpMesh(mesh_index)->setIndexArraySize(index_num);
	
	// インデックス取得
	int* index_array = nullptr;
	index_array = mesh->GetPolygonVertices();	// インデックス配列
	for (int i = 0; i < index_num; i += 3)
	{
		*md_bin_data_container_.getpMesh(mesh_index)->getpIndex(i) = index_array[i];
		*md_bin_data_container_.getpMesh(mesh_index)->getpIndex(i + 1) = index_array[i + 1];
		*md_bin_data_container_.getpMesh(mesh_index)->getpIndex(i + 2) = index_array[i + 2];
	}
}



void FbxConverter::ExtractVertexData(int mesh_index, FbxMesh* mesh)
{
	// 頂点数をインデックス数分確保
	md_bin_data_container_.getpMesh(mesh_index)->setPositionArraySize((md_bin_data_container_.getpMesh(mesh_index)->getIndexArraySize()));

	// 頂点取得
	FbxVector4* vertex_array = mesh->GetControlPoints();	// 頂点配列
	for (int i = 0; i < md_bin_data_container_.getpMesh(mesh_index)->getPositionArraySize(); i++)
	{
		FbxVector4 vertex = vertex_array[*md_bin_data_container_.getpMesh(mesh_index)->getpIndex(i)];
		*md_bin_data_container_.getpMesh(mesh_index)->getpPosition(i)->getpX() = (float)vertex[0];
		*md_bin_data_container_.getpMesh(mesh_index)->getpPosition(i)->getpY() = (float)vertex[1];
		*md_bin_data_container_.getpMesh(mesh_index)->getpPosition(i)->getpZ() = (float)vertex[2];
	}
}



void FbxConverter::ExtractNormalData(int mesh_index, FbxMesh* mesh)
{
	// 法線配列(インデックスと同じ並び)
	FbxArray<FbxVector4> normal_array;
	mesh->GetPolygonVertexNormals(normal_array);

	// 法線数をインデックス数分確保
	md_bin_data_container_.getpMesh(mesh_index)->setNormalArraySize(normal_array.Size());

	// 法線取得
	for (int i = 0; i < normal_array.Size(); i++)
	{
		*md_bin_data_container_.getpMesh(mesh_index)->getpNormal(i)->getpX() = (float)normal_array[i][0];
		*md_bin_data_container_.getpMesh(mesh_index)->getpNormal(i)->getpY() = (float)normal_array[i][1];
		*md_bin_data_container_.getpMesh(mesh_index)->getpNormal(i)->getpZ() = (float)normal_array[i][2];
	}
}



void FbxConverter::ExtractUVSetData(int mesh_index, FbxMesh* mesh)
{
	// UVセット名配列取得
	FbxStringList uv_set_name_array;
	mesh->GetUVSetNames(uv_set_name_array);
	md_bin_data_container_.getpMesh(mesh_index)->setUVSetArraySize(uv_set_name_array.GetCount());

	for (int i = 0; i < uv_set_name_array.GetCount(); i++)
	{
		// UV配列(インデックスと同じ並び)
		FbxArray<FbxVector2> uv_set_array;
		mesh->GetPolygonVertexUVs(uv_set_name_array.GetStringAt(i), uv_set_array);

		// UV数をインデックス数分確保
		md_bin_data_container_.getpMesh(mesh_index)->getpUVSet(i)->setUVArraySize(uv_set_array.Size());

		// UV取得
		for (int j = 0; j < md_bin_data_container_.getpMesh(mesh_index)->getpUVSet(i)->getUVArraySize(); j++)
		{
			*md_bin_data_container_.getpMesh(mesh_index)->getpUVSet(i)->getpU(j) = (float)uv_set_array[j][0];
			if (axis_system_ == FbxAxisSystem::DirectX)
			{
				*md_bin_data_container_.getpMesh(mesh_index)->getpUVSet(i)->getpV(j) = 1.0f - (float)uv_set_array[j][1];
			}
			else
			{
				*md_bin_data_container_.getpMesh(mesh_index)->getpUVSet(i)->getpV(j) = (float)uv_set_array[j][1];
			}
		}
	}
}



void FbxConverter::AssociateWithMaterialData(int mesh_index, FbxMesh* mesh)
{
	// レイヤーとマテリアル情報を関連付け
	int layer_num = mesh->GetLayerCount();
	if (layer_num > 0)
	{
		// メッシュが所属するノード内のマテリアル配列取得
		// 基本レイヤー0番にマテリアルとの関連情報がある
		FbxLayerElementMaterial* material_array = mesh->GetLayer(0)->GetMaterials();

		// メッシュにアタッチしているマテリアルを取得
		FbxLayerElementArrayTemplate<int>* material_index_array
			= &material_array->GetIndexArray();
		int material_index_array_num = material_index_array->GetCount();

		for (int i = 0; i < material_index_array_num; i++)
		{
			// インデックス番号からマテリアル取得
			int index_num = material_index_array->GetAt(i);
			FbxSurfaceMaterial* material = mesh->GetNode()->GetMaterial(index_num);

			// マテリアル名を用いてマテリアル配列と関連付け
			std::string material_name = material->GetName();
			for (int j = 0; j < md_bin_data_container_.getMaterialArraySize(); j++)
			{
				if (material_name
					!= *md_bin_data_container_.getpMaterial(j)->getpName()) continue;

				*md_bin_data_container_.getpMesh(mesh_index)->getpMaterialIndex()
					= j;
				break;
			}
		}

	}
	else
	{
		// マテリアルとの関連情報がない
		*md_bin_data_container_.getpMesh(mesh_index)->getpMaterialIndex() = -1;
	}
}



void FbxConverter::AssociatingUVSetDataAndTexture(int mesh_index)
{
	// UVSetがあるかどうか
	if (md_bin_data_container_.getpMesh(mesh_index)->getUVSetArraySize() <= 0) return;

	// マテリアル取得
	int material_index = *md_bin_data_container_.getpMesh(mesh_index)->getpMaterialIndex();
	MdBinDataContainer::Material* material = md_bin_data_container_.getpMaterial(material_index);

	// マテリアルのテクスチャからディヒューズテクスチャをUVの0番目と関連付ける
	for (int i = 0; i < (int)material->getTextureArraySize(); i++)
	{
		if (*material->getpTexture(i)->getpType()
			!= MdBinDataContainer::Material::Texture::Type::DIFFUSE) continue;

		md_bin_data_container_.getpMesh(mesh_index)->getpUVSet(0)
			->AddTextureArray(material->getpTexture(i));
	}
}



bool FbxConverter::ExportOfMdBinFile(std::string* file_path)
{
	std::cout << "\n[ExportOfMdBinFile]\n" << std::endl;

	// ファイル名を抽出&拡張子除去
	std::string file_name;
	ExtractFileName(&file_name, file_path);
	ExtensionRemoval(&file_name);

	// 出力ディレクトリパス作成
	std::string directory_path;
	CreateExportDirectoryPath(&directory_path, &file_name);

	// 出力フォルダ作成
	CreateExportDirectory(&directory_path);

	// 出力ファイル名作成
	std::string txt_data_file_name;
	CreateExportFileName(&file_name, &txt_data_file_name);

	// 出力パス作成
	std::string export_file_path;
	std::string export_txt_data_file_path;
	CreateExportPath(&export_file_path, &export_txt_data_file_path,
					 &file_name, &txt_data_file_name, &directory_path);

	// 出力
	std::ifstream ifstream(export_file_path.c_str(), std::ios::binary);
	if (ifstream)
	{
		ifstream.close();
		std::cout << "\n同じファイル名『" << file_name << "』があります。" << std::endl;
		std::cout << "上書きしますか？ Yes[y], No[n]" << std::endl;
		std::string temp;
		do
		{
			std::cout << "⇒";
			std::cin >> temp;
		} while (temp != "y" && temp != "n");

		if (temp == "n") return true;
	}
	MdBinDataContainer::ExportData(&md_bin_data_container_, export_file_path);
	//ExportTextData(&export_txt_data_file_path);

	MdBinDataContainer test;
	MdBinDataContainer::InportData(&test, export_file_path);

	std::cout << "\n下記のファイルを出力しました。" << std::endl;
	std::cout << "『" << export_file_path << "』" << std::endl;
	std::cout << "『" << export_txt_data_file_path << "』" << std::endl;
	
	return true;
}



void FbxConverter::ExtractFileName(std::string* export_file_name, std::string* file_path)
{
	// ファイル名を抽出(パス区切り"\"と"/"の両方に対応)
	auto temp_index = file_path->find_last_of("/");	// 最後のパス区切りのインデックスを取得
	if (temp_index != std::string::npos)
	{
		// ファイル名(最後のパス区切りから後ろ)を抽出
		*export_file_name = file_path->substr(temp_index + 1);
	}
	else
	{
		temp_index = file_path->find_last_of("\\");
		if (temp_index != std::string::npos)
		{
			// ファイル名(最後のパス区切りから後ろ)を抽出
			*export_file_name = file_path->substr(temp_index + 1);
		}
		else
		{
			// ファイル名を抽出
			*export_file_name = *file_path;
		}
	}
}



void FbxConverter::ExtensionRemoval(std::string* file_name)
{
	auto temp_index = file_name->find_last_of(".");
	if (temp_index != std::string::npos)
	{
		// ファイル名から拡張子を除く
		*file_name = file_name->substr(0, temp_index);
	}
}



void FbxConverter::CreateExportDirectory(std::string *directory_path)
{
	// 出力フォルダの作成
	if (!PathIsDirectory(directory_path->c_str()))
	{
		if (MakeSureDirectoryPathExists(directory_path->c_str()))
		{
			std::cout << "出力フォルダ『" << *directory_path << "』を作成しました。" << std::endl;
		}
	}
	else
	{
		std::cout << "出力フォルダ『" << *directory_path << "』は作成済みでした。" << std::endl;
	}
}



void FbxConverter::CreateExportDirectoryPath(std::string* directory_path,
											 std::string* file_name)
{
	*directory_path = (axis_system_ == FbxAxisSystem::DirectX ?
					   "MdBin\\mdbin_l\\" : "MdBin\\mdbin_r\\");
	*directory_path += *file_name + "\\";
}



void FbxConverter::CreateExportFileName(std::string* file_name,
										std::string* text_data_file_name)
{
	*text_data_file_name = *file_name + ".txt";
	*file_name += (axis_system_ == FbxAxisSystem::DirectX ?
				   ".mdbin_l" : ".mdbin_r");
}



void FbxConverter::CreateExportPath(std::string* export_file_path,
									std::string* export_txt_data_file_path,
									std::string* file_name,
									std::string* text_data_file_name,
									std::string* directory_path)
{
	*export_file_path = *directory_path + *file_name;
	*export_txt_data_file_path = *directory_path + *text_data_file_name;
}



void FbxConverter::ExportTextData(std::string* export_txt_data_file_path)
{
	std::ofstream ofstream(export_txt_data_file_path->c_str());

	// マテリアル
	ofstream << "[MaterialData]\n" << std::endl;
	int material_num = md_bin_data_container_.getMaterialArraySize();
	ofstream << "★マテリアル数 : " << material_num << "\n" << std::endl;
	for (int i = 0; i < material_num; i++)
	{
		ofstream << "==============================" << std::endl;
		ofstream << "★マテリアル番号 : " << i << "\n" << std::endl;
		ofstream << "Name        : "
			<< *md_bin_data_container_.getpMaterial(i)->getpName()
			<< "\n" << std::endl;
		ofstream << "Ambient     : "
			<< "{" << *md_bin_data_container_.getpMaterial(i)->getpAmbient()->getpR()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpAmbient()->getpG()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpAmbient()->getpB()
			<< "}\n" << std::endl;
		ofstream << "Diffuse     : "
			<< "{" << *md_bin_data_container_.getpMaterial(i)->getpDiffuse()->getpR()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpDiffuse()->getpG()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpDiffuse()->getpB()
			<< "}\n" << std::endl;
		ofstream << "Emissive    : "
			<< "{" << *md_bin_data_container_.getpMaterial(i)->getpEmissive()->getpR()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpEmissive()->getpG()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpEmissive()->getpB()
			<< "}\n" << std::endl;
		ofstream << "Bump        : "
			<< "{" << *md_bin_data_container_.getpMaterial(i)->getpBump()->getpR()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpBump()->getpG()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpBump()->getpB()
			<< "}\n" << std::endl;
		ofstream << "Transparent : "
			<< *md_bin_data_container_.getpMaterial(i)->getpTransparent()
			<< "\n" << std::endl;
		ofstream << "Specular    : "
			<< "{" << *md_bin_data_container_.getpMaterial(i)->getpSpecular()->getpR()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpSpecular()->getpG()
			<< ", " << *md_bin_data_container_.getpMaterial(i)->getpSpecular()->getpB()
			<< "}\n" << std::endl;
		ofstream << "Power       : "
			<< *md_bin_data_container_.getpMaterial(i)->getpPower()
			<< "\n" << std::endl;
		ofstream << "Reflection  : "
			<< *md_bin_data_container_.getpMaterial(i)->getpReflection()
			<< "\n" << std::endl;

		for (int j = 0; j < md_bin_data_container_.getpMaterial(i)->getTextureArraySize(); j++)
		{
			ofstream << "TextureName : " << *md_bin_data_container_.getpMaterial(i)->getpTexture(j)->getpFilePath() << std::endl;
		}

		ofstream << "==============================\n" << std::endl;
	}
	ofstream << "************************************************************" << std::endl;

	// メッシュ
	ofstream << "\n[MeshData]\n" << std::endl;
	int mesh_num = md_bin_data_container_.getMeshArraySize();
	ofstream << "★メッシュ数 : " << mesh_num << "\n" << std::endl;
	for (int i = 0; i < mesh_num; i++)
	{
		ofstream << "==============================" << std::endl;
		ofstream << "★メッシュ番号 : " << i << "\n" << std::endl;

		// インデックスデータの抽出
		int index_num = md_bin_data_container_.getpMesh(i)->getIndexArraySize();
		ofstream << "インデックス数 : " << index_num << std::endl;
		for (int j = 0; j < index_num; j += 3)
		{
			ofstream << "インデックス番号 : "
				<< "{ " << *md_bin_data_container_.getpMesh(i)->getpIndex(j)
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpIndex(j + 1)
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpIndex(j + 2)
				<< " }" << std::endl;
		}
		ofstream << std::endl;

		int vertex_num = md_bin_data_container_.getpMesh(i)->getVertexArraySize();
		ofstream << "頂点数 : " << vertex_num << std::endl;
		for (int j = 0; j < vertex_num; j++)
		{
			ofstream << "頂点番号" << j << " : "
				<< "{ " << *md_bin_data_container_.getpMesh(i)->getpPosition(j)->getpX()
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpPosition(j)->getpY()
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpPosition(j)->getpZ()
				<< " }" << std::endl;
		}
		ofstream << std::endl;

		int normal_num = md_bin_data_container_.getpMesh(i)->getNormalArraySize();
		ofstream << "法線数 : " << normal_num << std::endl;

		// 法線取得
		for (int j = 0; j < normal_num; j++)
		{
			ofstream << "法線番号" << j << " : "
				<< "{ " << *md_bin_data_container_.getpMesh(i)->getpNormal(j)->getpX()
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpNormal(j)->getpY()
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpNormal(j)->getpZ()
				<< " }" << std::endl;
		}
		ofstream << std::endl;

		// UVセット名配列取得
		int uv_set_num = md_bin_data_container_.getpMesh(i)->getUVSetArraySize();
		ofstream << "UVセット数 : " << uv_set_num << std::endl;


		for (int j = 0; j < uv_set_num; j++)
		{
			int uv_num = md_bin_data_container_.getpMesh(i)->getpUVSet(j)->getUVArraySize();
			ofstream << "UV数 : " << uv_num << std::endl;

			// UV取得
			for (int k = 0; k < uv_num; k++)
			{
				ofstream << "UV番号" << k << " : "
					<< "{ " << *md_bin_data_container_.getpMesh(i)->getpUVSet(j)->getpU(k)
					<< ", " << *md_bin_data_container_.getpMesh(i)->getpUVSet(j)->getpV(k)
					<< " }" << std::endl;
			}
		}
		ofstream << std::endl;

		int material_index_num = *md_bin_data_container_.getpMesh(i)->getpMaterialIndex();
		ofstream << "関連付けたマテリアル番号 : " << material_index_num << std::endl;
		ofstream << std::endl;

		for (int j = 0; j < md_bin_data_container_.getpMesh(i)->getpUVSet(0)->getTextureArraySize(); j++)
		{
			ofstream << "UVSetと関連付けたTexture名 : " << *md_bin_data_container_.getpMesh(i)->getpUVSet(0)->getpTexture(j)->getpFilePath() << std::endl;
		}
		ofstream << "==============================\n" << std::endl;
	}

	ofstream.close();
}



void FbxConverter::ConvertToFullPathOfUTF8(std::string* file_path)
{
	// フルパスへ変更
	char full_path[_MAX_PATH];
	_fullpath(full_path, file_path->c_str(), _MAX_PATH);

	// フルパスからUTF8パスのポインタを取得
	char* utf8_path;
	FbxAnsiToUTF8(full_path, utf8_path);

	// UTF8パスをstringへ変換
	*file_path = utf8_path;

	// FBXSDK内で確保されているUTF8パスのメモリを解放
	FbxFree(utf8_path);
}