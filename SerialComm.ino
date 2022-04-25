void help() {
	Serial.println(F("\n DC motor controller and stepper interface emulator"));
	Serial.println(F("Available serial commands: (lines end with CRLF or LF)"));
	Serial.println(F("? prints out current position and setpoint values"));
	Serial.println(F("X123 sets the target destination for the motor to 123 encoder pulses"));
	Serial.println(F("H prints this help message again"));
	Serial.println(F("B closes the connection\n"));
}

void printPos() {
	Serial.print(F("Position="));
	Serial.print(position);
	Serial.print(F(" Target="));
	Serial.print(setpoint);
	Serial.print(F(" EN="));
	Serial.println(GPIP(EN));
}


void process_line() {
	char cmd = Serial.read();
	if (cmd > 'Z') cmd -= 32;
	switch (cmd) {
	case '?': printPos(); break;
	case 'X': target1 = Serial.parseInt(); break;
	case 'H': help(); break;
	}
	//while (Serial.read() != 10); // dump extra characters till LF is seen (you can use CRLF or just LF)
}


