#ifndef MAP_H
#define MAP_H

#include "Object.h"
#include <vector>
#include <string>

using namespace std;

class Map {
private:
    string name;
    vector<Object> objList;
    string matrix[10][10];

public:
    Map(string index);
    string getIndex();
    const string (*getMatrix() const )[10];
    vector<Object> getList();
    string getElementAt(int x, int y);
    int getObjTotal(int& opt);
    int getTotalObj();

    void setName(string n);
    void setObjList(vector<Object> list);
    void setMatrix();

    void addObject(Object obj);
    void printMatrix();
    void printAllObjects();
    void printAnObject(int index);
};

#endif //MAP_H
