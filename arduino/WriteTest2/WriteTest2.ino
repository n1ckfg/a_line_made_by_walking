#include <SD.h>

#define STABILIZE_DELAY 10 // ms
#define RED_LED_PIN 3
#define GREEN_LED_PIN 4
#define LEFT_X_PIN 0
#define LEFT_Y_PIN 1
#define LEFT_Z_PIN 2

int data_pin = 10;
int power_pin = 8;

File data_file;

void setup() {

  Serial.begin(9600);
  Serial.println("Initializing card...");

  // default chip select pin is set to output, needed
  pinMode(10, OUTPUT);
  
  // card will draw power from pin 8, so set it high
  pinMode(power_pin, OUTPUT);  
  digitalWrite(power_pin, HIGH);
  
  // initialize card
  if (!SD.begin(data_pin)) {
      Serial.println("Card failure");
      return;
  }
  Serial.println("--> ready");

  // find the next unused filename
  char filename[] = "LOG00.CSV";  
  for (uint8_t i = 0; i < 100; i++) {
    filename[3] = i/10 + '0';
    filename[4] = i%10 + '0';
    if (!SD.exists(filename)) {
      break;
    }
  }  
  Serial.print("Logging to: ");
  Serial.println(filename);

  data_file = SD.open(filename, FILE_WRITE);
  
}

void loop() {

  uint32_t t = millis();
  String output = String(t);
  output += ",";

  analogRead(LEFT_X_PIN); 
  delay(STABILIZE_DELAY);
  int left_x = analogRead(LEFT_X_PIN); 
  output += String(left_x);
  output += ",";

  analogRead(LEFT_Y_PIN); 
  delay(STABILIZE_DELAY);
  int left_y = analogRead(LEFT_Y_PIN); 
  output += String(left_y);
  output += ",";

  analogRead(LEFT_Z_PIN); 
  delay(STABILIZE_DELAY);
  int left_z = analogRead(LEFT_Z_PIN); 
  output += String(left_z);

  if (data_file) {
    data_file.println(output);
    data_file.flush();
    Serial.println(output);
  } else {
    Serial.println("Couldn't open log file");
  }

}
