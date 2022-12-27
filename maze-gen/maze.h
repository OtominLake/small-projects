#pragma once
#include <list>
#include "canvas.h"

enum MazeWall {
	north = 0, east, south, west
};

class Entry {
private:
	MazeWall wall;
	int pos;

public:
	// Constructor with provided all data
	Entry(MazeWall _wall, int _pos);
	MazeWall getWall() const;
	int getPos() const;
};

class Maze
{
private:
	// top, left, width and height
	int top, left, w, h;

	// Entries
	std::list<Entry*> entries;

	// mazes that this consists of
	std::list<Maze*> subMazes;

public:
	Maze(int _left, int _top, int _w, int _h);
	~Maze();

	// Add entry
	void addEntry(Entry* entry);

	// Add entry but randomize position
	void addRandomEntry(MazeWall wall);

	void generate();

	// Draw content
	void draw(ASCIICanvas& canvas);
};
