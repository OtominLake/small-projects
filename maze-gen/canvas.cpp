#include "canvas.h"

// Drawing support
void ASCIICanvas::init() {
	// Initialize buffer
	for (int i = 0; i < (w + 2) * (h + 1) + 1; i++) {
		buffer[i] = initcell;
	}

#if defined(_DEBUG)
	// Indices
	for (int i = 0; i < w; i++) {
		buffer[i + 1] = '0' + i % 10;
	}
	for (int i = 0; i < h; i++) {
		buffer[(i + 1) * (w + 2)] = '0' + i % 10;
	}
#endif
	// Endl
	for (int i = 0; i < h + 1; i++) {
		buffer[(w + 2) * i + 1 + w] = eol;
	}

	// 0 ending string
	buffer[(w + 2) * (h + 1)] = 0;
}

void ASCIICanvas::drawAt(int x, int y, char c) {
	if (!(x >= 0 && x + pointerX < w) && (y >= 0 && y + pointerY < h)) {
		throw "Attempt to draw outside canvas";
	}
	buffer[(x + pointerX + 1) + (y + pointerY + 1) * (w + 2)] = c;
}

char ASCIICanvas::getAt(int x, int y) {
	if (!(x >= 0 && x + pointerX < w) && (y >= 0 && y + pointerY < h)) {
		throw "Attempt to read outside canvas";
	}
	return buffer[(x + pointerX + 1) + (y + pointerY + 1) * (w + 2)];
}

ASCIICanvas::ASCIICanvas(int _w, int _h) : w(_w), h(_h), pointerX(0), pointerY(0) {
	// Allocate buffer, add room for indices, endl and ends
	buffer = new char[(_w + 2) * (_h + 1) + 1];

	init();
}

ASCIICanvas::~ASCIICanvas() {
	delete buffer;
}

void ASCIICanvas::setAt(int x, int y) {
	drawAt(x, y, draw);
}

void ASCIICanvas::cleanAt(int x, int y) {
	drawAt(x, y, empty);
}

void ASCIICanvas::setPointer(int _pointerX, int _pointerY) {
	pointerX = _pointerX;
	pointerY = _pointerY;
}

void ASCIICanvas::clean() {
	init();
}

string ASCIICanvas::toString() {
	return buffer;
}

// HTML version of canvas. Produces HTML code in toString() method
HTMLCanvas::HTMLCanvas(int _w, int _h) : ASCIICanvas(_w, _h) {
}

const string HTMLCanvas::cellSize = "12px";

string HTMLCanvas::toString() {
	string output = "<html>\n<head>\n<style>\n";
	output += ".full  { background-color: black; }\n";
	output += ".empty { background-color: white; }\n";
	output += "td	  { width: " + cellSize + "; height: " + cellSize + "; padding : 0; margin : 0; empty - cells: show; }\n";
	output += "td div { width: " + cellSize + "; height: " + cellSize + "; overflow: hidden; }\n";
	output += "table  { table-layout : fixed; border-collapse: collapse; padding : 0; margin : auto; }\n";
	output += "</style>\n</head>\n";
	output += "<body>\n<table>\n";

	// Reset pointer
	setPointer(0, 0);

	// Read all cells of the maze (walls and aisles)
	for (int y = 0; y < h; y++) {
		output += "<tr>";
		for (int x = 0; x < w; x++) {
			switch (getAt(x, y)) {
			case draw:
				output += "<td class='full'><div/></td>";
				break;
			default:
				output += "<td class='empty'><div/></td>";
				break;
			}
		}
		output += "</tr>\n";
	}

	output += "</table>\n</body>\n</html>\n";
	return output;
}
