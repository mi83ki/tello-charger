<!-- omit in toc -->
# Tello Charger 設計

Tello Chargerの設計について記載する。

<!-- omit in toc -->
## 目次

- [1. Telloとは](#1-telloとは)
- [2. 要件定義](#2-要件定義)
  - [2.1. ユースケース図](#21-ユースケース図)
- [3. 機構設計](#3-機構設計)
  - [3.1. アイデアスケッチ](#31-アイデアスケッチ)
  - [3.2. 3DCAD設計](#32-3dcad設計)
- [4. 電気設計](#4-電気設計)
  - [4.1. 回路ブロック図](#41-回路ブロック図)
  - [4.2. 回路図](#42-回路図)
  - [4.3. 基板レイアウト](#43-基板レイアウト)
  - [4.4. 主要部品リスト](#44-主要部品リスト)
- [5. ソフト設計](#5-ソフト設計)
  - [5.1. API仕様](#51-api仕様)

## 1. Telloとは

Telloとは、Ryze Tech社が販売している100g以下のトイドローン。\
ここでは、SDKが公開されているTello EDUを自動充電するための装置を開発する。
Tello EDU: <https://www.ryzerobotics.com/jp/tello-edu>

## 2. 要件定義

### 2.1. ユースケース図

![usecase](img/usecase.drawio.svg)

## 3. 機構設計

### 3.1. アイデアスケッチ

開始当初の自動充電構想。
着陸後、中心に寄せて充電プラグを接続するイメージ。
![ideaSketch](img/ideaSketch.drawio.svg)

ドローンの位置を中央に寄せる機構と、自動充電プラグを接続する機構のイメージ。
RCサーボ2軸で簡単に実現したい。
![ideaSketch](img/ideaSketch2.drawio.svg)

### 3.2. 3DCAD設計

3DCADにて詳細設計。\
3Dプリンタで印刷する。
![mechaDesign](img/mechaDesign.drawio.svg)

## 4. 電気設計

### 4.1. 回路ブロック図

回路ブロック図は以下になります。

- マイコンはATOM Liteを採用
- 以下2点の目的で電流センサを搭載する
  - 充電電流を監視し、充電状態を取得する
  - サーボ電流を監視し、過負荷を検知する
- 充電電圧をMOSFETで制御（Telloの電源を入れる目的）

![blockDiagram](img/blockDiagram.drawio.svg)

### 4.2. 回路図

回路図を以下に示します。
![schematic](img/schematic.png)

### 4.3. 基板レイアウト

部品の配置図を示します。
![pcbLayout](img/pcbLayout.drawio.svg)

### 4.4. 主要部品リスト

| No. | 品名              | 型番                             | 個数 | 参考URL                                         |
| --- | ----------------- | -------------------------------- | ---- | ----------------------------------------------- |
| 1   | ATOM Lite         | ATOM Lite                        | 1    | <https://www.switch-science.com/products/6262>  |
| 2   | RCサーボ          | GWSMIRMGFA                       | 2    | <https://akizukidenshi.com/catalog/g/gM-01908/> |
| 3   | 電流センサ        | INA219使用電流センサーモジュール | 1    | <https://akizukidenshi.com/catalog/g/gM-08221/> |
| 4   | USB-A DIP化       | AE-USB-A-DIP                     | 1    | <https://akizukidenshi.com/catalog/g/gK-07429/> |
| 5   | USB-C DIP化       | AE-USB2.0-TYPE-C-CTRPB-1         | 1    | <https://akizukidenshi.com/catalog/g/gK-17123/> |
| 6   | Nch MOSFET        | 2SK4017(Q)                       | 1    | <https://akizukidenshi.com/catalog/g/gI-07597/> |
| 7   | フェライトビーズ  | BLM18RK121SN1D                   | 1    | <https://akizukidenshi.com/catalog/g/gP-04445/> |
| 8   | チップ抵抗        | 1608 10kΩ                        | 1    |                                                 |
| 9   | ピンヘッダ 1x5    | PH-1x40SG                        | 1    | <https://akizukidenshi.com/catalog/g/gC-00167/> |
| 9   | ピンヘッダ 1x4    | PH-1x40SG                        | 1    | <https://akizukidenshi.com/catalog/g/gC-00167/> |
| 9   | ピンヘッダL型 2x3 | PH-2x10RG                        | 1    | <https://akizukidenshi.com/catalog/g/gC-10903/> |
| 10  | ピンソケット 1x6  | FH-1x6SG/RH                      | 適量 | <https://akizukidenshi.com/catalog/g/gC-03784/> |

## 5. ソフト設計

### 5.1. API仕様

以下にAPI仕様を記載します。
[Tello Charger API](api.yaml)