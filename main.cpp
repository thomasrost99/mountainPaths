#include <iostream>
#include <fstream>
#include "functions.h"
#include <cstring>
using namespace std;

static int elevations[MAX_ROWS][MAX_COLS];
static Pixel image[MAX_ROWS][MAX_COLS];

int main() {
	ifstream inFS;
	ofstream outFS;
	int numRows = 0;
	int numCols = 0;
	char fileName[500];
	int max = 0;
	int min = 0;

	cout << "Please input the number of rows in the map: ";
	cin >> numRows;
	if(!(numRows > 0) || !(numRows < 1000))
	{
		cout << "Error: Problem reading in rows and columns." << endl;
		return 1;
	}

	cout << "Please input the number of columns in the map: ";
	cin >> numCols;
	if(!(numCols > 0) || !(numCols < 1000))
	{
		cout << "Error: Problem reading in rows and columns." << endl;
		return 1;
	}

	cout << "Please input the name of the file: ";
	cin >> fileName;

	inFS.open(fileName);
	if(!inFS.is_open())
	{
		cout << "Error: Unable to open file <" << fileName << ">." << endl;
		return 1;
	}

	outFS.open(strcat(fileName, ".ppm"));
	if(!outFS.is_open())
	{
		cout << "Error: Unable to open file <" << fileName << ">." << endl;
		return 1;
	}

	loadData(elevations, numRows, numCols, inFS);

	findMaxMin(elevations, numRows, numCols, max, min);

	loadGreyscale(image, elevations, numRows, numCols, max, min);

	int bestRow = 0;
	int bestLength = max*max;
	int currentLength = 0;
	Pixel one;
	one.red = 252; one.green = 25; one.blue = 63;
	for(int row = 0; row < numRows; ++row)
	{
		currentLength = colorPath(elevations, image, numRows, numCols, one, row);
		if(currentLength < bestLength)
		{
			bestRow = row;
			bestLength = currentLength;
		}
	}

	Pixel two;
	two.red = 31; two.green = 253; two.blue = 13;
	colorPath(elevations, image, numRows, numCols, two, bestRow);

	outputImage(image, numRows, numCols, outFS);
	inFS.close();
	outFS.close();

	return 0;
}
