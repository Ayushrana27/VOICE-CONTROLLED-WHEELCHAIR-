/*
  wheelchair_control.ino
  Voice + Obstacle Smart Wheelchair
  Author: Ayush rana
  GitHub: https://github.com/Ayushrana27
*/

#include <Arduino.h>

// Motor pins
const int motor1Pin1 = 8;
const int motor1Pin2 = 9;
const int motor2Pin1 = 10;
const int motor2Pin2 = 11;

// Ultrasonic pins
const int trigPin = 6;
const int echoPin = 7;

// Command variable
char command = '\0';
unsigned long lastCommandTime = 0;
const unsigned long commandTimeout = 1500; // Stop if no command received in 1.5 sec

void setup() {
  Serial.begin(9600);
  
  // Motor pin setup
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  // Ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  stopMotors();
  Serial.println("[INFO] Smart Wheelchair Initialized");
}

void loop() {
  // Read command from Bluetooth
  if (Serial.available() > 0) {
    command = tolower(Serial.read());
    lastCommandTime = millis();
    Serial.print("[CMD] ");
    Serial.println(command);
  }

  // Stop if no command received recently
  if (millis() - lastCommandTime > commandTimeout) {
    stopMotors();
    command = '\0';
  }

  // Execute movement based on command
  switch (command) {
    case 'f': handleForward(); break;
    case 'b': handleBackward(); break;
    case 'l': handleLeft(); break;
    case 'r': handleRight(); break;
    case 's': stopMotors(); break;
  }
}

// Check for obstacles using ultrasonic sensor
bool obstacleDetected() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // cm

  return distance < 25; // Stop if object within 25cm
}

// Movement functions
void handleForward() {
  if (!obstacleDetected()) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    Serial.println("Moving Forward ⬆️");
  } else {
    stopMotors();
    Serial.println("Obstacle detected, stopped 🛑");
  }
}

void handleBackward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  Serial.println("Moving Backward ⬇️");
}

void handleLeft() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  Serial.println("Turning Left ⬅️");
}

void handleRight() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  Serial.println("Turning Right ➡️");
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
  Serial.println("Stopped ✋");
}
