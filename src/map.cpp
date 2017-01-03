#include "map.hpp"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

int q_direction(int q, int dir) {return q + q_directions[dir%6];}
int r_direction(int r, int dir) {return r + r_directions[dir%6];}


double fade(double t) {return t * t * t * (t * (t * 6 - 15) + 10);}
double lerp(double a, double b, double x) {return a + x * (b - a);}

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

//Improved Perlin Noise function
//Supposedly somehow using (1, 1), (1, -1), (-1, 1), (-1, -1) is better looking and computationaly faster (true) 
//Than just forming random gradient vectors
//Basically automatically computes both dot product using the gradient and distance function.
double gradient(int hash, double x, double y)
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

    x1 = lerp(	gradient(aa, xf, yf),
    			gradient(ba, xf-1, yf), u);
    x2 = lerp(	gradient(ab,xf, yf-1), 
    			gradient(bb, xf-1, yf-1), u);
    return (lerp(x1, x2, v) + 1)/2;
}


vector<vector<double> > improved_perlin_noise(int len, int square_num, vector<int> ps)
{
	int size = len;
	vector<int> p = ps;

	vector<vector<double> > gradient_map(size);
	for(int i = 0; i < size; i++)
	{
		vector<double> x(size);
		gradient_map[i] = x;
	}

	for(int i = 0; i < size; i++)
	{
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

vector<vector<double> > quadratic_gradient(int len)
{
	
}



vector<vector<double> > improved_octave(int len, int square_num)
{
	vector<vector<double> > res = improved_perlin_noise(len, square_num);
	int snum = square_num;
	int index = 1;
	while(snum < len/8)
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

vector<vector<double> >


vector<vector<int> > map_generator(int width, int height, int num_seeds)
{



}


Hex get_hex(Hex origin, int dir)
{
	return Hex(0,0);
}



int main(){

	int len = 256;
	int square_num = 4;
	ofstream myfile;
	myfile.open ("data_256_4.csv");
	vector<vector<double> > x = improved_octave(len, square_num);
	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < len; j++)
		{
			myfile << i << "," << j << "," << x[i][j] << endl;
		}
	}
	myfile.close();
}