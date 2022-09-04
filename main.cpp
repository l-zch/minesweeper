#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <string.h>
#include <tuple>
#include <vector>
using std::cin;
using std::cout;
using std::vector;

using Map = vector<vector<int>>;

int SIZE = 10;
int MINE_NUMBER;
bool isEnded = false;

Map map;
Map backend;
int triggerd = 0;
int FIELD_X[] = {0, 1, 1, 1, 0, -1, -1, -1};
int FIELD_Y[] = {1, 1, 0, -1, -1, -1, 0, 1};

typedef struct Coordinate {
	int x;
	int y;
	bool operator<(const Coordinate &other) const {
		return x + y < other.y + other.y;
	}
} Coordinate;

void printMap() {
	std::system("clear");

	std::string l[] = {
		"⬜", "１", "２", "３", "４", "５", "６", "７", "８", "💣"};
	if (MINE_NUMBER + triggerd == SIZE * SIZE) {
		l[0] = "🎉";
		isEnded = true;
	}

	cout << "　";
	for (int col = 0; col < SIZE; ++col)
		cout << std::left << std::setw(2) << col;
	cout << "\n";

	int triggered = 0;
	for (int row = 0; row < SIZE; ++row) {
		cout << std::left << std::setw(2) << row;
		for (auto &it : map[row]) {
			if (it == -1)
				cout << "　";
			else
				cout << l[it];
		}
		cout << "\n";
	}
}

void makeMap(Coordinate firstCoord) {
	std::set<Coordinate> coords;
	std::random_device rd;
  coords.insert(firstCoord);
  
	while (coords.size() <= MINE_NUMBER){
    coords.insert(
      Coordinate{ (int)(rd() % SIZE), (int)(rd() % SIZE) }
    );
  }

  coords.erase(firstCoord);
	for (auto coord : coords) {
		int x = coord.x, y = coord.y;
		for (int i = 0; i < 9; ++i) {
			try {
				backend.at(y + FIELD_Y[i]).at(x + FIELD_X[i]) += 1;
			} catch (...) {
			}
		}
		backend[y][x] = 9;
	}
}

void mapUpdate(int x, int y) {
	int &field = backend.at(y).at(x);
	if (field == 0)
		field = -1;
	map[y][x] = field;
	field = -1;
	printMap();
}

void trigger(int x, int y) {
	std::queue<Coordinate> coords;
	coords.push(Coordinate{x, y});

	while (!coords.empty()) {
		int x = coords.front().x, y = coords.front().y;
		coords.pop();

		try {
			int &field = backend.at(y).at(x);
      
      if (field > 8)
        isEnded = true;
      if (field == -1)
        continue;
      else {
        ++triggerd;
        if (field == 0)
          for (int i = 0; i < 8; ++i)
            coords.push(Coordinate{x + FIELD_X[i], y + FIELD_Y[i]});
      }
      mapUpdate(x, y);
		} catch (...) {}
	}
}

int main() {
	cout << "請輸入大小:";
	cin >> SIZE;
	cout << "請輸入地雷數:";
	cin >> MINE_NUMBER;
	map.resize(SIZE);
	for (auto &row : map)
		row.resize(SIZE);
	backend = map;

  printMap();
  int x, y;
  cin >> x >> y;
	makeMap(Coordinate{ x, y });
  trigger(x,y);

	while (!isEnded) {
		cin >> x >> y;
		trigger(x, y);
	}

	return 0;
}