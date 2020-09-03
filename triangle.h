#include "matrix.h"

class vec // vector class
{
public:
	double x;
	double y;
	double z;

	// constructors
	vec()
	{
		x = 0; y = 0; z = 0;
	}
	vec(double a, double b, double c)
	{
		x = a; y = b; z = c;
	}
	vec(const vec& that)
	{
		this->x = that.x;
		this->y = that.y;
		this->z = that.z;
	}

	// operators
	void operator=(const vec& that)
	{
		this->x = that.x;
		this->y = that.y;
		this->z = that.z;
	}
	vec operator+(vec that)
	{
		vec ans;
		ans.x = this->x + that.x;
		ans.y = this->y + that.y;
		ans.z = this->z + that.z;
		return ans;
	}
	vec operator-(vec that)
	{
		vec ans;
		ans.x = this->x - that.x;
		ans.y = this->y - that.y;
		ans.z = this->z - that.z;
		return ans;
	}
	vec operator*(double m)
	{
		vec ans;
		ans.x = this->x*m;
		ans.y = this->y*m;
		ans.z = this->z*m;
		return ans;
	}
	vec operator/(double m)
	{
		vec ans;
		ans.x = this->x/m;
		ans.y = this->y/m;
		ans.z = this->z/m;
		return ans;
	}
	vec operator*(const matrix& m)
	{
		matrix v(1, 4);
		v[0][0] = this->x; v[0][1] = this->y; v[0][2] = this->z; v[0][3] = 1;
		matrix m_ans = v*m; 
		vec ans;
		ans.x = m_ans[0][0]; ans.y = m_ans[0][1]; ans.z = m_ans[0][2];
		return ans;
	}
	// vec operator*(const matrix& m)
	// {
	// 	//                         [ ...... ]
	// 	// [vec.x vec.y vec.z 1] x [ ...... ]
	// 	//                         [ ...... ]
	// 	if(m.ncol != 4) 
	// 	{
	// 		printf("vec matrix multiplication ncol = %d\n\n", m.ncol);
	// 		exit(1);
	// 	}
	// 	vec ans;
	// 	ans.x = this->x * m[0][0] + this->y * m[1][0] + this->z * m[2][0] + m[3][0];
	// 	ans.y = this->x * m[0][1] + this->y * m[1][1] + this->z * m[2][1] + m[3][1];
	// 	ans.z = this->x * m[0][2] + this->y * m[1][2] + this->z * m[2][2] + m[3][2];
	// 	return ans;
	// }
	static vec cross(vec a, vec b)
	{
		vec ans;
		ans.x = a.y*b.z - a.z*b.y;
		ans.y = -1.0*(a.x*b.z - a.z*b.x);
		ans.z = a.x*b.y - a.y*b.x;
		return ans;
	}
	static double dot(vec a, vec b)
	{
		double ans = 0;
		ans += (a.x * b.x);
		ans += (a.y * b.y);
		ans += (a.z * b.z);
		return ans;
	}
	void project(double d) // project on xy-plane with eye at (0, 0, d)
	{
		if(z > 0)
		{
			//endwin();
			//printf("vec projection z > 0: %.2lf\n\n", z);
			//fprintf(stderr, "vec projection z > 0: %.2lf\n", z);
			//exit(1);
			return;
		}
		x = (d*x)/(-z+d);
		y = (d*y)/(-z+d);
		// preserve z value for depth buffer
	}
	void unproject(double d) // unproject from xy plane [(0,0,d) te chokh rekhe]
	{
		x = (x*(-z+d))/d;
		y = (y*(-z+d))/d;
	}
	void show()
	{
		fprintf(stderr, "(%.2lf, %.2lf, %.2lf)", x, y, z); fflush(stderr);
	}
	double mag() // magnitue
	{
		double ans = sqrt(x*x + y*y + z*z);
		return ans;
	}
	void normalize()
	{
		double m = mag();
		x = x/m; y = y/m; z = z/m;
	}
};


