<!-- omit in toc -->
# ドローン制御システム自由研究

## 1. 自動充電

### 1.1. Fusion360で平歯車の作成

ツール⇒アドイン⇒スクリプトとアドイン⇒Spur Gear（下側）
https://www.fact-cam.co.jp/product/2019/10/fusion360/archives/000447.html

## 2. GPS

### 2.1. telloにtileを付けた事例

https://www.thingiverse.com/thing:2943670
![tello_tile](img/tello_tile.png)

## 3. センサ追加

| 品名                                                     | 価格 | URL                                          |
| -------------------------------------------------------- | ---- | -------------------------------------------- |
| TVOC/eCO2 ガスセンサユニット（SGP30）                    | 1606 | https://www.switch-science.com/products/6619 |
| M5Stack用 温湿度CO2センサ（SCD41）                       | 7117 | https://www.switch-science.com/products/8668 |
| ATOM Motionキット モーター/サーボドライバ（STM32F0搭載） | 3553 | https://www.switch-science.com/products/6993 |
| ATOMS3 Lite                                              | 1386 | https://www.switch-science.com/products/8778 |

## 4. PlatformIOを使ってみる

### 4.1. PlatformIOのインストール

以下のURLを参考にPlatformIOをインストールします。\
<https://takabus.com/tips/2402/>

1. VSCodeをインストールする
1. PlatformIOの拡張機能をインストールする
1. プロジェクトを新規作成する
1. 今回はATOM Liteを使用するため、Bord: M5Stack-ATOM を選択する

### 4.2. ライブラリのインストール

PlatformIOのLibrariesから以下をインストールする

1. M5Atom
1. FastLED

## 5. サンプルプログラムの実行

以下のLチカプログラムを実行する

~~~cpp
#include <M5Atom.h> //Atomのヘッダファイルを準備
//FastLED（CRGB構造体）設定
CRGB dispColor(uint8_t r, uint8_t g, uint8_t b) {
  return (CRGB)((r << 16) | (g << 8) | b);
}
// 初期設定 -----------------------------------------------
void setup() {
  // 本体初期化（UART有効, I2C無効, LED有効
  M5.begin(true, false, true);
  // LED全消灯（赤, 緑, 青)
  M5.dis.drawpix(0, dispColor(0, 0, 0));
}
// メイン -------------------------------------------------
void loop() {
  M5.update(); //本体のボタン状態更新
  if (M5.Btn.isPressed()) {  //ボタンが押されていれば
    M5.dis.drawpix(0, dispColor(0, 0, 255)); //LED（指定色）
  } else { //ボタンが押されてなければ
    M5.dis.drawpix(0, dispColor(20, 20, 20)); //LED（白）
  }
  delay(100); //100ms待機
}
~~~

## 6. 自作ライブラリの取り込み

~~~bash
cd src/TelloCharger/lib/
git submodule add https://github.com/mi83ki/ArduinoCommon.git
~~~

## 7. WebAPIサーバーにする

参考にしたURL
<https://qiita.com/poruruba/items/0e539875ecfbdbfb23db>

### 7.1. ライブラリのインストール

以下のライブラリインストールする

- bblanchon/ArduinoJson@^6.21.2
- ottowinter/ESPAsyncWebServer-esphome@^3.0.0

### 7.2. サンプルプログラムの実行

~~~cpp
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <M5Atom.h>


const char *wifi_ssid = "Your SSID";
const char *wifi_password = "Your Password";

#define HTTP_PORT 80
#define FORMAT_SPIFFS_IF_FAILED true
#define BUFFER_SIZE 30 * 1024
unsigned char buffer[BUFFER_SIZE];

AsyncWebServer server(HTTP_PORT);

void wifi_connect(const char *ssid, const char *password);

void notFound(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404);
  }
}

void setup() {
  Serial.begin(115200);

  wifi_connect(wifi_ssid, wifi_password);

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebParameter *p = request->getParam("download");
    if (p != NULL)
      Serial.println(p->value());

    request->send(200, "text/plain", "Hello, world");
  });

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
      "/post", [](AsyncWebServerRequest *request, JsonVariant &json) {
        JsonObject jsonObj = json.as<JsonObject>();
        int p1 = jsonObj["p1"] | -1;

        const char *p_param = jsonObj["param"];
        if (p_param != NULL) {
          Serial.println(p_param);
        }

        //    request->send(200, "application/json", "{}");
        AsyncJsonResponse *response = new AsyncJsonResponse();
        JsonObject root = response->getRoot();
        root["p1"] = p1;
        root["message"] = "Hello World";
        response->setLength();
        request->send(response);
      });
  server.addHandler(handler);

  server.onNotFound(notFound);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void wifi_connect(const char *ssid, const char *password) {
  Serial.println("");
  Serial.print("WiFi Connenting");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("");
  Serial.print("Connected : ");
  Serial.println(WiFi.localIP());
}
~~~
