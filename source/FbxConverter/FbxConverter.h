//================================================================================
//!	@file	 FbxConverter.h
//!	@brief	 FBX変換器Class
//! @details 
//!	@author  Kai Araki									@date 2018/12/29
//================================================================================
#ifndef	_FBX_CONVERTER_H_
#define _FBX_CONVERTER_H_



//****************************************
// インクルード文
//****************************************
#include <fbxsdk.h>
#include <string>

#include "../MdBinDataContainer/MdBinDataContainer.h"



//************************************************************														   
//! @brief   FBX変換器Class
//!
//! @details FBX変換器Class
//************************************************************
class FbxConverter
{
//====================
// 変数
//====================
private:
	FbxManager* manager_ = nullptr;				//!< FBXマネージャ
	FbxIOSettings* io_settings_ = nullptr;		//!< IOセッティング
	FbxScene* scene_ = nullptr;					//!< シーン
	FbxNode* root_node_ = nullptr;				//!< ルートノード
	FbxAxisSystem axis_system_;					//!< 座標系の種類
	bool is_reverse_alpha_ = false;				//!< α値の反転フラグ
	std::string directory_path_;				//!< ディレクトリパス
	MdBinDataContainer md_bin_data_container_;	//!< MdBinデータの入れ物
	FbxTime period_;							//!< 1フレームの時間
	int animation_start_frame_num_ = 0;			//!< アニメーション開始フレーム数
	int animation_stop_frame_num_ = 0;			//!< アニメーション停止フレーム数
	int all_animation_frame_num_ = -1;			//!< アニメーションフレーム数


//====================
// 関数
//====================
public:
	//----------------------------------------
	//! @brief 初期化関数
	//! @details
	//! @param void なし
	//! @retval bool 初期化成功の有無
	//----------------------------------------
	bool Init();

	//----------------------------------------
	//! @brief 終了関数
	//! @details
	//! @param void なし
	//! @retval void なし
	//----------------------------------------
	void Uninit();

	//----------------------------------------
	//! @brief MdBinへ変換関数
	//! @details
	//! @param *file_path ファイルパス
	//! @retval bool 変換成功の有無
	//----------------------------------------
	bool ConvertToMdBin(std::string* file_path);

private:
	//----------------------------------------
	//! @brief 読み込み関数
	//! @details
	//! @param *file_path ファイルパス
	//! @retval bool 読み込み成功の有無
	//----------------------------------------
	bool Load(std::string* file_path);

	//----------------------------------------
	//! @brief フレーム情報抽出関数
	//! @details
	//! @param *importer インポーター
	//! @retval void なし
	//----------------------------------------
	void ExtractFrameData(FbxImporter* importer);

	//----------------------------------------
	//! @brief データ抽出関数
	//! @details
	//! @param void なし
	//! @retval bool データの抽出成功の有無
	//----------------------------------------
	bool ExtractData();

	//----------------------------------------
	//! @brief マテリアルデータの抽出関数
	//! @details
	//! @param void なし
	//! @retval void なし
	//----------------------------------------
	void ExtractMaterialData();

	//----------------------------------------
	//! @brief メッシュデータの抽出関数
	//! @details
	//! @param void なし
	//! @retval void なし
	//----------------------------------------
	void ExtractMeshData();

	//----------------------------------------
	//! @brief マテリアル名の抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractMaterialName(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief アンビエントデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractAmbientData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief ディフューズデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractDiffuseData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief エミッシブデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractEmissiveData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief バンプデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractBumpData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief トランスペアレントデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractTransparentData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief スペキュラデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractSpecularData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief パワー(シャイネス)データの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractPowerData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief リフレクションデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *material       マテリアル
	//! @retval void なし
	//----------------------------------------
	void ExtractReflectionData(int material_index, FbxSurfaceMaterial* material);

	//----------------------------------------
	//! @brief テクスチャデータの抽出関数
	//! @details
	//! @param *material_index マテリアルのインデックス
	//! @param *property       FBXプロパティ
	//! @param texture_type    テクスチャタイプ
	//! @retval void なし
	//----------------------------------------
	void ExtractTextureData(int material_index, FbxProperty property,
							MdBinDataContainer::Material::Texture::Type texture_type);

	//----------------------------------------
	//! @brief インデックスデータの抽出関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @param *mesh       メッシュ
	//! @retval void なし
	//----------------------------------------
	void ExtractIndexData(int mesh_index, FbxMesh* mesh);

	//----------------------------------------
	//! @brief 頂点データの抽出関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @param *mesh       メッシュ
	//! @retval void なし
	//----------------------------------------
	void ExtractVertexData(int mesh_index, FbxMesh* mesh);

	//----------------------------------------
	//! @brief 法線データの抽出関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @param *mesh       メッシュ
	//! @retval void なし
	//----------------------------------------
	void ExtractNormalData(int mesh_index, FbxMesh* mesh);

