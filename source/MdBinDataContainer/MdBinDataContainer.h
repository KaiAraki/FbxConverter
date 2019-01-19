//================================================================================
//!	@file	 MdBin.h
//!	@brief	 バイナリーモデルデータの入れ物Class
//! @details 
//!	@author  Kai Araki									@date 2018/12/29
//================================================================================
#ifndef	_MD_BIN_DATA_CONTAINER_H_
#define _MD_BIN_DATA_CONTAINER_H_



//****************************************
// インクルード文
//****************************************
#include <stdio.h>
#include <vector>
#include <string>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>



//************************************************************														   
//! @brief   バイナリーモデルデータの入れ物Class
//!
//! @details バイナリーモデルデータの入れ物のClass
//************************************************************
class MdBinDataContainer
{
//====================
// クラス定義
//====================
public:
	//****************************************											   
	//! @brief   ベクター2Class
	//!
	//! @details float変数を2つもつClass
	//****************************************
	class Vector2
	{
	//====================
	// 変数
	//====================
	private:
		float x_;		//!< X成分
		float y_;		//!< Y成分


	//====================
	// プロパティ
	//====================
	public:
		//----------------------------------------
		//! @brief X成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* X成分
		//----------------------------------------
		float* getpX();

		//----------------------------------------
		//! @brief Y成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* Y成分
		//----------------------------------------
		float* getpY();

