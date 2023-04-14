#include <Arduino.h>
#include <M5Atom.h>
#include <ServoESP32.h>
#include <Timer.h>

static const int16_t DRONE_RELEASE_ANGLE = -31;
static const int16_t DRONE_CATCH_ANGLE = 8;
static const int16_t USB_OFF_ANGLE = -56;
static const int16_t USB_ON_ANGLE = 50;
static const float SERVO1_ANG_VEL = 45;
static const float SERVO2_ANG_VEL = 150;
static int16_t mainServoAngle = DRONE_RELEASE_ANGLE;
static int16_t subServoAngle = USB_OFF_ANGLE;
static uint8_t chargeStep = 0;

// サーボの指示角度[deg]（-90deg～90deg）
float angle = 0.0f;
// サーボ指示角度の増減値
float delta = 90.0f;
// サーボの回転角速度[deg/s]
float speed = 90.0f;

// 1つめのRCサーボをledcチャンネル0でIO26に設定
ServoESP32 servo1 = ServoESP32(0, 19, DRONE_RELEASE_ANGLE, SERVO1_ANG_VEL);
// 2つめのRCサーボをledcチャンネル1でIO27に設定
ServoESP32 servo2 = ServoESP32(2, 23, USB_OFF_ANGLE, SERVO2_ANG_VEL);
// 繰り返し動作用のタイマーを作成
Timer timer = Timer(3000);

void GetStatus() {
  Serial.printf("Servo Angle (1, 2) = (%f, %f)\n", servo1.getTargetAngle(),
                servo2.getTargetAngle());
}

// MOSFET
#define CHARGE_CONTROL_PIN G22
Timer chargeTimer = Timer(3000);
bool enableCharge = false;

void chargeOn(void) {
  digitalWrite(CHARGE_CONTROL_PIN, 1);
  enableCharge = true;
}

void chargeOff(void) {
  digitalWrite(CHARGE_CONTROL_PIN, 0);
  enableCharge = false;
}

bool isEnableCharge(void) {
  return enableCharge;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start example of ServoESP32");

  pinMode(CHARGE_CONTROL_PIN, OUTPUT);
  chargeOff();

  delay(3000);
  chargeOn();
  Serial.println("chargeOn");
}

void loop() {
  M5.update();
  if (M5.Btn.wasPressed()) {
    chargeStep = (chargeStep + 1) % 4;
    Serial.println("BUTTON!!! " + String(chargeStep));
    switch (chargeStep) {
    case 0:
      // ドローンをリリースする
      mainServoAngle = DRONE_RELEASE_ANGLE;
      subServoAngle = USB_OFF_ANGLE;
      break;
    case 1:
      // ドローンをキャッチする
      mainServoAngle = DRONE_CATCH_ANGLE;
      subServoAngle = USB_OFF_ANGLE;
      break;
    case 2:
      // 充電開始する
      subServoAngle = USB_ON_ANGLE;
      break;
    case 3:
      // 充電終了する
      subServoAngle = USB_OFF_ANGLE;
      break;
    default:
      break;
    }
    servo1.setTargetAngle(mainServoAngle);
    servo2.setTargetAngle(subServoAngle);
    GetStatus();
  }
  if (Serial.available()) { // シリアルデータが利用可能であれば
    char input = Serial.read(); // シリアルデータを読み込む
    if (input == 'w') {         // もし 'w' キーが押されたら
      subServoAngle++;          // 変数Aをインクリメントする
      servo2.setTargetAngle(subServoAngle);
      GetStatus();
    } else if (input == 's') { // もし 's' キーが押されたら
      subServoAngle--;         // 変数Aをデクリメントする
      servo2.setTargetAngle(subServoAngle);
      GetStatus();
    } else if (input == 'a') { // もし 'a' キーが押されたら
      mainServoAngle--;        // 変数Bをデクリメントする
      servo1.setTargetAngle(mainServoAngle);
      GetStatus();
    } else if (input == 'd') { // もし 'd' キーが押されたら
      mainServoAngle++;        // 変数Bをインクリメントする
      servo1.setTargetAngle(mainServoAngle);
      GetStatus();
    }
  }
  servo1.loop();
  servo2.loop();
}
