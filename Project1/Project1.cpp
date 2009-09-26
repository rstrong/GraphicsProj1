#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <IOSTREAM>
#include <GL/glut.h>
#include <ImathVec.h>
#include <vector>
#include <fstream>
#include <time.h>

#define debug 1
#define PI 3.14159265
#define ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
	a[k][l]=h+s*(g-h*tau);

void start_and_finish(void);
void car(void);
void move_car(void);
void jacobi(float**,int,float, float**,int*);
typedef Imath::Vec3<float> Vec3f;
typedef Imath::Vec2<float> Vec2f;

struct Mesh
{
	// Vertex coordinates
	std::vector<Vec3f> m_v;
	std::vector<Vec3f> m_color;

	std::vector<int> m_vi;
	std::vector<int> m_ci;

	// Axis Oriented BB
	float aa_xmax;
	float aa_xmin;
	float aa_ymax;
	float aa_ymin;
	float aa_zmax;
	float aa_zmin;

	// Object Oriented BB
	float oo_xmax;
	float oo_xmin;
	float oo_ymax;
	float oo_ymin;
	float oo_zmin;
	float oo_zmax;
};

Mesh* load(const char*);
void setAABB(Mesh);


int HEIGHT = 1024;
int WIDTH = 768;


// Control stuff
int mouse_button;
int mouse_x = 0;
int mouse_y = 0;
float scale = 0.16;
void top_menu(int);
void redisplay_all();

int window, top, bottom;

int bottom_width, bottom_height, top_width, top_height;

// Options
float x_angle, y_angle;
float viewing_angle = 0.0;
int dispBox = 0;



GLvoid *font_style = GLUT_BITMAP_HELVETICA_12;

Vec3f steer;
float car_speed = 0.0;
int car_angle = -90;
float x_car = -7.0; 
float y_car = 3.2;

int sm;
int sub_view;

Mesh elephant;
Mesh vehicle;
Mesh plane;
Mesh cylinder;
Mesh cube;

// timer
time_t start;
time_t end;
bool hasStarted = false;
bool isFinished = false;

void
main_reshape(int width,  int height) 
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

#define GAP  15				/* gap between subwindows */

    glutSetWindow(top);
    glutPositionWindow(GAP, GAP);
	top_width = width - (2 * GAP);
	top_height = (height * 0.8) - (2 * GAP);
    glutReshapeWindow(top_width, top_height);
   
	glutSetWindow(bottom);
	bottom_width = width - (2 * GAP);
	bottom_height = (height * 0.2) - GAP;
    glutPositionWindow(GAP,(height * 0.8));
    glutReshapeWindow(bottom_width, bottom_height);
}

