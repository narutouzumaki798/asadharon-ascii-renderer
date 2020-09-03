#include <bits/stdc++.h>
using namespace std;

#include <unistd.h>
#include <ncurses.h>
#include "mesh.cpp"
#include "camera.cpp"


int main()
{
	freopen("debug.txt", "w", stderr);
	initscr();
	noecho();
	curs_set(0);
	refresh();
	setlocale(LC_ALL, "");

	camera a; // default camera
	int x1 = 0, y1 = 0;
	int x2 = -10, y2 = 20; 
	int x3 = 20, y3 = 10;
	while(1)
	{
		a.line(x1, y1, x2, y2);
		a.line(x1, y1, x3, y3);
		usleep(1000000);
		break;
	}

	getch();
	endwin();
	return 0;
}

/*
int main()
{
	// transformation 1
	matrix a(4, 4);
	a[0][0] =  0.718762; a[0][1] =  0.615033; a[0][2] = -0.324214; a[0][3] = 0;
	a[1][0] = -0.393732; a[1][1] =  0.744416; a[1][2] =  0.539277; a[1][3] = 0;
	a[2][0] =  0.573024; a[2][1] = -0.259959; a[2][2] =  0.777216; a[2][3] = 0;
	a[3][0] =  0.526967; a[3][1] =  1.254234; a[3][2] = -2.532150; a[3][3] = 1;
	a.show("Transformation");

	//matrix b(4, 4);
	//b[0][0] = 1; b[0][1] = 3; b[0][2] = 1; b[0][3] = 4;
	//b[1][0] = 3; b[1][1] = 9; b[1][2] = 5; b[1][3] = 15;
	//b[2][0] = 0; b[2][1] = 2; b[2][2] = 1; b[2][3] = 1;
	//b[3][0] = 0; b[3][1] = 4; b[3][2] = 2; b[3][3] = 3;
	//b.show("Test");
	//printf("Test Det: %lf\n\n", b.det());

	// point local
	vec plocal_v;
	plocal_v.x = -0.5; plocal_v.y = 0.5; plocal_v.z = -0.5;
	vec pworld_v;
	pworld_v = plocal_v*a;
	printf("vec: x=%lf  y=%lf  z=%lf\n\n", pworld_v.x, pworld_v.y, pworld_v.z);

	return 0;
}
*/
