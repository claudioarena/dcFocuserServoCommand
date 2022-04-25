/// DEBUG OPTIONS ///
#define DEBUG

/// PINS ///
#define BUTTON_IN			13		// D7
#define BUTTON_OUT			12		// D6
#define Step				14		// D5
#define M1					0		// D3
#define M2					2		// D4
#define DIR					4		// D2
#define EN					5		// D81
#define SPEED_SELECT_SWITCH	A0		//A0, connected to a switch to change speed when focusing manually

#define COMMAND_TIMING		10		//ms between each position increment/decrement when receiving a step command
#define COMMAND_WAIT		100		//ms to wait between move command received and start of moving

#define OUT_LIM_MAX			255
#define OUT_LIM_MIN			-255
#define HIGH_SPEED			255
#define LOW_SPEED			127
#define COMMAND_SPEED		255
/// VARIABLES ///

double setpoint = 0, position = 0;
volatile long target1 = 0;  // destination location at any moment

bool manuallyMoving = 0;
bool previousButtonInState = 1, previousButtonOutState = 1, previousSpeedState = 1;
bool receivingMoveCommand = 0;

unsigned long timeCommandReceived = 0UL;
unsigned long previousCommandMove = 0UL;
unsigned long curTime = 0UL;  // will store current time to avoid multiple millis() calls