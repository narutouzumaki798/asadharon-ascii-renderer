#include <bits/stdc++.h>
using namespace std;
#define INF 1000000


#include <unistd.h>
#include <ncurses.h>
#include "triangle.h"
#include "scanline.h"

mesh cube()
{
	mesh ans;
	vec p2(-10,  10, -5);  vec p3( 10,  10, -5);  // front
	vec p1(-10, -10, -5);  vec p4( 10, -10, -5);
	vec p6(-10,  10, -25); vec p7( 10,  10, -25); // back
	vec p5(-10, -10, -25); vec p8( 10, -10, -25);
	vec p9(0, 0, -15); // pivot

	p1 = p1*5;
	p2 = p2*5;
	p3 = p3*5;
	p4 = p4*5;
	p5 = p5*5;
	p6 = p6*5;
	p7 = p7*5;
	p8 = p8*5;
	p9 = p9*5;

	vec shift(0, 0, -20);
	p1 = p1 + shift;
	p2 = p2 + shift;
	p3 = p3 + shift;
	p4 = p4 + shift;
	p5 = p5 + shift;
	p6 = p6 + shift;
	p7 = p7 + shift;
	p8 = p8 + shift;
	p9 = p9 + shift;

	// front face
	triangle f1(p1, p3, p2);  triangle f2(p1, p4, p3); 
	ans.push(f1); ans.push(f2);

	// left face
	triangle f3(p5, p1, p2);  triangle f4(p2, p6, p5); 
	ans.push(f3); ans.push(f4);

	// back face
	triangle f5(p5, p6, p7);  triangle f6(p7, p8, p5); 
	ans.push(f5); ans.push(f6);

	// right face
	triangle f7(p3, p4, p8);  triangle f8(p8, p7, p3); 
	ans.push(f7); ans.push(f8);

	// top face
	triangle f9(p7, p6, p2);  triangle f10(p2, p3, p7);
	ans.push(f9); ans.push(f10);

	// bottom face
	triangle f11(p1, p5, p8); triangle f12(p8, p4, p1);
	ans.push(f11); ans.push(f12);

	ans.pivot = p9;
	return ans;
}


mesh tri()
{
	mesh ans;
	vec p1(-10, 10, 0); vec p2(10, 10, 0); vec p3(0, -10, 0);
	p1 = p1*2;
	p2 = p2*2;
	p3 = p3*2;
	p1.z = -35;
	p2.z = -35;
	p3.z = -35;

	triangle f1(p3, p2, p1);
	ans.push(f1);
	ans.pivot = (p1 + p2 + p3)/3.0;
	return ans;
}



void curses_init()
{
	initscr();
	noecho();
	curs_set(0);
	refresh();
	setlocale(LC_ALL, "");
}

void curses_end()
{
	getch();
	endwin();
}


mesh tmp_tri1()
{
	mesh ans;
	vec P1(0.00, -23.81, -55.00);
	vec P2(-20.23, 40.54, -11.67);
	vec P3(8.41, 16.85, -98.33);

	triangle f(P1, P2, P3);
	ans.push(f);
	ans.pivot = (P1 + P2 + P3)/3.0;
	return ans;
}

mesh tmp_tri2()
{
	mesh ans;
	vec P1(0.00, -23.81, -55.00);
	vec P2(20.27, 40.51, -11.71);
	vec P3(-8.44, 16.86, -98.29);

	triangle f(P1, P2, P3);
	ans.push(f);
	ans.pivot = (P1 + P2 + P3)/3.0;
	return ans;
}

mesh pyramid()
{
	mesh ans;
	vec p2(-10, -10, -25); vec p3(10, -10, -25);
	vec p1(-10, -10, -5);  vec p4(10, -10, -5);  // square base
	vec p5(0, 8.5, -15); // apex

	vec p6(0, 1, -15); // pivot

	p1 = p1*2;
	p2 = p2*2;
	p3 = p3*2;
	p4 = p4*2;
	p5 = p5*2;
	p6 = p6*2;

	triangle f1(p5, p1, p4); // front
	triangle f2(p5, p2, p1); // left
	triangle f3(p5, p3, p2); // back
	triangle f4(p5, p4, p3); // right

	triangle f5(p1, p2, p3); triangle f6(p1, p3, p4); // square base

	ans.push(f1);
	ans.push(f2);
	ans.push(f3);
	ans.push(f4);
	ans.push(f5);
	ans.push(f6);

	ans.pivot = p6;
	return ans;
}

int main()
{
	freopen("debug.txt", "w", stderr); fprintf(stderr, "aaa\n\n"); fflush(stderr);
	curses_init();

	//mesh c = cube();
	//c.translate(0, 0, -50);
	scanline_renderer a; // default renderer

	mesh link1;
	link1.from_obj("n64link_1.obj", 650);
	link1.rotatey(30);
	link1.translate(0, -80, -300);

	mesh mario1;
	mario1.from_obj("n64mario_1.obj", 500);
	mario1.rotatey(-30);
	mario1.translate(0, -80, -250);

	int movement_period = 50;
	int zoom_period = 50;
	double movement = 0;  // vertical movement speed
	double zoom = 0; // zoom speed
	double ampl;

	long long time1 = 0;
	double theta = 0;
	while(1) {
		//a.render(c);
		//c.rotatey(3);
		//c.rotatex(1);
		//usleep(8000);
		//break;

		a.render(link1);
		link1.rotatey(2.5);
		link1.translate(0, movement, zoom);
		ampl = -3.5;

		//a.render(mario1);
		//mario1.rotatey(2.5);
		//mario1.translate(0, movement, zoom);
		//ampl = -2.5;

		movement = ampl*cos(theta);
		theta = theta + 0.018;

		time1 = (time1+1)%1000;
		usleep(5000);
	}

	getch();
	endwin();
	return 0;
}




