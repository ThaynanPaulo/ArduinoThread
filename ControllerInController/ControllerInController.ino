#include <Scheduler.h>

typedef unsigned int ui;

int ledPin = 13;

// callback for myThread
void niceCallback(){
	Serial.print("COOL! I'm running on: ");
	Serial.println(millis());
}

// callback for hisThread
void boringCallback(){
	Serial.println("BORING...");
}

// callback for blinkLedThread
void blinkLed(){
	static bool ledStatus = false;
	ledStatus = !ledStatus;

	digitalWrite(ledPin, ledStatus);

	Serial.print("blinking: ");
	Serial.println(ledStatus);
}

void setup() {
	Serial.begin(9600);

	pinMode(ledPin, OUTPUT);

	// Configure myThread
        ui niceID = Scheduler.createThread(niceCallback, 500);

        ui boringID = Scheduler.createThread(boringCallback, 250);

        ui blinkID = Scheduler.createThread(blinkLed, 100);
}

void loop() {
 
  	// run ThreadController
	// this will check every thread inside ThreadController,
	// if it should run. If yes, he will run it;
        Scheduler.run();

	// Rest of code
	float h = 3.1415;
	h/=2;
}

