#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define MAX_ADC_VALUE 1029

const int thresholdDistance = 10;
const int thresholdLight = 600;

const char *ssid = "F41";      // Your WiFi SSID
const char *password = "123456798";  // Your WiFi password
const char *api_key = "2J52TPRIR1GYARS4";  // Your ThingSpeak API Key

WiFiClient client;  // Declare WiFiClient object

int channelNumber =2374374;  // Replace with your ThingSpeak channel number

void setup() {
  pinMode(A0, INPUT); // LDR input at A0 pin.
  pinMode(D5, OUTPUT); // Sets the trigPin as an Output
  pinMode(D6, INPUT);  // Sets the echoPin as an Input
  Serial.begin(9600);  // Starts the serial communication

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);
}

void loop() {
  loop1();
  delay(1000); // Add a small delay to make the output readable
}

void loop1() {
  // Clears the trigPin
  digitalWrite(D5, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(D5, HIGH);
  delayMicroseconds(10);
  digitalWrite(D5, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(D6, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  // Read LDR input.
  int LDRReading = analogRead(A0);

  // Invert the LDRReading to represent light intensity.
  int lightIntensity = MAX_ADC_VALUE - LDRReading;

  Serial.print("Light Intensity: ");
  Serial.println(lightIntensity);

  // Update ThingSpeak channels
  updateThingSpeak(distance, lightIntensity);
}

void updateThingSpeak(int distance, int lightIntensity) {
  // Create a ThingSpeak update object
  ThingSpeak.setField(1, distance);
  ThingSpeak.setField(2, lightIntensity);

  // Write the update to ThingSpeak
  int status = ThingSpeak.writeFields(channelNumber, api_key);

  if (status == TS_OK_SUCCESS) {
    Serial.println("Update sent to ThingSpeak");
  } else {
    Serial.println("Error sending update to ThingSpeak");
  }
}

