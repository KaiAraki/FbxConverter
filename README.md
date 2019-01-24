FBXConverter
====================

FBXファイルから独自形式へ変換するコンバーター。

![Build status][Mark_build]
[![Build status][Mark_build]](#)

## Description
FBXからDirectXとOpenGLそれぞれで使いたいデーターを抽出し、boost::serializationでシリアライズ化している。

## Features
・スキン：1番目のスキンのみ対応。
・UVSet：1番目のUVSetをディヒューズテクスチャ用と想定し実装。
・テクスチャパス：変換後のバイナリーファイルと同じ階層でコンバート。
・ログ：変換後のバイナリーデータのログも確認用に出力される。

## Requirement
コンパイルする場合は以下をインストールすること。
・[FBXSDK2018.1.1](https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-archives)
･[boost1.69.0](https://www.boost.org/users/history/version_1_69_0.html)

## Usage
画面の指示に従い入力していくと、MdBinフォルダを生成しその中に変換済みデータが保存される。

## Install
ダウンロードしたファイルを展開するだけで使用可能。

## Author
[Coron4444][author]


[author]: https://github.com/Coron4444
[Mark_build]: https://img.shields.io/badge/build-passing-brightgreen.svg