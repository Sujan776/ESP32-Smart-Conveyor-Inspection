#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define IR_PIN 26
#define TRIG_PIN 5
#define ECHO_PIN 19
#define PRODUCT_POT_PIN 34
#define OLED_SDA 21
#define OLED_SCL 22
#define GREEN_LED 25
#define YELLOW_LED 33
#define RED_LED 32
#define BUZZER_PIN 23
#define SERVO_PIN 18
#define BUTTON_PIN 27

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int servoFrequency = 50;
const int servoResolution = 16;

bool systemRunning = false;
bool lastButtonState = HIGH;
unsigned long lastButtonTime = 0;
const unsigned long debounceDelay = 250;

int totalProducts = 0;
int goodProducts = 0;
int rejectedProducts = 0;

bool previousObject = false;

void setServoAngle(int angle)
{
  int minDuty = 1638;
  int maxDuty = 8192;
  int duty = map(angle, 0, 180, minDuty, maxDuty);
  ledcWrite(SERVO_PIN, duty);
}

void allLEDOff()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

float getDistance()
{
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.0343 / 2.0;
}

void showDisplay(String status, int productValue, float distance, bool objectDetected)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  display.println("SMART CONVEYOR");
  display.println("----------------");
  display.print("STATUS : ");
  display.println(status);
  display.print("OBJECT : ");
  display.println(objectDetected ? "YES" : "NO");
  display.print("VALUE  : ");
  display.println(productValue);
  display.print("RANGE  : ");
  if (distance >= 999) display.println("---");
  else { display.print(distance, 0); display.println(" cm"); }
  display.print("G:");
  display.print(goodProducts);
  display.print(" R:");
  display.println(rejectedProducts);
  display.display();
}

void setup()
{
  Serial.begin(115200);

  pinMode(IR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PRODUCT_POT_PIN, INPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  ledcAttach(SERVO_PIN, servoFrequency, servoResolution);
  setServoAngle(0);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    Serial.println("OLED ERROR");
  else
    Serial.println("OLED OK");

  allLEDOff();
  digitalWrite(GREEN_LED, HIGH);
  noTone(BUZZER_PIN);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 8);
  display.println("SMART CONVEYOR");
  display.setCursor(0, 25);
  display.println("INSPECTION SYSTEM");
  display.setCursor(0, 45);
  display.println("PRESS BUTTON");
  display.display();

  Serial.println("================================");
  Serial.println("SMART CONVEYOR SYSTEM");
  Serial.println("SYSTEM READY");
  Serial.println("PRESS BUTTON TO START");
  Serial.println("================================");
  delay(2000);
}

void loop()
{
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == LOW && lastButtonState == HIGH &&
      millis() - lastButtonTime > debounceDelay)
  {
    systemRunning = !systemRunning;
    lastButtonTime = millis();

    if (systemRunning)
      Serial.println("SYSTEM: RUNNING");
    else
    {
      Serial.println("SYSTEM: STOPPED");
      setServoAngle(0);
      noTone(BUZZER_PIN);
    }
  }

  lastButtonState = buttonState;

  int irState = digitalRead(IR_PIN);
  int productADC = analogRead(PRODUCT_POT_PIN);
  int productValue = map(productADC, 0, 4095, 0, 100);
  float distance = getDistance();
  bool objectDetected = (irState == LOW);

  if (!systemRunning)
  {
    allLEDOff();
    digitalWrite(GREEN_LED, HIGH);
    noTone(BUZZER_PIN);
    setServoAngle(0);

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 5);
    display.println("SMART CONVEYOR");
    display.println("----------------");
    display.println("SYSTEM: STOPPED");
    display.print("TOTAL : "); display.println(totalProducts);
    display.print("GOOD  : "); display.println(goodProducts);
    display.print("REJECT: "); display.println(rejectedProducts);
    display.display();

    delay(300);
    return;
  }

  if (objectDetected && !previousObject)
  {
    totalProducts++;
    previousObject = true;

    Serial.println("--------------------------------");
    Serial.println("OBJECT DETECTED");
    Serial.print("PRODUCT VALUE: "); Serial.println(productValue);
    Serial.print("DISTANCE: "); Serial.print(distance); Serial.println(" cm");

    bool goodProduct = (productValue >= 30 && productValue <= 70);

    if (goodProduct)
    {
      goodProducts++;
      allLEDOff();
      digitalWrite(GREEN_LED, HIGH);
      noTone(BUZZER_PIN);
      setServoAngle(0);
      showDisplay("GOOD", productValue, distance, true);
      Serial.println("QUALITY: GOOD");
      Serial.println("ACTION: CONTINUE");
    }
    else
    {
      rejectedProducts++;
      allLEDOff();
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER_PIN, 2000, 300);
      setServoAngle(110);
      showDisplay("REJECT", productValue, distance, true);
      Serial.println("QUALITY: DEFECTIVE");
      Serial.println("ACTION: REJECT");
      delay(700);
      setServoAngle(0);
    }
  }

  if (!objectDetected)
  {
    previousObject = false;
    allLEDOff();
    digitalWrite(GREEN_LED, HIGH);
    noTone(BUZZER_PIN);
    setServoAngle(0);
    showDisplay("RUNNING", productValue, distance, false);
  }

  Serial.print("IR: "); Serial.print(irState);
  Serial.print(" | Product: "); Serial.print(productValue);
  Serial.print(" | Distance: "); Serial.print(distance);
  Serial.print(" cm | Total: "); Serial.print(totalProducts);
  Serial.print(" | Good: "); Serial.print(goodProducts);
  Serial.print(" | Reject: "); Serial.println(rejectedProducts);

  delay(250);
}
