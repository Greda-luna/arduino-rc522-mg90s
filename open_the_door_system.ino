#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);  // 创建 MFRC522 实例
Servo servo;  // 创建舵机实例
bool cardRecognized = false;
void setup() 
{
  Serial.begin(9600);  // 初始化串口通信
  SPI.begin();  // 启动 SPI 总线
  rfid.PCD_Init();  // 初始化 MFRC522
  servo.attach(7);  // 将舵机连接到数字引脚 7
  servo.write(90);
}

void loop() 
{
  // 检测是否有卡片靠近
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) 
  {
    String cardID = "";
  
    // 将卡片 ID 转换为字符串
    for (byte i = 0; i < rfid.uid.size; i++) 
    {
      cardID.concat(String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""));
      cardID.concat(String(rfid.uid.uidByte[i], HEX));
    }
    Serial.println(cardID);
  
    // 如果检测到 ID 为 bb6ae7cd 的卡片
    if (cardID == "bb6ae7cd") 
    {
      servo.write(180);  // 舵机顺时针旋转至 90 度
      delay(1000);
      servo.write(90);  // 舵机回到初始位置
      cardRecognized = true;  // 设置标志位为已识别
    }
  
    
  }
  rfid.PICC_HaltA();          // 停止卡片通信
  rfid.PCD_StopCrypto1();     // 停止加密
  rfid.PCD_Init();            // 重新初始化RFID模块

}
/*连接方法：
MFRC522                Arduino uno             mg90s
SDA                      10                    红色——电源线——>5v
SCK                      13                    橙色——信号线——>7
MOSI                     11                    褐色——接地——>GND
MISO                     12
IRO                      blank
GND                      GND
RST                       9
3.3V                     3.3V
*/