void
main_display(void)
{
	std::cout << "main_display" << std::endl;
    glClearColor(0.8f, 0.8f, 0.8f, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    //glColor3ub(1, 0, 0);
    glutSwapBuffers();
}

void
main_keyboard(unsigned char key, int x, int y)
{
	std::cout << "See Key: " << key << std::endl;
    switch (key) {
	case 'B':
		viewing_angle += 0.01;
		break;
	default:
		break;
    }

	std::cout << "Viewing Angle: " << viewing_angle << std::endl;
	redisplay_all();
}

void special_keyboard(int key, int x, int y)
{
	std::cout << "Special Key" << std::endl;
	switch(key)
	{
	case GLUT_KEY_LEFT:
		car_angle += 4;
		break;
	case GLUT_KEY_RIGHT: 
		car_angle -= 4;
		break;
	case GLUT_KEY_UP:
		car_speed += 0.01;
		if(!hasStarted)
		{
			hasStarted = true;
			start = time(NULL);
		}
		break;
	case GLUT_KEY_DOWN:
		car_speed -= 0.01;
		break;
	default:
		break;
	}
	
	steer.setValue((float)(car_speed * cos(car_angle*PI/180)), (float)(car_speed * sin(car_angle*PI/180)), 0.0f);
	if((x_car > 6.3) && (y_car < -3.3))
	{
		isFinished = true;
		end = time(NULL);
	}
	else
	{
		move_car();
	}
	std::cout << "X,Y of car: " << x_car << "," << y_car << std::endl;
	std::cout << "Car angle: " << car_angle << std::endl;
	redisplay_all();
}

void move_car(void)
{
	x_car += steer.x;
	y_car += steer.y;
}

void
top_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 10.0f);
	gluLookAt(0.0, viewing_angle, 2.10, 
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void generatePlane(void)
{
	int i,j;
	int x = 1;
	int y =1;
	
	int total = 0;
	int n = 0;
	for(i = -5; i < 6; i++)
	{
		n = 0;
		for(j = -9; j < 10; j++)
		{
			n++;
			plane.m_v.push_back(Vec3f(j * x, y * i, 0));
			total++;
		}
	}
	for(j = 0; j < 10; j++)
	{
		for(i = 0; i < n -1 ; i++)
		{
			// first triangle
			plane.m_vi.push_back(i + (j* n) + 1);
			plane.m_vi.push_back(i+1 + (j*n) + 1);
			plane.m_vi.push_back(i+n+0 + (j*n) +1);
			
			//second triangle
			plane.m_vi.push_back(i+1 + (j*n) +1);
			plane.m_vi.push_back(i+n+1 + (j*n) +1);
			plane.m_vi.push_back(i+n+0 + (j*n) +1);
		}
	}
}


void generateCylinder(void)
{
	int i;
	for(i = 0; i < 362; i++)
	{
		cylinder.m_v.push_back(Vec3f(sin(i*PI/180), cos(i*PI/180), 0));
		cylinder.m_v.push_back(Vec3f(sin(i*PI/180), cos(i*PI/180), 2));
	}
	cylinder.m_v.push_back(Vec3f(0, 0, 0));
	cylinder.m_v.push_back(Vec3f(0, 0, 2));

	for(i = 0; i < (cylinder.m_v.size() - 2) -2; i+= 2)
	{
		// side of cylinder, triangle one
		cylinder.m_vi.push_back(i +1);
		cylinder.m_vi.push_back(i+2 +1);
		cylinder.m_vi.push_back(i+1 +1);

		// side of cylinder triangle two
		cylinder.m_vi.push_back(i+2 +1);
		cylinder.m_vi.push_back(i+3 +1);
		cylinder.m_vi.push_back(i+1 +1);

		// top of cylinder
		cylinder.m_vi.push_back(i+1 +1);
		cylinder.m_vi.push_back(i+3 +1);
		cylinder.m_vi.push_back(cylinder.m_v.size());

		//bottom of cylinder
		cylinder.m_vi.push_back(i +1);
		cylinder.m_vi.push_back(i + 2 +1);
		cylinder.m_vi.push_back(cylinder.m_v.size() -1);
	}
}


void generateCube(void)
{
	// First face
	float i;
	for(i = -1; i < 1.1; i += 0.1)
	{
		cube.m_v.push_back(Vec3f(i, 1.0, -1.0));
		cube.m_v.push_back(Vec3f(i, 1.0, 1.0));
	}
	std::cout << "f1 done" << std::endl;
	// second
	for(i = 0.9; i > -1.1; i -= 0.1)
	{
		cube.m_v.push_back(Vec3f(1.0, i, -1.0));
		cube.m_v.push_back(Vec3f(1.0, i, 1.0));
	}

	// third
	for(i = 0.9; i > -1.1; i -= 0.1)
	{
		cube.m_v.push_back(Vec3f(i, -1.0, -1.0));
		cube.m_v.push_back(Vec3f(i, -1.0, 1.0));
	}

	// four
	for(i = -0.9; i < 1.1; i += 0.1)
	{
		cube.m_v.push_back(Vec3f(-1.0, i, -1.0));
		cube.m_v.push_back(Vec3f(-1.0, i, 1.0));
	}

	cube.m_v.push_back(Vec3f(0.0, 0.0, -1.0));
	cube.m_v.push_back(Vec3f(0.0, 0.0, 1.0));
	int j;
	for(j = 0; j < (cube.m_v.size() - 2) -2; j+= 2)
	{
		cube.m_vi.push_back(j +1);
		cube.m_vi.push_back(j+2 +1);
		cube.m_vi.push_back(j+1 +1);

		cube.m_vi.push_back(j+2 +1);
		cube.m_vi.push_back(j+3 +1);
		cube.m_vi.push_back(j+1 +1);

		cube.m_vi.push_back(j+1 +1);
		cube.m_vi.push_back(j+3 +1);
		cube.m_vi.push_back(cube.m_v.size());

		cube.m_vi.push_back(j +1);
		cube.m_vi.push_back(j + 2 +1);
		cube.m_vi.push_back(cube.m_v.size() -1);
	}

}

void jacobi(float **a, int n, float d[], float **v, int *nrot)
{
	int j,iq,ip,i;
	float tresh,theta,tau,t,sm,s,h,g,c,*b,*z;
	b = new float[n+1];
	z = new float[n+1];
	for (ip=1;ip<=n;ip++) {
		for (iq=1;iq<=n;iq++) v[ip][iq]=0.0;
		v[ip][ip]=1.0;
	}
	for (ip=1;ip<=n;ip++) {
		b[ip]=d[ip]=a[ip][ip];
		z[ip]=0.0;
	}
	*nrot=0;
	for (i=1;i<=50;i++) {
		sm=0.0;
		for (ip=1;ip<=n-1;ip++) {
			for (iq=ip+1;iq<=n;iq++)
				sm += fabs(a[ip][iq]);
		}
		if (sm == 0.0) { 
			delete z;
			delete b;
			return;
		}
		if (i < 4)
			tresh=0.2*sm/(n*n);
		else
			tresh=0.0; 
		for (ip=1;ip<=n-1;ip++) {
			for (iq=ip+1;iq<=n;iq++) {
				g=100.0*fabs(a[ip][iq]);
				if (i > 4 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip])
					&& (float)(fabs(d[iq])+g) == (float)fabs(d[iq]))
					a[ip][iq]=0.0;
				else if (fabs(a[ip][iq]) > tresh) {
					h=d[iq]-d[ip];
					if ((float)(fabs(h)+g) == (float)fabs(h))
						t=(a[ip][iq])/h; 
					else {
						theta=0.5*h/(a[ip][iq]); 
						t=1.0/(fabs(theta)+sqrt(1.0+theta*theta));
						if (theta < 0.0) t = -t;
					}
					c=1.0/sqrt(1+t*t);
					s=t*c;
					tau=s/(1.0+c);
					h=t*a[ip][iq];
					z[ip] -= h;
					z[iq] += h;
					d[ip] -= h;
					d[iq] += h;
					a[ip][iq]=0.0;
					for (j=1;j<=ip-1;j++) {
						ROTATE(a,j,ip,j,iq)
					}
					for (j=ip+1;j<=iq-1;j++) { 
						ROTATE(a,ip,j,j,iq)
					}
					for (j=iq+1;j<=n;j++) { 
						ROTATE(a,ip,j,iq,j)
					}
					for (j=1;j<=n;j++) {
						ROTATE(v,j,ip,j,iq)
					}
					++(*nrot);
				}
			}
		}
		for (ip=1;ip<=n;ip++) {
			b[ip] += z[ip];
			d[ip]=b[ip]; 
			z[ip]=0.0;
		}
	}
	std::cout << "Too many iterations in routine jacobi" << std::endl;
	exit(1);
}


