FBXConverter
====================

FBXファイルから独自形式へ変換するコンバーター。

[![version_status][url_shields_version]](#)
[![build_status][url_shields_build]](#)
[![coverage_status][url_shields_coverage]](#)
[![coverage_license][url_shields_license]](#)

Table of Contents
--------------------
* [Description](#Description)
* [Description](#Description)
* [Requirements](#Requirement)
* [Install](#Install)
* [Usage](#Usage)
* [Document](#Document)
* [Author](#Author)
* [License](#License)

Description
--------------------
FBXからDirectXとOpenGLそれぞれで使いたいデーターを抽出し、boost::serializationでシリアライズ化している。

![screen][url_screen]

Features
--------------------
* スキン：1番目のスキンのみ対応。
* UVSet：1番目のUVSetをディヒューズテクスチャ用と想定し実装。
* テクスチャパス：変換後のバイナリーファイルと同じ階層でコンバート。
* ログ：変換後のバイナリーデータのログも確認用に出力される。

Requirement
--------------------
コンパイルする場合は以下をインストールすること。
* [FBXSDK2018.1.1][url_fbx_sdk]
* [boost1.69.0][url_boost]

Install
--------------------
ダウンロードしたファイルを展開するだけで使用可能。

Usage
--------------------
画面の指示に従い入力していくと、MdBinフォルダを生成しその中に変換済みデータが保存される。

Document
--------------------
* [Document][url_document] by [Doxygen][url_doxygen].

Author
--------------------
[Kai Araki][url_author]

License
--------------------
Paddington is licensed under the [MIT][url_mit] license.  
Copyright &copy; 2019 Kai Araki


[url_shields_version]: https://img.shields.io/badge/version-v1.0-blue.svg
[url_shields_build]: https://img.shields.io/badge/build-passing-brightgreen.svg
[url_shields_coverage]: https://img.shields.io/badge/build-passing-brightgreen.svg
[url_shields_license]: https://img.shields.io/badge/license-MIT-blue.svg
[url_screen]: https://coron4444.github.io/FbxConverter/resource/image.png
[url_fbx_sdk]: https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-archives
[url_boost]: https://www.boost.org/users/history/version_1_69_0.html
[url_document]: https://coron4444.github.io/FbxConverter/Document/html/index.html
[url_doxygen]: http://www.doxygen.jp/
[url_author]: https://github.com/Coron4444
[url_mit]: https://opensource.org/licenses/mit-license.php