double signedvolume(vec a, vec b, vec c, vec d) // eta ekhon lagche na, ray-triangle intersection er z-coord emniteo lagbe mone hoe
{
	vec b_a = b - a;
	vec c_a = c - a;
	vec d_a = d - a;
	double ans = (1.0/6.0)*vec::dot(vec::cross(b_a, c_a), d_a);
	return ans;
}

class triangle{
public:
	vec p,q,r; // vertices
	double a,b,c,d; // plane coeff
	vec normal;

	// construct from 3 points
	triangle(vec p1, vec p2, vec p3)
	{
		vec v1 = p2 - p1;
		vec v2 = p3 - p2;
		normal = vec::cross(v1, v2);
		a = normal.x; b = normal.y; c = normal.z;
		d = -(a*p1.x + b*p1.y + c*p1.z);

		p = p1; q = p2; r = p3;
	}

	void reset() // recalculate plane from modified points
	{
		vec v1 = q - p;
		vec v2 = r - q;
		normal = vec::cross(v1, v2);
		a = normal.x; b = normal.y; c = normal.z;
		d = -(a*p.x + b*q.y + c*r.z);
	}

	triangle(const triangle& that) // copy constructor
	{
		this->p = that.p;
		this->q = that.q;
		this->r = that.r;
		this->a = that.a;
		this->b = that.b;
		this->c = that.c;
		this->d = that.d;
		this->normal = that.normal;
	}
	void operator=(const triangle& that)
	{
		this->p = that.p;
		this->q = that.q;
		this->r = that.r;
		this->a = that.a;
		this->b = that.b;
		this->c = that.c;
		this->d = that.d;
		this->normal = that.normal;
	}

	bool inside(vec u) // check if point u is inside the triangle
	{
		double test;
		// PQ
		test = vec::dot(vec::cross(q - p, r - p), vec::cross(q - p, u - p)); 
		if(test < 0) return false;

		// QR
		test = vec::dot(vec::cross(r - q, p - q), vec::cross(r - q, u - q)); 
		if(test < 0) return false;

		// RP
		test = vec::dot(vec::cross(p - r, q - r), vec::cross(p - r, u - r)); 
		if(test < 0) return false;

		return true;
	}

	void transform(const matrix& m)
	{
		p = p*m;
		q = q*m;
		r = r*m;

		// fprintf(stderr, "triangle: (%.2lf,%.2lf,%.2lf), (%.2lf,%.2lf,%.2lf), (%.2lf,%.2lf,%.2lf)\n\n",
		// p.x, p.y, p.z, q.x, q.y, q.z, r.x, r.y, r.z); fflush(stderr);

		reset();
	}

	void project(double d) // project triangle on xy-plane
	{
		p.project(d);
		q.project(d);
		r.project(d);
	}
	void unproject(double d) // unproject triangle from xy-plane
	{
		p.unproject(d);
		q.unproject(d);
		r.unproject(d);
	}

	void show(string s="Triangle")
	{
		fprintf(stderr, "%s:\n", s.c_str());
		fprintf(stderr, "P1: "); p.show(); fprintf(stderr, "\n");
		fprintf(stderr, "P2: "); q.show(); fprintf(stderr, "\n");
		fprintf(stderr, "P3: "); r.show(); fprintf(stderr, "\n");
		fprintf(stderr, "Plane Coefficients: %.2lf, %.2lf, %.2lf, %.2lf\n", a, b, c, d);
		fflush(stderr);
	}

};


struct triplet{
	int i;
	int j;
	int k;
};