void setupOOBB(Mesh *t)
{
	int i = 0;
	t->oo_xmin = 1000000;
	t->oo_xmax = -10000000;
	t->oo_ymin = 1000000;
	t->oo_ymax = -10000000;
	t->oo_zmin = 1000000;
	t->oo_zmax = -1000000;
	
	// Average vertex
	Vec3f avg;
	avg.x = 0;
	avg.y = 0;
	avg.z = 0;

	// covariance matrix
	float **M = new float* [4];
	float lambda[4];
	float **v = new float* [4];
	int nrot = 0;
	M[0] = new float[4];
	M[1] = new float[4];
	M[2] = new float[4];
	M[3] = new float[4];
	v[0] = new float[4];
	v[1] = new float[4];
	v[2] = new float[4];
	v[3] = new float[4];
	for(i = 0; i < 4; i++)
	{
		lambda[i] = 0.0;
		for(int j = 0; j < 4; j++)
		{
			M[i][j] = 0.0;
			v[i][j] = 0.0;
		}
	}

	// Get the average vertex
	for(i = 0; i < t->m_v.size(); i++)
	{
		avg.x += t->m_v[i].x;
		avg.y += t->m_v[i].y;
		avg.z += t->m_v[i].z;
	}
	avg.x = avg.x / t->m_v.size();
	avg.y = avg.y / t->m_v.size();
	avg.z = avg.z / t->m_v.size();

	// 1 = x; 2 = y; 3 = z;
	// Calculate covariance matrix
	float val = 0.0;
	//(x,x)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].x - avg.x) * (t->m_v[i].x - avg.x));
	}
	val = val / t->m_v.size();
	M[1][1] = val;
	val = 0.0;

	//(x,y)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].x - avg.x) * (t->m_v[i].y - avg.y));
	}
	val = val / t->m_v.size();
	M[1][2] = val;
	val = 0.0;

	//(x,z)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].x - avg.x) * (t->m_v[i].z - avg.z));
	}
	val = val / t->m_v.size();
	M[1][2] = val;
	val = 0.0;

	// (y,x)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].y - avg.y) * (t->m_v[i].x - avg.x));
	}
	val = val / t->m_v.size();
	M[2][1] = val;
	val = 0.0;

	// (y,y)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].y - avg.y) * (t->m_v[i].y - avg.y));
	}
	val = val / t->m_v.size();
	M[2][2] = val;
	val = 0.0;

	// (y,z)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].y - avg.y) * (t->m_v[i].y - avg.y));
	}
	val = val / t->m_v.size();
	M[2][3] = val;
	val = 0.0;

	// (z,x)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].z - avg.z) * (t->m_v[i].x - avg.x));
	}
	val = val / t->m_v.size();
	M[3][1] = val;
	val = 0.0;

	// (z,y)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].z - avg.z) * (t->m_v[i].y - avg.y));
	}
	val = val / t->m_v.size();
	M[3][2] = val;
	val = 0.0;

	// (z,z)
	for(i = 0; i < t->m_v.size(); i++)
	{
		val += ( (t->m_v[i].z - avg.z) * (t->m_v[i].z - avg.z));
	}
	val = val / t->m_v.size();
	M[3][3] = val;
	val = 0.0;

	// Pass to jacobi
	jacobi(M, 3, lambda, v, & nrot);

	// get our eigen vectors
	std::vector<Vec3f> eigen;
	for(i = 1; i < 4; i++)
	{
		eigen.push_back(Vec3f(v[1][i],v[2][i],v[3][i]));
	}
	
	for(i = 0; i < t->m_v.size(); i++)
	{
		for(int j = 0; j < eigen.size(); j++)
		{
			if(eigen[j].dot(t->m_v[i]) < t->oo_xmin)
			{
				t->oo_xmin = eigen[j].dot(t->m_v[i]);
			}
			if(eigen[j].dot(t->m_v[i]) < t->oo_ymin)
			{
				t->oo_ymin = eigen[j].dot(t->m_v[i]);
			}
			if(eigen[j].dot(t->m_v[i]) < t->oo_zmin)
			{
				t->oo_zmin = eigen[j].dot(t->m_v[i]);
			}
			if(eigen[j].dot(t->m_v[i]) > t->oo_xmax)
			{
				t->oo_xmax = eigen[j].dot(t->m_v[i]);
			}
			if(eigen[j].dot(t->m_v[i]) > t->oo_ymax)
			{
				t->oo_ymax = eigen[j].dot(t->m_v[i]);
			}
			if(eigen[j].dot(t->m_v[i]) > t->oo_zmax)
			{
				t->oo_zmax = eigen[j].dot(t->m_v[i]);
			}
		}
	}


}

