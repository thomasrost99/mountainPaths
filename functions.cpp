#include <iostream>
#include <fstream>
#include <cmath>
#include "functions.h"
#include <cstring>
using namespace std;

void findMaxMin(const int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, int& max, int& min)
{
	max = elevations[0][0];
	min = elevations[0][0];
	for(int row = 0; row < rows; ++row)
	{
		for(int col = 0; col < cols; ++col)
		{
			if(elevations[row][col] > max)
				max = elevations[row][col];
			if(elevations[row][col] < min)
				min = elevations[row][col];
		}
	}
}

void loadData(int elevations[MAX_ROWS][MAX_COLS], int rows, int cols, istream& inData)
{
	for(int row = 0; row < rows; ++row)
	{
		for(int col = 0; col < cols; ++col)
		{
			if(inData.eof())
			{
				cout << "Error: End of file reached prior to getting all required data." << endl;
				exit(1);
			}
			char temp[500];
			inData >> temp;
			for(int i = 0; i < strlen(temp);++i)
			{
				if(!isdigit(temp[i]) && temp[i] != '-')
				{
					cout << "Error: Read a non-integer value." << endl;
					exit(1);
				}
			}
			int temp2 = atoi(temp);
			elevations[row][col] = temp2;
		}
	}
	if(!(inData.eof()))
	{
		char temp4[50];
		inData >> temp4;
		if(!(inData.fail()))//isdigit(temp4[0]))
		{
			cout << "Error: Too many data points." << endl;
			exit(1);
		}
	}
}

int scaleValue(int value, int max, int min)
{
	double val = value;
	return round(((val-min)/(max-min))*255.0);
}

void loadGreyscale(Pixel image[MAX_ROWS][MAX_COLS], const int elevations[MAX_ROWS][MAX_COLS],int rows, int cols, int max, int min)
{
	for(int row = 0; row < rows; ++row)
	{
		for(int col = 0; col < cols; ++col)
		{
			image[row][col].red = scaleValue(elevations[row][col], max, min);
			image[row][col].green = scaleValue(elevations[row][col], max, min);
			image[row][col].blue = scaleValue(elevations[row][col], max, min);
		}
	}
}

void outputImage(const Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, ostream& outData) {
	outData << "P3\r\n";
	outData << cols << " " << rows << "\r\n";
	outData << "255\r\n";
	for(int row = 0; row < rows; ++row)
	{
		for(int col = 0; col < cols; ++col)
		{
			outData << image[row][col].red << " " << image[row][col].green << " " << image[row][col].blue << " ";
		}
		outData << "\r\n";
	}
}

int colorPath(const int elevations[MAX_ROWS][MAX_COLS], Pixel image[MAX_ROWS][MAX_COLS], int rows, int cols, Pixel color, int start_row)
{
	int row = start_row;
	int length = 0;
	for(int col = 0; col < cols - 1; ++col)
	{
		int compare = elevations[row][col];
		image[row][col] = color;
		if(row == 0)
		{
			int diff2 = abs(compare-elevations[row][col + 1]);
			int diff3 = abs(compare-elevations[row + 1][col + 1]);
			if(diff3 < diff2)
			{
				++row;
				length += diff3;
			}
			else
			{
				length += diff2;
			}
		}
		else if(row == rows - 1)
		{
			int diff1 = abs(compare-elevations[row - 1][col + 1]);
			int diff2 = abs(compare-elevations[row][col + 1]);
			if(diff2 <= diff1)
			{
				length += diff2;
			}
			else
			{
				--row;
				length += diff1;
			}
		}
		else
		{
			int diff1 = abs(compare-elevations[row - 1][col + 1]);
			int diff2 = abs(compare-elevations[row][col + 1]);
			int diff3 = abs(compare-elevations[row + 1][col + 1]);
			if(diff1 < diff2 && diff1 < diff3)
			{
				--row;
				length += diff1;
			}
			else if(diff2 <= diff3)
			{
				length += diff2;
			}
			else
			{
				++row;
				length += diff3;
			}
		}
	}
	image[row][cols - 1] = color;
	return length;
}

