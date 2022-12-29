// Maze generator
// Create a rectangle shaped maze with 1 entry and 1 exit. It is guaranteed that there is exactly 1 solution to get
// from the entry to the exit

using namespace std;


#include <fstream>
#include <iostream>
#include <time.h>

#include "canvas.h"
#include "maze.h"

int main()
{
	const int width = 181, height = 87;
	srand((unsigned int)time(NULL));

	Maze maze(0, 0, width, height);
	maze.addRandomEntry(east);
	maze.addRandomEntry(west);
	maze.generate();

	// draw to console
	//ASCIICanvas canvas(width, height);
	//maze.draw(canvas);
	//cout << canvas.toString();

	// draw to HTML file
	HTMLCanvas htmlCanvas(width, height);
	maze.draw(htmlCanvas);
	ofstream html;
	html.open("maze.html");
	html << htmlCanvas.toString();
	html.close();
}

