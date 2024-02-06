#include "LandScape.h"

LandScape::LandScape()
{
	Buildings = LoadTexture("Resources/buildings.png");
	Ground = LoadTexture("Resources/asphalt.png");
	Roof = LoadTexture("Resources/roof.png");

	//Adding constant startpoint
	StartPoint = { {-SPAWN_SIZE,0,-SPAWN_SIZE},{SPAWN_SIZE,1,SPAWN_SIZE} };
	//Undefined target
	Target.push_back({ {FIELD_SIZE,0,FIELD_SIZE},{FIELD_SIZE,0,FIELD_SIZE} });

	//Adding ground
	DeathBlock.push_back({ {-FIELD_SIZE,0,-FIELD_SIZE},{FIELD_SIZE,0,FIELD_SIZE} });
	DeathModel.push_back(LoadModelFromMesh(LoadMeshFromCube(DeathBlock[0])));
	DeathModel[0].materials[0].maps[0].texture = Ground;
}

LandScape::~LandScape()
{
	UnloadTexture(Buildings);
	UnloadTexture(Ground);
	UnloadTexture(Roof);

	for (int i = 0; i < DeathModel.size(); i++)	UnloadModel(DeathModel[i]);
}

void LandScape::AutoGenBuildings(int amount)
{
	//Generate target
	Target.clear();
	for (int i = 0; i < MAX_AMOUNT_OF_TARGETS; i++)
	{
		Vector3 minT = { GetRandomValue(-FIELD_SIZE, FIELD_SIZE - TARGET_SIZE),0,GetRandomValue(-FIELD_SIZE, FIELD_SIZE - TARGET_SIZE) };
		Target.push_back({ minT, { minT.x + TARGET_SIZE,1, minT.z + TARGET_SIZE } });
	}

	bool TargetCollisions;
	//Generate new houses
	for (int i = 0; i < amount; i++)
	{
		//Generate building
		Vector3 min, max;
		//Check, if building is too close to spawn or target
		do
		{
			min = { (float)GetRandomValue(-FIELD_SIZE, FIELD_SIZE - MAX_BUILDING_SIZE),0,(float)GetRandomValue(-FIELD_SIZE, FIELD_SIZE - MAX_BUILDING_SIZE) };
			max = { min.x + (float)GetRandomValue(MIN_BUILDING_SIZE * 10, MAX_BUILDING_SIZE * 10) / 10, (float)GetRandomValue(MIN_BUILDING_HEIGHT * 10, MAX_BUILDING_HEIGHT * 10) / 10, min.z + (float)GetRandomValue(MIN_BUILDING_SIZE * 10, MAX_BUILDING_SIZE * 10) / 10 };
			
			TargetCollisions = 0;
			for (int i = 0; i < MAX_AMOUNT_OF_TARGETS; i++)		TargetCollisions += CheckCollisionBoxes({ min,max }, Target[i]);
		} while (TargetCollisions or CheckCollisionBoxes({ min,max }, StartPoint));

		//Add building
		DeathBlock.push_back({ min,max });
		//Model Building
		DeathModel.push_back(LoadModelFromMesh(LoadMeshFromCube(DeathBlock[DeathBlock.size() - 1])));
		DeathModel[DeathModel.size() - 1].materials[0].maps[0].texture = Buildings;
		//Add roof
		max.y += 0.03;
		DeathBlock.push_back({ { min.x,max.y,min.z }, max });
		//Model roof
		DeathModel.push_back(LoadModelFromMesh(LoadMeshFromCube(DeathBlock[DeathBlock.size() - 1])));
		DeathModel[DeathModel.size() - 1].materials[0].maps[0].texture = Roof;
	}
}

