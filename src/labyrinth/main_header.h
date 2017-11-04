
#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

/*
Includes
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <GL/glui.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GL/GL.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <strings.h>
#endif

#include <AR/gsub.h>
#include <AR/matrix.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>

#include "glm.h"
#include "RGBpixmap.h"

/* set up the video format globals */

#ifdef _WIN32
char    *vconf = "Data\\WDM_camera_flipV.xml";
#else
char    *vconf = "";
#endif

int             xsize;
int             ysize;
int             thresh = 100;
ARParam         cparam;

int             mouse_ox;
int             mouse_oy;
int             mouse_st = 0;
double          a =   0.0;
double          b = -45.0;
double          r = 500.0;

int             target_id;
int             target_id2;
double          target_center[2] = {0.0, 0.0};
double          target_width = 80.0;

//using namespace std;

/*
Game related structs
*/
typedef struct pair{
	float x;
	float y;
};

typedef struct piece {
	float radius;

	struct pair init;	// initial position of sphere to be randomly calculated
	struct pair prev;	// previous saved position of sphere at a saved state so user can UNDO, starts with same val as initxy
	struct pair pos;	// current position of sphere, changes each time that plane is inclinated in relation to camera
};

typedef struct hole {
	struct pair pos;
};

typedef struct game {

	struct pair dimension;
	
	struct piece sphere;
	struct piece sphere2;

	struct hole goal;
	struct hole goal2;

	float mindist;

	float acx;
	float acy;
	float acxp;
	float acyp;
	float acx2;
	float acy2;

	int obst;

	pair ob[50];
	pair ob2[50];

	int p1;
	int p2;
};

struct game g;

/*
Defines
*/
#define TRUE  = 1;
#define FALSE = 0;

/*
GL
*/
GLUI  *glui;
GLUquadric* glQ;
RGBpixmap pixmap;
float xy_aspect;
//variaveis para criacao de botao de rotacao e de zoom
GLUI_Rotation *view_rot;
//GLUI_Translation * trans_z;
// matriz de transf. geometrica utilizada pelo botao esferico
float view_rotate[16] = { 1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          0,0,0,1 };

float iden_matrix[16] = { 1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          0,0,0,1 };

// vector de posicao utilizado pelo botao de afastamento
//float obj_pos[] = { 0.0, 0.0, 0.0 };

GLMmodel* pmodel1 = NULL;

int modo_jogo=0;
float dimx=150.0;
float dimy=150.0;
float mindis=10.0;
int nob=5;
int curo=5;

GLUI_Button *save_button;
GLUI_Button *undo_button;
GLUI_Button *reset_button;
GLUI_Spinner *spin1;
GLUI_Spinner *spin2;
GLUI_Spinner *spin3;
GLUI_Spinner *spin4;
GLUI_RadioGroup *modo;

//matrizes
double quat[4], pos[3];
double  fixed_trans[3][4];
double  target_trans[3][4];
double  cam_trans[3][4];

//bools
//bool goal = false;
//bool outofbounds = false;
bool glon = true;
bool twop = false;

float anglex=0;
float angley=0;
float anglex2=0;
float angley2=0;
char string[256];

/* function definitions */
static int    init(void);
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mouseEvent(int button, int state, int x, int y);
static void   motionEvent( int x, int y );
static void   mainLoop(void);
static void   getResult( ARMarkerInfo *marker_info , int player);

void callback(int id);
void myGlutReshape(int w, int h);
void myGlutIdle( void );

/*
draw
*/
void drawmodel_box(void);
void setup_light( void );
void draw_camera( double trans[3][4] );
int  draw_object( char *name, double gl_para[16], int xwin, int ywin , int player);
void get_trans( double a, double b, double r, double trans[3][4] );
void draw_axis( void );
void print_string( char *string );
int  draw( char name[], double trans[3][4], int xwin, int ywin , int player);
int  draw_exview( double a, double b, double r, double trans[3][4], int xwin, int ywin );
void normalise(GLdouble *vec);
void newellSquare(GLdouble *vec1,GLdouble *vec2,GLdouble *vec3,GLdouble *vec4,GLdouble *normal);
void drawBox(GLdouble dimx, GLdouble dimy, GLdouble dimz, float s_factor, float t_factor);
void draw_table( int player );
void draw_sphere( int player);
void gluCylinder_closed(GLUquadric * glQ,GLdouble raio_ini,GLdouble raio_final,GLdouble altura,GLint slices,GLint stacks,int cima,int baixo);

/*
game
*/
// init game with table dimensions, shpere radius and minimum distance between sphere and goal
void init_game(float dimx, float dimy, float radius, float mindist, int player, int ob);
bool checkgoal(int player);
bool checkcollision (int player);
bool checkbounds(int player);
int get_angle( double trans[3][4], double *wa, double *wb, double *wc );
int check_incline( double card_trans[3][4], double base_trans[3][4], double *angle );

#endif