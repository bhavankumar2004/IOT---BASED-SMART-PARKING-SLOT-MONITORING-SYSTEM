#include <WiFi.h>
#include <HTTPClient.h>
#define WIFI_SSID "Bhavankumar"
#define WIFI_PASSWORD "12345689"
int trigPin = 5;
int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;
bool slotstatus;

const char *serverUrl = "https://console.thingzmate.com/api/v1/device-types/bhavan-iot/devices/bhavan-iot/uplink"; // Replace with your server endpoint
String AuthorizationToken = "Bearer 12cdc3fbc1f0aa5ab911b836496071ea";
void setup() {
  Serial.begin(115200);
  delay(4000); 
   // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(22,OUTPUT);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
 if(distanceCm<30)
 {
  slotstatus=0;
  Serial.println("VACANT");
   digitalWrite(22,LOW);
 }
 else
 {
  slotstatus=1;
  Serial.println("OCCUPIED");
  digitalWrite(22,HIGH);
 }
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
http.addHeader("Authorization", AuthorizationToken); //Authorization token
  // Create JSON payload
  String payload = "{\"DistanceCm\":" + String(distanceCm) +",\"SLOTSTATUS\":" + String(slotstatus) + "}";
  
  // Send POST request
  int httpResponseCode = http.POST(payload);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println(response);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end(); // Free resources
  delay(1000); // Wait for 1 minute before sending next request
}
