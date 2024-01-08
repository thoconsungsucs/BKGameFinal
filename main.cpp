#include <iostream>
#include "Map.h"
#include "Object.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>  // Bao gồm thư viện này
#include <iomanip>
#include <filesystem>
#include <unordered_set>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;
struct Position {
    int x;
    int y;
};

vector<Map> mapList;

// Đọc file
bool readFile(const string &filename) {
    string line, nameMap, objID, objName, modelName;
    string posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ;
    fstream input;
    input.open(filename, ios::in);

    // Kiểm tra xem file mở được không
    if (!input.is_open()) {
        cout << "FILE ERROR. Cannot open file: " << filename << ". Please try again!\n";
        return false;
    }

    Map curMap(""); // Tạo một bản đồ tạm
    vector<Object> curObjList;
    while (getline(input, line)) {
        if (line.empty()) { // Khi gặp một dòng trống(chuyển sang map khác)
            if (!curMap.getIndex().empty()) { // Kiểm tra xem có bản đồ hiện tại không
                curMap.setObjList(curObjList);
                curMap.setMatrix();
                mapList.push_back(curMap); // Lưu bản đồ hiện tại vào danh sách
                curMap = Map(""); // Reset bản đồ hiện tại
                curObjList.clear();
            }
            continue;
        }

        if (line[0] == 'M') { // Đây là tên bản đồ
            nameMap = line.substr(3); // tên bản đồ bắt đầu sau 3 ký tự đầu
            curMap = Map(nameMap); // Tạo một bản đồ mới với tên được đọc
        } else if (line[0] == 'O' && line[1] == 'B' && line[2] == 'J') {
            // Đọc thông tin đối tượng
            objID = line;
            getline(input, objName);
            getline(input, posX, ',');
            getline(input, posY, ',');
            getline(input, posZ);
            getline(input, scaleX, ',');
            getline(input, scaleY, ',');
            getline(input, scaleZ);
            getline(input, rotX, ',');
            getline(input, rotY, ',');
            getline(input, rotZ);
            getline(input, modelName);

            // Tạo đối tượng và thêm vào danh sách đối tượng hiện tại
            Object obj(modelName, objName, posX, posY, posZ, scaleX, scaleY, scaleZ, rotX, rotY, rotZ);
            curObjList.push_back(obj); // Thêm đối tượng vào bản đồ hiện tại
        }
        // Tiếp tục đọc các dòng tiếp theo
    }

    if (!curMap.getIndex().empty()) { // Lưu bản đồ cuối cùng nếu có
        curMap.setObjList(curObjList);
        curMap.setMatrix();
        mapList.push_back(curMap);

    }

    input.close();
    return true;
}

