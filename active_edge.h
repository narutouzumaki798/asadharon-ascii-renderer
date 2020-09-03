//#include <bits/stdc++.h>
//using namespace std;
//#include "triangle.cpp"

class edge{
public:
	int lower_y; // y coord of lower point of edge
	int upper_y; // y coord of upper point of edge
	int curr_x;  // current x
	int original_x; // original x required for mirroring
	int dy, dx;     // DX and DY
	int mirror_flag; // if DX < 0 mirror about original x
	int p; // decision parameter
	edge(vec p1, vec p2)
	{
		if(p1.y < p2.y)
		{
			lower_y = (int)p1.y;
			upper_y = (int)p2.y;
			curr_x = (int)p1.x;
			original_x = (int)p1.x;
			dy = upper_y - lower_y;
			dx = p2.x - p1.x;
			curr_x = original_x;
		}
		else 
		{
			lower_y = (int)p2.y;
			upper_y = (int)p1.y;
			curr_x = (int)p2.x;
			original_x = (int)p2.x;
			dy = upper_y - lower_y;
			dx = p1.x - p2.x;
			curr_x = original_x;
		}

		mirror_flag = 0;
		if(dx < 0) { dx = -dx; mirror_flag = 1; }

		if(abs(dy) >= abs(dx)) // jodio dy and dx positive
		{
			p = 2*dx - dy;
		}
		else
		{
			p = 2*dy - dx;
		}
	}
	void update1() // |m| >= 1 (bresenham)
	{
		if(p >= 0)
		{
			curr_x++;
			p = p + 2*dx - 2*dy;
		}
		else
		{
			p = p + 2*dx;
		}
	}
	void update2() // |m| <  1 (inverse bresenham)
	{
		while(p < 0)
		{
			curr_x++;
			p = p + 2*dy;
			// printf("update2 curr_x: %d  p = %d\n", curr_x, p); fflush(stdout);
		}
		curr_x++; // also curr_y++
		p = p + 2*dy - 2*dx;
	}
	void update() // update curr_x such that curr_y = curr_y + 1
	{
		if(dy == 0) // no intersection if y increases
		{
			return;
			//printf("edge update: DY = 0\n\n");
			//exit(1);
		}
		if(abs(dy) >= abs(dx)) // jodio dx and dy are necessarily positive
			update1();
		else
			update2();
	}
	int curr()
	{
		int ans = curr_x;
		if(mirror_flag)
		{
			ans = 2*original_x - curr_x;
		}
		return ans;
	}

};


//    |m| >= 1
//    --------------
//    P =  P + 2DX - 2DY , if P >= 0
//         P + 2DX       , if P < 0
//    P0 = 2DX - DY;
//
//    |m| <  1
//    ---------------
//    P =  P + 2DY - 2DX , if P >= 0
//         P + 2DY       , if P < 0
//    P0 = 2DY - DX;


//int main()
//{
//	vec p1(0, 0, 0); vec p2(10, 1, 0); 
//	edge a(p1, p2);
//	int y = 0;
//	while(1)
//	{
//		printf("y = %d   edge.curr_x: %d\n", y, a.curr());
//		y++; a.update();
//		if(y == 9) break;
//	}
//	return 0;
//}

