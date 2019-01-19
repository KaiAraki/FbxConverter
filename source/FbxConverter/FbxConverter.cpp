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

	// シーンの流し込み
	importer->Import(scene_);
	std::cout << "★シーンを作成" << std::endl;

	// フレーム情報抽出
	ExtractFrameData(importer);

	// インポーター解放
	importer->Destroy();

	// 座標系の変換
	std::string temp;
	std::cout << "\n★どちらの座標系にしますか？ DirectX[y], OpenGL[n]" << std::endl;
	do
	{
		std::cout << "⇒";
		std::cin >> temp;
	} while (temp != "y" && temp != "n");

	axis_system_ = temp == "y" ? FbxAxisSystem::DirectX : FbxAxisSystem::OpenGL;

	if (scene_->GetGlobalSettings().GetAxisSystem() != axis_system_)
	{
		std::cout << "※元データが指定座標系と異なる為変換\n⇒";

		if (axis_system_ == FbxAxisSystem::DirectX)
		{
			FbxAxisSystem::DirectX.ConvertScene(scene_);
			std::cout << "左手系座標へ変換完了" << std::endl;
		}
		else
		{
			FbxAxisSystem::OpenGL.ConvertScene(scene_);
			std::cout << "右手系座標へ変換完了" << std::endl;
		}
	}

	// α値反転の確認
	std::cout << "\n★α値を反転しますか？ Yes[y], No[n]" << std::endl;
	do
	{
		std::cout << "⇒";
		std::cin >> temp;
	} while (temp != "y" && temp != "n");

	is_reverse_alpha_ = temp == "y";

	// スケールを㎝単位へ変換
	FbxSystemUnit system_unit = scene_->GetGlobalSettings().GetSystemUnit();
	if (system_unit.GetScaleFactor() != 1.0f)
	{
		FbxSystemUnit::cm.ConvertScene(scene_);
		std::cout << "\n★スケールを㎝単位へ変換" << std::endl;
	}

	// 全ポリゴンを三角形ポリゴンへ変換
	FbxGeometryConverter geometry_converter(manager_);
	if (geometry_converter.Triangulate(scene_, true))
	{
		std::cout << "\n★全ポリゴンを三角形ポリゴンへ変換" << std::endl;
		geometry_converter.RemoveBadPolygonsFromMeshes(scene_);
	}

	// マテリアルごとにメッシュを分割
	if (geometry_converter.SplitMeshesPerMaterial(scene_, true))
	{
		std::cout << "\n★マテリアルごとにメッシュを分割" << std::endl;
	}

	// ルートノードの取得
	root_node_ = scene_->GetRootNode();
	if (!root_node_)
	{
		std::cout << "\n※ルートノードの取得に失敗" << std::endl;
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
		FbxSurfaceMaterial* material = scene_->GetMaterial(i);

		// 各種マテリアル情報抽出
		ExtractMaterialName(i, material);
		ExtractAmbientData(i, material);
		ExtractDiffuseData(i, material);
		ExtractEmissiveData(i, material);
		ExtractBumpData(i, material);
		ExtractTransparentData(i, material);
		ExtractSpecularData(i, material);
		ExtractPowerData(i, material);
		ExtractReflectionData(i, material);
	}
}



