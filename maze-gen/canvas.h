#pragma once
#include <string>

using namespace std;

// Drawing support
class ASCIICanvas {
private:
	// Pointer (for relative values in drawAt and cleanAt
	int pointerX, pointerY;

	// Content for lines of canvas. Each line is ended by eol and the entire content is ended by 0
	char* buffer;

protected:
	// Width and height
	int w, h;

	// ascii used in string reprezentation
#if defined(_DEBUG)
	static const char initcell = '.';
#else
	static const char initcell = ' ';
#endif
	static const char draw = '#';
	static const char empty = ' ';
	static const char eol = '\n';

	void init();
	void drawAt(int x, int y, char c);
	char getAt(int x, int y);

public:
	ASCIICanvas(int _w, int _h);
	~ASCIICanvas();
	void setAt(int x, int y);
	void cleanAt(int x, int y);
	void setPointer(int _pointerX, int _pointerY);
	void clean();
	string toString();
};

// HTML version of canvas. Produces HTML code in toString() method
class HTMLCanvas : public ASCIICanvas {
private:
	const static string cellSize;
	const static string wallSize;
public:
	HTMLCanvas(int _w, int _h);
	string toString();
};