	//====================
	// シリアライズ
	//====================
	private:
		//----------------------------------------
		//! @brief シリアライズ(I/O)関数
		//! @details
		//! @param archive アーカイブ用クラス
		//! @param version バージョン
		//! @retval void なし
		//----------------------------------------
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive& archive, const unsigned version)
		{
			if (version) {}

			archive & x_;
			archive & y_;
		}
	};

	//****************************************											   
	//! @brief   ベクター3Class
	//!
	//! @details float変数を3つもつClass
	//****************************************
	class Vector3
	{
	//====================
	// 変数
	//====================
	private:
		Vector2 vector2_;	//!< ベクター2
		float z_;			//!< Z成分


	//====================
	// プロパティ
	//====================
	public:
		//----------------------------------------
		//! @brief X成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* X成分
		//----------------------------------------
		float* getpX();

		//----------------------------------------
		//! @brief Y成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* Y成分
		//----------------------------------------
		float* getpY();

		//----------------------------------------
		//! @brief Z成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* Z成分
		//----------------------------------------
		float* getpZ();


	//====================
	// シリアライズ
	//====================
	private:
		//----------------------------------------
		//! @brief シリアライズ(I/O)関数
		//! @details
		//! @param archive アーカイブ用クラス
		//! @param version バージョン
		//! @retval void なし
		//----------------------------------------
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive& archive, const unsigned version)
		{
			if (version) {}

			archive & vector2_;
			archive & z_;
		}
	};

	//****************************************											   
	//! @brief   色Class
	//!
	//! @details 色のClass
	//****************************************
	class Color
	{
	//====================
	// 変数
	//====================
	private:
		float r_;	//!< 赤成分
		float g_;	//!< 緑成分
		float b_;	//!< 青成分


	//====================
	// プロパティ
	//====================
	public:
		//----------------------------------------
		//! @brief 赤成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* 赤成分
		//----------------------------------------
		float* getpR();

		//----------------------------------------
		//! @brief 緑成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* 緑成分
		//----------------------------------------
		float* getpG();

		//----------------------------------------
		//! @brief 青成分取得関数
		//! @details
		//! @param void なし
		//! @retval float* 青成分
		//----------------------------------------
		float* getpB();


	//====================
	// シリアライズ
	//====================
	private:
		//----------------------------------------
		//! @brief シリアライズ(I/O)関数
		//! @details
		//! @param archive アーカイブ用クラス
		//! @param version バージョン
		//! @retval void なし
		//----------------------------------------
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive& archive, const unsigned version)
		{
			if (version) {}

			archive & r_;
			archive & g_;
			archive & b_;
		}
	};

	//****************************************											   
	//! @brief   行列Class
	//!
	//! @details 行列のClass
	//****************************************
	class Matrix
	{
	//====================
	// 定数
	//====================
	public:
		static const unsigned ARRAY_WIDTH = 4;
		static const unsigned ARRAY_HEIGHT = 4;

	//====================
	// 変数
	//====================
	private:
		float array_[ARRAY_HEIGHT][ARRAY_WIDTH];	//!< 4x4行列


	//====================
	// プロパティ
	//====================
	public:
		//----------------------------------------
		//! @brief 配列要素取得関数
		//! @details
		//! @param index0 0番目のインデックス
		//! @param index1 1番目のインデックス
		//! @retval float 配列要素
		//----------------------------------------
		float getMatrixElement(unsigned index0, unsigned index1);

		//----------------------------------------
		//! @brief 配列要素設定関数
		//! @details
		//! @param value  値
		//! @param index0 0番目のインデックス
		//! @param index1 1番目のインデックス
		//! @retval void なし
		//----------------------------------------
		void setMatrixElement(float value, unsigned index0, unsigned index1);


	//====================
	// シリアライズ
	//====================
	private:
		//----------------------------------------
		//! @brief シリアライズ(I/O)関数
		//! @details
		//! @param archive アーカイブ用クラス
		//! @param version バージョン
		//! @retval void なし
		//----------------------------------------
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive& archive, const unsigned version)
		{
			if (version) {}

			archive & array_;
		}
	};


	//****************************************											   
	//! @brief   マテリアルClass
	//!
	//! @details マテリアルのClass
	//****************************************
	class Material
	{
	//====================
	// クラス定義
	//====================
	public:
		//****************************************											   
		//! @brief   テクスチャClass
		//!
		//! @details テクスチャの種類ごとのClass
		//****************************************
		class Texture
		{
		//====================
		// 列挙型定義
		//====================
		public:
			enum class Type
			{
				NONE,
				AMBIENT,
				DIFFUSE,
				EMISSIVE,
				NORMAL,
				SPECULAR,
				MAX
			};


		//====================
		// 変数
		//====================
		private:
			std::string file_path_;		//!< ファイルパス
			Type type_;					//!< テクスチャタイプ


		//====================
		// プロパティ
		//====================
		public:
			//----------------------------------------
			//! @brief ファイルパス取得関数
			//! @details
			//! @param void なし
			//! @retval std::string* ファイルパス
			//----------------------------------------
			std::string* getpFilePath();

			//----------------------------------------
			//! @brief テクスチャタイプ取得関数
			//! @details
			//! @param void なし
			//! @retval Type* テクスチャタイプ
			//----------------------------------------
			Type* getpType();

		//====================
		// シリアライズ
		//====================
		private:
			//----------------------------------------
			//! @brief シリアライズ(I/O)関数
			//! @details
			//! @param archive アーカイブ用クラス
			//! @param version バージョン
			//! @retval void なし
			//----------------------------------------
			friend class boost::serialization::access;
			template <class Archive>
			void serialize(Archive& archive, const unsigned version)
			{
				if (version) {}

				archive & file_path_;
				archive & type_;
			}
		};


	//====================
	// 変数
	//====================
	private:
		std::string name_;					//!< 名前
		Color ambient_;						//!< アンビエント(環境光)
		Color diffuse_;						//!< ディヒューズ(拡散反射光)
		Color emissive_;					//!< エミッシブ(放射光)
		Color bump_;						//!< バンプ(凹凸具合)
		float transparent_;					//!< トランスペアレント(透過度)
		Color specular_;					//!< スペキュラ(鏡面反射)
		float power_;						//!< パワー(スペキュラの減衰強度)
		float reflection_;					//!< リフレクション(反射強度)
		std::vector<Texture*> texture_;		//!< テクスチャ配列


	//====================
	// プロパティ
	//====================
	public:
		//----------------------------------------
		//! @brief 名前取得関数
		//! @details
		//! @param void なし
		//! @retval std::string* 名前
		//----------------------------------------
		std::string* getpName();

		//----------------------------------------
		//! @brief アンビエント取得関数
		//! @details
		//! @param void なし
		//! @retval Color* アンビエント
		//----------------------------------------
		Color* getpAmbient();

		//----------------------------------------
		//! @brief ディフューズ取得関数
		//! @details
		//! @param void なし
		//! @retval Color* ディフューズ
		//----------------------------------------
		Color* getpDiffuse();

		//----------------------------------------
		//! @brief エミッション取得関数
		//! @details
		//! @param void なし
		//! @retval Color* エミッション
		//----------------------------------------
		Color* getpEmissive();

		//----------------------------------------
		//! @brief バンプ取得関数
		//! @details
		//! @param void なし
		//! @retval Color* バンプ
		//----------------------------------------
		Color* getpBump();

		//----------------------------------------
		//! @brief トランスペアレント取得関数
		//! @details
		//! @param void なし
		//! @retval float* トランスペアレント
		//----------------------------------------
		float* getpTransparent();

		//----------------------------------------
		//! @brief スペキュラ取得関数
		//! @details
		//! @param void なし
		//! @retval Color* スペキュラー
		//----------------------------------------
		Color* getpSpecular();

		//----------------------------------------
		//! @brief パワー取得関数
		//! @details
		//! @param void なし
		//! @retval float* パワー
		//----------------------------------------
		float* getpPower();

		//----------------------------------------
		//! @brief リフレクション取得関数
		//! @details
		//! @param void なし
		//! @retval float* リフレクション
		//----------------------------------------
		float* getpReflection();

		//----------------------------------------
		//! @brief テクスチャ配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getTextureArraySize();

		//----------------------------------------
		//! @brief テクスチャ取得関数
		//! @details
		//! @param index インデックス
		//! @retval Texture テクスチャ
		//----------------------------------------
		Texture* getpTexture(int index);


	//====================
	// 関数
	//====================
	public:
		//----------------------------------------
		//! @brief テクスチャ配列追加関数
		//! @details
		//! @param *texture テクスチャ
		//! @retval void なし
		//----------------------------------------
		void AddTextureArray(Texture* texture);

		//----------------------------------------
		//! @brief テクスチャ解放関数
		//! @details
		//! @param void なし
		//! @retval void なし
		//----------------------------------------
		void ReleaseTextureArray();


	//====================
	// シリアライズ
	//====================
	private:
		//----------------------------------------
		//! @brief シリアライズ(I/O)関数
		//! @details
		//! @param archive アーカイブ用クラス
		//! @param version バージョン
		//! @retval void なし
		//----------------------------------------
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive& archive, const unsigned version)
		{
			if (version) {}

			archive & name_;
			archive & ambient_;
			archive & diffuse_;
			archive & emissive_;
			archive & bump_;
			archive & transparent_;
			archive & specular_;
			archive & power_;
			archive & reflection_;
			archive & texture_;
		}
	};


	//****************************************											   
	//! @brief   メッシュClass
	//!
	//! @details メッシュのClass
	//****************************************
	class Mesh
	{
	//====================
	// クラス定義
	//====================
	public:
		//****************************************											   
		//! @brief   UVセットClass
		//!
		//! @details テクスチャの種類ごとのUVのClass
		//****************************************
		class UVSet
		{
		//====================
		// 変数
		//====================
		private:
			std::vector<Vector2> uv_array_;				//!< UV配列
			std::vector<Material::Texture*> texture_;	//!< テクスチャ配列


		//====================
		// プロパティ
		//====================
		public:
			//----------------------------------------
			//! @brief UV配列サイズ取得関数
			//! @details
			//! @param void なし 
			//! @retval int サイズ
			//----------------------------------------
			int getUVArraySize();

			//----------------------------------------
			//! @brief UV配列サイズ設定関数
			//! @details
			//! @param value サイズ 
			//! @retval void なし
			//----------------------------------------
			void setUVArraySize(int value);

			//----------------------------------------
			//! @brief U成分取得関数
			//! @details
			//! @param index インデックス
			//! @retval float* U成分
			//----------------------------------------
			float* getpU(int index);

			//----------------------------------------
			//! @brief V成分取得関数
			//! @details
			//! @param index インデックス
			//! @retval float* V成分
			//----------------------------------------
			float* getpV(int index);

			//----------------------------------------
			//! @brief テクスチャ配列サイズ取得関数
			//! @details
			//! @param void なし 
			//! @retval int サイズ
			//----------------------------------------
			int getTextureArraySize();

			//----------------------------------------
			//! @brief テクスチャ取得関数
			//! @details
			//! @param index インデックス
			//! @retval Texture テクスチャ
			//----------------------------------------
			Material::Texture* getpTexture(int index);

		//====================
		// 関数
		//====================
		public:
			//----------------------------------------
			//! @brief テクスチャ配列追加関数
			//! @details
			//! @param *texture テクスチャ
			//! @retval void なし
			//----------------------------------------
			void AddTextureArray(Material::Texture* texture);


		//====================
		// シリアライズ
		//====================
		private:
			//----------------------------------------
			//! @brief シリアライズ(I/O)関数
			//! @details
			//! @param archive アーカイブ用クラス
			//! @param version バージョン
			//! @retval void なし
			//----------------------------------------
			friend class boost::serialization::access;
			template <class Archive>
			void serialize(Archive& archive, const unsigned version)
			{
				if (version) {}

				archive & uv_array_;
				archive & texture_;
			}
		};


		//****************************************											   
		//! @brief   ボーンClass
		//!
		//! @details ボーンのClass
		//****************************************
		class Bone
		{
		//====================
		// 変数
		//====================
		private:
			std::string name_;								//!< ボーン名
			Matrix offset_matrix_;							//!< オフセット行列(初期姿勢の逆行列)
			std::vector<Matrix> animetion_matrix_array_;	//!< アニメーション行列


		//====================
		// プロパティ
		//====================
		public:
			//----------------------------------------
			//! @brief ボーン名取得関数
			//! @details
			//! @param void なし
			//! @retval std::string* ボーン名
			//----------------------------------------
			std::string* getpName();

			//----------------------------------------
			//! @brief オフセット行列取得関数
			//! @details
			//! @param void なし
			//! @retval float* オフセット配列
			//----------------------------------------
			Matrix* getpOffsetMatrix();

			//----------------------------------------
			//! @brief アニメーション行列配列サイズ取得関数
			//! @details
			//! @param void なし
			//! @retval int アニメーション行列配列サイズ
			//----------------------------------------
			int getAnimationMatrixArraySize();

			//----------------------------------------
			//! @brief アニメーション行列配列サイズ設定関数
			//! @details
			//! @param value アニメーション行列配列サイズ
			//! @retval void なし
			//----------------------------------------
			void setAnimationMatrixArraySize(int value);

			//----------------------------------------
			//! @brief アニメーション行列取得関数
			//! @details
			//! @param index インデックス
			//! @retval Matrix* アニメーション行列
			//----------------------------------------
			Matrix* getpAnimationMatrixArray(int index);


		//====================
		// シリアライズ
		//====================
		private:
			//----------------------------------------
			//! @brief シリアライズ(I/O)関数
			//! @details
			//! @param archive アーカイブ用クラス
			//! @param version バージョン
			//! @retval void なし
			//----------------------------------------
			friend class boost::serialization::access;
			template <class Archive>
			void serialize(Archive& archive, const unsigned version)
			{
				if (version) {}

				archive & name_;
				archive & offset_matrix_;
				archive & animetion_matrix_array_;
			}
		};


		//****************************************											   
		//! @brief   ボーン重みClass
		//!
		//! @details ボーンの重みClass
		//****************************************
		class BoneWeight
		{
		//====================
		// 定数
		//====================
		public:
			static const unsigned MAX_BONE_NUM = 4;		//!< 最大ボーン数


		//====================
		// 変数
		//====================
		private:
			int bone_index_[MAX_BONE_NUM];		//!< ボーンインデックス
			float bone_weight_[MAX_BONE_NUM];	//!< ボーンの重み


		//====================
		// プロパティ
		//====================
		public:
			//----------------------------------------
			//! @brief ボーンインデックス取得関数
			//! @details
			//! @param index インデックス
			//! @retval int ボーンインデックス
			//----------------------------------------
			int getBoneIndex(int index);

			//----------------------------------------
			//! @brief ボーンの重み取得関数
			//! @details
			//! @param index インデックス
			//! @retval float ボーンの重み
			//----------------------------------------
			float getBoneWeight(int index);

			//----------------------------------------
			//! @brief ボーンインデックス&重み設定関数
			//! @details
			//! @param bone_index ボーンインデックス
			//! @param bone_weight ボーンの重み
			//! @retval int* ボーンインデックス
			//----------------------------------------
			void setBoneIndexAndWeight(int bone_index, float bone_weight);


		//====================
		// 関数
		//====================
		public:
			//----------------------------------------
			//! @brief 初期化関数
			//! @details
			//! @param void なし
			//! @retval void なし
			//----------------------------------------
			void Init();


		//====================
		// シリアライズ
		//====================
		private:
			//----------------------------------------
			//! @brief シリアライズ(I/O)関数
			//! @details
			//! @param archive アーカイブ用クラス
			//! @param version バージョン
			//! @retval void なし
			//----------------------------------------
			friend class boost::serialization::access;
			template <class Archive>
			void serialize(Archive& archive, const unsigned version)
			{
				if (version) {}

				archive & bone_index_;
				archive & bone_weight_;
			}
		};


	//====================
	// 変数
	//====================
	private:
		std::vector<Vector3> position_array_;		//!< 座標配列
		std::vector<Vector3> normal_array_;			//!< 法線配列
		std::vector<UVSet> uv_set_array_;			//!< UVセット配列
		std::vector<int> index_array_;				//!< インデックス配列
		std::vector<Bone> bone_array_;				//!< ボーン配列
		std::vector<BoneWeight> bone_weight_array_;	//!< ボーンの重み配列
		int material_index_;						//!< マテリアルインデックス

	//====================
	// プロパティ
	//====================
	public:
		//----------------------------------------
		//! @brief 頂点配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getVertexArraySize();

		//----------------------------------------
		//! @brief 座標配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getPositionArraySize();

		//----------------------------------------
		//! @brief 座標配列サイズ設定関数
		//! @details
		//! @param value サイズ 
		//! @retval void なし
		//----------------------------------------
		void setPositionArraySize(int value);

		//----------------------------------------
		//! @brief 座標取得関数
		//! @details
		//! @param index インデックス
		//! @retval Vector3* 頂点
		//----------------------------------------
		Vector3* getpPosition(int index);

		//----------------------------------------
		//! @brief 法線配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getNormalArraySize();

		//----------------------------------------
		//! @brief 法線配列サイズ設定関数
		//! @details
		//! @param value サイズ 
		//! @retval void なし
		//----------------------------------------
		void setNormalArraySize(int value);

		//----------------------------------------
		//! @brief 法線取得関数
		//! @details
		//! @param index インデックス
		//! @retval Vector3* 法線
		//----------------------------------------
		Vector3* getpNormal(int index);

		//----------------------------------------
		//! @brief UVセット配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getUVSetArraySize();

		//----------------------------------------
		//! @brief UVセット配列サイズ設定関数
		//! @details
		//! @param value サイズ 
		//! @retval void なし
		//----------------------------------------
		void setUVSetArraySize(int value);

		//----------------------------------------
		//! @brief UVセット取得関数
		//! @details
		//! @param index インデックス
		//! @retval UVSet* UVセット
		//----------------------------------------
		UVSet* getpUVSet(int index);

		//----------------------------------------
		//! @brief インデックス配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getIndexArraySize();

		//----------------------------------------
		//! @brief インデックス配列サイズ設定関数
		//! @details
		//! @param value サイズ 
		//! @retval void なし
		//----------------------------------------
		void setIndexArraySize(int value);

		//----------------------------------------
		//! @brief インデックス取得関数
		//! @details
		//! @param index インデックス
		//! @retval int インデックス
		//----------------------------------------
		int* getpIndex(int index);

		//----------------------------------------
		//! @brief ボーン配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getBoneArraySize();

		//----------------------------------------
		//! @brief ボーン配列サイズ設定関数
		//! @details
		//! @param value サイズ 
		//! @retval void なし
		//----------------------------------------
		void setBoneArraySize(int value);

		//----------------------------------------
		//! @brief ボーン取得関数
		//! @details
		//! @param index インデックス
		//! @retval Bone* ボーン
		//----------------------------------------
		Bone* getpBone(int index);

		//----------------------------------------
		//! @brief ボーン配列末尾インデックス取得関数
		//! @details
		//! @param void なし
		//! @retval int ボーン配列末尾インデックス
		//----------------------------------------
		int getBoneArrayEndIndex();

		//----------------------------------------
		//! @brief ボーンの重み配列サイズ取得関数
		//! @details
		//! @param void なし 
		//! @retval int サイズ
		//----------------------------------------
		int getBoneWeightArraySize();

		//----------------------------------------
		//! @brief ボーンの重み配列サイズ設定関数
		//! @details
		//! @param value サイズ 
		//! @retval void なし
		//----------------------------------------
		void setBoneWeightArraySize(int value);

		//----------------------------------------
		//! @brief ボーンの重み取得関数
		//! @details
		//! @param index インデックス
		//! @retval BoneWeight* ボーンの重み
		//----------------------------------------
		BoneWeight* getpBoneWeight(int index);

		//----------------------------------------
		//! @brief マテリアルインデックス取得関数
		//! @details
		//! @param void なし
		//! @retval int* マテリアルインデックス
		//----------------------------------------
		int* getpMaterialIndex();

	//====================
	// 関数
	//====================
	public:
		//----------------------------------------
		//! @brief ボーン配列追加関数
		//! @details
		//! @param bone ボーン
		//! @retval void なし
		//----------------------------------------
		void AddBoneArray(Bone* bone);


	//====================
	// シリアライズ
	//====================
	private:
		//----------------------------------------
		//! @brief シリアライズ(I/O)関数
		//! @details
		//! @param archive アーカイブ用クラス
		//! @param version バージョン
		//! @retval void なし
		//----------------------------------------
		friend class boost::serialization::access;
		template <class Archive>
		void serialize(Archive& archive, const unsigned version)
		{
			if (version) {}

			archive & position_array_;
			archive & normal_array_;
			archive & uv_set_array_;
			archive & index_array_;
			archive & bone_weight_array_;
			archive & material_index_;
		}
	};


