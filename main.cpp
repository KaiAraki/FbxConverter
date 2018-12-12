//================================================================================
//!	@file	 main.cpp
//!	@brief	 メイン
//! @details 
//!	@author  Kai Araki									@date 2018/12/12
//================================================================================



//****************************************
// インクルード文
//****************************************
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <iostream>
#include <string>
#include <fbxsdk.h>



//****************************************
// 定数定義
//****************************************



//****************************************
// プロトタイプ宣言
//****************************************
bool Init();
bool Load(std::string* file_path, FbxAxisSystem fbx_axis_system);
void ConvertToFullPathOfUTF8(std::string* file_path);

bool ExportOfConvertedFBXFile(std::string* file_path);
void CreateExportDirectory();
void CreateExportFileName(std::string* export_file_path, std::string* file_path);



//****************************************
// グローバル変数宣言
//****************************************
FbxManager* manager_ = nullptr;
FbxIOSettings* io_settings_ = nullptr;
FbxScene* scene_ = nullptr;


//--------------------------------------------------
// -アプリケーションのエントリー関数
//--------------------------------------------------
int main( void )
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	// 初期化
	if (!Init())
	{
		std::cout << "初期化に失敗" << std::endl;
		rewind(stdin);
		getchar();
		return -1;
	}

	// 読み込み
	std::string file_path = "BoxUnityChan.fbx";
	//std::cout << "FBXファイルパスの入力(Drag&Dropも可)\n⇒";
	//std::cin >> file_path;
	if (!Load(&file_path, FbxAxisSystem::DirectX))
	{
		std::cout << "FBXの読み込み失敗" << std::endl;
		rewind(stdin);
		getchar();
		return -1;
	}

	// マネージャ解放(関連する全オブジェクトが解放される)
	manager_->Destroy();

	std::cout << "FBXの読み込み成功" << std::endl;

	rewind(stdin);
	getchar();

	return 0;
}



//--------------------------------------------------
// -初期化関数
//--------------------------------------------------
bool Init()
{
	// マネージャの生成
	manager_ = FbxManager::Create();
	if (!manager_)
	{
		std::cout << "マネージャの作成に失敗" << std::endl;
		return false;
	}

	// IOSettingsオブジェクト(全Inport/Exportの設定を保持する)の作成
	io_settings_ = FbxIOSettings::Create(manager_, IOSROOT);
	if (!io_settings_)
	{
		std::cout << "FbxIOSettingsの作成に失敗" << std::endl;
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
		std::cout << "シーンの作成に失敗" << std::endl;
		return false;
	}

	return true;
}



//--------------------------------------------------
// -読み込み関数
//--------------------------------------------------
bool Load(std::string* file_path, FbxAxisSystem fbx_axis_system)
{
	// インポータ―の作成
	FbxImporter* importer = FbxImporter::Create(manager_, "");
	if (!importer)
	{
		std::cout << "インポータ―の作成に失敗" << std::endl;
		return false;
	}

	// インポータ―の初期化
	std::string temp_file_path = *file_path;
	ConvertToFullPathOfUTF8(&temp_file_path);
	if (!importer->Initialize(temp_file_path.c_str(), -1, manager_->GetIOSettings()))
	{
		std::cout << "インポータ―の初期化に失敗" << std::endl;
		return false;
	}

	// シーンの流し込みとインポーターの解放
	importer->Import(scene_);
	importer->Destroy();

	// 座標系の変換
	if (scene_->GetGlobalSettings().GetAxisSystem() != fbx_axis_system)
	{
		std::cout << "指定座標系と異なる為変換 ⇒ ";

		if (fbx_axis_system == FbxAxisSystem::DirectX)
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
		std::cout << "スケールを㎝単位へ変換" << std::endl;
	}

	// 全ポリゴンを三角形ポリゴンへ変換
	FbxGeometryConverter geometry_converter(manager_);
	if (geometry_converter.Triangulate(scene_, true))
	{
		std::cout << "全ポリゴンを三角形ポリゴンへ変換" << std::endl;
		geometry_converter.RemoveBadPolygonsFromMeshes(scene_);
	}

	// マテリアルごとにメッシュを分割
	if (geometry_converter.SplitMeshesPerMaterial(scene_, true))
	{
		std::cout << "マテリアルごとにメッシュを分割" << std::endl;
	}
	

	// ルートノードの取得
	FbxNode* root_node = scene_->GetRootNode();
	if (!root_node)
	{
		std::cout << "ルートノードの取得に失敗" << std::endl;
		return false;
	}

	// 変換済みFBXファイルの出力
	if (ExportOfConvertedFBXFile(file_path))
	{
		std::cout << "変換済みFBXファイルを出力しました" << std::endl;
	}
	else
	{
		std::cout << "変換済みFBXファイルを出力しませんでした" << std::endl;
	}

	return true;
}



//--------------------------------------------------
// -UTF8のフルパスへ変換関数
//--------------------------------------------------
void ConvertToFullPathOfUTF8(std::string* file_path)
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



//--------------------------------------------------
// -変換済みFBXファイルの出力関数
//--------------------------------------------------
bool ExportOfConvertedFBXFile(std::string* file_path)
{
	// 出力フォルダの作成
	CreateExportDirectory();

	// 出力ファイル名作成
	std::string export_file_path;
	CreateExportFileName(&export_file_path, file_path);

	// 出力ファイルの書き込み
	std::string temp = "mdbin/" + export_file_path;
	FILE* file = fopen(temp.c_str(), "rb");
	if (file != nullptr)
	{
		std::cout << "同じファイル名『" << export_file_path << "』があります。" << std::endl;
		std::cout << "上書きしますか？ Yes[y], No[n]" << std::endl;
		do
		{
			std::cout << "⇒"; 
			std::cin >> temp;
		} while (temp != "y" && temp != "n");
		
		if (temp == "n") return false;
	}

	file = fopen(export_file_path.c_str(), "wb");


	fclose(file);

	return true;
}



//--------------------------------------------------
// -出力フォルダの作成関数
//--------------------------------------------------
void CreateExportDirectory()
{
	// 出力フォルダの作成
	if (_mkdir("mdbin") == 0)
	{
		std::cout << "出力フォルダを作成" << std::endl;
	}
	else
	{
		std::cout << "出力フォルダは既に作成済みでした" << std::endl;
	}
}



//--------------------------------------------------
// -出力ファイル名の作成関数
//--------------------------------------------------
void CreateExportFileName(std::string* export_file_path, std::string* file_path)
{
	// ファイル名を抽出(パス区切り"\"と"/"の両方に対応)
	auto temp_index = file_path->find_last_of("/");	// 最後のパス区切りのインデックスを取得
	if (temp_index != std::string::npos)
	{
		// ファイル名(最後のパス区切りから後ろ)を抽出
		*export_file_path = file_path->substr(temp_index + 1);
	}
	else
	{
		temp_index = file_path->find_last_of("\\");
		if (temp_index != std::string::npos)
		{
			// ファイル名(最後のパス区切りから後ろ)を抽出
			*export_file_path = file_path->substr(temp_index + 1);
		}
		else
		{
			// ファイル名を抽出
			*export_file_path = *file_path;
		}
	}

	// 拡張子を変更
	temp_index = export_file_path->find_last_of(".");
	if (temp_index != std::string::npos)
	{
		// ファイル名から拡張子を除く
		*export_file_path = export_file_path->substr(0, temp_index + 1);

		// 新規拡張子
		*export_file_path += "mdbin";
	}

	std::cout << "ファイル名：" << *export_file_path << std::endl;
}