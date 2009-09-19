#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <IOSTREAM>
#include <GL/glut.h>
#include <ImathVec.h>
#include <vector>

#define debug 1

typedef Imath::Vec3<float> Vec3f;
typedef Imath::Vec2<float> Vec2f;

struct Mesh
{
	// Vertex coordinates
	std::vector<Vec3f> m_v;
	std::vector<Vec3f> m_color;

	std::vector<int> m_vi;
	std::vector<int> m_ci;
};

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

float x_angle, y_angle;
float viewing_angle = 0.0;
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

float x_car = 0.0; 
float y_car = 0.0;

int sm;
int sub_view;



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
		x_car -= 0.1;
		break;
	case GLUT_KEY_RIGHT: 
		x_car += 0.1;
		break;
	case GLUT_KEY_UP:
		y_car += 0.1;
		break;
	case GLUT_KEY_DOWN:
		y_car -= 0.1;
		break;
	default:
		break;
	}
	
	std::cout << "X,Y of car: " << x_car << "," << y_car << std::endl;
	redisplay_all();
}

void
top_reshape(int width, int height)
{
	std::cout << "top_reshape" << std::endl;
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

void render_plane(void)
{
	std::cout<< "render_plane" << std::endl; 

	/**
	Plane (probably should load elsewhere....
	**/
	Mesh plane;

	//load vertices
	plane.m_v.push_back(Vec3f(8.5, 5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.25));
	plane.m_v.push_back(Vec3f(8.5, 5.0, 0.25));
	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.0));
	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.0));
	
	plane.m_color.push_back(Vec3f(1.0, 0.0, 0.0));
	plane.m_color.push_back(Vec3f(0.9, 0.0, 0.0));
	plane.m_color.push_back(Vec3f(0.8, 0.0, 0.0));
	plane.m_color.push_back(Vec3f(0.7, 0.0, 0.0));
	plane.m_color.push_back(Vec3f(0.6, 0.0, 0.0));
	plane.m_color.push_back(Vec3f(0.5, 0.0, 0.0));
	plane.m_color.push_back(Vec3f(0.5, 0.1, 0.0));
	plane.m_color.push_back(Vec3f(0.5, 0.2, 0.0));

	plane.m_vi.push_back(0);
	plane.m_vi.push_back(1);
	plane.m_vi.push_back(2);
	plane.m_vi.push_back(3);
	plane.m_vi.push_back(4);
	plane.m_vi.push_back(5);
	plane.m_vi.push_back(6);
	plane.m_vi.push_back(7);
	plane.m_vi.push_back(3);
	plane.m_vi.push_back(2);
	plane.m_vi.push_back(5);
	plane.m_vi.push_back(4);
	plane.m_vi.push_back(7);
	plane.m_vi.push_back(6);
	plane.m_vi.push_back(1);
	plane.m_vi.push_back(0);
	plane.m_vi.push_back(2);
	plane.m_vi.push_back(2);
	plane.m_vi.push_back(6);
	plane.m_vi.push_back(5);
	plane.m_vi.push_back(0);
	plane.m_vi.push_back(1);
	plane.m_vi.push_back(4);
	plane.m_vi.push_back(7);

	/* End Plane */ 

	std::vector<Vec3f> v = plane.m_v;
	std::vector<int> vi = plane.m_vi;
	std::vector<Vec3f> v_color = plane.m_color;
	// Need to change to vi 
	std::cout << "Scale is: " << scale << std::endl;
//	glScalef(scale, scale, scale);
//	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
//	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
//	glTranslatef(0.0, 0.0, -10.0);
	glBegin(GL_QUADS);
	glEnable(GL_BLEND);
	for(unsigned int i = 0; i < vi.size(); i++)
	{
		glColor3f(v_color[vi[i]].x, v_color[vi[i]].y, v_color[vi[i]].z); 
		glVertex3f(v[vi[i]].x, v[vi[i]].y, v[vi[i]].z);

	}
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glTranslatef(x_car, y_car, 0.25f);
	glutWireSphere(0.25f, 10, 10);

//	glLoadIdentity();
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

void createCylinder(void)
{
	Mesh cyl;

	float i;
	int size = 0;	
	for(i = 0; i <= 6.4; i += 0.2)
	{
		cyl.m_v.push_back(Vec3f(sin(i), cos(i), 0));
		cyl.m_v.push_back(Vec3f(sin(i), cos(i), 2));
		size += 2;
	}
	int j;
	for(j = 0; j < size; j += 2)
	{
		if( (j + 1) < size)
		{
			cyl.m_vi.push_back(j);
			cyl.m_vi.push_back(j+1);
		}
	}


	std::vector<Vec3f> v = cyl.m_v;
	std::vector<int> vi = cyl.m_vi;
	//std::vector<Vec3f> v_color = box.m_color;
	
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.0, 0.0, 1.0);
	for(unsigned int i = 0; i < vi.size(); i++)
	{
		//glColor3f(v_color[vi[i]].x, v_color[vi[i]].y, v_color[vi[i]].z); 
		glVertex3f(v[vi[i]].x, v[vi[i]].y, v[vi[i]].z);

	}
	glEnd();

}

void
top_display(void)
{
	std::cout << "top_display" << std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
	
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);

	glShadeModel(GL_SMOOTH);
	render_plane();

	glTranslatef(0.0, 0.0, 1.0);
	createBox();

	glTranslatef(0.0, 0.0, 1.0);
	createCylinder();

	glutSwapBuffers();
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

void 
bottom_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 0.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINES);
		glVertex3f(20, 20, 0);
		glVertex3f(50, 50, 0);
	glEnd();

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

	if(mouse_button == GLUT_RIGHT_BUTTON)
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

void setupMenus(void)
{
	
	sub_view = glutCreateMenu(changeView);
	glutAddMenuEntry("Top Down", 0);
	glutAddMenuEntry("45 Degree Down", 1);

	// Other sub menus


	sm = glutCreateMenu(top_menu);
	glutAddSubMenu("View", sub_view);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int
main(int argc, char** argv)
{
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
