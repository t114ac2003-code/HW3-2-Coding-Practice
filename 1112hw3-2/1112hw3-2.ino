// ===================== HW3-2 第五題：按鈕切換 LED 狀態 =====================
// 狀態 0：恆亮
// 狀態 1：慢閃爍（1000 ms）
// 狀態 2：不快不慢閃爍（400 ms）
// 狀態 3：快閃爍（100 ms）
// 按第 4 下回到恆亮

const int BUTTON_PIN = 2;   // 按鈕腳位
const int LED_PIN    = 3;   // LED 腳位（題目指定 D3）

const bool BUTTON_ACTIVE_HIGH = true;  // 按下讀到 HIGH 設 true；讀到 LOW 改成 false

// 三種閃爍頻率（毫秒）
const unsigned long SLOW_INTERVAL   = 1000;
const unsigned long MEDIUM_INTERVAL = 400;
const unsigned long FAST_INTERVAL   = 100;

int state = 0;              // 目前狀態（0~3）
const int STATE_COUNT = 4;

// 按鈕去彈跳
bool lastReading = false;
bool buttonState = false;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_TIME = 30;

// 閃爍用
unsigned long lastBlinkTime = 0;
bool ledOn = true;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, BUTTON_ACTIVE_HIGH ? INPUT : INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("State 0: Constant ON");
}

void loop() {
  checkButton();
  updateLED();
}

// ----- 讀取按鈕是否按下 -----
bool isPressed() {
  int raw = digitalRead(BUTTON_PIN);
  return BUTTON_ACTIVE_HIGH ? (raw == HIGH) : (raw == LOW);
}

// ----- 偵測「按下一次」並切換狀態 -----
void checkButton() {
  bool reading = isPressed();

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > DEBOUNCE_TIME && reading != buttonState) {
    buttonState = reading;
    if (buttonState) {                       // 只在「按下的瞬間」切換一次
      state = (state + 1) % STATE_COUNT;     // 第 4 下會回到 0（恆亮）
      ledOn = true;
      lastBlinkTime = millis();
      printState();
    }
  }

  lastReading = reading;
}

// ----- 依照狀態控制 LED -----
void updateLED() {
  unsigned long interval;

  switch (state) {
    case 0:                                  // 恆亮
      digitalWrite(LED_PIN, HIGH);
      return;
    case 1: interval = SLOW_INTERVAL;   break;  // 慢閃爍
    case 2: interval = MEDIUM_INTERVAL; break;  // 不快不慢
    case 3: interval = FAST_INTERVAL;   break;  // 快閃爍
  }

  if (millis() - lastBlinkTime >= interval) {
    lastBlinkTime = millis();
    ledOn = !ledOn;
  }
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
}

// ----- 序列埠顯示目前狀態（方便檢查）-----
void printState() {
  switch (state) {
    case 0: Serial.println("State 0: Constant ON"); break;
    case 1: Serial.println("State 1: Slow blink");  break;
    case 2: Serial.println("State 2: Medium blink"); break;
    case 3: Serial.println("State 3: Fast blink");  break;
  }
}