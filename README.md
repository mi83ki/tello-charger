# Tello Charger

小型ドローンTelloの自動充電を可能にする装置です

## 開発環境の構築

開発環境の構築手順を以下に示します

### PlatformIOのインストール

以下のURLを参考にPlatformIOをインストールします\
<https://takabus.com/tips/2402/>

1. VSCodeをインストールする
1. PlatformIOの拡張機能をインストールする
1. プロジェクトを新規作成する
1. 今回はATOM Liteを使用するため、Bord: M5Stack-ATOM を選択する

### ライブラリのインストール

PlatformIOのLibrariesから以下をインストールする

1. M5Atom
1. FastLED

### サブモジュールの取得

以下のコマンドでサブモジュールを取得する

~~~bash
git submodule update --init
~~~
