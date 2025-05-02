<!-- omit in toc -->
# MQTT通信仕様書

<!-- omit in toc -->
## 改訂履歴

| 日付 | 版 | 変更者 | 変更内容 |
|------|----|--------|----------|
| 2025/5/2 | 0.1.0 | Miyazaki | 初版作成 |

<!-- omit in toc -->
## 目次

- [1. 目的](#1-目的)
- [2. システム全体概要](#2-システム全体概要)
- [3. ブローカー設定](#3-ブローカー設定)
- [4. トピック定義](#4-トピック定義)
- [5. ペイロード仕様](#5-ペイロード仕様)
  - [5.1. 共通ヘッダー (要求/応答系)](#51-共通ヘッダー-要求応答系)
  - [5.2. `ChargeStatusPayload`](#52-chargestatuspayload)
  - [5.3. `ChargeStartRequestPayload`](#53-chargestartrequestpayload)
  - [5.4. `ChargeStartResponsePayload`](#54-chargestartresponsepayload)
  - [5.5. `ChargeStopRequestPayload`](#55-chargestoprequestpayload)
  - [5.6. `ChargeStopResponsePayload`](#56-chargestopresponsepayload)
  - [5.7. `PowerOnRequestPayload`](#57-poweronrequestpayload)
  - [5.8. `PowerOnResponsePayload`](#58-poweronresponsepayload)

---

## 1. 目的

このドキュメントは、システム内で使用する **MQTT 通信** の仕様を統一し、開発者・運用者間で共通理解を得ることを目的とします。

---

## 2. システム全体概要

![MQTT](img/mqttDiagram.drawio.svg)

---

## 3. ブローカー設定

| 項目 | 値 | 備考 |
|------|----|------|
| ホスト名 | `XX.XX.XX.XX` | MQTTブローカーのホスト |
| ポート | `1883` | — |
| プロトコル | MQTT v3.1 | — |

---

## 4. トピック定義

> `{device_id}` は実際のデバイス ID に置換して利用してください。

| トピック | 方向<br>(Pub/Sub) | QoS | Retain | Payloadスキーマ | 説明 |
|----------|:---------------:|:---:|:------:|------------------------------|------|
| `drone-charger/{device_id}/charge/status` | Pub | 1 | No | `ChargeStatusPayload` | 充電状態の定期通知 |
| `drone-charger/{device_id}/charge/start/request` | Sub | 1 | No | `ChargeStartRequestPayload` | 充電開始要求 |
| `drone-charger/{device_id}/charge/start/response` | Pub | 1 | No | `ChargeStartResponsePayload` | 充電開始応答 |
| `drone-charger/{device_id}/charge/stop/request` | Sub | 1 | No | `ChargeStopRequestPayload` | 充電停止要求 |
| `drone-charger/{device_id}/charge/stop/response` | Pub | 1 | No | `ChargeStopResponsePayload` | 充電停止応答 |
| `drone-charger/{device_id}/power/on/request` | Sub | 1 | No | `PowerOnRequestPayload` | 電源ON要求 |
| `drone-charger/{device_id}/power/on/response` | Pub | 1 | No | `PowerOnResponsePayload` | 電源ON応答 |

---

## 5. ペイロード仕様

### 5.1. 共通ヘッダー (要求/応答系)

| フィールド | 型 | 必須 | 説明 |
|------------|----|:----:|------|
| `timestamp` | string (ISO 8601) | Yes | 送信日時 (UTC) |
| `req_id` | string (UUIDv4) | Yes | 要求一意識別子 |
| `status` | string | Yes | 処理結果 (`SUCCESS`, `FAILURE` など) |
| `error` | string | No | エラー詳細 (成功時は空文字) |

### 5.2. `ChargeStatusPayload`

```json
{
  "timestamp": "2025-05-02T11:00:00Z",
  "charge": true,
  "current": 0,
  "chargingTime": 0,
  "isStartChargeExecuting": true,
  "isStopChargeExecuting": true,
  "isPowerOnExecuting": true
}
```

| フィールド | 型 | 必須 | 説明 |
|------------|----|:----:|------|
| `charge` | boolean | Yes | 充電中かどうか (`true` = 充電中) |
| `current` | number | Yes | 充電電流 (mA) |
| `chargingTime` | number | Yes | 充電経過時間 (秒) |
| `isStartChargeExecuting` | boolean | Yes | 充電開始処理が実行中か |
| `isStopChargeExecuting` | boolean | Yes | 充電停止処理が実行中か |
| `isPowerOnExecuting` | boolean | Yes | 電源ON処理が実行中か |

### 5.3. `ChargeStartRequestPayload`

```json
{
  "timestamp": "2025-05-02T11:00:00Z",
  "req_id": "550e8400-e29b-41d4-a716-446655440000"
}
```

### 5.4. `ChargeStartResponsePayload`

```json
{
  "timestamp": "2025-05-02T11:00:00Z",
  "req_id": "550e8400-e29b-41d4-a716-446655440000",
  "status": "SUCCESS",
  "error": ""
}
```

### 5.5. `ChargeStopRequestPayload`

```json
{
  "timestamp": "2025-05-02T11:05:00Z",
  "req_id": "3fa85f64-5717-4562-b3fc-2c963f66afa6"
}
```

### 5.6. `ChargeStopResponsePayload`

```json
{
  "timestamp": "2025-05-02T11:05:01Z",
  "req_id": "3fa85f64-5717-4562-b3fc-2c963f66afa6",
  "status": "SUCCESS",
  "error": ""
}
```

### 5.7. `PowerOnRequestPayload`

```json
{
  "timestamp": "2025-05-02T11:10:00Z",
  "req_id": "1c16f6e0-6737-4d62-8c06-9f9fbc2384d8"
}
```

### 5.8. `PowerOnResponsePayload`

```json
{
  "timestamp": "2025-05-02T11:10:02Z",
  "req_id": "1c16f6e0-6737-4d62-8c06-9f9fbc2384d8",
  "status": "SUCCESS",
  "error": ""
}
```
