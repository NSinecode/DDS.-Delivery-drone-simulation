#pragma once
#include <cmath>
#include "raylib.h"
#include "raymath.h"

//Keys
#define FORVARD	KEY_KP_5
#define LEFT	KEY_KP_4
#define RIGHT	KEY_KP_6
#define UP		KEY_KP_8
#define DOWN	KEY_KP_2

//Movement constants
#define ACCELERATIONY	10		// up or down
#define ACCELERATIONGRV	9.81	// gravity
#define ACCELERATIONFVD	5		// forvard
#define STOPACC			3		// multiply by it while stopping
#define STOPMIN			0,01	// min stopping vel
#define ANGLE_VEL		1		// angle in rad

//Misc
#define LINELEN			20		// Misc
#define POWERTIME		200		// Time,  drone can fly without charge

struct PolarVector2
{
	Vector3 origin;	//Start pos or just {0,0,0} for radius vec
	float r;		//Distance to point
	float fi;		//X - angle
};

Vector3 Polar_to_Decard(PolarVector2 other);

class Drone
{
public:
	Drone();
	~Drone();
	
	//getters
	Vector3 getVel();
	Vector3 getAx();
	Vector3 getCylinderSize();
	Ray getForvard();
	PolarVector2 getRotation();
	float getPowerRemaining();
	double getPowerTimer();

	//setters
	void setVel(Vector3 Vel);
	void setAx(Vector3 Ax);
	void setCylinderSize(Vector3 CylinderSize);
	void setForvard(Ray forvard);
	void setPowerRemaining(float PowerRemaining);

	//Draw drone
	void Draw();

	//Drone movement
	void UpdatePos();
	void UpdateVel();

	//Drone death
	void Kill();

	//Drone power
	void Charge();


private:
	Vector3 Vel, Ax, CylinderSize;
	Ray forvard;
	PolarVector2 Rotation;

	float PowerRemaining;
	double PowerTimer;
};

