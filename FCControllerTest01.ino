// ファミコンコントローラテストプログラム その３
// arduino(atmega328  5V  16MHz)
//
// コネクタ端子
// +--------------------------------+
// | (8) (7) (6) (5) (4) (3) (2) (1)|
//  |                              |
//   |(15)(14)(13)(12)(11)(10)( 9)|
//   +----------------------------+
//
// ( 1)GND    茶 : ground
// ( 7)P2_DAT 赤 : 2P data(low=on) ※使わない
// ( 9)P2_CLK 橙 : 2P clock        ※使わない
// (12)P/S    黄 : latch (low=latch / high=load)
// (13)P1_DAT 緑 : 1P data(low=on)
// (14)P1_CLK 青 : 1P clock
// (15)VCC    紫 : 5V

//*** コネクタのピン
#define LATCH_PIN   A0
#define P1CLK_PIN   A1
#define P1DAT_PIN   A2
//*** コネクタのビット
#define LATCH_BIT   0b00000001
#define P1CLK_BIT   0b00000010
#define P1DAT_BIT   0b00000100

/* 初期化処理 */
void setup() {
  //*** コネクタのピンを初期化
  // ファミコン <- PAD
  pinMode(P1DAT_PIN, OUTPUT);
  // ファミコン -> PAD
  pinMode(LATCH_PIN, INPUT_PULLUP);
  pinMode(P1CLK_PIN, INPUT_PULLUP);

  //*** ポートを初期化
  // ボタンが離れている(HIGH)
  PORTC = P1DAT_BIT;
}

/* メインループ処理 */
void loop() {
  //*** 変数定義
  unsigned int button;  // 各ビットがボタンの状態(A B SELECT START UP DOWN LEFT RIGHT)
  unsigned int mask;
  unsigned char bitcnt;

  while(1) {
    button = 0b10000000;              // Aボタンのみ押された状態
    mask = 0b10000000;

    // P/SがHIGHになるまで待つ
    while((PINC & LATCH_BIT) != 0);   // while(digitalRead(LATCH_PIN) == HIGH);
    // P/SがLOWになるまで待つ
    while((PINC & LATCH_BIT) == 0);   // while(digitalRead(LATCH_PIN) == LOW);

    // A 〜 RIGHTまでのボタン分繰り返す
    for(bitcnt = 0; bitcnt < 8; bitcnt++) {
      // CLKがLOWになるまで待つ
      while((PINC & P1CLK_BIT) == 0); // while(digitalRead(P1CLK_PIN) == LOW);

      //*** ボタンの状態によってDATAを変える
      if(button & mask) {
        // ボタンが押された(LOW)
        PORTC &= ~P1DAT_BIT;          // digitalWrite(P1DAT_PIN, LOW);
      } else {
        // ボタンが離れている(HIGH)
        PORTC |= P1DAT_BIT;           // digitalWrite(P1DAT_PIN, HIGH);
      }
      // マスクを右にシフト
      mask >>= 1;

      // CLKがHIGHになるまで待つ
      while((PINC & P1CLK_BIT) != 0); // while(digitalRead(P1CLK1_PIN) == HIGH);
    }
  }
}
