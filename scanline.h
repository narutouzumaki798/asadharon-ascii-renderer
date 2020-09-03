#include "active_edge.h"

class scanline_renderer{
public:
	vector<triangle> triangle_list;
	int depth_buffer[1000][1000]; // depth_buffer buffer
	char screen_buffer[1000][1000]; // screen_buffer buffer
	int width, height;
	int marginx, marginy;
	matrix pos; // position+orientation of the camera
	WINDOW* win;
	vec light_source; // position of light_source with respect ot pos coordinate system
	vec eye; // positon of eye with respect to pos coordinate system

	void reset()
	{
		for(int i=0; i<1000; i++)
		{
			for(int j=0; j<1000; j++)
			{
				depth_buffer[i][j] = -INF;
				screen_buffer[i][j] = 'x';
			}
		}
		werase(win);
		//box(win, 0, 0);
	}

	scanline_renderer() // constructor
	{
		int font_size = 10;
		height = 480/font_size;
		width = 1720/font_size;
		int startx = 8, starty = 2;
		marginx = 5; marginy = 2;
		win = newwin(height, width, starty, startx);
		//box(win, 0, 0);
		wrefresh(win);

		pos = matrix(4, 4);
		pos[0][1] = 1; pos[0][2] = 0; pos[0][3] = 0; pos[0][4] = 0;
		pos[1][1] = 0; pos[1][2] = 1; pos[1][3] = 0; pos[1][4] = 0;
		pos[2][1] = 0; pos[2][2] = 0; pos[2][3] = 1; pos[2][4] = 0;
		pos[3][1] = 0; pos[3][2] = 0; pos[3][3] = 0; pos[3][4] = 1;  // alligned with world coordinate system by default

		eye.x = 0; eye.y = 0; eye.z = 100;
		light_source = eye; // for now

		reset();
	}

	void pixel(int x, int y, char ch) // put char ch at pixel position (x, y)
	{
		int pixel_width = 2, pixel_height = 1;
		int r = height/2 - y/(2); // screen_buffer row corresponding to y
		int c = width/2 + x; // screen_buffer column corresponding to x
		if(r < marginy || r > height - marginy) return;
		if(c < marginx || c > width - marginx) return;

		mvwaddch(win, r, c, ch);
	}

	void paint()
	{
		for(int x = -width/2; x <= width/2; x++)
		{
			for(int y = -height; y<=height; y++)
			{
				if(screen_buffer[y+500][x+500] != 'x')
				pixel(x, y, screen_buffer[y+500][x+500]);
			}
		}
		wrefresh(win);
	}

	char illumination(vec incident_ray, vec surface_normal, vec eye_dir) // returns character depending on illumination
	{                                                 // (which will depend on angle between reflected ray and eye dir)
		//light_dir.x = 0; light_dir.y = 0; light_dir.z = -1; // fix light dir for debug [sun at (0, 0, infinity)]
		if(vec::dot(surface_normal, eye_dir) < 0) return 'x'; // not visible

		surface_normal.normalize();
		vec reflected_ray = incident_ray - surface_normal*(2*vec::dot(incident_ray, surface_normal)); // calculate direction of reflected ray

		double c = vec::dot(reflected_ray, eye_dir)/(reflected_ray.mag()*eye_dir.mag()); // cos(angle between reflected ray and eye dir);

		char I[12] = { '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@' }; // char list corresponding to increasing light
		                                                                             // based on donut.c

		c = (c+1.0)/2.0; // normalizing cos value
		int idx = (c*11);
		return I[idx];
	}


	double get_depth(vec ray, triangle t) // returns depth of ray triangle intersection 
	{
		vec l = ray; l.normalize();
		vec l0 = eye;
		vec p0 = t.p;
		vec n = t.normal; n.normalize();
		double denominator = vec::dot(l, n); 
		if(abs(denominator) < 0.001)  // no intersection
		{
			return 1;
		}
		double numerator = vec::dot(p0 - l0, n);
		double r = numerator/denominator;
		vec phit = l0 + l*r;
		return phit.z;
	}

	void vec_swap(vec &a, vec &b)
	{
		vec tmp = a; a = b; b = tmp;
	}


	void draw_triangle(triangle t_original)
	{
		triangle t_projected = t_original; t_projected.project(eye.z);
		//t_projected.show("projected");

		// rearrange
		vec a = t_projected.p; vec b = t_projected.q; vec c = t_projected.r;
		if(b.y > a.y) vec_swap(a, b);
		if(c.y > a.y) vec_swap(a, c); // now a has the highest y
		if(b.y < c.y) vec_swap(b, c); // now c has the lowest y

		edge ca(c, a); edge cb(c, b); edge ba(b, a);

		//fprintf(stderr, "ca: %d %d, cb: %d %d, ba: %d %d\n", 
		// ca.lower_y, ca.upper_y, cb.lower_y, cb.upper_y, ba.lower_y, ba.upper_y);
		//fflush(stderr);


		vec centroid = (t_original.p + t_original.q + t_original.r)/3.0;
		vec incident_ray = centroid - light_source;
		vec eye_dir = eye - centroid; 
		char brt = illumination(incident_ray, t_original.normal, eye_dir);

		for(int y = cb.lower_y; y <= cb.upper_y; y++)
		{
			int left = min(ca.curr(), cb.curr());
			int right = max(ca.curr(), cb.curr());
			// fill segment
			for(int x = left; x <= right; x++)
			{
				vec screen_pixel((double)x, (double)y, 0);
				vec ray = screen_pixel - eye;
				double pixel_depth = get_depth(ray, t_original);
				// double pixel_depth = max(t_original.p.z, max(t_original.q.z, t_original.r.z));
				if(pixel_depth > depth_buffer[y+500][x+500])
				{
					screen_buffer[y+500][x+500] = brt;
					depth_buffer[y+500][x+500] = pixel_depth;
				}
			}
			cb.update();
			ca.update();
		}
		ba.update();
		for(int y = ba.lower_y + 1; y <= ba.upper_y; y++)
		{
			int left = min(ca.curr(), ba.curr());
			int right = max(ca.curr(), ba.curr());
			// fill segment
			for(int x = left; x <= right; x++)
			{
				vec screen_pixel((double)x, (double)y, 0);
				vec ray = screen_pixel - eye;
				// double pixel_depth = get_depth(ray, t_original);
				double pixel_depth = max(t_original.p.z, max(t_original.q.z, t_original.r.z));
				if(pixel_depth > depth_buffer[y+500][x+500])
				{
					screen_buffer[y+500][x+500] = brt;
					depth_buffer[y+500][x+500] = pixel_depth;
				}
			}
			ba.update();
			ca.update();
		}
	}

	void render(mesh mesh1)
	{
		reset();
		// matrix caminv = pos.inv();
		for(int j=0; j<mesh1.size(); j++)
		{
			//if(j != 0) continue;
			triangle t = mesh1[j];
			// transform with pos^-1 (change of basis) when camera moves 
			// t.transform(camerinv);
			// t.show();

			if(t.p.z > 0 || t.q.z > 0 || t.r.z > 0) continue;

			//line_draw_triangle(t);
			draw_triangle(t);
		}
		paint();
	}
};


