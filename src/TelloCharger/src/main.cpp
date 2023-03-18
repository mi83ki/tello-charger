#include "AtomMotion.h"
#include <M5Atom.h> //Atomのヘッダファイルを準備

AtomMotion Atom;

static const uint8_t DRONE_RELEASE_ANGLE = 18;
static const uint8_t DRONE_CATCH_ANGLE = 94;
static const uint8_t USB_OFF_ANGLE = 41;
static const uint8_t USB_ON_ANGLE = 130;
static uint8_t mainServoAngle = 90;
static uint8_t subServoAngle = 90;
static uint8_t chargeStep = 0;

void GetStatus() {
  Serial.printf("Servo Angle (1, 2, 3, 4) = (%d, %d, %d, %d)\n",
                Atom.ReadServoAngle(1), Atom.ReadServoAngle(2),
                Atom.ReadServoAngle(3), Atom.ReadServoAngle(4));
}

void setup() {
  M5.begin(true, false, true);
  Atom.Init(); // sda  25     scl  21
  delay(1000);
  subServoAngle = USB_OFF_ANGLE;
  Atom.SetServoAngle(2, subServoAngle);
  delay(3000);
  mainServoAngle = DRONE_RELEASE_ANGLE;
  Atom.SetServoAngle(1, mainServoAngle);
  delay(3000);
}

void loop() {
  M5.update();
  if (M5.Btn.wasPressed()) {
    chargeStep = (chargeStep + 1) % 4;
    switch (chargeStep) {
    case 0:
      // ドローンをキャッチする
      mainServoAngle = DRONE_CATCH_ANGLE;
      subServoAngle = USB_OFF_ANGLE;
      break;
    case 1:
      // 充電開始する
      subServoAngle = USB_ON_ANGLE;
      break;
    case 2:
      // 充電終了する
      subServoAngle = USB_OFF_ANGLE;
      break;
    case 3:
      // ドローンをリリースする
      mainServoAngle = DRONE_RELEASE_ANGLE;
      subServoAngle = USB_OFF_ANGLE;
      break;
    default:
      break;
    }
    Atom.SetServoAngle(2, subServoAngle);
    Atom.SetServoAngle(1, mainServoAngle);
    GetStatus();
  }
  if (Serial.available()) { // シリアルデータが利用可能であれば
    char input = Serial.read(); // シリアルデータを読み込む
    if (input == 'w') {         // もし 'w' キーが押されたら
      subServoAngle++;          // 変数Aをインクリメントする
      Atom.SetServoAngle(2, subServoAngle);
      GetStatus();
    } else if (input == 's') { // もし 's' キーが押されたら
      subServoAngle--;         // 変数Aをデクリメントする
      Atom.SetServoAngle(2, subServoAngle);
      GetStatus();
    } else if (input == 'a') { // もし 'a' キーが押されたら
      mainServoAngle--;        // 変数Bをデクリメントする
      Atom.SetServoAngle(1, mainServoAngle);
      GetStatus();
    } else if (input == 'd') { // もし 'd' キーが押されたら
      mainServoAngle++;        // 変数Bをインクリメントする
      Atom.SetServoAngle(1, mainServoAngle);
      GetStatus();
    }
  }
}