void setupAABB(Mesh *t)
{
	int i = 0;
    t->aa_xmax = -1000000;
	t->aa_xmin = 1000000;
	t->aa_ymax = -1000000;
	t->aa_ymin = 10000000;
	t->aa_zmax = -1000000;
	t->aa_zmin = 10000000;
	for(i = 0; i < t->m_v.size(); i++)
	{
		if(t->m_v[i].x > t->aa_xmax)
		{
			t->aa_xmax = t->m_v[i].x;
		}
		if(t->m_v[i].x < t->aa_xmin)
		{
			t->aa_xmin = t->m_v[i].x;
		}
		if(t->m_v[i].y > t->aa_ymax)
		{
			t->aa_ymax = t->m_v[i].y;
		}
		if(t->m_v[i].y < t->aa_ymin)
		{
			t->aa_ymin = t->m_v[i].y;
		}
		if(t->m_v[i].z > t->aa_zmax)
		{
			t->aa_zmax = t->m_v[i].z;
		}
		if(t->m_v[i].z < t->aa_zmin)
		{
			t->aa_zmin = t->m_v[i].z;
		}
	}
}

void load_models(void)
{
	Mesh* tmp;

	std::cout << "Loading elephant..." << std::endl;
	tmp = load("data/elephav.obj");
	elephant = *tmp;

	setupAABB(&elephant);
	setupOOBB(&elephant);

	std::cout << "loading car..." << std::endl;
	tmp = load("data/dirtbug.obj");
	vehicle = *tmp;
	setupAABB(&vehicle);
	setupOOBB(&vehicle);

	std::cout << "generating plane..." << std::endl;
	generatePlane();
	setupAABB(&plane);
	setupOOBB(&plane);

	std::cout << "generating cylinder..." << std::endl;
	generateCylinder();
	setupAABB(&cylinder);
	setupOOBB(&cylinder);

	std::cout << "generating cube..." << std::endl;
	generateCube();
	setupAABB(&cube);
	setupOOBB(&cylinder);
}

