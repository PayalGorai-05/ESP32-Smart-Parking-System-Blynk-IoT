#define BLYNK_TEMPLATE_ID "TMPL33TjDN84x"
#define BLYNK_TEMPLATE_NAME "IoT App"
#define BLYNK_AUTH_TOKEN "g7zWfM6ISqKo9havuQ7K_C21Piuo-MDD"

#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>

// LCD setup - if 0x27 doesn't work, try 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SENSOR1 32
#define SENSOR2 33

char auth[] = "g7zWfM6ISqKo9havuQ7K_C21Piuo-MDD";
char ssid[] = "GT";
char pass[] = "12345678";

BlynkTimer timer;

void readSensors() {
    int L1 = digitalRead(SENSOR1);
    int L2 = digitalRead(SENSOR2);

    // Sensor 1
    if (L1 == 1) {
        Serial.println("Sensor 1 - No Object");
        lcd.setCursor(0, 0);
        lcd.print("1:Empty     ");
        Blynk.virtualWrite(V0, 255); // Turn on LED widget
    } else {
        Serial.println("IR Sensor 1 detected - Object Present");
        lcd.setCursor(0, 0);
        lcd.print("1:Full      ");
        Blynk.virtualWrite(V0, 0); // Turn off LED widget
    }

    // Sensor 2
    if (L2 == 1) {
        Serial.println("Sensor 2 - No Object");
        lcd.setCursor(0, 1);
        lcd.print("2:Empty     ");
        Blynk.virtualWrite(V1, 255); // Turn on LED widget
    } else {
        Serial.println("IR Sensor 2 detected - Object Present");
        lcd.setCursor(0, 1);
        lcd.print("2:Full      ");
        Blynk.virtualWrite(V1, 0); // Turn off LED widget
    }
}

void setup() {
    Serial.begin(115200);
    
    // Initialize LCD with proper delays
    lcd.init();
    delay(100);
    lcd.backlight();
    delay(100);
    
    // Show welcome message
    lcd.setCursor(0, 0);
    lcd.print("  Welcome To  ");
    lcd.setCursor(0, 1);
    lcd.print(" Car Parking");
    delay(3000);
    
    // Clear and show connecting message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Connecting to");
    lcd.setCursor(0, 1);
    lcd.print("WiFi...");
    
    // Initialize Blynk with timeout
    WiFi.begin(ssid, pass);
    
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
        delay(500);
        Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi & Blynk");
        lcd.setCursor(0, 1);
        lcd.print("Connected!   ");
        Serial.println("\nWiFi and Blynk connected!");
    } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WiFi Failed  ");
        lcd.setCursor(0, 1);
        lcd.print("Check Credentials");
        Serial.println("\nWiFi connection failed!");
    }
    
    // Setup sensor pins
    pinMode(SENSOR1, INPUT_PULLUP);
    pinMode(SENSOR2, INPUT_PULLUP);
    
    // Setup timer to read sensors every 500ms
    timer.setInterval(500L, readSensors);
    
    delay(2000);
    lcd.clear();
}

void loop() {
    Blynk.run();
    timer.run();
}