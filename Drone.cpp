#include "Drone.h"

Drone::Drone()
{
	//Loading quad model
	/*quadrocopter = LoadModel("Resources/quadcopter.obj");
	carbon = LoadTexture("Resources/carbon.png");
	quadrocopter.materials[0].maps[0].texture = carbon;*/

	Vel = Ax = { 0 };
	CylinderSize = { 1,1,1 };
	forvard = { { 0,2,0 }, { 0,0,90 } };
	Rotation = { { 0,1,0 },LINELEN,0 };
	PowerTimer = 0;

}

Drone::~Drone()
{
	//UnloadTexture(carbon);
	//UnloadModel(quadrocopter);
}

Vector3 Drone::getVel()
{
	return Vel;
}
Vector3 Drone::getAx()
{
	return Ax;
}
Vector3 Drone::getCylinderSize()
{
	return CylinderSize;
}
Ray Drone::getForvard()
{
	return forvard;
}
PolarVector2 Drone::getRotation()
{
	return Rotation;
}
double Drone::getPowerTimer()
{
	return PowerTimer;
}
float Drone::getRadius()
{
	return CylinderSize.x;
}
int Drone::getScore()
{
	return Score;
}
void Drone::setVel(Vector3 Vel)
{
	this->Vel = Vel;
}
void Drone::setAx(Vector3 Ax)
{
	this->Ax = Ax;
}
void Drone::setCylinderSize(Vector3 CylinderSize)
{
	this->CylinderSize = CylinderSize;
}
void Drone::setForvard(Ray forvard)
{
	this->forvard = forvard;
}

void Drone::ScoreUP()
{
	Score++;
}

void Drone::Draw()
{
#ifdef HITBOX_MODE
	DrawSphere(forvard.position, CylinderSize.x, { 0, 121, 241, 127 });
#endif // HITBOX_MODE
	//Model
	//DrawModel(quadrocopter, forvard.position, 0.01, WHITE);

	//DrawRay(forvard, DARKBLUE);
	DrawLine3D(forvard.position, Polar_to_Decard(Rotation), DARKBLUE);
}

void Drone::UpdatePos()
{
	//About Velocity
	UpdateVel();
	forvard.position = Vector3Add(forvard.position, Vector3Scale(Vel, GetFrameTime()));
	Rotation.origin = forvard.position;

	// Rotate update
	if (IsKeyPressed(LEFT) or IsKeyDown(LEFT)) 
	{ 
		//forvard.direction.z -= ANGLE_VEL * GetFrameTime();
		//forvard.direction.x += ANGLE_VEL * GetFrameTime();
		Rotation.fi += ANGLE_VEL * GetFrameTime();
	}
	else if (IsKeyPressed(RIGHT) or IsKeyDown(RIGHT)) 
	{
		//forvard.direction.z -= ANGLE_VEL * GetFrameTime();
		//forvard.direction.x -= ANGLE_VEL * GetFrameTime();
		Rotation.fi -= ANGLE_VEL * GetFrameTime();
	}

	//Power update
	//if (GetTime() - PowerTimer > POWERTIME) Kill();
}

void Drone::UpdateVel()
{
	// Y - movement
	if (IsKeyPressed(UP) or IsKeyDown(UP))	Ax.y = ACCELERATIONY;
	else if ((IsKeyPressed(DOWN) or IsKeyDown(DOWN)) && CanFlyDown)	Ax.y = -ACCELERATIONY - ACCELERATIONGRV;
	else if (IsKeyUp(UP) or IsKeyUp(DOWN))
	{
		Ax.y = -Vel.y * STOPACC;
		//Absolute stop
		if (abs(Vel.y) < 0.1)
		{
			Vel.y = 0;
			Ax.y = 0;
		}
	}

	// XZ - movement
	if (IsKeyPressed(FORVARD) or IsKeyDown(FORVARD))
	{
		Ax.x = ACCELERATIONFVD * sin(Rotation.fi);
		Ax.z = ACCELERATIONFVD * cos(Rotation.fi);
	}
	else if (IsKeyUp(FORVARD))
	{
		Ax.x = -Vel.x * STOPACC;
		Ax.z = -Vel.z * STOPACC;
		//Absolute stop
		if (abs(Vel.x) < 0.1)
		{
			Vel.x = 0;
			Ax.x = 0;
		}
		if (abs(Vel.z) < 0.1)
		{
			Vel.z = 0;
			Ax.z = 0;
		}
	}

	
	Vel = Vector3Add(Vel, Vector3Scale(Ax, GetFrameTime()));
	if (!CanFlyDown && Vel.y < 0)	Vel.y = 0;
}

void Drone::InTarget()
{
	WasInTarget = 1;
	CanFlyDown = 0;
}

void Drone::OnSpawn(int& currentTarget, int amount)
{
	//Charging
	Charge();
	//Check mission complete
	if (WasInTarget)
	{
		WasInTarget = 0;
		ScoreUP();
		currentTarget++;
		if (currentTarget > amount) currentTarget = 0;
	}
	
	CanFlyDown = 0;
}

void Drone::InAir()
{
	CanFlyDown = 1;
}

void Drone::Kill()
{
	forvard.position = { 0,2,0 };
	Vel = { 0,0,0 };
	Ax = { 0,0,0 };
	WasInTarget = 0;
	PowerTimer = GetTime();
	Score = 0;
}

void Drone::Charge()
{
	PowerTimer = GetTime();
}

Vector3 Polar_to_Decard(PolarVector2 other)
{
	return Vector3Add(other.origin, { other.r * sin(other.fi),0,other.r * cos(other.fi) });
}
