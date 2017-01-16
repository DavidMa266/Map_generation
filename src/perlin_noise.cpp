#include "perlin_noise.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "..\include\GL\glew.h"
#include "..\include\GL\glut.h"

double fade(double t) {return t * t * t * (t * (t * 6 - 15) + 10);}
double lerp(double a, double b, double x) {return a + x * (b - a);}
double screen_coordinate(int value, int size) {return (double)value / size * 2 - 1;}


//Produces a random array, sized 512 of elements 0-255
vector<int> random_arr(int num)
{
    srand ( time(NULL) );
	vector<int> p(num);
	vector<int> tmp(num);
	vector<int> res(num * 2);

	for(int i = 0; i < num; i ++) {tmp[i] = i;}
	for(int i = 0; i < num; i++)
	{
		int r_num = rand() % (num-i);
		p[i] = tmp[r_num];

		tmp.erase(tmp.begin() + r_num);
	}

    for(int i = 0; i < num*2; i++) {res[i] = p[i % num];}
	return res;
}

vector<vector<vector<int> > > gradient_arr(int square_num)
{
	vector<vector<vector<int> > > x(square_num);
	int constant = 100;
	srand(time(NULL));
	for(int i = 0; i < square_num; i++)
	{
		vector<vector<int> > y(square_num);
		for(int j = 0; j < square_num; j++)
		{
			vector<int> z(2);
			z[0] = fade((rand() % constant)/constant - 0.5);
			z[1] = fade((rand() % constant)/constant - 0.5);
			y[j] = z;
		} 
		x[i] = y;
	}
	return x;
}


vector<vector<double> > quadratic_gradient(int len)
{
	vector<vector<double> > res(len);
	for(int i = 0; i < len; i++)
	{
		vector<double> tmp(len);
		res[i] = tmp;
		for(int j = 0; j < len; j++)
		{
			double x = (double) i / len - 0.5;
			double y = (double) j / len - 0.5;
			res[i][j] = sqrt(x * x + y * y);
		}
	}
	return res;
}


double perlin_gradient(int hash, double x, double y)
{
    switch(hash & 0x3)
    {
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        default: return 0; // never happens
    }
}

double improved_perlin(double x, double y, vector<int> p)
{

	int xi = (int) x & 255;
	int yi = (int) y & 255;

	double xf = x - (int) x;
	double yf = y - (int) y;
    double u = fade(xf);
    double v = fade(yf);

    int aa, ab, ba, bb;

    aa = p[p[xi] + yi];
    ab = p[p[xi] + yi + 1];
    ba = p[p[xi+1] + yi];
    bb = p[p[xi+1] + yi + 1];

    double x1, x2;

    x1 = lerp(	perlin_gradient(aa, xf, yf),
    			perlin_gradient(ba, xf-1, yf), u);
    x2 = lerp(	perlin_gradient(ab,xf, yf-1), 
    			perlin_gradient(bb, xf-1, yf-1), u);
    return (lerp(x1, x2, v) + 1)/2;
}


vector<vector<double> > improved_perlin_noise(int len, int square_num, vector<int> ps)
{
	int size = len;
	vector<int> p = ps;
	vector<vector<double> > gradient_map (size);

	for(int i = 0; i < size; i++)
	{
		vector<double> tmp(size);
		gradient_map[i] = tmp;
		for(int j = 0; j < size; j++)
		{
			gradient_map[i][j] = improved_perlin(((double) i) / size*square_num, ((double)j) / size*square_num, p);
		}
	}

	return gradient_map;

}


vector<vector<double> > improved_perlin_noise(int len, int square_num)
{
	vector<int> p = random_arr(256);
	return improved_perlin_noise(len, square_num, p);
}



vector<vector<double> > improved_octave(int len, int square_num, int lim)
{
	vector<vector<double> > res = improved_perlin_noise(len, square_num);
	int snum = square_num;
	int index = 1;
	while(snum < len/lim)
	{
		snum *= 2;
		vector<vector<double> > octave = improved_perlin_noise(len, snum);
		for(int i = 0; i < len; i++){
			for(int j = 0; j < len; j++){
				res[i][j] += octave[i][j];
			}
		}
		index++;
	}
	for(int i = 0; i < len; i++){
		for(int j = 0; j < len; j++){
			res[i][j] /= index;
		}
	}
	return res;
}

vector<vector<double> > island_perlin(int len, int square_num, int lim)
{
	vector<vector<double> > octave = improved_octave(len, square_num, lim);
	vector<vector<double> > gradient = quadratic_gradient(len);
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < len; j++)
		{
			octave[i][j] -= gradient[i][j];
			//I can apply different functions to make it look cooler!!!!
			//A cube function for example, can make more land
		}
	}
	return octave;
}


void func()
{

	int len = 256;
	int square_num = 4;
	int lim = 16;
	vector<vector<double> > x = island_perlin(len, square_num, lim);
}

void init(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowPosition(500,500);
	glutInitWindowSize(1024,1024);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	char name[] = "hello";
	glutCreateWindow(name);
	glutDisplayFunc(func);
	glutMainLoop();
}

void make_map()
{
	int len = 256;
	int square_num = 4;
	int lim = 16;
	ofstream myfile;
	myfile.open ("data-island.csv");
	vector<vector<double> > x = island_perlin(len, square_num, lim);
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < len; j++)
		{
			myfile << i << "," << j << "," << x[i][j] << endl;
		}
	}
}

