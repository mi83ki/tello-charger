<!-- omit in toc -->
# Tello Charger 設計

Tello Chargerの設計について記載する。

<!-- omit in toc -->
## 目次

- [Telloとは](#telloとは)
- [1. 要件定義](#1-要件定義)
  - [1.1. ユースケース図](#11-ユースケース図)
- [2. 機構設計](#2-機構設計)
  - [2.1. アイデアスケッチ](#21-アイデアスケッチ)
  - [2.2. 3DCAD設計](#22-3dcad設計)
- [電気設計](#電気設計)
  - [回路ブロック図](#回路ブロック図)
- [ソフト設計](#ソフト設計)

## Telloとは

Telloとは、Ryze Tech社が販売している100g以下のトイドローン。\
ここでは、SDKが公開されているTello EDUを自動充電するための装置を開発する。
Tello EDU: <https://www.ryzerobotics.com/jp/tello-edu>

## 1. 要件定義

### 1.1. ユースケース図

![usecase](img/usecase.drawio.svg)

## 2. 機構設計

### 2.1. アイデアスケッチ

開始当初の自動充電構想。
着陸後、中心に寄せて充電プラグを接続するイメージ。
![ideaSketch](img/ideaSketch.drawio.svg)

ドローンの位置を中央に寄せる機構と、自動充電プラグを接続する機構のイメージ。
RCサーボ2軸で簡単に実現したい。
![ideaSketch](img/ideaSketch2.drawio.svg)

### 2.2. 3DCAD設計

3DCADにて詳細設計。\
3Dプリンタで印刷する。
![mechaDesign](img/mechaDesign.drawio.svg)

## 電気設計

### 回路ブロック図

回路ブロック図は以下になります。
![blockDiagram](img/blockDiagram.drawio.svg)

## ソフト設計

