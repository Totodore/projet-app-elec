#include "BaseWorker.cpp"

class MicWorker : public BaseWorker
{
public:
	void init()
	{
		Serial.println("Starting microphone...");
		pinMode(PIN_MICROPHONE, INPUT);
		Serial.println("Microphone started");
	}
	void loop()
	{
		Serial.println(analogRead(PIN_MICROPHONE));
	}
private:
	const int pin = 25:
};