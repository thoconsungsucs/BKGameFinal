#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>

using namespace std;

class Object {
private:
    string path, name;
    string posX, posY, posZ;
    string scaleX, scaleY, scaleZ;
    string rotX, rotY, rotZ;

public:
    Object();
    Object(string path, string name, string posX, string posY, string posZ,
           string scaleX, string scaleY, string scaleZ,
           string rotX, string rotY, string rotZ);

    string getPath();

    string getName();

    string getPosX();

    string getPosY();

    string getPosZ();

    string getScaleX();

    string getScaleY();

    string getScaleZ();

    string getRotX();

    string getRotY();

    string getRotZ();

    // Các phương thức setter
    void setName(string n);

    void setPosX(string x);

    void setPosY(string y);

    void setPosZ(string z);

    void setScaleX(string x);

    void setScaleY(string y);

    void setScaleZ(string z);

    void setRotX(string x);

    void setRotY(string y);

    void setRotZ(string z);
};

#endif //OBJECT_H
