<!-- omit in toc -->
# Tello Charger

小型ドローンTelloの自動充電を可能にする装置です。

<!-- omit in toc -->
## 目次

- [1. Tello Chargerとは](#1-tello-chargerとは)
- [2. 製作するために必要なデータ](#2-製作するために必要なデータ)
  - [2.1. 3Dデータ](#21-3dデータ)
  - [2.2. 回路図](#22-回路図)
  - [2.3. ソースコード](#23-ソースコード)
    - [2.3.1. 開発環境の構築](#231-開発環境の構築)
      - [2.3.1.1. VSCodeのインストール](#2311-vscodeのインストール)
      - [2.3.1.2. PlatformIOのインストール](#2312-platformioのインストール)
    - [2.3.2. 初期設定](#232-初期設定)
      - [2.3.2.1. サブモジュールの取得](#2321-サブモジュールの取得)
      - [2.3.2.2. ssid.hの作成](#2322-ssidhの作成)
    - [2.3.3. ソフトウェアの書き込み](#233-ソフトウェアの書き込み)
      - [2.3.3.1. ワークスペースを開く](#2331-ワークスペースを開く)
      - [2.3.3.2. ビルド](#2332-ビルド)
      - [2.3.3.3. 書き込み](#2333-書き込み)
- [3. WebAPI仕様](#3-webapi仕様)
- [4. 概要設計書](#4-概要設計書)

## 1. Tello Chargerとは

Tello EDUの自動充電装置です。

以下はドローンにUSBコネクタを接続する機構の動画です。

[![ドローンキャッチ](doc/design/img/drone_catch.drawio.svg)](https://youtube.com/shorts/hyMHSOp_BFY)

以下はドローンを自動充電する一連動作の動画です。

[![ドローン着陸](doc/design/img/drone_land.drawio.svg)](https://youtube.com/shorts/O7KXj6DnHRI)

## 2. 製作するために必要なデータ

製作するために必要なデータを以下に説明します。

### 2.1. 3Dデータ

3Dデータ（STEP）は`step/`ディレクトリに保存しています。\
STLデータ等に変換し、3Dプリンタで印刷します。\
3Dデータの品番リストは[概要設計書](doc/design/design.md)を参照してください。

### 2.2. 回路図

回路図（Eegleデータ）は`eagle/`ディレクトリに保存しています。\
必要な部品を購入し、ユニバーサル基板で手はんだします。\
必要な部品のリストは[概要設計書](doc/design/design.md)を参照してください。

### 2.3. ソースコード

ATOM Liteに書き込むソースコードは`src/`ディレクトリに保存しています。

#### 2.3.1. 開発環境の構築

##### 2.3.1.1. VSCodeのインストール

Arduinoマイコンへの書き込みも可能なエディタとしてVSCodeを推奨する。\
以下のURLに従ってVSCodeをインストールする。\
<https://miya-system-works.com/blog/detail/vscode-install/>

##### 2.3.1.2. PlatformIOのインストール

以下URLを参考に、VSCodeの拡張機能であるPlatformIOをインストールする。\
<https://zenn.dev/kotaproj/articles/esp32_vscode_pio>

#### 2.3.2. 初期設定

初回に必要な作業を記載します。

##### 2.3.2.1. サブモジュールの取得

以下のコマンドでサブモジュールを取得する必要があります。

~~~bash
git submodule update --init
~~~

##### 2.3.2.2. ssid.hの作成

WiFiの接続先SSIDとパスワードは以下の手順で設定します。

1. 「src/TelloCharger/src/」フォルダの「ssid_example.h」を同じフォルダにコピーして「ssid.h」に名称変更する

2. 「ssid.h」の各項目を自分の環境に合わせて変更する

    ~~~h
    /**
     * @file ssid_example.h
     * @brief SSID情報を記述するヘッダファイルの例
     *
     * @details 環境に合わせて、ファイル名をssid.hにリネームして利用してください（ssid.hはgitignoreされている）
     */

    #pragma once

    #define WIFI_SSID "XXXXXXXXXXX"
    #define WIFI_PASSWORD "XXXXXXXX"

    /** 固定IPアドレス（"MY_FIXED_IP_1"."MY_FIXED_IP_2"."MY_FIXED_IP_3"."MY_FIXED_IP_4"） */
    #define MY_FIXED_IP_1 (192)
    #define MY_FIXED_IP_2 (168)
    #define MY_FIXED_IP_3 (1)
    #define MY_FIXED_IP_4 (12)

    /** ゲートウェイのIPアドレス（"MY_FIXED_IP_1"."MY_FIXED_IP_2"."MY_FIXED_IP_3"."GATEWAY_IP_4"） */
    #define GATEWAY_IP_4 (1)
    ~~~

#### 2.3.3. ソフトウェアの書き込み

##### 2.3.3.1. ワークスペースを開く

「xsa-cloud/xsa-cloud.code-workspace」をダブルクリックしてVSCodeで開く。

##### 2.3.3.2. ビルド

左下の`✓`ボタンを押下する。

##### 2.3.3.3. 書き込み

クナイデバイスをUSBケーブルでPCに接続し、左下の`→`ボタンを押下する。

## 3. WebAPI仕様

TelloChargerは上位システムからWebAPIで制御します。\
以下のAPI仕様を参照してください。

[Tello Charger API](doc/openapi/api.html)

## 4. 概要設計書

概要設計内容は以下を参照してください。

[概要設計書](doc/design/design.md)