void LandScape::AddBuildings(std::vector<BoundingBox> BuildingsV)
{
	for (int i = 0; i < BuildingsV.size(); i++)
	{
		//Add building
		DeathBlock.push_back(BuildingsV[i]);
		//Model Building
		DeathModel.push_back(LoadModelFromMesh(LoadMeshFromCube(BuildingsV[i])));
		DeathModel[DeathModel.size() - 1].materials[0].maps[0].texture = Buildings;
		//Add roof
		BuildingsV[i].max.y += 0.03;
		DeathBlock.push_back({ { BuildingsV[i].min.x,BuildingsV[i].max.y,BuildingsV[i].min.z }, BuildingsV[i].max });
		//Model roof
		DeathModel.push_back(LoadModelFromMesh(LoadMeshFromCube(BuildingsV[i])));
		DeathModel[DeathModel.size() - 1].materials[0].maps[0].texture = Roof;
	}
}

void LandScape::ReAutoGenBuildings(int amount)
{
	//Clearing array
	BoundingBox holder = DeathBlock[0];
	DeathBlock.clear();
	DeathBlock.push_back(holder);
	//Unloading models
	for (int i = 0; i < DeathModel.size(); i++)	UnloadModel(DeathModel[i]);
	DeathModel.clear();
	DeathModel.push_back(LoadModelFromMesh(LoadMeshFromCube(DeathBlock[0])));
	DeathModel[0].materials[0].maps[0].texture = Ground;

	//Generate new
	AutoGenBuildings(amount);
}

void LandScape::ChangeTarget()
{
	CurrentTarget++;
	if (CurrentTarget == MAX_AMOUNT_OF_TARGETS)		CurrentTarget = 0;
}

void LandScape::AutoGenTarget()
{
	Target.clear();
	for (int i = 0; i < MAX_AMOUNT_OF_TARGETS; i++)
	{
		Vector3 minT = { GetRandomValue(-FIELD_SIZE, FIELD_SIZE - TARGET_SIZE),0,GetRandomValue(-FIELD_SIZE, FIELD_SIZE - TARGET_SIZE) };
		Target.push_back({ minT, { minT.x + TARGET_SIZE,1, minT.z + TARGET_SIZE } });
	}
}

BoundingBox LandScape::getCurrentTarget()
{
	return Target[CurrentTarget];
}

void LandScape::Draw()
{
	for (int i = 0; i < DeathModel.size(); i++)		DrawModel(DeathModel[i], BoxCenter(DeathBlock[i]), 1, WHITE);

	DrawBoundingBox(StartPoint, DARKGREEN);
	DrawBoundingBox(Target[CurrentTarget], RED);
}

void LandScape::CheckCollision(Drone& other)
{
	//Collisions with buildings
	for (int i = 0; i < DeathBlock.size(); i++)	if (CheckCollisionBoxSphere(DeathBlock[i], other.getForvard().position, other.getRadius()))	other.Kill();
	//Collision with spawn
	if (CheckCollisionBoxSphere(StartPoint, other.getForvard().position, other.getRadius()))	other.OnSpawn(CurrentTarget, MAX_AMOUNT_OF_TARGETS);
	//Collision with target
	else if (CheckCollisionBoxSphere(Target[CurrentTarget], other.getForvard().position, other.getRadius()))	other.InTarget();
	else other.InAir();
}

std::vector<BoundingBox> LandScape::getDeathBlock()
{
	return DeathBlock;
}

Mesh LoadMeshFromCube(BoundingBox other)
{
	return GenMeshCube(other.max.x - other.min.x, other.max.y - other.min.y, other.max.z - other.min.z);
}

Vector3 BoxCenter(BoundingBox other)
{
	return { other.max.x - (other.max.x - other.min.x) / 2,other.min.y + (other.max.y - other.min.y) / 2,other.max.z - (other.max.z - other.min.z) / 2 };
}

std::string AiData2D(Drone other, LandScape city)
{
	std::string data;

	//Adding drone pos
	data += to_string(Vec3toVec2(other.getForvard().position));
	//Adding target pos
	//data += to_string(Vec3toVec2(BoxCenter(city.getCurrentTarget())));

	return data;
}