class mesh{
public:
	vector<triangle> list;
	vec pivot;
	double thetax;
	double thetay;
	double thetaz;
	mesh() // constructor
	{
		thetax = 1.0*3.14/180.9; // all x rotations with this theta for now
		thetay = 20.0*3.14/180.9; // all y rotations with this theta for now
		thetaz = 3.0*3.14/180.9; // all z rotations with this theta for now
		pivot.x = 0; pivot.y = 0; pivot.z = 0;
	}
	mesh(const mesh& that) // copy
	{
		for(int i=0; i<that.list.size(); i++)
			this->list.push_back(that.list[i]);
	}
	void operator=(mesh that)
	{
		for(int i=0; i<that.list.size(); i++)
			this->list.push_back(that.list[i]);
	}
	void push(triangle t) // add triangle t to mesh
	{
		list.push_back(t);
	}
	int size()
	{
		return (int)list.size();
	}
	triangle& operator[](int x)
	{
		return list[x];
	}
	void transform(const matrix& T)
	{
		for(int i = 0; i<list.size(); i++) list[i].transform(T);
	}
	void rotatex(double theta) // rotate about x-axis after translating pivot to origin
	{
		theta = theta*3.14/180.0;
		matrix T(4, 4);
		T[3][0] = -pivot.x; T[3][1] = -pivot.y; T[3][2] = -pivot.z;
		for(int i = 0; i<list.size(); i++) list[i].transform(T);

		matrix R(4, 4);
		R[0][0] = 1; R[0][1] = 0;          R[0][2] = 0;           R[0][3] = 0;
		R[1][0] = 0; R[1][1] = cos(theta); R[1][2] = -sin(theta); R[1][3] = 0;
		R[2][0] = 0; R[2][1] = sin(theta); R[2][2] =  cos(theta); R[2][3] = 0;
		R[3][0] = 0; R[3][1] = 0;          R[3][2] = 0;           R[3][3] = 1;
		for(int i = 0; i<list.size(); i++) list[i].transform(R);

		T[3][0] = pivot.x; T[3][1] = pivot.y; T[3][2] = pivot.z;
		for(int i = 0; i<list.size(); i++) list[i].transform(T);
	}
	void rotatey(double theta)// pivot origin e niye jete hobe...
	{
		theta = theta*3.14/180.0;
		matrix T(4, 4);
		T[3][0] = -pivot.x; T[3][1] = -pivot.y; T[3][2] = -pivot.z;
		for(int i = 0; i<list.size(); i++) 
		{
			list[i].transform(T);
			//printf("transform 1 triangle: %d / %d\n", i+1, list.size()); fflush(stdout);
		}

		matrix R(4, 4);
		R[0][0] =  cos(theta); R[0][1] = 0; R[0][2] = sin(theta); R[0][3] = 0;
		R[1][0] = 0;           R[1][1] = 1; R[1][2] = 0;          R[1][3] = 0;
		R[2][0] = -sin(theta); R[2][1] = 0; R[2][2] = cos(theta); R[2][3] = 0;
		R[3][0] = 0;           R[3][1] = 0; R[3][2] = 0;          R[3][3] = 1;
		for(int i = 0; i<list.size(); i++) list[i].transform(R);

		T[3][0] = pivot.x; T[3][1] = pivot.y; T[3][2] = pivot.z;
		for(int i = 0; i<list.size(); i++) list[i].transform(T);
	}
	void rotatez(double theta)
	{
		theta = theta*3.14/180.0;
		matrix T(4, 4);
		T[3][0] = -pivot.x; T[3][1] = -pivot.y; T[3][2] = -pivot.z;

		for(int i = 0; i<list.size(); i++) list[i].transform(T);
		matrix R(4, 4);
		R[0][0] = cos(theta); R[0][1] = -sin(theta); R[0][2] = 0; R[0][3] = 0;
		R[1][0] = sin(theta); R[1][1] =  cos(theta); R[1][2] = 0; R[1][3] = 0;
		R[2][0] = 0;          R[2][1] = 0;           R[2][2] = 1; R[2][3] = 0;
		R[3][0] = 0;          R[3][1] = 0;           R[3][2] = 0; R[3][3] = 1;
		for(int i = 0; i<list.size(); i++) list[i].transform(R);

		T[3][0] = pivot.x; T[3][1] = pivot.y; T[3][2] = pivot.z;
		for(int i = 0; i<list.size(); i++) list[i].transform(T);
	}
	void translate(double tx, double ty, double tz)
	{
		matrix R(4, 4);
		R[0][0] = 1;  R[0][1] = 0;  R[0][2] = 0;  R[0][3] = 0;
		R[1][0] = 0;  R[1][1] = 1;  R[1][2] = 0;  R[1][3] = 0;
		R[2][0] = 0;  R[2][1] = 0;  R[2][2] = 1;  R[2][3] = 0;
		R[3][0] = tx; R[3][1] = ty; R[3][2] = tz; R[3][3] = 1;
		for(int i = 0; i<list.size(); i++) list[i].transform(R);

		pivot.x += tx; pivot.y += ty; pivot.z += tz;
	}


