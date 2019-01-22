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
#include <iostream>

#include "FbxConverter/FbxConverter.h"



//--------------------------------------------------
// -アプリケーションのエントリー関数
//--------------------------------------------------
int main(void)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	
	// 初期化
	FbxConverter fbx_converter;
	if (!fbx_converter.Init())
	{
		fbx_converter.Uninit();
		std::cout << "※初期化に失敗" << std::endl;
		rewind(stdin);
		getchar();
		return -1;
	}
	
	// 変換
	std::string file_path;
	std::cout << "FBXファイルパスの入力(Drag&Dropも可)\n⇒";
	std::cin >> file_path;
	if (!fbx_converter.ConvertToMdBin(&file_path))
	{
		std::cout << "※FBXの変換に失敗" << std::endl;
	}
	
	// 終了
	fbx_converter.Uninit();

	rewind(stdin);
	getchar();

	return 0;
}