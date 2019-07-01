#include <Arduino.h>


#define captureSize  49 // 25
byte sensorValue[captureSize - 1];

const uint8_t shiftSensor = 16;

volatile uint8_t offset;
const uint8_t pinInput = 3;
const uint8_t ledPin = 13;
unsigned long pulseLength;
byte pulseValue;
byte temp = 0;
byte distance = 0;

const uint16_t initPulseLow = 800;
const uint8_t pulseMinLength = 190;
const uint16_t pulseMaxLength = 210;


// const sensor address's
const byte sensorAddress_A = B11110000;
const byte sensorAddress_B = B11010010;
const byte sensorAddress_C = B11100001;
const byte sensorAddress_D = B11000011;

static void pulseDistance(int i);

void setup() {
    pinMode(pinInput, INPUT);
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
}

void loop() {

//look for starter pulse
    pulseLength = pulseIn(pinInput, LOW);
    while (pulseLength < initPulseLow) {
        pulseLength = pulseIn(pinInput, LOW);
    }


    for (offset = 0; offset < captureSize; offset = offset + 1) {
        pulseLength = pulseIn(pinInput, HIGH);
        if (pulseLength > pulseMinLength && pulseLength < pulseMaxLength) {
            pulseValue = B0;
            digitalWrite(ledPin, LOW);
        } else {
            pulseValue = B1;
            digitalWrite(ledPin, HIGH);
        }
        sensorValue[offset] = pulseValue;
    }

    Serial.println(" ");

    for (offset = 0; offset < captureSize; offset = offset + 1) {
        Serial.print(sensorValue[offset], BIN);
    }
    Serial.println(" ");

    for (offset = 0; offset < captureSize; offset = offset + 1) {
        pulseValue = sensorValue[offset];

// write pulseValue to bit 0
        bitWrite(temp, 0, pulseValue);
        switch (temp) {
            default:
                Serial.print("Address des not exist: ");
                Serial.print(temp);
                break;

            case sensorAddress_A:
                Serial.print("Sensor A: ");
                Serial.print(temp, BIN);
                Serial.print(" ");
                pulseDistance(offset + 1);
                offset = offset + shiftSensor;
                break;
            case sensorAddress_B:
                Serial.print("Sensor B: ");
                Serial.print(temp, BIN);
                Serial.print(" ");
                pulseDistance(offset + 1);
                offset = offset + shiftSensor;
                break;
            case sensorAddress_C:
                Serial.print("Sensor C: ");
                Serial.print(temp, BIN);
                Serial.print(" ");
                pulseDistance(offset + 1);
                offset = offset + shiftSensor;
                break;
            case sensorAddress_D:
                Serial.print("Sensor D: ");
                Serial.print(temp, BIN);
                Serial.print(" ");
                pulseDistance(offset + 1);
                offset = offset + shiftSensor;
                break;
        }

// shift left
        temp = temp << 1;
    }

}

volatile uint8_t distanceOffset;

void pulseDistance(int i) {
//read next 8 bits
    distance = 0;
    for (distanceOffset = 0; distanceOffset < 8; distanceOffset = distanceOffset + 1) {
        pulseValue = sensorValue[i];
// shift left
        distance = distance << 1;
// write pulseValue to bit 0
        bitWrite(distance, 0, pulseValue);
        i = i + 1;
    }
    distance = ~distance;

    Serial.println(distance, BIN);

}