int strToInt(const char* c)
{
	int i;
	if(sscanf(c, "%i", &i) == 1)
	{
		return i;
	}
	else
	{
		return 0;
	}
}

Mesh* load(const char* file)
{
	std::ifstream infile(file);
	if(infile.fail())
	{
		std::cout << "Error Opening File: " << file << std::endl;
		return false;
	}

	Mesh* model = new Mesh;
	char current_line[1024];

	while(!infile.eof())
	{
		infile.getline(current_line, 1024);

		switch (current_line[0])
		{
		case 'v':
			{
				float x,y,z;
				switch(current_line[1])
				{
				case 'n':
					break;
				case 't':
					break;
				default:
					sscanf(current_line, "v %f %f %f", &x, &y, &z);
					//std::cout << "Loaded X,Y,Z: " << x << "," << y << "," << z << std::endl;
					model->m_v.push_back(Vec3f(x,y,z));
					break;
				}
			}
			break;

		case 'f':
			{
				char* match;
				match = strtok (current_line, " ");
				while(match != NULL)
				{
					std::string whole = match;
					if(match[0] != 'f')
					{
						int i = 0;
						int start = 0;
						int seen = 0;
						int data;
						while(match[i] != NULL)
						{
							if(match[i] == '/')
							{
								data = strToInt(whole.substr(start,i).c_str());
								//std::cout << "Match: " << data << std::endl;
								start = i + 1;
								if(seen == 0)
								{
									model->m_vi.push_back(data);
									seen++;
								}
								else
								{
									// Texture
								}
									
							}
							if(match[i + 1] == NULL)
							{
								// Color
								data = strToInt(whole.substr(start,i+1).c_str()); 
								//std::cout << "Match: " << data << std::endl;
							}
							i++;
						}
						//std::cout << "Found Match: " << match << std::endl;
					}
					match = strtok(NULL, " ");
				}
				break;
			}
		default: break;
		}
	}

	infile.close();

	return model;


}

