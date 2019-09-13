#include "Motor.h"
#include "ColorSensor.h"
#include "util.h"

using namespace ev3api;

class Tracer {
public:
	Tracer();
	void run(int target, int mode);
	void init(char course);
	void terminate();
    char getSelect();
    void setSelect(char select);

private:
	ColorSensor colorSensor;
	const int8_t mThreshold = 20;
	int diff[2] = {};
	char select;
	char select2;
	
	float strI = 0.7;
	float curI = 0.2;

	//Lコース
	float deltaP[15] = {0.4, 0.8, 0.4, 0.7, 0.9, 0.9, 0.7, 0.4, 0.8, 0.4, 0.9, 0.4, 0.6, 0.8, 0.4};
	float deltaI[15] = {strI, curI, strI, curI, curI, curI, curI, strI, curI, strI, curI, strI, strI, curI, curI};
	float deltaD[15] = {0, 0.01, 0, 0.01, 0.03, 0.02, 0.01, 0, 0.01, 0, 0.02, 0, 0, 0, 0};

	//Rコース
	float RdeltaP[15] = {0.4, 0.7, 0.4, 0.7, 0.9, 0, 0.9, 0.2, 0.4, 0.7, 0.9, 0.4, 0.6, 0.8, 0.4};
	float RdeltaI[15] = {strI, curI, strI, curI, curI, 0, curI, 0.2 , 0.2, curI, curI, strI, strI, curI, curI};
	float RdeltaD[15] = {0, 0.01, 0, 0.01, 0.03, 0, 0.02, 0, 0, 0.01, 0.02, 0, 0, 0, 0};

	
	float speed[15] = {25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 10, 10 };
	
	Motor leftWheel;
	Motor rightWheel;
};
