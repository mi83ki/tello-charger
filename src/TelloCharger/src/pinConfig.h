// build_flags で与えた名前
#ifdef M5STACK_M5ATOMS3
// Atom S3 系
/** 捕獲サーボのピン番号 */
#define SERVO_CATCH_PIN 6
/** USBサーボのピン番号 */
#define SERVO_USB_PIN 7
/** MOSFETのピン番号 */
#define CHARGE_CONTROL_PIN 5
#define INA_SDA_PIN 38
#define INA_SCL_PIN 39
#else
// Atom 系
/** 捕獲サーボのピン番号 */
#define SERVO_CATCH_PIN 19
/** USBサーボのピン番号 */
#define SERVO_USB_PIN 23
/** MOSFETのピン番号 */
#define CHARGE_CONTROL_PIN 22
#define INA_SDA_PIN 25
#define INA_SCL_PIN 21
#endif
