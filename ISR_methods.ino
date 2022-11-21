void ICACHE_RAM_ATTR countStep() {
	if (GPIP(DIR) == LOW) {// GPIP = digitalRead
		target1 = target1 - 1;
	}
	else {
		target1 = target1 + 1;
	}
}

void pwmOut(int out) {

#ifdef INVERT_DIR
	out = -out;
#endif

	if (out > 0) {
		analogWrite(M1, OUT_LIM_MAX);
		analogWrite(M2, OUT_LIM_MAX - out);
	}
	else {
		analogWrite(M2, OUT_LIM_MAX);
		analogWrite(M1, out - OUT_LIM_MIN);
	}
}
