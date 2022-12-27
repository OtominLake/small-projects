#include <cstdlib>
#include "maze.h"

using namespace std;

Entry::Entry(MazeWall _wall, int _pos) : wall(_wall), pos(_pos) {
}

MazeWall Entry::getWall() const { return wall; }
int Entry::getPos() const { return pos; }

Maze::Maze(int _left, int _top, int _w, int _h) : left(_left), top(_top), w(_w), h(_h) {
}

Maze::~Maze() {
	for (list<Maze*>::iterator m = subMazes.begin(); m != subMazes.end(); m++) {
		delete (*m);
	}
	subMazes.clear();

	for (list<Entry*>::iterator e = entries.begin(); e != entries.end(); e++) {
		delete (*e);
	}
	entries.clear();
}

// Add entry
void Maze::addEntry(Entry* entry) {
	// Entry must be on odd position
	if ((entry->getPos() % 2) != 1) {
		throw "Entry must be on odd position";
	}
	entries.push_back(entry);
}

	// Add entry but randomize position
void Maze::addRandomEntry(MazeWall wall) {
	int entryPos;

	if (wall == north || wall == south) {
		entryPos = (rand() % (w / 2)) * 2 + 1;
	}
	else
	{
		entryPos = (rand() % (h / 2)) * 2 + 1;
	}
	Entry* newEntry = new Entry(wall, entryPos);
	entries.push_back(newEntry);
}

void Maze::generate() {
	// Width or height == 3 is the lowest possible level of maze (tunnel-like)
	if (w <= 3 || h <= 3) {
		return;
	}

	// Split current rectangle with a new wall and recurse into new couple of sub-mazes
	int newWall = rand() % (w + h);

	// Select horizontal or vertical wall
	if (newWall < w) {
		// Vertical wall
		int pos = (rand() % ((w - 3) / 2)) * 2 + 2;

		// Entry point
		int entryPos = (rand() % (h / 2)) * 2 + 1;

		//			cout << "[debug] vertical wall; pos=" << pos << " entryPos=" << entryPos << endl;

					// Add left sub-maze
		{
			Maze* leftSubMaze = new Maze(left, top, pos + 1, h);
			Entry* leftSubMazeEntry = new Entry(east, entryPos);
			leftSubMaze->addEntry(leftSubMazeEntry);

			// Copy current entries, if placed in left sub-maze
			for (list<Entry*>::iterator eIt = entries.begin(); eIt != entries.end(); eIt++) {
				Entry* e = *eIt;
				if (((e->getWall() == north || e->getWall() == south) && e->getPos() < pos) || e->getWall() == west) {
					Entry* entryCopy = new Entry(e->getWall(), e->getPos());
					leftSubMaze->addEntry(entryCopy);
				}
			}
			subMazes.push_back(leftSubMaze);
			leftSubMaze->generate();
		}

		// Add right sub-maze
		{
			Maze* rightSubMaze = new Maze(left + pos, top, w - pos, h);
			Entry* rightSubMazeEntry = new Entry(west, entryPos);
			rightSubMaze->addEntry(rightSubMazeEntry);

			// Copy current entries, if placed in right sub-maze
			for (list<Entry*>::iterator eIt = entries.begin(); eIt != entries.end(); eIt++) {
				Entry* e = *eIt;
				if ((e->getWall() == north || e->getWall() == south) && e->getPos() > pos) {
					Entry* entryCopy = new Entry(e->getWall(), e->getPos() - pos);
					rightSubMaze->addEntry(entryCopy);
				}
				else if (e->getWall() == east) {
					Entry* entryCopy = new Entry(e->getWall(), e->getPos());
					rightSubMaze->addEntry(entryCopy);
				}
			}
			subMazes.push_back(rightSubMaze);
			rightSubMaze->generate();
		}
	}
	else
	{
		// Horizontal wall
		int pos = (rand() % ((h - 3) / 2)) * 2 + 2;

		// Entry point
		int entryPos = (rand() % (w / 2)) * 2 + 1;

		//			cout << "[debug] horizontal wall; pos=" << pos << " entryPos=" << entryPos << endl;

					// Add top sub-maze
		{
			Maze* topSubMaze = new Maze(left, top, w, pos + 1);
			Entry* topSubMazeEntry = new Entry(south, entryPos);
			topSubMaze->addEntry(topSubMazeEntry);

			// Copy current entries, if placed in top sub-maze
			for (list<Entry*>::iterator eIt = entries.begin(); eIt != entries.end(); eIt++) {
				Entry* e = *eIt;
				if (((e->getWall() == east || e->getWall() == west) && e->getPos() < pos) || e->getWall() == north) {
					Entry* entryCopy = new Entry(e->getWall(), e->getPos());
					topSubMaze->addEntry(entryCopy);
				}
			}
			subMazes.push_back(topSubMaze);
			topSubMaze->generate();
		}

		// Add bottom sub-maze
		{
			Maze* bottomSubMaze = new Maze(left, top + pos, w, h - pos);
			Entry* bottomSubMazeEntry = new Entry(north, entryPos);
			bottomSubMaze->addEntry(bottomSubMazeEntry);

			// Copy current entries, if placed in bottom sub-maze
			for (list<Entry*>::iterator eIt = entries.begin(); eIt != entries.end(); eIt++) {
				Entry* e = *eIt;
				if ((e->getWall() == east || e->getWall() == west) && e->getPos() > pos) {
					Entry* entryCopy = new Entry(e->getWall(), e->getPos() - pos);
					bottomSubMaze->addEntry(entryCopy);
				}
				else if (e->getWall() == south) {
					Entry* entryCopy = new Entry(e->getWall(), e->getPos());
					bottomSubMaze->addEntry(entryCopy);
				}
			}
			subMazes.push_back(bottomSubMaze);
			bottomSubMaze->generate();
		}
	}
}

	// Draw content
void Maze::draw(ASCIICanvas& canvas) {
	// A special case, where there are not sub-mazes. Allocates drawing buffer and draws its own content
	if (subMazes.size() == 0) {
		//			cout << "[debug] Drawing on canvas (" << left << ", " << top << ", " << w << ", " << h << ")" << endl;
		int x, y;
		canvas.setPointer(left, top);

		// North and soutch walls
		for (x = 0; x < w; x++) {
			canvas.setAt(x, 0);
			canvas.setAt(x, h - 1);
		}

		// East and west walls
		for (y = 0; y < h; y++) {
			canvas.setAt(0, y);
			canvas.setAt(w - 1, y);
		}

		// Draw entry and exit
		for (list<Entry*>::iterator e = entries.begin(); e != entries.end(); e++) {
			switch ((*e)->getWall()) {
			case north:
				canvas.cleanAt((*e)->getPos(), 0);
				break;
			case east:
				canvas.cleanAt(w - 1, (*e)->getPos());
				break;
			case south:
				canvas.cleanAt((*e)->getPos(), h - 1);
				break;
			case west:
				canvas.cleanAt(0, (*e)->getPos());
				break;
			default:
				break;
			}
		}

		return;
	}

	// Otherwise - recurse to drawing sub-mazes
	for (list<Maze*>::iterator m = subMazes.begin(); m != subMazes.end(); m++) {
		(*m)->draw(canvas);
	}
}

