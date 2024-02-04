#pragma once
#include <vector>

#include "raylib.h"

#define FIELD_SIZE			100
#define BUILDINGS_COUNT		70
#define MIN_BUILDING_SIZE	7
#define MAX_BUILDING_SIZE	13
#define MIN_BUILDING_HEIGHT	5
#define MAX_BUILDING_HEIGHT	20

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

	//Draw it
	void Draw();

	//Collision
	std::vector<BoundingBox> getDeathBlock();


private:
	std::vector<BoundingBox> DeathBlock;
	std::vector<Model> DeathModel;

	BoundingBox StartPoint, Target;

	Texture2D Ground, Buildings, Roof;
};
