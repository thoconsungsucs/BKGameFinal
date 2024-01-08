#include "Object.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

Object::Object() {
    srand(time(0));
    path = "";
    name = "";
    posX = "0";
    posY = "0";
    posZ = "0";
    scaleX = "1";
    scaleY = "1";
    scaleZ = "1";
    // Random rotation
    rotX = to_string(rand() % 365);
    rotY = to_string(rand() % 365);
    rotZ = to_string(rand() % 365);
}

Object::Object(string path, string name, string posX, string posY, string posZ,
               string scaleX, string scaleY, string scaleZ,
               string rotX, string rotY, string rotZ)
        : path(path), name(name), posX(posX), posY(posY), posZ(posZ),
          scaleX(scaleX), scaleY(scaleY), scaleZ(scaleZ),
          rotX(rotX), rotY(rotY), rotZ(rotZ) {}

// Definitions of getters
string Object::getPath() { return path; }
//... other getters

string Object::getName() { return name; }

string Object::getPosX() { return posX; }

string Object::getPosY() { return posY; }

string Object::getPosZ() { return posZ; }

string Object::getScaleX() { return scaleX; }

string Object::getScaleY() { return scaleY; }

string Object::getScaleZ() { return scaleZ; }

string Object::getRotX() { return rotX; }

string Object::getRotY() { return rotY; }

string Object::getRotZ() { return rotZ; }

// Definitions of setters
void Object::setName(string n) { name = n; }

void Object::setPosX(string x) { posX = x; }

void Object::setPosY(string y) { posY = y; }

void Object::setPosZ(string z) { posZ = z; }

void Object::setScaleX(string x) { scaleX = x; }

void Object::setScaleY(string y) { scaleY = y; }

void Object::setScaleZ(string z) { scaleZ = z; }

void Object::setRotX(string x) { rotX = x; }

void Object::setRotY(string y) { rotY = y; }

void Object::setRotZ(string z) { rotZ = z; }
