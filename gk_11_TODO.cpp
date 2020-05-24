#undef UNICODE
#include <windows.h>
#include <iostream>
#include <vector>
#include <list>
#define GLUT_DISABLE_ATEXIT_HACK

#include <math.h>
#include <ctime>
#include "GL/glut.h"
#include <GL/gl.h>
#include "teksture.h"
#include <math.h>

#define pi 3.14159

using namespace std;

static double x, y;
static double dx = 1;
static double dy = 0.5;
static double cr = 0;

static float yRot = 0.0f;
static float xRot = 0.0f;
static float zRot = 0.0f;


static int wall = 1;

static bool mouse_pressed = false;
static bool mouse_released = false;
struct SParticle
{
	GLfloat lifetime;					//current lifetime of the particle
	GLfloat maximum_lifetime;                       //time of death of particle 
	GLfloat r, g, b;                          // color values of the particle
	GLfloat xpos, ypos, zpos;                 // position of the particle
	GLfloat xspeed, yspeed, zspeed;           // speed of the particle
	GLfloat ax, ay, az;
	GLfloat scale;
	//TODO: add new parameters if needed
};
struct SVertex {
	GLfloat tx, ty;
	GLfloat x, y, z;

};
const float s = 0.5;
SVertex bilboard[] =
{
	{ 0, 0, -s, -s, 0 },
	{ 0, 1, -s, s, 0 },
	{ 1, 1, s, s, 0 },
	{ 1, 0, s, -s, 0 }
};
const int MAX_PARTICLES = 500;
list<SParticle> Particles;
list<SParticle>::iterator particle_iter;
//TODO: texture for particles - change if You want
const char texture_name[] = "media//portal2.bmp";
Ctexture *bilboard_tex;
//Gaussian random number generator - mean 0.0, standard deviation 1.0
float	NRand()
{
	static int q = 15;
	static float c1 = (1 << q) - 1;
	static float c2 = ((int)(c1 / 3)) + 1;
	static float c3 = 1.f / c1;
	/* random number in range 0 - 1 not including 1 */
	float random = 0.f;

	/* the white noise */
	float noise = 0.f;
	random = ((float)(rand()) / (float)(RAND_MAX + 1));
	random = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
	return random;
}
//single bilboard drawing
void bilboarddrawinglist()
{

	glNewList(1, GL_COMPILE);
	glPushMatrix();
	// Wlaczamy tablice koordynatow tekstur
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	// Wlaczamy tablice wspolrzednych wierzcholkow
	glEnableClientState(GL_VERTEX_ARRAY);


	glInterleavedArrays(GL_T2F_V3F, 0, (GLvoid*)bilboard);
	glDrawArrays(GL_QUADS, 0, 4);
	glPopMatrix();
	glEndList();
}

int initTexture(void)
{
	glEnable(GL_TEXTURE_2D);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	// texture for particle

	bilboard_tex = new Ctexture(texture_name);

	//empty texture, currently not needed
	//tekstury[0] = new Ctexture();
	return true;
}
void drawScene(void)
{
	// Clear screen to background color.
	glClear(GL_COLOR_BUFFER_BIT);


	// Set foreground (or drawing) color.
	glColor3f(1.0, 0.0, 0.0);

	if (bilboard_tex)
	{
		//cout << "tekstura" << endl;
		bilboard_tex->Bind();
	}
	list<SParticle>::iterator currP = Particles.begin();
	


	glEnable( GL_BLEND); 
	glBlendFunc( GL_ONE, GL_ONE ); 
	glDisable( GL_DEPTH_TEST);
	//notice what happens if they are not set
	
	//...

	
	for (unsigned int i = 0; i<Particles.size(); i++, currP++)
	{
		glPushMatrix();

		//TODO 1: set particles params like color or size (glColor3f glScalef), position is done for example
		glTranslatef(0.0f,0.0f,-5.0f);
		glRotatef(xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRot, 0.0f, 1.0f, 0.0f);
		glRotatef(zRot, 0.0f, 0.0f, 1.0f);

		glTranslatef(currP->xpos, currP->ypos, currP->zpos);

		glRotatef(-xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(-yRot, 0.0f, 1.0f, 0.0f);
		glRotatef(-zRot, 0.0f, 0.0f, 1.0f);

		glScalef(currP->scale, currP->scale, currP->scale);
		glColor3f(currP->r, currP->g, currP->b);

		//glEnd();


		//Draw particle
		glCallList(1);
		glPopMatrix();
	}



	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	x = 0;
	y = 0;
	// Set background (or clearing) color.
	glClearColor(0.0, 0.0, 0.0, 0.0);
	initTexture();
	bilboarddrawinglist();

}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	// Set viewport size to be entire OpenGL window.
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Set matrix mode to projection.
	glMatrixMode(GL_PROJECTION);

	// Clear current projection matrix to identity.
	glLoadIdentity();

	// Specify the orthographic (or perpendicular) projection, 
	// i.e., define the viewing box.
	gluPerspective(90, (float)w / (float)h, 1, 200);
	//glOrtho(-10.0, 10.0, -10.0, 10.0,-10.0, 10.0);
	//glFrustum(-10.0, 10.0, -10.0, 10.0, 5.0, 100.0); 

	// Set matrix mode to modelview.
	glMatrixMode(GL_MODELVIEW);

	// Clear current modelview matrix to identity.
	glLoadIdentity();
}


