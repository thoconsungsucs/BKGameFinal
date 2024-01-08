#include "Map.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

Map::Map(string index) : name(index) {
    name = index;
}

string Map::getIndex() {
    return name;
}

const string (*Map::getMatrix() const )[10] {
    return matrix;
}

vector<Object> Map::getList() {
    return objList;
}

string Map::getElementAt(int x, int y) {
    return matrix[x][y];
}

int Map::getObjTotal(int &opt) {

    int count = 0;
    if (opt == 1) {
        for (int i = 0; i < objList.size(); i++) {
            if (objList[i].getName().substr(0, 4) == "TREE")
                count++;
        }
    } else if (opt == 2) {
        for (int i = 0; i < objList.size(); i++) {
            if (objList[i].getName().substr(0, 5) == "HOUSE")
                count++;
        }
    } else if (opt == 3) {
        for (int i = 0; i < objList.size(); i++) {
            if (objList[i].getName().substr(0, 4) == "CAR")
                count++;
        }
    }
    return count;
}

int Map::getTotalObj() {
    return objList.size();
}

void Map::setName(string n) { name = n; };

void Map::setObjList(vector<Object> list) {
    objList = list;
}

void Map::setMatrix() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = "0";
        }
    }

    for (int i = 0; i < objList.size(); i++) {
        int x = stoi(objList[i].getPosX());
        int y = stoi(objList[i].getPosY());

        if (x >= 0 && x < 10 && y >= 0 && y < 10) {
            matrix[x][y] = objList[i].getName();
        }
    }
}

void Map::addObject(Object obj) {
    objList.push_back(obj);
    setMatrix();
}

void Map::printMatrix() {
    cout << "Map " << name << endl;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (matrix[i][j] == "0") {
                cout << setw(6) << matrix[i][j][0] << matrix[i][j][matrix[i][j].size() - 1];
            } else {
                // Print the content with brackets and ensure it's padded to width 5
                cout << setw(4) << "[" << matrix[i][j][0] << matrix[i][j][matrix[i][j].size() - 1] << "]";
            }
        }
        cout << endl;
    }
    cout << endl;
}

void Map::printAllObjects() {
    cout << "Objects in Map " << name << ":" << endl;
    for (int i = 0; i < objList.size(); i++) {
        cout << "  Object " << i + 1 << ":" << endl;
        cout << "  Name: " << objList[i].getName() << endl;
        cout << "  Position: (" << objList[i].getPosX() << ", " << objList[i].getPosY() << ", "
             << objList[i].getPosZ() << ")" << endl;
        cout << "  Scale: (" << objList[i].getScaleX() << ", " << objList[i].getScaleY() << ", "
             << objList[i].getScaleZ() << ")" << endl;
        cout << "  Rotation: (" << objList[i].getRotX() << ", " << objList[i].getRotY() << ", "
             << objList[i].getRotZ() << ")" << endl;
        cout << endl;
    }
}

void Map::printAnObject(int index) {
    cout << "  Name: " << objList[index].getName() << endl;
    cout << "  Position: (" << objList[index].getPosX() << ", " << objList[index].getPosY() << ", "
         << objList[index].getPosZ() << ")" << endl;
    cout << "  Scale: (" << objList[index].getScaleX() << ", " << objList[index].getScaleY() << ", "
         << objList[index].getScaleZ() << ")" << endl;
    cout << "  Rotation: (" << objList[index].getRotX() << ", " << objList[index].getRotY() << ", "
         << objList[index].getRotZ() << ")" << endl;
}