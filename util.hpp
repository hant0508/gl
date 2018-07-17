#ifndef UTIL_HPP
#define UTIL_HPP

//#include <fstream>

const int SIZE = 5;
const int HEIGHT = 600, WIDTH = 600;

typedef bool Side;
const Side PLAYER = 0, AI = 1;

typedef int Action;
const Action IDLE = 0, MOVE = 1, ATTACK = 2;

typedef int UnitType;
const UnitType PEASANT = 1, ARCHER = 2;

typedef int Id;
const Id BATTLE_BG = 0;
const Id PEASANT_IDLE = 10;
const Id ARCHER_IDLE = 20;
const Id PEASANT_MOVE = 11;
const Id ARCHER_MOVE = 21;
const Id PEASANT_ATTACK = 12;
const Id ARCHER_ATTACK = 22;
const Id EXPLOSION = 100;

//extern std::ofstream logger;

#endif // UTIL_HPP
