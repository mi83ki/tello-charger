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
#define MY_FIXED_IP_4 (110)

/** ゲートウェイのIPアドレス（"MY_FIXED_IP_1"."MY_FIXED_IP_2"."MY_FIXED_IP_3"."GATEWAY_IP_4"） */
#define GATEWAY_IP_4 (1)

/** MQTTのホスト */
#define MQTT_HOST "192.168.0.100"
/** MQTTのポート番号 */
#define MQTT_PORT 1883
/** MQTTのバッファサイズ(デフォルトは256バイト) */
// #define MQTT_BUFFER_SIZE 32768
#define MQTT_BUFFER_SIZE 256