void createBox(void)
{
	Mesh box;

	box.m_v.push_back(Vec3f(1.0, 1.0, -1.0));
	box.m_v.push_back(Vec3f(-1.0, 1.0, -1.0));
	box.m_v.push_back(Vec3f(-1.0, 1.0, 1.0));
	box.m_v.push_back(Vec3f(1.0, 1.0, 1.0));
	box.m_v.push_back(Vec3f(1.0, -1.0, 1.0));
	box.m_v.push_back(Vec3f(-1.0, -1.0, 1.0));
	box.m_v.push_back(Vec3f(-1.0, -1.0, -1.0));
	box.m_v.push_back(Vec3f(1.0, -1.0, -1.0));

	box.m_color.push_back(Vec3f(0.0, 1.0, 0.0));
	box.m_color.push_back(Vec3f(0.0, 1.0, 0.2));
	box.m_color.push_back(Vec3f(0.0, 1.0, 0.4));
	box.m_color.push_back(Vec3f(0.0, 1.0, 0.6));
	box.m_color.push_back(Vec3f(0.2, 1.0, 0.0));
	box.m_color.push_back(Vec3f(0.4, 1.0, 0.0));
	box.m_color.push_back(Vec3f(0.6, 1.0, 0.0));
	box.m_color.push_back(Vec3f(0.2, 1.0, 0.2));

	box.m_vi.push_back(0);
	box.m_vi.push_back(1);
	box.m_vi.push_back(2);
	box.m_vi.push_back(3);
	box.m_vi.push_back(4);
	box.m_vi.push_back(5);
	box.m_vi.push_back(6);
	box.m_vi.push_back(7);
	box.m_vi.push_back(3);
	box.m_vi.push_back(2);
	box.m_vi.push_back(5);
	box.m_vi.push_back(4);
	box.m_vi.push_back(7);
	box.m_vi.push_back(6);
	box.m_vi.push_back(1);
	box.m_vi.push_back(0);
	box.m_vi.push_back(2);
	box.m_vi.push_back(1);
	box.m_vi.push_back(6);
	box.m_vi.push_back(5);
	box.m_vi.push_back(0);
	box.m_vi.push_back(3);
	box.m_vi.push_back(4);
	box.m_vi.push_back(7);


	std::vector<Vec3f> v = box.m_v;
	std::vector<int> vi = box.m_vi;
	std::vector<Vec3f> v_color = box.m_color;
	
	glBegin(GL_QUADS);
	for(unsigned int i = 0; i < vi.size(); i++)
	{
		glColor3f(v_color[vi[i]].x, v_color[vi[i]].y, v_color[vi[i]].z); 
		glVertex3f(v[vi[i]].x, v[vi[i]].y, v[vi[i]].z);

	}
	glEnd();

}