	//----------------------------------------
	//! @brief UVセットデータの抽出関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @param *mesh       メッシュ
	//! @retval void なし
	//----------------------------------------
	void ExtractUVSetData(int mesh_index, FbxMesh* mesh);

	//----------------------------------------
	//! @brief アニメーションデータの抽出関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @param *mesh       メッシュ
	//! @retval void なし
	//----------------------------------------
	void ExtractAnimationData(int mesh_index, FbxMesh* mesh);

	//----------------------------------------
	//! @brief ボーンデータの抽出関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @param *cluster    クラスタ
	//! @retval void なし
	//----------------------------------------
	void ExtractBoneData(int mesh_index, FbxCluster* cluster);

	//----------------------------------------
	//! @brief ボーンの重みデータの抽出関数
	//! @details
	//! @param *mesh_index             メッシュのインデックス
	//! @param *save_bone_weight_array ボーンの重み保存用配列
	//! @param *cluster                クラスタ
	//! @retval void なし
	//----------------------------------------
	void ExtractBoneWeightData(int mesh_index,
							   std::vector<MdBinDataContainer::Mesh::BoneWeight>* save_bone_weight_array,
							   FbxCluster* cluster);

	//----------------------------------------
	//! @brief 行列変換関数
	//! @details
	//! @param *bone_matrix ボーン行列
	//! @param *fbx_matrix  FBX行列
	//! @retval void なし
	//----------------------------------------
	void ChangeMatrix(MdBinDataContainer::Matrix* bone_matrix,
					  FbxAMatrix* fbx_matrix);

	//----------------------------------------
	//! @brief マテリアルデータとの関連付け関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @param *mesh       メッシュ
	//! @retval void なし
	//----------------------------------------
	void AssociateWithMaterialData(int mesh_index, FbxMesh* mesh);

	//----------------------------------------
	//! @brief UVセットデータとテクスチャの関連付け関数
	//! @details
	//! @param *mesh_index メッシュのインデックス
	//! @retval void なし
	//----------------------------------------
	void AssociatingUVSetDataAndTexture(int mesh_index);

	//----------------------------------------
	//! @brief MdBinファイルの出力関数
	//! @details
	//! @param *file_path ファイルパス
	//! @retval void なし
	//----------------------------------------
	bool ExportOfMdBinFile(std::string* file_path);

	//----------------------------------------
	//! @brief 出力フォルダの作成関数
	//! @details
	//! @param *directory_path ディレクトリパス
	//! @retval void なし
	//----------------------------------------
	void CreateExportDirectory(std::string* directory_path);

	//----------------------------------------
	//! @brief ファイル名抽出関数
	//! @details
	//! @param *export_file_name 出力ファイル名
	//! @param *file_path        ファイルパス
	//! @retval void なし
	//----------------------------------------
	void ExtractFileName(std::string* export_file_name, std::string* file_path);

	//----------------------------------------
	//! @brief 拡張子除去関数
	//! @details
	//! @param *file_name ファイル名
	//! @retval void なし
	//----------------------------------------
	void ExtensionRemoval(std::string* file_name);

	//----------------------------------------
	//! @brief 出力ディレクトリ名作成関数
	//! @details
	//! @param *directory_path ディレクトリパス
	//! @param *file_name      ファイル名
	//! @retval void なし
	//----------------------------------------
	void CreateExportDirectoryPath(std::string* directory_path,
								   std::string* file_name);

	//----------------------------------------
	//! @brief 出力ファイル名作成関数
	//! @details
	//! @param *file_name           ファイル名
	//! @param *text_data_file_name テキストデータのファイル名
	//! @retval void なし
	//----------------------------------------
	void CreateExportFileName(std::string* file_name,
							  std::string* text_data_file_name);

	//----------------------------------------
	//! @brief 出力パス作成関数
	//! @details
	//! @param *export_file_path          出力パス
	//! @param *export_txt_data_file_path テキストデータの出力パス
	//! @param *file_name                 ファイル名
	//! @param *text_data_file_name       テキストデータのファイル名
	//! @param *directory_path            ディレクトリパス
	//! @retval void なし
	//----------------------------------------
	void CreateExportPath(std::string* export_file_path,
						  std::string* export_txt_data_file_path,
						  std::string* file_name,
						  std::string* text_data_file_name,
						  std::string* directory_path);

	//----------------------------------------
	//! @brief テキストデータ出力関数
	//! @details
	//! @param *export_txt_data_file_path テキストデータの出力パス
	//! @retval void なし
	//----------------------------------------
	void ExportTextData(std::string* export_txt_data_file_path);


	//----------------------------------------
	//! @brief UTF8のフルパスへ変換関数
	//! @details
	//! @param *file_path ファイルパス
	//! @retval void なし
	//----------------------------------------
	void ConvertToFullPathOfUTF8(std::string* file_path);
};



#endif