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
	std::vector<Vec3f> m_v;
	std::vector<Vec3f> m_n; 
	
	std::vector<int> m_vi;
	std::vector<int> m_ni;

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

int sub_width = WIDTH * 0.9;
int top_height = HEIGHT * 0.8;
int bottom_height = HEIGHT - top_height;

float test[3] = {0.0, 0.0, -40.0};

float x_angle, y_angle;
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

float x_car = 0.0; 
float y_car = 0.0;

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
    glutReshapeWindow(width - (2 * GAP), (height * 0.8) - (2 *GAP));
   
	glutSetWindow(bottom);
    glutPositionWindow(GAP,(height * 0.8));
    glutReshapeWindow(width - (2 * GAP), (height * 0.2) - GAP);
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
		break;
	default:
		break;
    }

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
	gluLookAt(0.0, 0.0, 2.10, 
				0.0, 0.0, 0.0,
				0.0, 1.0, 0.0);
	//	gluOrtho2D ((GLdouble) 100-width/2, (GLdouble) 100+width/2, 
//		(GLdouble) 100-height/2, (GLdouble) 100+height/2);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void render_plane(void)
{
	std::cout<< "render_plane" << std::endl; 
	Mesh plane;
	//generate vertices...
	plane.m_v.push_back(Vec3f(8.5, 5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.25));
	plane.m_v.push_back(Vec3f(8.5, 5.0, 0.25));

	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.0));
	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.0));

	plane.m_v.push_back(Vec3f(8.5, 5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.25));
	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.25));

	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.0));
	plane.m_v.push_back(Vec3f(8.5, 5.0, 0.0));

	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.25));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, -5.0, 0.25));

	plane.m_v.push_back(Vec3f(8.5, 5.0, 0.0));
	plane.m_v.push_back(Vec3f(-8.5, 5.0, 0.0));
	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.25));
	plane.m_v.push_back(Vec3f(8.5, -5.0, 0.0));
	
	std::vector<Vec3f> v = plane.m_v;
	// Need to change to vi 
	std::cout << "Scale is: " << scale << std::endl;
	glScalef(scale, scale, scale);
	glRotatef(x_angle, 1.0f, 0.0f, 0.0f);
	glRotatef(y_angle, 0.0f, 1.0f, 0.0f);
//	glTranslatef(0.0, 0.0, -10.0);
	glBegin(GL_QUADS);
	glColor3f(1.0, 0.0, 0.0);
	for(unsigned int i = 0; i < v.size(); i++)
	{
		glVertex3f(v[i].x, v[i].y, v[i].z);

	}
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glTranslatef(x_car, y_car, 0.25f);
	glutWireSphere(0.25f, 10, 10);

	glLoadIdentity();
}

void
top_display(void)
{
	std::cout << "top_display" << std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
	//glTranslatef(test[0], test[1], test[2]);	
	render_plane();
//	glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
//		glColor3f(1.0f,0.0f,0.0f);						// Set Top Point Of Triangle To Red
//		glVertex3f( 0.0f, 1.0f, 0.0f);					// First Point Of The Triangle
//		glColor3f(0.0f,1.0f,0.0f);						// Set Left Point Of Triangle To Green
//		glVertex3f(-1.0f,-1.0f, 0.0f);					// Second Point Of The Triangle
//		glColor3f(0.0f,0.0f,1.0f);						// Set Right Point Of Triangle To Blue
//		glVertex3f( 1.0f,-1.0f, 0.0f);					// Third Point Of The Triangle
//	glEnd();			






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
	//sm = glutCreateMenu(top_menu);
    //glutAddMenuEntry("Options", 0);
    //glutAddMenuEntry("", 0);
    //glutAddMenuEntry("Toggle big vertices", 'B');
    //glutAddMenuEntry("Specify colors@vertices", 'C');
    //glutAddMenuEntry("Toggle drawing outlines", 'O');
    //glutAttachMenu(GLUT_RIGHT_BUTTON);

	glClearColor(0.0, 0.0, 0.0, 0.0);
    bottom = glutCreateSubWindow(window, 1000, 1000, 50, 50);
    glutReshapeFunc(bottom_reshape);
    glutDisplayFunc(bottom_display);
    glutKeyboardFunc(main_keyboard);
	glutSpecialFunc(special_keyboard);

    glutMainLoop();

    return 0;
}