void printMatrix(string matrix[10][10]) {
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

void printMap()
{
    for (int i = 0; i < mapList.size(); i++) {
        mapList[i].printMatrix();
    }
}

// In tất cả tên map
void printMapName(){
    cout << "Available maps:\n";
    for (int i = 0; i < mapList.size(); i++) {
        cout << i + 1 << ". " << mapList[i].getIndex() << endl;
    }
}

//Function 1:
// Sử dụng 'w', 'a', 's', 'd' để di chuyển
// Nhấn 'e' để thoát
void play() {
    int index;
    do {
        // Chọn map muốn chơi
        cout << "There are " << mapList.size() << ", choose map you want to go to";
        cin >> index;
    } while (index > mapList.size() && index < 0);
    index--;
    bool flag = false; // First time
    string matrix[10][10];
    int curX = 0;
    int curY = 0;
    do {
        if (index < mapList.size()) {
            if (!flag || (curX == 9 && curY == 9)) {
                // Lấy dữ liệu map hiện tại
                const string (*matrixPtr)[10] = mapList[index].getMatrix();
                for (int i = 0; i < 10; i++) {
                    for (int j = 0; j < 10; j++) {
                        matrix[i][j] = matrixPtr[i][j];
                    }
                }
                matrix[0][0] = "NV";
                curX = 0;
                curY = 0;
                index++;
            }
        }
        flag = true;
        cout << "Map" << mapList[index - 1].getIndex() << endl;
        printMatrix(matrix);
        cout << "Input 'w', 'a', 's', 'd' to move" << endl;
        cout << "'e' to exit";
        char move;
        cin >> move;
        switch (move) {
            case 'w':
                if (curX == 0 || (matrix[curX - 1][curY] != "0" && matrix[curX - 1][curY].substr(0, 4) != "GOTO")) continue;
                matrix[curX][curY] = "0";
                matrix[curX - 1][curY] = "NV";
                curX--;
                break;
            case 'a':
                if (curY == 0 || (matrix[curX][curY - 1] != "0" && matrix[curX][curY - 1].substr(0, 4) != "GOTO")) continue;
                matrix[curX][curY] = "0";
                matrix[curX][curY - 1] = "NV";
                curY--;
                break;
            case 's':
                if (curX == 9 || (matrix[curX + 1][curY] != "0" && matrix[curX + 1][curY].substr(0, 4) != "GOTO")) continue;
                matrix[curX][curY] = "0";
                matrix[curX + 1][curY] = "NV";
                curX++;
                break;
            case 'd':
                if (curY == 9 || (matrix[curX][curY + 1] != "0" && matrix[curX][curY + 1].substr(0, 4) != "GOTO")) continue;
                matrix[curX][curY] = "0";
                matrix[curX][curY + 1] = "NV";
                curY++;
                break;
            case 'e':
                flag = false;
                break;
        }
    } while (flag);
}


// Function 2: Find patch
bool isValidMove(int x, int y, const vector<vector<int>>& matrix, const vector<vector<bool>>& visited) {
    return x >= 0 && x < matrix.size() && y >= 0 && y < matrix[0].size() && matrix[x][y] == 0 && !visited[x][y];
}

// Tìm đường đi từ điểm xuất phát đến điểm đích
vector<Position> findPath(const vector<vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    // Điểm xuất phát và điểm đích là cố định
    Position start = {0, 0};
    Position end = {8, 9};

    // Khởi tạo ma trận visited để theo dõi các ô đã được đi qua
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    // Hướng di chuyển ban đầu
    vector<Position> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    // Lưu trữ kết quả
    vector<Position> path;
    path.push_back(start);

    // Quy hoạch động để tìm đường đi
    Position current = start;
    while (current.x != end.x || current.y != end.y) {
        bool foundMove = false;

        for (const auto& direction : directions) {
            int new_x = current.x + direction.x;
            int new_y = current.y + direction.y;

            if (isValidMove(new_x, new_y, matrix, visited)) {
                visited[new_x][new_y] = true;
                path.push_back({new_x, new_y});
                current = {new_x, new_y};
                foundMove = true;
                break;
            }
        }

        if (!foundMove) {
            // Nếu không tìm thấy bước di chuyển hợp lệ, quay lui
            path.pop_back();
            if (!path.empty()) {
                current = path.back();
            } else {
                // Không còn đường để quay lui
                break;
            }
        }
    }

    return path;
}

void printpathMatrix(const string (*matrix)[10], const vector<Position>& path) {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            bool isPath = false;
            for (const auto& pos : path) {
                if (pos.x == i && pos.y == j) {
                    isPath = true;
                    break;
                }
            }

            if (isPath) {
                // Nếu là đường đi, in "**"
                cout << setw(7) << "**";
            } else {
                if (matrix[i][j] == "0") {
                    // Nếu là giá trị "0", in "00"
                    cout << setw(6) << matrix[i][j][0] << matrix[i][j][matrix[i][j].size() - 1];
                } else {
                    // Nếu là giá trị khác "0", in giá trị với dấu ngoặc vuông và đảm bảo căn chỉnh với độ rộng 5
                    cout << setw(4) << "[" << matrix[i][j][0] << matrix[i][j][matrix[i][j].size() - 1] << "]";
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}
void countObject(const string (*matrix)[10], const vector<Position>& path, unordered_set<string>& hSet, unordered_set<string>& cSet, unordered_set<string>& tSet) {
    for (const auto& pos : path) {
        int x = pos.x;
        int y = pos.y;

        int radius = 1;
        for (int i = -radius; i <= radius; ++i) {
            for (int j = -radius; j <= radius; ++j) {
                int newX = x + i;
                int newY = y + j;

                if (newX >= 0 && newX < 10 && newY >= 0 && newY < 10 && matrix[newX][newY] != "00") {
                    string symbol = matrix[newX][newY];

                    // Handle objects with brackets
                    if (symbol[0] == '[') {
                        symbol = symbol.substr(1, symbol.size() - 2);
                    }if (symbol[0] == 'H') {
                        hSet.insert(symbol);
                    } else if (symbol[0] == 'C') {
                        cSet.insert(symbol);
                    }else if (symbol[0] == 'T') {
                        tSet.insert(symbol);
                    }
                }
            }
        }
    }
}

void printSets( const unordered_set<string>& hSet, const unordered_set<string>& cSet, const unordered_set<string>& tSet ) {

    if (!hSet.empty()) {
        cout << "Objects in H set:" << endl;
        for (const auto &symbol: hSet) {
            cout << symbol << " ";
        }
    }
    cout << endl;
    if(!cSet.empty()) {
        cout << "Objects in C set:" << endl;
        for (const auto &symbol: cSet) {
            cout << symbol << " ";
        }
    }
    cout << endl;

    if (!tSet.empty()) {
        cout << "Objects in T set:" << endl;
        for (const auto &symbol: tSet) {
            cout << symbol << " ";
        }
    }
    cout << endl;
    size_t maxSize = max({ hSet.size(), cSet.size(),tSet.size()});
    if (maxSize==0){
        cout << "There is no object visible "<<endl;


    } else if (maxSize == hSet.size() && maxSize != 0) {
        cout << " H has the maximum spots: " <<maxSize<< endl;
    } else if (maxSize == cSet.size() && maxSize != 0) {
        cout << " C set has the maximum spots: " <<maxSize<< endl;
    } else if (maxSize == tSet.size() && maxSize != 0) {
        cout << " T set has the maximum spots: " <<maxSize<< endl;
    }
};
void function2(){
    //                 Hỏi người dùng muốn bắt đầu từ Map nào
    string mapfirstIndex, maplastIndex;
    printMapName();
    cout << "Please enter your start map you want to find path: ";
    cin >> mapfirstIndex;
    cout << endl;
    cout << "Please enter your last map you want to finish : ";
    cin >> maplastIndex;
    cout << endl;
    int firstIndex, lastIndex;
    for (int i = 0; i < mapList.size(); i++) {
        if (mapfirstIndex == mapList[i].getIndex()) {
            firstIndex = i;
        }

    }
    for (int i = 0; i < mapList.size(); i++) {
        if (maplastIndex == mapList[i].getIndex()) {
            lastIndex = i;
        }

    }


    for (int t = firstIndex ; t < lastIndex+1; t++ ) {
        // Khởi tạo vector
        cout <<"MAP"<< mapList[t].getIndex()<<endl;
        vector<vector<int>> inputMatrix3;
        unordered_set<string> gSet, hSet, cSet, tSet;
        for (int i = 0; i < 10; i++) {
            vector<int> row;
            for (int j = 0; j < 10; j++) {
                if (mapList[t].getElementAt(i, j) == "0") {
                    row.push_back(0);
                } else {
                    row.push_back(1);
                }
            }
            inputMatrix3.push_back(row);
        }

        // In dữ liệu của inputMatrix3 để kiểm tra
        // mapList[2].printMatrix();

        vector<Position> path = findPath(inputMatrix3);
        countObject(mapList[t].getMatrix(), path, hSet, cSet,tSet);
        // Print the results

        cout << endl;
        printpathMatrix(mapList[t].getMatrix(), path);
        printSets( hSet, cSet, tSet);

    }

};


//Function 3: Tạo Map
void saveNewObject(Object newObject) {
    string folderPath = "model/";
    string fileName = newObject.getName();
    string fullPath = folderPath + fileName + ".obj";
    ofstream file(fullPath);
    if(file.is_open()) {
        file << newObject.getName()<< "\n";
        file << 100;
    }
}

void createMap() {
    string name_map;
    cout << "Enter a name for a new map:";
    getline(cin, name_map);
    Map mymap(name_map);
    int index = mapList.size();
    cout << "Succesfully create a new map name:" << name_map << endl;

    char continues;
    do {
        cout << "Put object into the new map.(Y or N)";
        cin >> continues;
        continues = toupper(continues);

        if (continues == 'Y') {
            int option;
            cout << "1.Tree\n" << "2.House\n" << "3.Car\n";
            cout << "Enter option: ";
            cin >> option;
            int poX, poY, poZ;
            cout << "Enter position X: ";
            cin >> poX;
            cout << "Enter position Y: ";
            cin >> poY;
            cout << "Enter position Z: ";
            cin >> poZ;

            if (poX >= 0 && poX < 10 && poY >= 0 && poY < 10) {
                Object myObject;
                myObject.setName(to_string(rand() % 100));
                if (option == 1) {
                    int tree_total = mymap.getObjTotal(option);
                    myObject.setName("TREE" + to_string(rand() % 100));
                } else if (option == 2) {
                    int house_total = mymap.getObjTotal(option);
                    myObject.setName("HOUSE" + to_string(rand() % 100));
                } else if (option == 3) {
                    int car_total = mymap.getObjTotal(option);
                    myObject.setName("CAR" + to_string(rand() % 100));
                }

                myObject.setPosX(to_string(poX));
                myObject.setPosY(to_string(poY));
                myObject.setPosZ(to_string(poZ));
                mymap.addObject(myObject);
                saveNewObject(myObject);
            } else {
                cout << "Invalid coordinates. Coordinates must be between 0 and 9." << endl;
            }
        }
    } while (continues == 'Y');
    Object newgoto;
    newgoto.setName("GOTO" + to_string(rand() % 100));
    newgoto.setPosX("9");
    newgoto.setPosY("9");
    newgoto.setPosZ("9");
    saveNewObject(newgoto);
    mymap.addObject(newgoto);
    mapList.push_back(mymap);
    printMap();
}


/*Function 4:
1.Add object
2.Change object information
3.Remove object
4.Remove map
*/

//Kiểm tra trùng lặp vị trí object
bool isPositionValid(int x, int y, int z, vector<Object> objList) {
    if (x < 0 || x >= 10 || y < 0 || y >= 10 || z < 0 || z >= 10) return false;

    for (int i = 0; i < objList.size(); i++) {
        if (objList[i].getPosX() == to_string(x) && objList[i].getPosY() == to_string(y) && objList[i].getPosZ() == to_string(z)) {
            return false;
        }
    }
    return true;
}

// Thêm 1 object
void addObjectInMap(int mapIndex) {
    mapIndex--; // Chỉ số mảng bắt đầu từ 0
    // Nhập thông tin đối tượng
    string posX, posY, posZ;
    int option;
    cout << "1.Tree\n" << "2.House\n" << "3.Car\n" << "4.GOTO\n";
    cout << "Enter option: ";
    cin >> option;
    Object myObject;
    do {
        cout << "Enter the new X position: ";
        cin >> posX;
        cout << "Enter the new Y position: ";
        cin >> posY;
        cout << "Enter the new Z position: ";
        cin >> posZ;

        if (!isPositionValid(stoi(posX),stoi(posY),stoi(posZ),mapList[mapIndex].getList())) {
            cout << "Has an object in its position.Please try again.\n";
        } else {
            break;
        }
    } while (true);

    if (stoi(posX) >= 0 && stoi(posX) < 10 && stoi(posY) >= 0 && stoi(posY) < 10) {
        if (option == 1) {
            int tree_total = mapList[mapIndex].getObjTotal(option);
            myObject.setName("TREE" + to_string(rand() % 100));
        } else if (option == 2) {
            int house_total = mapList[mapIndex].getObjTotal(option);
            myObject.setName("HOUSE" + to_string(rand() % 100));
        } else if (option == 3) {
            int car_total = mapList[mapIndex].getObjTotal(option);
            myObject.setName("CAR" + to_string(rand() % 100));
        }
        myObject.setPosX(posX);
        myObject.setPosY(posY);
        myObject.setPosZ(posZ);

        // Tạo đối tượng mới và thêm vào bản đồ
        mapList[mapIndex].addObject(myObject);
        mapList[mapIndex].setMatrix();
        mapList[mapIndex].printAllObjects();
        saveNewObject(myObject);
        cout << "Object added successfully to the map: " << mapList[mapIndex].getIndex() << endl;
    }
}

// Xóa 1 object
void removeObjectInMap(int mapIndex) {
    int objectIndex = 0;
    mapIndex--;
    mapList[mapIndex].printAllObjects();

    do{
        cout << "Select an object (1 - " << mapList[mapIndex].getTotalObj() << "): ";
        cin >> objectIndex;
    } while (objectIndex < 1 || objectIndex > mapList[mapIndex].getTotalObj());
    objectIndex--;
    vector<Object> objList;
    objList = mapList[mapIndex].getList();
    objList.erase(objList.begin() + objectIndex);
    mapList[mapIndex].setObjList(objList);
    mapList[mapIndex].setMatrix();
    cout << "Object removed successfully in map: " << mapList[mapIndex].getIndex() << endl;
    mapList[mapIndex].printAllObjects();
}


// Xóa 1 map
void removeMap(int mapIndex){
    mapIndex--;
    string folderPath = "model/";
    vector<Object> objList;
    objList = mapList[mapIndex].getList();
    for (int i = 0; i < objList.size(); i++)
    {
        string fileName = objList[i].getName();
        string fullPath = folderPath + fileName + ".obj";
        remove(fullPath.c_str());
    }
    mapList.erase(mapList.begin() + mapIndex);
    cout << "Succesfully remove map.\n";
    printMapName();
}


void changeMap() {
    int mapIndex, objectIndex, option,opt;
    bool continues = true;

    // Display available maps
    printMapName();

    // Select a map
    do {
        cout << "Select a map (1 - " << mapList.size() << "): ";
        cin >> mapIndex;
    } while (mapIndex < 1 || mapIndex > mapList.size());

    do {
        cout << "\nCHANGEMAP\n";
        cout << "1.Add Object in map.\n";
        cout << "2.Change Object in map.\n";
        cout << "3.Remove Object in map.\n";
        cout << "4.Remove map.\n";
        cout << "5.Exit.\n";
        cout << "Enter option: ";
        cin >> opt;
        if (opt == 1){
            addObjectInMap(mapIndex);
        }
        else if (opt == 2) {
            // Display objects in the selected map
            mapList[mapIndex - 1].printAllObjects();

            // Select an object
            do {
                cout << "Select an object (1 - " << mapList[mapIndex - 1].getTotalObj() << "): ";
                cin >> objectIndex;
            } while (objectIndex < 1 || objectIndex > mapList[mapIndex - 1].getTotalObj());

            // Display the current attributes of the selected object
            cout << "Current attributes of the selected object:\n";
            mapList[mapIndex - 1].printAnObject(objectIndex - 1);

            // Choose the type of attribute to change
            cout << "Choose the type of attribute to change:\n";
            cout << "1. Change name\n";
            cout << "2. Change position\n";
            cout << "3. Change scale\n";
            cout << "4. Change rotation\n";
            cout << "Enter your choice (1 - 4): ";
            cin >> option;

            // Perform the change based on the chosen option
            switch (option) {
                case 1: // Change name
                {
                    string newName;
                    cout << "Enter the new name for the object: ";
                    cin >> newName;
                    mapList[mapIndex - 1].getList()[objectIndex - 1].setName(newName);
                    break;
                }
                case 2: // Change position
                {
                    vector<Object> newObjList;

                    string  newX,newY,newZ;

                    newObjList = mapList[mapIndex - 1].getList();

                    do {
                        cout << "Enter the new X position: ";
                        cin >> newX;
                        cout << "Enter the new Y position: ";
                        cin >> newY;
                        cout << "Enter the new Z position: ";
                        cin >> newZ;

                        if (!isPositionValid(stoi(newX),stoi(newY),stoi(newZ),newObjList)) {
                            cout << "Invalid position. Either out of bounds or position is already taken. Please try again.\n";
                        } else {
                            break;
                        }
                    } while (true);

                    newObjList[objectIndex - 1].setPosX(newX);
                    newObjList[objectIndex - 1].setPosY(newY);
                    newObjList[objectIndex - 1].setPosZ(newZ);
                    mapList[mapIndex - 1].setObjList(newObjList);
                    mapList[mapIndex - 1].setMatrix();
                    break;
                }
                case 3: // Change scale
                {
                    string newScaleX, newScaleY, newScaleZ;
                    cout << "Enter the new X scale: ";
                    cin >> newScaleX;
                    cout << "Enter the new Y scale: ";
                    cin >> newScaleY;
                    cout << "Enter the new Z scale: ";
                    cin >> newScaleZ;

                    vector<Object> newObjList;
                    newObjList = mapList[mapIndex - 1].getList();
                    newObjList[objectIndex - 1].setScaleX(newScaleX);
                    newObjList[objectIndex - 1].setScaleY(newScaleY);
                    newObjList[objectIndex - 1].setScaleZ(newScaleZ);
                    mapList[mapIndex - 1].setObjList(newObjList);
                    break;
                }
                case 4: // Change rotation
                {
                    string newRotX, newRotY, newRotZ;
                    cout << "Enter the new X rotation: ";
                    cin >> newRotX;
                    cout << "Enter the new Y rotation: ";
                    cin >> newRotY;
                    cout << "Enter the new Z rotation: ";
                    cin >> newRotZ;

                    vector<Object> newObjList;
                    newObjList = mapList[mapIndex - 1].getList();
                    newObjList[objectIndex - 1].setRotX(newRotX);
                    newObjList[objectIndex - 1].setRotY(newRotY);
                    newObjList[objectIndex - 1].setRotZ(newRotZ);
                    mapList[mapIndex - 1].setObjList(newObjList);
                    break;
                }
                default:
                    cout << "Invalid choice.";
                    break;
            }

            // Display the updated attributes of the selected object
            cout << "Updated attributes of the selected object:\n";
            mapList[mapIndex - 1].printAnObject(objectIndex - 1);

            mapList[mapIndex - 1].printMatrix();
        }
        else if (opt == 3) {
            removeObjectInMap(mapIndex);
        }
        else if (opt == 4) {
            removeMap(mapIndex);
        }
        else if (opt == 5) {
            continues = false;
        }

    } while (continues);
}


//Lưu thông tin thay đổi vào file
void saveToFile(const string& filename, vector<Map>& mapList) {
    ofstream file;
    file.open("map.txt");

    if (!file.is_open()) {
        cout << "Cannot open file: " << filename << endl;
        return;
    }
    for (auto& map : mapList) {
        file << "MAP" << map.getIndex() << "\n";
        vector<Object> objList = map.getList();

        for (int i = 0; i < objList.size(); i++) {
            file << "OBJ\n";
            file << objList[i].getName() << "\n";
            file << objList[i].getPosX() << "," << objList[i].getPosY() << "," << objList[i].getPosZ() << "\n";
            file << objList[i].getScaleX() << "," << objList[i].getScaleY() << "," << objList[i].getScaleZ() << "\n";
            file << objList[i].getRotX() << "," << objList[i].getRotY() << "," << objList[i].getRotZ() << "\n";
            file << "model\\" << objList[i].getName() << ".obj" << "\n";
        }
        file << "\n";
    }

    file.close();
    cout << "Maps and objects saved to " << filename << endl;
}

//Function 6:Kiểm tra hợp lệ
void checkValid() {
    for (int i = 0; i < mapList.size(); i++) {
        vector<Object> curList = mapList[i].getList();
        for (int j = 0; j < curList.size(); j++) {
            string filePath = curList[j].getPath();

            if (fs::exists(filePath)) {
                cout << "Object " << curList[j].getName() << " has file path \n";

            } else {
                cout << "Object " << curList[j].getName() << " doesn't have file path \n";
            }
        }
    }
    cout << endl;

    for (int i = 0; i < mapList.size(); i++) {
        vector<Object> curList = mapList[i].getList();
        for (int j = 0; j < curList.size(); j++) {
            string filePath = curList[j].getPath();
            if (!fs::exists(filePath))
                continue;
            auto fileSize = fs::file_size(filePath);

            ifstream file(filePath);
            string line;

            if (file.is_open()) {
                // Bỏ qua dòng đầu tiên
                getline(file, line);
                // Đọc dòng thứ hai
                getline(file, line);
                file.close();
            } else {
                cout << "Cannot open file" << endl;
            }
            if (fileSize > stoi(line)) {
                cout << "Invalid defined byte size in file " << curList[j].getName() << ".obj (" << fileSize << " > "
                     << line << ") \n";
            }
        }
    }
}

int main() {
    readFile("map.txt");
    int choice;

    while (true) {
        cout << "MENU\n";
        cout << "0:Print all map.\n";
        cout << "1:Play.\n";
        cout << "2:Find path.\n";
        cout << "3:Create map.\n";
        cout << "4:Change in map.\n";
        cout << "5:Cheking for validity.\n";
        cout << "6:Exit\n";
        cout << "Enter choice:";
        cin >> choice;
        cin.ignore();
        if (choice == 0) {
            printMap();
        }

        else if (choice == 1)
        {
            play();
        }



        else if (choice == 2) {

            function2();
        }
        else if (choice == 3)
        {
            createMap();
        }

        else if (choice == 4)
        {
            changeMap();
        }

        else if (choice == 5)
        {
            checkValid();
        }

        else if (choice == 6)
        {
            cout << "ENDING PROGRAM!" << endl;
            saveToFile("map.txt",mapList);
            break;
        }
    }
}