	void to_obj(string file_name)
	{
		freopen(file_name.c_str(), "w", stdout);
		int idx = 1;
		for(int i=0; i<list.size(); i++)
		{
			printf("%c %lf %lf %lf\n", 'v', list[i].p.x, list[i].p.y, list[i].p.z); idx++;
			printf("%c %lf %lf %lf\n", 'v', list[i].q.x, list[i].q.y, list[i].q.z); idx++;
			printf("%c %lf %lf %lf\n", 'v', list[i].r.x, list[i].r.y, list[i].r.z); idx++;
			printf("%c %d %d %d\n", 'f', idx-3, idx-2, idx-1);
		}
		printf("x");
	}

	void from_obj(string file_name, double scale = 250)
	{
		ifstream infile(file_name.c_str());
		//freopen(file_name.c_str(), "r", stdin);
		char ch;
		vector<vec> points;
		vector<struct triplet> tmp_list;
		double x,y,z; int a, b, c;

		double max_x=-INF, max_y=-INF, max_z=-INF;
		double min_x=INF, min_y=INF, min_z=INF;
		int itr = 2000;
		while(itr--)
		{
			infile >> ch;
			//cin >> ch;
			if(ch == 'v') // vertex
			{
				infile >> x >> y >> z;
				//cin >> x >> y >> z;
				max_x = max(max_x, x); max_y = max(max_y, y); max_z = max(max_z, z);
				min_x = min(min_x, x); min_y = min(min_y, y); min_z = min(min_z, z);
				vec p(x, y, z);
				points.push_back(p);
			}
			else if(ch == 'f')
			{
				infile >> a >> b >> c;
				//cin >> a >> b >> c;
				struct triplet t; t.i = a-1; t.j = b-1; t.k = c-1;
				tmp_list.push_back(t);
			}
			else break;
		}

		fprintf(stderr, "min_x: %.2lf  max_x:%.2lf\n", min_x, max_x);
		fprintf(stderr, "min_y: %.2lf  max_y:%.2lf\n", min_y, max_y);
		fprintf(stderr, "min_z: %.2lf  max_z:%.2lf\n", min_z, max_z);

		double scaler = max(max_x - min_x, max(max_y - min_y, max_z - min_z));

		//normalize
		for(int i=0; i<points.size(); i++)
		{
			points[i].x = (points[i].x)/(scaler); points[i].x = points[i].x*scale;
			points[i].y = (points[i].y)/(scaler); points[i].y = points[i].y*scale;
			points[i].z = (points[i].z)/(scaler); points[i].z = points[i].z*scale;

			//points[i] = points[i] - vec(0, 120, 0); // ontoto ei dutor y khub besi thakche
			pivot = pivot + points[i];
		}
		pivot = pivot/(double)points.size();


		for(int i = 0; i<tmp_list.size(); i++)
		{
			triangle t(points[tmp_list[i].i], points[tmp_list[i].j], points[tmp_list[i].k]);
			list.push_back(t);
		}
		
		//for(int i=0; i<points.size(); i++)
		//{
		//	points[i].show(); fprintf(stderr, "\n");
		//}
		//for(int i=0; i<list.size(); i++)
		//{
		//	list[i].show();
		//}
	}
};