void FbxConverter::ExtractMeshData()
{
	// メッシュ数取得
	int mesh_num = scene_->GetSrcObjectCount<FbxMesh>();
	md_bin_data_container_.setMeshArraySize(mesh_num);

	// インデックスの並びで各データを抽出(複数UVの関係上インデックスの最適化は行わない)
	//for (int i = 0; i < 1; i++)
	for (int i = 0; i < mesh_num; i++)
	{
		// メッシュ取得
		FbxMesh* mesh = scene_->GetSrcObject<FbxMesh>(i);

		// 各種メッシュ情報抽出
		ExtractIndexData(i, mesh);
		ExtractVertexData(i, mesh);
		ExtractNormalData(i, mesh);
		ExtractUVSetData(i, mesh);
		ExtractAnimationData(i, mesh);

		// マテリアルデータとの関連付け
		AssociateWithMaterialData(i, mesh);

		// UVセットデータとテクスチャの関連付け
		AssociatingUVSetDataAndTexture(i);
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
		FbxDouble transparent = property.Get<FbxDouble>();
		if (is_reverse_alpha_)
		{
			// Blenderで確認すると何故か透明値が逆になる為
			*md_bin_data_container_.getpMaterial(material_index)->getpTransparent() = 1.0f - (float)transparent;
		}
		else
		{
			*md_bin_data_container_.getpMaterial(material_index)->getpTransparent() = (float)transparent;
		}
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



void FbxConverter::ExtractFrameData(FbxImporter* importer)
{
	// 1フレームの時間を取得
	FbxTime::EMode time_mode = scene_->GetGlobalSettings().GetTimeMode();
	period_.SetTime(0, 0, 0, 1, 0, time_mode);

	int animation_stack_count = importer->GetAnimStackCount();
	std::cout << "\n★テイク数：" << animation_stack_count << std::endl;

	bool is_animation = false;
	for (int i = 0; i < animation_stack_count; i++)
	{
		FbxTakeInfo* take_info = importer->GetTakeInfo(i);
		if (!take_info) continue;	// デフォルトテイクなら無視

		// アニメーションフレーム数を取得
		FbxTime animation_start_time = take_info->mLocalTimeSpan.GetStart();
		FbxTime animation_stop_time = take_info->mLocalTimeSpan.GetStop();
		animation_start_frame_num_ = (int)(animation_start_time.Get() / period_.Get());
		animation_stop_frame_num_ = (int)(animation_stop_time.Get() / period_.Get());
		all_animation_frame_num_ = animation_stop_frame_num_ - animation_start_frame_num_ + 1;

		is_animation = true;
		break;
	}
	if (is_animation)
	{
		std::cout << "⇒アニメーション情報：有" << std::endl;
		std::cout << "⇒START_FRAME_NUM：" << animation_start_frame_num_ << std::endl;
		std::cout << "⇒STOP_FRAME_NUM：" << animation_stop_frame_num_ << std::endl;
		std::cout << "⇒ALL_FRAME_NUM：" << all_animation_frame_num_ << std::endl;
	}
	else
	{
		std::cout << "⇒アニメーション情報：無" << std::endl;
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



void FbxConverter::ExtractAnimationData(int mesh_index, FbxMesh* mesh)
{
	// ボーンの重み保存用配列を頂点数分確保
	std::vector<MdBinDataContainer::Mesh::BoneWeight> save_bone_weight_array;
	int save_bone_weight_num = mesh->GetControlPointsCount();
	save_bone_weight_array.resize(save_bone_weight_num);
	for (int i = 0; i < save_bone_weight_num; i++)
	{
		save_bone_weight_array[i].Init();
	}

	// スキン情報の取得(スキン1つのみ取得)
	int skin_num = mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < skin_num; i++)
	{
		FbxSkin* skin = (FbxSkin*)(mesh->GetDeformer(i, FbxDeformer::eSkin));

		// クラスタ(ボーン1本分の情報の塊)からボーンデータを抽出
		int cluster_num = skin->GetClusterCount();
		for (int j = 0; j < cluster_num; j++)
		{
			FbxCluster* cluster = skin->GetCluster(j);

			// 各種データ抽出
			ExtractBoneData(mesh_index, cluster);
			ExtractBoneWeightData(mesh_index, &save_bone_weight_array, cluster);
		}
		break;
	}

	// ボーンの重みを頂点へ代入
	int bone_weight_num = md_bin_data_container_.getpMesh(mesh_index)->getIndexArraySize();
	md_bin_data_container_.getpMesh(mesh_index)->setBoneWeightArraySize(bone_weight_num);
	for (int i = 0; i < bone_weight_num; i++)
	{
		int index = *md_bin_data_container_.getpMesh(mesh_index)->getpIndex(i);
		*md_bin_data_container_.getpMesh(mesh_index)->getpBoneWeight(i) = save_bone_weight_array[index];
	}
}



void FbxConverter::ExtractBoneData(int mesh_index, FbxCluster* cluster)
{

	int affected_vertices_num = cluster->GetControlPointIndicesCount();
	if (affected_vertices_num <= 0) return;

	// ボーン名取得
	MdBinDataContainer::Mesh::Bone bone;
	*bone.getpName() = cluster->GetName();

	// オフセット行列取得
	FbxAMatrix init_matrix;
	cluster->GetTransformLinkMatrix(init_matrix);
	FbxAMatrix offset_matrix = init_matrix.Inverse();
	ChangeMatrix(bone.getpOffsetMatrix(), &offset_matrix);

	// アニメーション行列取得
	if (all_animation_frame_num_ != -1)
	{
		bone.setAnimationMatrixArraySize(all_animation_frame_num_);
		for (int i = 0; i < all_animation_frame_num_; i++)
		{
			FbxAMatrix animation_matrix;
			FbxTime time = (animation_start_frame_num_ + i) * period_.Get();
			animation_matrix = cluster->GetLink()->EvaluateGlobalTransform(time);
			ChangeMatrix(bone.getpAnimationMatrixArray(i), &animation_matrix);
		}
	}
	else
	{
		int default_array_num = 1;
		bone.setAnimationMatrixArraySize(default_array_num);
		FbxAMatrix default_matrix;
		default_matrix.SetIdentity();
		ChangeMatrix(bone.getpAnimationMatrixArray(0), &default_matrix);
	}
	
	// メッシュのボーン配列へ追加
	md_bin_data_container_.getpMesh(mesh_index)->AddBoneArray(&bone);
}



void FbxConverter::ExtractBoneWeightData(int mesh_index,
										 std::vector<MdBinDataContainer::Mesh::BoneWeight>* save_bone_weight_array,
										 FbxCluster* cluster)
{
	// メッシュのボーン配列の末尾のインデックス取得
	int bone_index = md_bin_data_container_.getpMesh(mesh_index)->getBoneArrayEndIndex();

	// 影響する頂点配列数取得
	int affected_vertices_num = cluster->GetControlPointIndicesCount();

	// 影響する頂点配列&重み配列取得
	int* affected_vertices_array = cluster->GetControlPointIndices();
	double* weight_array_ = cluster->GetControlPointWeights();

	// ボーンの重みを取得
	for (int i = 0; i < affected_vertices_num; i++)
	{
		int vertex_index = affected_vertices_array[i];
		float bone_weight = (float)weight_array_[i];

		save_bone_weight_array->at(vertex_index)
			.setBoneIndexAndWeight(bone_index,
								   bone_weight);
	}
}



void FbxConverter::ChangeMatrix(MdBinDataContainer::Matrix* bone_matrix,
								FbxAMatrix* fbx_matrix)
{
	for (int i = 0; i < MdBinDataContainer::Matrix::ARRAY_HEIGHT; i++)
	{
		for (int j = 0; j < MdBinDataContainer::Matrix::ARRAY_WIDTH; j++)
		{
			bone_matrix
				->setMatrixElement((float)fbx_matrix->Get(i, j), i, j);
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
	ExportTextData(&export_txt_data_file_path);

	std::cout << "\n下記のファイルを出力しました。\n" << std::endl;
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

		// インデックス
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

		// 頂点
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

		// 法線
		int normal_num = md_bin_data_container_.getpMesh(i)->getNormalArraySize();
		ofstream << "法線数 : " << normal_num << std::endl;

		for (int j = 0; j < normal_num; j++)
		{
			ofstream << "法線番号" << j << " : "
				<< "{ " << *md_bin_data_container_.getpMesh(i)->getpNormal(j)->getpX()
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpNormal(j)->getpY()
				<< ", " << *md_bin_data_container_.getpMesh(i)->getpNormal(j)->getpZ()
				<< " }" << std::endl;
		}
		ofstream << std::endl;

		// UVセット
		int uv_set_num = md_bin_data_container_.getpMesh(i)->getUVSetArraySize();
		ofstream << "UVセット数 : " << uv_set_num << std::endl;

		for (int j = 0; j < uv_set_num; j++)
		{
			int uv_num = md_bin_data_container_.getpMesh(i)->getpUVSet(j)->getUVArraySize();
			ofstream << "UV数 : " << uv_num << std::endl;

			for (int k = 0; k < uv_num; k++)
			{
				ofstream << "UV番号" << k << " : "
					<< "{ " << *md_bin_data_container_.getpMesh(i)->getpUVSet(j)->getpU(k)
					<< ", " << *md_bin_data_container_.getpMesh(i)->getpUVSet(j)->getpV(k)
					<< " }" << std::endl;
			}
		}
		ofstream << std::endl;

		// ボーン
		int bone_num = md_bin_data_container_.getpMesh(i)->getBoneArraySize();
		ofstream << "ボーン数 : " << bone_num << std::endl;

		for (int j = 0; j < bone_num; j++)
		{
			ofstream << "ボーン番号" << j << " : " << std::endl;
			ofstream << "ボーン名 : "
				<< "{ " << *md_bin_data_container_.getpMesh(i)->getpBone(j)->getpName()
				<< " }" << std::endl;

			ofstream << "オフセット行列 : " << std::endl;
			for (int k = 0; k < MdBinDataContainer::Matrix::ARRAY_HEIGHT; k++)
			{
				ofstream << "{ ";
				for (int l = 0; l < MdBinDataContainer::Matrix::ARRAY_WIDTH; l++)
				{
					ofstream << md_bin_data_container_.getpMesh(i)
						->getpBone(j)->getpOffsetMatrix()->getMatrixElement(k, l)
						<< " ";
				}
				ofstream << " }" << std::endl;
			}

			ofstream << "アニメーション行列 : " << std::endl;
			for (int k = 0; k < md_bin_data_container_.getpMesh(i)->getpBone(j)->getAnimationMatrixArraySize(); k++)
			{
				ofstream << k << "フレーム目 : " << std::endl;
				for (int l = 0; l < MdBinDataContainer::Matrix::ARRAY_HEIGHT; l++)
				{
					ofstream << "{ ";
					for (int m = 0; m < MdBinDataContainer::Matrix::ARRAY_WIDTH; m++)
					{
						ofstream << md_bin_data_container_.getpMesh(i)
							->getpBone(j)->getpOffsetMatrix()->getMatrixElement(l, m)
							<< " ";
					}
					ofstream << " }" << std::endl;
				}
			}
			
		}
		ofstream << std::endl;

		// ボーンの重み
		int bone_weight_num = md_bin_data_container_.getpMesh(i)->getBoneWeightArraySize();
		ofstream << "ボーンの重み数 : " << bone_weight_num << std::endl;

		for (int j = 0; j < bone_weight_num; j++)
		{
			ofstream << "ボーンの重み頂点番号" << j << " : ";
			for (int k = 0; k < MdBinDataContainer::Mesh::BoneWeight::MAX_BONE_NUM; k++)
			{
				ofstream << "{ " << md_bin_data_container_.getpMesh(i)->getpBoneWeight(j)->getBoneIndex(k)
					<< ", " << md_bin_data_container_.getpMesh(i)->getpBoneWeight(j)->getBoneWeight(k)
					<< " } ";
			}
			ofstream << std::endl;
		}
		ofstream << std::endl;

		// 関連付けたマテリアル番号
		int material_index_num = *md_bin_data_container_.getpMesh(i)->getpMaterialIndex();
		ofstream << "関連付けたマテリアル番号 : " << material_index_num << std::endl;
		ofstream << std::endl;

		// 関連付けたテクスチャ名
		if (md_bin_data_container_.getpMesh(i)->getUVSetArraySize() > 0)
		{
			for (int j = 0; j < md_bin_data_container_.getpMesh(i)->getpUVSet(0)->getTextureArraySize(); j++)
			{
				ofstream << "UVSetと関連付けたTexture名 : " << *md_bin_data_container_.getpMesh(i)->getpUVSet(0)->getpTexture(j)->getpFilePath() << std::endl;
			}
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