void EmitParticles()
{
	//TODO 2:
	//Emit particles:
	//set parameters for new particles (SParticle structure), 
	//You can add new parameters to the SParticle structure
	//For random numbers use NRand() - Gaussian  random number generator - mean 0.0, standard deviation 1.0
	//You should use "for" loop. In each iteration new particle (the variable of type SParticle) should be created.
	//After creation new particle should be pushed front to the list of particles Particles.
	float particlesCount = 200;
	float radius = 2.0f;
	int numberOfCoils;
	static float r = 0.0f;
	static float g = 0.0f;
	static float b = 0.0f; 
	float maxLifeTime;

	if (mouse_pressed)//laduje
	{
		if (particlesCount < 700)
			particlesCount += 0.01f;
		numberOfCoils = 6;
		if (r < 0.4f)
		{
			r += 0.02f;
			g += 0.02f;
			b += 0.02f;
			//cout << " r: " << r <<  "g: " << g  << "b: " << b << endl;
		}

		maxLifeTime = 1.0f;
	}
	else if(mouse_released) //wystrzal
	{
		mouse_released = false;
		 particlesCount = 200;
		 numberOfCoils = 4;
		 r = 70.0f/255.0f;
		 g = 0.0f;
		 b = 70.0f/255.0f;
		 particle_iter = Particles.begin();
		 while (particle_iter != Particles.end())
		 {
			 particle_iter->r = r;
			 particle_iter->g = g;
			 particle_iter->b = b;
			 particle_iter->yspeed = 1.0f + ((NRand() + 1.0f)/10.0f+ 0.1f);
			 particle_iter->xspeed = NRand() * 0.1f;;
			 particle_iter->zspeed = NRand()*0.1f;
			 particle_iter++;
		 }
		 maxLifeTime = 1.0f;
	}
	else//idle
	{
		particlesCount = 200;
		numberOfCoils = 4;
		r = 0.2f;
		g = 0.2f;
		b = 0.2f;
		maxLifeTime = 1.0f;
	}


	for (int t = 0; t < particlesCount; t+=1)
	{
		SParticle particle = { 0 };

		particle.scale = 0.3f;
		
		float randRadius = radius + (NRand() * 0.2f);
		float randAngle = (t / particlesCount) * 2 * pi +  (NRand() / particlesCount*2*pi)/2.0f;

		particle.xpos = randRadius * cos(randAngle * numberOfCoils);
		particle.zpos =  -1.0f * randRadius * sin(randAngle *numberOfCoils);


		float temp = mouse_pressed ? particlesCount * 2 / pi *3/2* sin(t * pi / (2 * particlesCount)) : t;
		particle.ypos = ((float)temp + NRand() * (particlesCount / numberOfCoils)) / ((float)particlesCount / 3.0f) - 2.5f;;

		particle.ay = (NRand() + 1.0f) / 300.0f;
		particle.yspeed = 0.0f;


		particle.r = r;
		particle.g = g;
		particle.b = b;
		particle.maximum_lifetime = maxLifeTime;
		Particles.push_front(particle);
	}
	
	//...
}
void AffectParticles()
{
	list<SParticle> ParticlesToRemove;
	static std::clock_t  oldtime = std::clock();
	std::clock_t newtime = std::clock();
	double timebetweenframes = (double)(newtime - oldtime) / (double)CLOCKS_PER_SEC;
	int frametogrey = 7;
	double greyscale = 0.025;

	particle_iter = Particles.begin();
	while (particle_iter != Particles.end())
	{
		//TODO 3:
		//update particle parameters:
		//position, color (fading color for example), speed(gravity for example)
		
		//...
		particle_iter->yspeed += particle_iter->ay;
		particle_iter->ypos += particle_iter->yspeed;
		
		particle_iter->xpos += particle_iter->xspeed;
		particle_iter->zpos += particle_iter->zspeed;
		
		
		//removing particles
		particle_iter->lifetime += timebetweenframes;
		if (particle_iter->lifetime >= particle_iter->maximum_lifetime)
		{
			//Then You can create them here like in TODO 1
			particle_iter = Particles.erase(particle_iter);
		}
		else
			particle_iter++;

	}
	cout << Particles.size() << endl;

	oldtime = newtime;
}
void animate() {
	EmitParticles();
	AffectParticles();
	glutPostRedisplay();
}

// Keyboard input processing routine.

void keyInput(unsigned char key, int x, int y)
{
	

	switch (key)
	{
	case 'w':
		yRot -= 1.5f;
		break;
	case 's':
		yRot += 1.5f;
		break;
	case 'a':
		xRot -= 1.5f;
		break;
	case 'd':
		xRot += 1.5f;
		break;
	case 'q':
		zRot -= 1.5f;
		break;
	case 'e':
		zRot += 1.5f;
		break;
	default:
		break;
	}
}


void mouseControl(int button, int state, int x, int y)
{
	if (state == GLUT_UP)
	{
		mouse_pressed = false;
		mouse_released = true;
	}
	else if (state == GLUT_DOWN)
	{
		mouse_released = false;
		mouse_pressed = true;
	}

}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv)
{
	// Initialize GLUT.
	glutInit(&argc, argv);

	// Set display mode as single-buffered and RGB color.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// Set OpenGL window size.
	glutInitWindowSize(500, 500);

	// Set position of OpenGL window upper-left corner.
	glutInitWindowPosition(100, 100);

	// Create OpenGL window with title.
	glutCreateWindow("Laboratorium GK: 11_OGL");

	// Initialize.
	setup();

	// Register display routine.
	glutDisplayFunc(drawScene);

	// Register reshape routine.
	glutReshapeFunc(resize);


	// Register the mouse and keyboard callback function. 
	glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);


	glutIdleFunc(animate);
	// Begin processing.
	glutMainLoop();

	if (bilboard_tex)
	{
		delete bilboard_tex;
		cout << "tekstura istniala" << endl;
	}
	return 0;
}