void renderObject(Mesh obj)
{
	std::vector<Vec3f> v = obj.m_v;
	std::vector<int> vi = obj.m_vi;
	std::vector<Vec3f> v_color = obj.m_color;
	
	glBegin(GL_TRIANGLES);
	for(unsigned int i = 0; i < vi.size(); i++)
	{
		//std::cout << "1079 test: " << vi[1079] << std::endl;
		glVertex3f(v[vi[i] - 1].x, v[vi[i] - 1].y, v[vi[i] -1].z);
	}
	glEnd();

	if(dispBox == 1)
	{
		glBegin(GL_LINES);
		glLineWidth(3.0);
		glColor3f(0.0, 0.0, 0.0);

		// Bottom Square
		glVertex3f(obj.aa_xmin, obj.aa_ymax, obj.aa_zmin);
		glVertex3f(obj.aa_xmax, obj.aa_ymax, obj.aa_zmin);
		glVertex3f(obj.aa_xmax, obj.aa_ymax, obj.aa_zmin);
		glVertex3f(obj.aa_xmax, obj.aa_ymin, obj.aa_zmin);
		glVertex3f(obj.aa_xmax, obj.aa_ymin, obj.aa_zmin);
		glVertex3f(obj.aa_xmin, obj.aa_ymin, obj.aa_zmin);
		glVertex3f(obj.aa_xmin, obj.aa_ymin, obj.aa_zmin);
		glVertex3f(obj.aa_xmin, obj.aa_ymax, obj.aa_zmin);

		// top square
		glVertex3f(obj.aa_xmin, obj.aa_ymax, obj.aa_zmax);
		glVertex3f(obj.aa_xmax, obj.aa_ymax, obj.aa_zmax);
		glVertex3f(obj.aa_xmax, obj.aa_ymax, obj.aa_zmax);
		glVertex3f(obj.aa_xmax, obj.aa_ymin, obj.aa_zmax);
		glVertex3f(obj.aa_xmax, obj.aa_ymin, obj.aa_zmax);
		glVertex3f(obj.aa_xmin, obj.aa_ymin, obj.aa_zmax);
		glVertex3f(obj.aa_xmin, obj.aa_ymin, obj.aa_zmax);
		glVertex3f(obj.aa_xmin, obj.aa_ymax, obj.aa_zmax);


		// lines
		glVertex3f(obj.aa_xmin, obj.aa_ymax, obj.aa_zmin);
		glVertex3f(obj.aa_xmin, obj.aa_ymax, obj.aa_zmax);
		glVertex3f(obj.aa_xmin, obj.aa_ymin, obj.aa_zmin);
		glVertex3f(obj.aa_xmin, obj.aa_ymin, obj.aa_zmax);
		glVertex3f(obj.aa_xmax, obj.aa_ymin, obj.aa_zmin);
		glVertex3f(obj.aa_xmax, obj.aa_ymin, obj.aa_zmax);
		glVertex3f(obj.aa_xmax, obj.aa_ymax, obj.aa_zmin);
		glVertex3f(obj.aa_xmax, obj.aa_ymax, obj.aa_zmax);

		glEnd();
	}
	if(dispBox == 2)
	{
		glBegin(GL_LINES);
		glLineWidth(3.0);
		glColor3f(0.0, 0.0, 0.0);

		// Bottom Square
		glVertex3f(obj.oo_xmin, obj.oo_ymax, obj.oo_zmin);
		glVertex3f(obj.oo_xmax, obj.oo_ymax, obj.oo_zmin);
		glVertex3f(obj.oo_xmax, obj.oo_ymax, obj.oo_zmin);
		glVertex3f(obj.oo_xmax, obj.oo_ymin, obj.oo_zmin);
		glVertex3f(obj.oo_xmax, obj.oo_ymin, obj.oo_zmin);
		glVertex3f(obj.oo_xmin, obj.oo_ymin, obj.oo_zmin);
		glVertex3f(obj.oo_xmin, obj.oo_ymin, obj.oo_zmin);
		glVertex3f(obj.oo_xmin, obj.oo_ymax, obj.oo_zmin);

		// top square
		glVertex3f(obj.oo_xmin, obj.oo_ymax, obj.oo_zmax);
		glVertex3f(obj.oo_xmax, obj.oo_ymax, obj.oo_zmax);
		glVertex3f(obj.oo_xmax, obj.oo_ymax, obj.oo_zmax);
		glVertex3f(obj.oo_xmax, obj.oo_ymin, obj.oo_zmax);
		glVertex3f(obj.oo_xmax, obj.oo_ymin, obj.oo_zmax);
		glVertex3f(obj.oo_xmin, obj.oo_ymin, obj.oo_zmax);
		glVertex3f(obj.oo_xmin, obj.oo_ymin, obj.oo_zmax);
		glVertex3f(obj.oo_xmin, obj.oo_ymax, obj.oo_zmax);


		// lines
		glVertex3f(obj.oo_xmin, obj.oo_ymax, obj.oo_zmin);
		glVertex3f(obj.oo_xmin, obj.oo_ymax, obj.oo_zmax);
		glVertex3f(obj.oo_xmin, obj.oo_ymin, obj.oo_zmin);
		glVertex3f(obj.oo_xmin, obj.oo_ymin, obj.oo_zmax);
		glVertex3f(obj.oo_xmax, obj.oo_ymin, obj.oo_zmin);
		glVertex3f(obj.oo_xmax, obj.oo_ymin, obj.oo_zmax);
		glVertex3f(obj.oo_xmax, obj.oo_ymax, obj.oo_zmin);
		glVertex3f(obj.oo_xmax, obj.oo_ymax, obj.oo_zmax);

		glEnd();
	}	
}

void
top_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
	
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);

	glShadeModel(GL_SMOOTH);

//	render_plane();
	glColor3f(0.5, 0.5, 0.0);
	renderObject(plane);

	glColor3f(0.3, 1.0, 0.0);
	renderObject(cube);
	start_and_finish();

	car();

	glPushMatrix();
	
	glTranslatef(7.0, 3.6, 0.0);
	glScalef(0.002, 0.002, 0.002);
	
	glRotated(90, 1.0, 0.0, 0.0);
	
	glColor3f(0.3, 1.0, 0.0);
	renderObject(elephant);
	glPopMatrix();
	
	glutSwapBuffers();
}

void car(void)
{
	move_car();
	glPushMatrix();
	glTranslatef(x_car, y_car, 1.0);
	glScalef(0.004, 0.004, 0.004);
	glColor3f(1.0, 0.23, 0.43);
	
	glRotated(car_angle + 90, 0.0, 0.0, 1.0);
	glRotated(90, 1.0, 0.0, 0.0);
	renderObject(vehicle);
	glPopMatrix();
}

