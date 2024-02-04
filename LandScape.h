#pragma once
#include <vector>

#include "raylib.h"
#include "Drone.h"

#define FIELD_SIZE				100
#define BUILDINGS_COUNT			70
#define MIN_BUILDING_SIZE		7
#define MAX_BUILDING_SIZE		13
#define MIN_BUILDING_HEIGHT		5
#define MAX_BUILDING_HEIGHT		20
#define MAX_AMOUNT_OF_TARGETS	3
#define TARGET_SIZE				4
#define SPAWN_SIZE				4

Mesh LoadMeshFromCube(BoundingBox other);

Vector3 BoxCenter(BoundingBox other);



class LandScape
{
public:
	LandScape();
	~LandScape();

	//Generate Buildings
	void AutoGenBuildings(int amount);
	void AddBuildings(std::vector<BoundingBox> BuildingsV);
	void ReAutoGenBuildings(int amount);
	void ChangeTarget();

	//Draw it
	void Draw();

	//Collision
	void CheckCollision(Drone &other);
	std::vector<BoundingBox> getDeathBlock();


private:
	std::vector<BoundingBox> DeathBlock, Target;
	std::vector<Model> DeathModel;

	BoundingBox StartPoint;

	Texture2D Ground, Buildings, Roof;

	int CurrentTarget = 0;
};
