//================================================================================
//!	@file	 ExportFile.h
//!	@brief	 ファイル出力Class
//! @details License : MIT
//!	@author  Kai Araki									@date 2019/01/22
//================================================================================
#ifndef	_EXPORT_FILE_H_
#define _EXPORT_FILE_H_



//****************************************
// インクルード文
//****************************************
#include <fbxsdk.h>
#include <string>

#include "../MdBinData/MdBinData.h"



//************************************************************														   
//! @brief   ファイル出力Class
//!
//! @details ファイル出力用Class
//************************************************************
class ExportFile
{
//====================
// 変数
//====================
private:
	MdBinData* md_bin_data_;				//!< MdBinデータ
	FbxAxisSystem axis_;					//!< 軸
	std::string file_name_;					//!< 出力ディレクトリパス
	std::string export_directory_path_;		//!< 出力ディレクトリパス


//====================
// プロパティ
//====================
public:
	//----------------------------------------
	//! @brief MdBinデータ設定関数
	//! @details
	//! @param *md_bin_data MdBinデータ
	//! @retval void なし
	//----------------------------------------
	void setMdBinData(MdBinData* md_bin_data);

	//----------------------------------------
	//! @brief 軸設定関数
	//! @details
	//! @param axis 軸
	//! @retval void なし
	//----------------------------------------
	void setAxis(FbxAxisSystem axis);


//====================
// 関数
//====================
public:
	//----------------------------------------
	//! @brief 出力関数
	//! @details
	//! @param *file_path ファイルパス
	//! @retval void なし
	//----------------------------------------
	void Export(std::string* file_path);

private:
	//----------------------------------------
	//! @brief MdBinファイルの出力関数
	//! @details
	//! @param void なし
	//! @retval bool 上書きの有無
	//----------------------------------------
	bool ExportOfMdBinFile();

	//----------------------------------------
	//! @brief ログファイルの出力関数
	//! @details
	//! @param void なし
	//! @retval void なし
	//----------------------------------------
	void ExportOfLogFile();

	//----------------------------------------
	//! @brief マテリアルログファイルの出力関数
	//! @details
	//! @param *directory_path ディレクトリパス
	//! @retval void なし
	//----------------------------------------
	void ExportOfMaterialLogFile(std::string* directory_path);

	//----------------------------------------
	//! @brief メッシュログファイルの出力関数
	//! @details
	//! @param *directory_path ディレクトリパス
	//! @retval void なし
	//----------------------------------------
	void ExportOfMeshLogFile(std::string* directory_path);

	//----------------------------------------
	//! @brief ファイル名作成関数
	//! @details
	//! @param *file_path ファイルパス
	//! @retval void なし
	//----------------------------------------
	void CreateFileName(std::string* file_path);

	//----------------------------------------
	//! @brief 出力ディレクトリ名作成関数
	//! @details
	//! @param void なし
	//! @retval void なし
	//----------------------------------------
	void CreateExportDirectoryPath();

	//----------------------------------------
	//! @brief ログディレクトリ名作成関数
	//! @details
	//! @param *material_directory_path マテリアルディレクトリパス
	//! @param *mesh_directory_path     メッシュディレクトリパス
	//! @retval void なし
	//----------------------------------------
	void CreateLogDirectoryPath(std::string* material_directory_path,
								std::string* mesh_directory_path);

	//----------------------------------------
	//! @brief インデックスディレクトリ名作成関数
	//! @details
	//! @param *index_directory_path インデックスディレクトリパス
	//! @param *directory_path       ディレクトリパス
	//! @param index				 インデックス
	//! @retval void なし
	//----------------------------------------
	void CreateIndexDirectoryPath(std::string* index_directory_path,
								  std::string* directory_path,
								  int index);

	//----------------------------------------
	//! @brief 出力ディレクトリの作成関数
	//! @details
	//! @param *directory_path ディレクトリパス
	//! @retval void なし
	//----------------------------------------
	void CreateExportDirectory(std::string* directory_path);

	//----------------------------------------
	//! @brief 出力ファイル名作成関数
	//! @details
	//! @param *file_name ファイル名
	//! @retval void なし
	//----------------------------------------
	void CreateExportFileName(std::string* file_name);

	//----------------------------------------
	//! @brief インデックスログファイル名作成関数
	//! @details
	//! @param *file_name ファイル名
	//! @param index	  インデックス
	//! @retval void なし
	//----------------------------------------
	void CreateIndexLogFileName(std::string* file_name,
								int index);

	//----------------------------------------
	//! @brief 出力パス作成関数
	//! @details
	//! @param *export_file_path 出力パス
	//! @param *directory_path   ディレクトリパス
	//! @param *export_file_name 出力ファイル名
	//! @retval void なし
	//----------------------------------------
	void CreateExportPath(std::string* export_file_path,
						  std::string* directory_path,
						  std::string* export_file_name);
};



#endif