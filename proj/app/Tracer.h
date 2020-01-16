#include "Motor.h"
#include "ColorSensor.h"
#include "util.h"
#include "app.h"

using namespace ev3api;

  extern int dateCnt;
  
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
	float	integral;
	char lineBlock;

	//float strI = 0.7;
	//float curI = 0.2;
	
	float Pd = 0;
	float P1 = 0.4 + Pd;
	float P2 = 1.0 + Pd;
	float P3 = 1.2 + Pd;
	
	float I1 = 0.2;
	float I2 = 0.4;
	
	float Dd = 0;
	float D1 = 0.05 + Dd;
	float D2 = 0.16 + Dd;
	float D3 = 0.2 + Dd;

	float RdeltaP[15] = {P1, P2, P1, P2, P2, P3, P2, P1, P2, P2, P2, P1, P1, P2, P1};
	float RdeltaD[15] = {D1, D2, D1, D2, D2, D3, D2, D1, D2, D2, D2, D1, D1, D1, D1};
	float RdeltaI[15] = {I1, I2, I1, I2, I2, I2, I2, I1, I2, I1, I2, I1, I1, I2, I2};
	
	//L�R�[�X
	//float LdeltaP[15] = {0.4, 0.8, 0.4, 0.7, 0.9, 0.9, 0.7, 0.4, 0.8, 0.4, 0.9, 0.4, 0.6, 0.8, 0.4};
	//float LdeltaI[15] = {strI, curI, strI, curI, curI, curI, curI, strI, curI, strI, curI, strI, strI, curI, curI};
	//float LdeltaD[15] = {0, 0.01, 0, 0.01, 0.03, 0.02, 0.01, 0, 0.01, 0, 0.02, 0, 0, 0, 0};

	//R�R�[�X
	//float RdeltaP[15] = {0.4, 0.7, 0.4, 0.7, 0.9, 0, 0.9, 0.2, 0.4, 0.7, 0.9, 0.4, 0.6, 0.8, 0.4};
	//float RdeltaI[15] = {strI, curI, strI, curI, curI, 0, curI, 0.2 , 0.2, curI, curI, strI, strI, curI, curI};
	//float RdeltaD[15] = {0, 0.01, 0, 0.01, 0.03, 0, 0.02, 0, 0, 0.01, 0.02, 0, 0, 0, 0};

	float S1 = 45;
	float S2 = 25;
	
	float speed[15] = {S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S2, S2, S2};
	
	Motor leftWheel;
	Motor rightWheel;
};