//====================
// static関数
//====================
public:
	//----------------------------------------
	//! @brief インポートデータ関数
	//! @details
	//! @param *md_bin_data_container インポート先
	//! @param file_path              ファイルパス
	//! @retval bool 読み込み成功の有無
	//----------------------------------------
	static bool InportData(MdBinDataContainer* md_bin_data_container,
						   std::string file_path);

	//----------------------------------------
	//! @brief エクスポートデータ関数
	//! @details
	//! @param *md_bin_data_container エクスポート先
	//! @param file_path              ファイルパス
	//! @retval bool 書き込み成功の有無
	//----------------------------------------
	static bool ExportData(MdBinDataContainer* md_bin_data_container,
						   std::string file_path);

//====================
// 変数
//====================
private:
	std::vector<Mesh>     mesh_array_;		//!< メッシュ配列
	std::vector<Material> material_array_;	//!< マテリアル配列


//====================
// プロパティ
//====================
public:
	//----------------------------------------
	//! @brief メッシュ配列サイズ取得関数
	//! @details
	//! @param void なし 
	//! @retval int サイズ
	//----------------------------------------
	int getMeshArraySize();

	//----------------------------------------
	//! @brief メッシュ配列サイズ設定関数
	//! @details
	//! @param value サイズ
	//! @retval void なし
	//----------------------------------------
	void setMeshArraySize(int value);

	//----------------------------------------
	//! @brief メッシュ取得関数
	//! @details
	//! @param index インデックス
	//! @retval Mesh* メッシュ
	//----------------------------------------
	Mesh* getpMesh(int index);

	//----------------------------------------
	//! @brief マテリアル配列サイズ取得関数
	//! @details
	//! @param void なし 
	//! @retval int サイズ
	//----------------------------------------
	int getMaterialArraySize();

	//----------------------------------------
	//! @brief マテリアル配列サイズ設定関数
	//! @details
	//! @param value サイズ
	//! @retval void なし
	//----------------------------------------
	void setMaterialArraySize(int value);

	//----------------------------------------
	//! @brief マテリアル取得関数
	//! @details
	//! @param index インデックス
	//! @retval Material* マテリアル
	//----------------------------------------
	Material* getpMaterial(int index);


//====================
// 関数
//====================
public:
	//----------------------------------------
	//! @brief デストラクタ関数
	//! @details
	//! @param void なし
	//----------------------------------------
	~MdBinDataContainer();


//====================
// シリアライズ
//====================
private:
	//----------------------------------------
	//! @brief シリアライズ(I/O)関数
	//! @details
	//! @param archive アーカイブ用クラス
	//! @param version バージョン
	//! @retval void なし
	//----------------------------------------
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive& archive, const unsigned version)
	{
		if (version) {}

		archive & mesh_array_;
		archive & material_array_;
	}
};



#endif