void start_and_finish (void)
{
	// Create Start and Finish boxes

	//Start
	glPushMatrix();
	glScalef(1, 1, 0.01);
	glTranslatef(-7.2, 3.8, 1.0);
	createBox();
	glPopMatrix();

	//end
	glPushMatrix();
	glScalef(1, 1, 0.01);
	glTranslatef(7.2, -3.8, 1.0);
	createBox();
	glPopMatrix();
}

void
bottom_reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void drawString(float x, float y, float z, const char *string)
{
	glRasterPos3f(x, y, z);
	for (const char* c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font_style, *c);
	}
}

inline std::string stringify(int x)
{
	std::ostringstream o;
	o << x;
	return o.str();
}

void 
bottom_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	std::string timer;
	std::string finish = "FINISHED!!!!";
	time_t now = time(NULL);
	int diff = now - start;
	if(hasStarted)
	{
		if(isFinished)
		{
			diff = end - start;
		}
		else
		{
			diff = now - start;
		}
	}
	else
	{
		diff = 0;
	}
	
	timer = "Time Elapsed: " + stringify(diff) + " sec";
	drawString(15, 15, 1, timer.c_str());

	if(isFinished)
	{
		drawString(150, 15, 1, finish.c_str());
	}

    glutSwapBuffers();
}

void
top_menu(int value)
{
    main_keyboard((unsigned char)value, 0, 0);
}


void
bottom_menu(int value)
{
    main_keyboard((unsigned char)value, 0, 0);
}

void redisplay_all()
{
	glutSetWindow(top);
	glutPostRedisplay();
	glutSetWindow(bottom);
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	std::cout << "MouseX: " << mouse_x << std::endl;
	mouse_x = x;
	mouse_y = y;
	mouse_button = button;
}

void motion(int x, int y)
{
	if(mouse_button == GLUT_LEFT_BUTTON)
	{
		y_angle += (float(x - mouse_x)/WIDTH)*360.0;
		x_angle += (float(y - mouse_y)/HEIGHT)*360.0;
	}

	if(mouse_button == GLUT_MIDDLE_BUTTON)
	{
		scale += (y-mouse_y)/100.0;
	}
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();
}

void changeView(int option)
{
	// change camera here
	if(option == 0)
	{
		viewing_angle = 0.0;
	}
	if(option == 1)
	{
		viewing_angle = -1.4;	
	}
	top_reshape(top_width, top_height);
	redisplay_all();
}

void boundingBox(int option)
{
	if(option == 0)
	{
		dispBox = 0;
	}
	if(option == 1)
	{
		dispBox = 1;
	}
	if(option == 2)
	{
		dispBox = 2;
	}
	std::cout << "Bounding box now set: " << dispBox << std::endl;
	top_reshape(top_width, top_height);
	redisplay_all();
}

void setupMenus(void)
{
	
	sub_view = glutCreateMenu(changeView);
	glutAddMenuEntry("Top Down", 0);
	glutAddMenuEntry("45 Degree Down", 1);
	int bb = glutCreateMenu(boundingBox);
	glutAddMenuEntry("No Bounding Box", 0);
	glutAddMenuEntry("AA Bounding Box", 1);
	glutAddMenuEntry("O Bounding Box", 2);
	// Other sub menus


	sm = glutCreateMenu(top_menu);
	glutAddSubMenu("View", sub_view);
	glutAddSubMenu("Bounding Box", bb);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int
main(int argc, char** argv)
{
	// Set up stuff
	steer.setValue(0.0, 0.0,0.0);

	// Load models
	load_models();

    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(HEIGHT, WIDTH);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);

    window = glutCreateWindow("Project 1");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
    glutKeyboardFunc(main_keyboard);
	glutSpecialFunc(special_keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

    top = glutCreateSubWindow(window, 1, 1, 10, 10);
    glutReshapeFunc(top_reshape);
    glutDisplayFunc(top_display);
	glutIdleFunc(redisplay_all);
    glutKeyboardFunc(main_keyboard);
    glutSpecialFunc(special_keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	setupMenus();
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
    bottom = glutCreateSubWindow(window, 1000, 1000, 50, 50);
    glutReshapeFunc(bottom_reshape);
    glutDisplayFunc(bottom_display);
    glutKeyboardFunc(main_keyboard);
	glutSpecialFunc(special_keyboard);

    glutMainLoop();

    return 0;
}
