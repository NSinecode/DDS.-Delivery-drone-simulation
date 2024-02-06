#pragma once
#include "raylib.h"
#include <string>
#include <chrono>
#include <thread>
//#include <iostream>

#include <zmq.hpp>

std::string to_string(Vector3 other);
std::string to_string(Vector2 other);

Vector2 Vec3toVec2(Vector3 other);

std::string SendRecieveData(std::string data);