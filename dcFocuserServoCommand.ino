/*
 * Miguel Sanchez 2106
   Mauro Manco 2016 Porting on ESP8266
   Claudio Arena 2022 Adapted for open loop focuser
*/

#include "constantsAndPins.h"
#include <ESP8266WiFi.h>
//TODO: re-enable EN pin read/use?

void ICACHE_RAM_ATTR countStep();

void setPins() {
	pinMode(BUTTON_IN, INPUT_PULLUP);
	pinMode(BUTTON_OUT, INPUT_PULLUP);

	pinMode(Step, INPUT);
	pinMode(DIR, INPUT);

	pinMode(M1, OUTPUT);
	digitalWrite(M1, HIGH);
	pinMode(M2, OUTPUT);
	digitalWrite(M2, HIGH);

	pinMode(EN, OUTPUT);
	digitalWrite(EN, HIGH);

	analogWriteFreq(25000);  // set PWM to 25Khz
	analogWriteRange(255);   // set PWM to 255 levels

	attachInterrupt(Step, countStep, RISING);
	previousSpeedState = analogRead(SPEED_SELECT_SWITCH) > 512 ? true : false;
}

void setup() {
	//Disable software watchdog
	ESP.wdtDisable();

	WiFi.begin();
	WiFi.mode(WIFI_AP);
	WiFi.softAPdisconnect(true);

#ifdef DEBUG
	Serial.begin(115200);
	help();
#endif
	setPins();
}

void loop() {
	loop1();
}

void loop1() {

	while (true) {
		curTime = millis(); //Offset time to make sure this works from the start
		ESP.wdtFeed(); //Feed the hardware watchdog

#ifdef DEBUG
		if (Serial.available()) process_line(); // it may induce a glitch to move motion, so use it sparingly
#endif

		//Process manual moves (button pressed)
		manualMove();
		//If motor enable, process move commands (STEP, DIR from main board)
		///if (GPIP(EN) == HIGH) {
		if (true && manuallyMoving == false) {

			setpoint = target1;
			if (curTime - previousCommandMove >= COMMAND_TIMING) {
					commandedMove();
			}
		}
		else { // so (manuallyMoving || GPIP(EN) == LOW)
			setpoint = position;
			target1 = position;
		}

	}
}

void manualMove() {
	bool buttonInState = digitalRead(BUTTON_IN);
	bool buttonOutState = digitalRead(BUTTON_OUT);
	bool speedSelect = analogRead(SPEED_SELECT_SWITCH) > 512 ? true : false;

	if (buttonInState == previousButtonInState && buttonOutState == previousButtonOutState && speedSelect == previousSpeedState) {
		return;
	}

	//If we're here, a button press happened
	previousButtonInState = buttonInState;
	previousButtonOutState = buttonOutState;
	previousSpeedState = speedSelect;
	manuallyMoving = true;
	delay(2); //debounce

	//false means low, means button is pressed. If both pressed, switch off motor
	//if both pressed or none pressed
	if ((buttonInState == false && buttonOutState == false) ||
		(buttonInState == true && buttonOutState == true)) {
		pwmOut(0);
		manuallyMoving = false;
	}
	else if (buttonInState == false) {
		if (speedSelect) {
			pwmOut(LOW_SPEED);
		}
		else {
			pwmOut(HIGH_SPEED);
		}
	}
	else if (buttonOutState == false) {
		if (speedSelect) {
			pwmOut(-LOW_SPEED);
		}
		else {
			pwmOut(-HIGH_SPEED);
		}
	}
}

void commandedMove() {
	if (setpoint != position && receivingMoveCommand == false) {
		receivingMoveCommand = true;
		timeCommandReceived = curTime;
	}

	if (receivingMoveCommand && (curTime - timeCommandReceived > COMMAND_WAIT)) {
		if (setpoint == position) {
			pwmOut(0);
			receivingMoveCommand = false;
		}
		if (setpoint > position) {
			pwmOut(COMMAND_SPEED);
			position++;
		}
		else if (setpoint < position) {
			pwmOut(-COMMAND_SPEED);
			position--;
		}

	}

	previousCommandMove = curTime;
}