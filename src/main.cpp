#include <Arduino.h>

#include "ESP8266WiFi.h"
#include "ESPasyncUDP.h"

#include "Direction.h"
#include "Motor.h"

#define UDPPort 1173

#define MIN_PULSE_WIDTH 0
#define MAX_PULSE_WIDTH 1023

#define LEFT_MOTOR_PIN_A D10
#define LEFT_MOTOR_PIN_B D2
#define RIGHT_MOTOR_PIN_A D6
#define RIGHT_MOTOR_PIN_B D7

#define SSID ""
#define PASS ""

AsyncUDP udp;

Motor leftMotor(LEFT_MOTOR_PIN_A, LEFT_MOTOR_PIN_B);
Motor rightMotor(RIGHT_MOTOR_PIN_A, RIGHT_MOTOR_PIN_B);

void handlePacket(AsyncUDPPacket packet);
void dispatch(int receivedValues[]);

void setup()
{
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID, PASS);
	if (WiFi.waitForConnectResult() != WL_CONNECTED) {
		Serial.println("WiFi Failed");
		while (1) {
			delay(1000);
		}
	}
	if (udp.listen(UDPPort)) {
		Serial.print("UDP Listening on IP: ");
		Serial.println(WiFi.localIP());
		udp.onPacket([](AsyncUDPPacket packet) {
			handlePacket(packet);
		});
	}
}

void loop()
{
	//main loop
}


void handlePacket(AsyncUDPPacket packet)
{
	int receivedValues[4];

	receivedValues[0] = String( (char*) packet.data()).substring(0, 4).toInt();
	receivedValues[1] = String( (char*) packet.data()).substring(4, 8).toInt();
	receivedValues[2] = String( (char*) packet.data()).substring(8, 12).toInt();
	receivedValues[3] = String( (char*) packet.data()).substring(12, 16).toInt();

	dispatch(receivedValues);
}

void dispatch(int receivedValues[]) 
{
	if (receivedValues[1] < 1500) {
		leftMotor.setDirection(Direction::FORWARDS);
		leftMotor.setPWMLevel(map(receivedValues[1], 1500, 1000, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH));
	}
	else if (receivedValues[1] == 1500) {
		leftMotor.setDirection(Direction::NONE);
		leftMotor.setPWMLevel(0);
	}
	else if (receivedValues[1] > 1500) {
		leftMotor.setDirection(Direction::BACKWARDS);
		leftMotor.setPWMLevel(map(receivedValues[1], 1500, 2000, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH));
	}

	if (receivedValues[3] < 1500) {
		rightMotor.setDirection(Direction::FORWARDS);
		rightMotor.setPWMLevel(map(receivedValues[3], 1500, 1000, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH));
	}
	else if (receivedValues[3] == 1500) {
		rightMotor.setDirection(Direction::NONE);
		rightMotor.setPWMLevel(0);
	}
	else if (receivedValues[3] > 1500) {
		rightMotor.setDirection(Direction::BACKWARDS);
		rightMotor.setPWMLevel(map(receivedValues[3], 1500, 2000, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH));
	}
}