
#include "main_header.h"

/*
draw.c
*/

//funcao para criar cilindro especificando quantas e quais faces estarao fechadas
void gluCylinder_closed(GLUquadric * glQ,GLdouble raio_ini,GLdouble raio_final,GLdouble altura,GLint slices,GLint stacks,bool cima,bool baixo)
{
    //criar cilindro
    gluCylinder(glQ, raio_ini,raio_final,altura,slices,stacks);
    //fechar cilindro
    if(cima){
        glPushMatrix();
            glTranslatef(0,0,altura);
            gluDisk(glQ, 0, raio_final, slices, 4);
        glPopMatrix();
    }
    if(baixo){
        glPushMatrix();
            gluQuadricOrientation(glQ, GLU_INSIDE);
            glTranslatef(0,0,0);
            gluDisk(glQ, 0, raio_final, slices, 4);
        glPopMatrix();
    }
}

void drawmodel_box(void)
{
	// Load the model only if it hasn't been loaded before
	// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
    if (!pmodel1) 
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel1 = glmReadOBJ("golf.obj");	
        if (!pmodel1) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(pmodel1);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        glmFacetNormals(pmodel1);        
		glmVertexNormals(pmodel1, 90.0);
    }
    // This is the call that will actualy draw the model
	// Don't forget to tell it if you want textures or not :))
    glmDraw(pmodel1, GLM_SMOOTH);
	
}

void print_string( char *string )
{
  int i;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /* display the position data */
  glTranslatef(-0.95, -0.20, 0.0);

  /* draw a white polygon */
  /*glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
    glVertex2f(1.50, 0.10);
    glVertex2f(1.50, -0.12);
    glVertex2f(0.001, -0.12);
    glVertex2f(0.001, 0.10);
  glEnd();*/

  /* draw red text on the polygon */
  glColor3f(0.75, 0.0, 0.0);
  glRasterPos2i(0.0, 0.0);
  for (i=0; i<(int)strlen(string); i++) {
      if(string[i] != '\n' ) {
          glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
      }
      else {
          glTranslatef(0.0, -0.07, 0.0);
          glRasterPos2i(0.0, 0.0);
      }
  }

  return;
}

int draw_exview( double a, double b, double r, double trans[3][4], int xwin, int ywin )
{
    double      vtrans[3][4];
    double      gl_para[16];
    int         i, j;

    argDrawMode3D();
    argDraw3dCamera( xwin, ywin );
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    get_trans( a, b, r, vtrans );
    argConvGlpara(vtrans, gl_para);
    glMatrixMode(GL_PROJECTION);
    glMultMatrixd( gl_para );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setup_light();

    glPushMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
          
    for( j = -300; j <= 200; j+= 100 ) {
        for( i = -300; i <= 200; i+= 100 ) {
            glBegin(GL_QUADS);
            glNormal3f( 0.0, 0.0, 1.0 );
            if( (j/100+i/100)%2 ) glColor4f( 0.6, 0.6, 0.6, 1.0 );
             else                 glColor4f( 0.0, 0.3, 0.0, 1.0 );
            glVertex3f( i,     j,     0.0 );
            glVertex3f( i,     j+100, 0.0 );
            glVertex3f( i+100, j+100, 0.0 );
            glVertex3f( i+100, j,     0.0 );
            glEnd();
        }
    }
    draw_axis();

    /*glColor4f( 0.0, 0.0, 0.5, 1.0 );
    glTranslatef( 0.0, 0.0, 25.0 );
    glutSolidCube(50.0);*/
	
    glDisable( GL_LIGHTING );
    glPopMatrix();

    draw_camera( trans );

    glDisable(GL_NORMALIZE);
    glDisable( GL_DEPTH_TEST );
    argDrawMode2D();

    return 0;
}

static void draw_camera( double trans[3][4] )
{
/*
    double      gl_para[16];
*/
    double      btrans[3][4];
    double      quat[4], pos[3], angle;

    arUtilMatInv( trans, btrans );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
      arUtilMat2QuatPos( btrans, quat, pos );
      angle = -acos(quat[3])*360.0/3.141592;
      glTranslatef( pos[0], pos[1], pos[2] );
      glRotated( angle, quat[0], quat[1], quat[2] );
/*
      argConvGlpara(btrans, gl_para);
      glMultMatrixd( gl_para );
*/

      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);

      glPushMatrix();
        glColor4f( 0.9, 0.9, 0.0, 1.0 );
        glTranslatef( 0.0, 0.0, -10.0 );
        glScalef( 10.0, 10.0, 20.0 );
        glutSolidCube(1.0);
      glPopMatrix();

      glColor4f( 0.9, 0.0, 0.9, 1.0 );
      glPushMatrix();
        glTranslatef( 0.0, 0.0, -40.0 );
        glScalef( 30.0, 30.0, 50.0 );
        glutSolidCube(1.0);
      glPopMatrix();

      glDisable( GL_LIGHTING );
    glPopMatrix();

    return;
}

int draw( char *name, double trans[4][4], int xwin, int ywin , int player)
{
    double      gl_para[16];

    argConvGlpara(trans, gl_para);
	
    draw_object( name, gl_para, xwin, ywin , player);
    
    return(0);
}

/* draw the user object */
static int  draw_object( char *name, double gl_para[16], int xwin, int ywin , int player)
{
    argDrawMode3D();
    argDraw3dCamera( xwin, ywin );

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    /* load the camera transformation matrix */
    glMatrixMode(GL_PROJECTION);
    glMultMatrixd( gl_para );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    setup_light();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

	// desenho 
    if( strcmp(name, "target") == 0 ) {
		glMultMatrixf( view_rotate );
		draw_table( player );
		draw_sphere( player );
    }
    else {
        printf("unknown object type!!\n");
    }

    glDisable( GL_LIGHTING );
    glDisable( GL_NORMALIZE );
    glDisable( GL_DEPTH_TEST );
    argDrawMode2D();

    return 0;
}

static void setup_light()
{
    static int  mat_f = 1;
    GLfloat     mat_amb_diff[]  = {0.9, 0.9, 0.0, 1.0};
    GLfloat     mat_specular[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat     mat_shininess[] = {10.0};
    GLfloat     light_ambient[] = { 0.01, 0.01, 0.01, 1.0 };
    GLfloat     light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat     light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat     light_position[] = { 100.0, 300.0, 700.0, 1.0 };

    if( mat_f ) {
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
      glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);	
      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
      glEnable(GL_COLOR_MATERIAL);
      mat_f = 0;
    }

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

static void get_trans( double a, double b, double r, double trans[3][4] )
{
    ARMat   *mat;
    double  sa, ca, sb, cb;
    double  x, y, z;
    int     i, j;

    sa = sin(a*3.141592/180.0);
    ca = cos(a*3.141592/180.0);
    sb = sin(b*3.141592/180.0);
    cb = cos(b*3.141592/180.0);

    x = 0.0;
    y = -r * cb;
    z = -r * sb;

    mat = arMatrixAlloc( 4, 4 );

    mat->m[0*4+0] =  ca;
    mat->m[0*4+1] =  sa*sb;
    mat->m[0*4+2] =  sa*cb;
    mat->m[1*4+0] = -sa;
    mat->m[1*4+1] =  ca*sb;
    mat->m[1*4+2] =  ca*cb;
    mat->m[2*4+0] =  0;
    mat->m[2*4+1] = -cb;
    mat->m[2*4+2] =  sb;
    mat->m[0*4+3] =  x*ca + y*sa;
    mat->m[1*4+3] = -x*sa + y*ca;
    mat->m[2*4+3] = z;
    mat->m[3*4+0] = 0;
    mat->m[3*4+1] = 0;
    mat->m[3*4+2] = 0;
    mat->m[3*4+3] = 1;

    arMatrixSelfInv( mat );
    for( j = 0; j < 3; j++ ) {
        for( i = 0; i < 4; i++ ) {
            trans[j][i] = mat->m[j*4+i];
        }
    }
    arMatrixFree( mat );

    return;
}

static void draw_axis( void )
{
    glPushMatrix();
        glRotatef( 90.0, 0.0, 1.0, 0.0 );
        glColor4f( 1.0, 0.0, 0.0, 1.0 );
        glutSolidCone(5.0, 100.0, 20, 24);
    glPopMatrix();

    glPushMatrix();
        glRotatef( -90.0, 1.0, 0.0, 0.0 );
        glColor4f( 0.0, 1.0, 0.0, 1.0 );
        glutSolidCone(5.0, 100.0, 20, 24);
    glPopMatrix();

    glPushMatrix();
        glRotatef( 00.0, 0.0, 0.0, 1.0 );
        glColor4f( 0.0, 0.0, 1.0, 1.0 );
        glutSolidCone(5.0, 100.0, 20, 24);
    glPopMatrix();

    return;
}

void drawBox(GLdouble dimx, GLdouble dimy, GLdouble dimz, float s_factor, float t_factor)
{

	GLdouble dx=dimx/2, dy=dimy/2, dz=dimz/2;
	
	GLdouble v1[3] = {dx,-dy,dz};
	GLdouble v2[3] = {dx,-dy,-dz};
	GLdouble v3[3] = {dx,dy,dz};
	GLdouble v4[3] = {dx,dy,-dz};
	GLdouble v5[3] = {-dx,-dy,dz};
	GLdouble v6[3] = {-dx,dy,dz};
	GLdouble v7[3] = {-dx,dy,-dz};
	GLdouble v8[3] = {-dx,-dy,-dz};
	GLdouble normal[3];

	//glColor3f(1, 1, 0);
	//float mat_shininess[] = {20.0}; /* How shiny is the object (specular exponent)  */
	//float mat_specular[] = {0.3, 0.3, 0.3, 0.3}; /* specular reflection. */
	//float mat_diffuse[] = {0.3, 0.3, 0.3, 0.3}; /* diffuse reflection. */
	//float amb[] = {0.5, 0.5, 0.5, 0.5}; /* diffuse reflection. */
	// define as caracteristicas do material (dos materiais seguintes, i.e. ate nova alteracao
	/*glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glDisable(GL_COLOR_MATERIAL);*/

	//Face frente - 0
	glEnable(GL_TEXTURE_2D);
	newellSquare(v5,v6,v3,v1,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0);			glVertex3dv(v1);
		glTexCoord2f(0.0,t_factor);		glVertex3dv(v3);
		glTexCoord2f(s_factor,t_factor);glVertex3dv(v6);
		glTexCoord2f(s_factor,0.0);		glVertex3dv(v5);
	glEnd();

	// face anterior - 1
	newellSquare(v8,v7,v4,v2,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0);			glVertex3dv(v8);
		glTexCoord2f(0.0,t_factor);		glVertex3dv(v7);
		glTexCoord2f(s_factor,t_factor);glVertex3dv(v4);
		glTexCoord2f(s_factor,0.0);		glVertex3dv(v2);
	glEnd();

	// face lateral - 2
	newellSquare(v2,v4,v3,v1,normal);
	glBegin(GL_POLYGON);
 	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0);			glVertex3dv(v2);
		glTexCoord2f(0.0,t_factor);		glVertex3dv(v4);
		glTexCoord2f(s_factor,t_factor);glVertex3dv(v3);
		glTexCoord2f(s_factor,0.0);		glVertex3dv(v1);
	glEnd();

	newellSquare(v5,v6,v7,v8,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0);			glVertex3dv(v5);
		glTexCoord2f(0.0,t_factor);		glVertex3dv(v6);
		glTexCoord2f(s_factor,t_factor);glVertex3dv(v7);
		glTexCoord2f(s_factor,0.0);		glVertex3dv(v8);
	glEnd();

	// base
	newellSquare(v1,v5,v8,v2,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0);			glVertex3dv(v1);
		glTexCoord2f(0.0,t_factor);		glVertex3dv(v5);
		glTexCoord2f(s_factor,t_factor);glVertex3dv(v8);
		glTexCoord2f(s_factor,0.0);		glVertex3dv(v2);
	glEnd();

	// topo 
	newellSquare(v3,v4,v7,v6,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0);			glVertex3dv(v3);
		glTexCoord2f(0.0,t_factor);		glVertex3dv(v4);
		glTexCoord2f(s_factor,t_factor);glVertex3dv(v7);
		glTexCoord2f(s_factor,0.0);		glVertex3dv(v6);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void newellSquare(GLdouble *vec1,GLdouble *vec2,GLdouble *vec3,GLdouble *vec4,GLdouble *normal)
{
  normal[0] = (vec1[1]-vec2[1])*(vec1[2]+vec2[2]) + 
	      (vec2[1]-vec3[1])*(vec2[2]+vec3[2]) + 
	      (vec3[1]-vec4[1])*(vec3[2]+vec4[2]) +
	      (vec4[1]-vec1[1])*(vec4[2]+vec1[2]);
  normal[1] = (vec1[2]-vec2[2])*(vec1[0]+vec2[0]) + 
	      (vec2[2]-vec3[2])*(vec2[0]+vec3[0]) + 
	      (vec3[2]-vec4[2])*(vec3[0]+vec4[0]) +
	      (vec4[2]-vec1[2])*(vec4[0]+vec1[0]);
  normal[2] = (vec1[0]-vec2[0])*(vec1[1]+vec2[1]) + 
	      (vec2[0]-vec3[0])*(vec2[1]+vec3[1]) + 
	      (vec3[0]-vec4[0])*(vec3[1]+vec4[1]) +
	      (vec4[0]-vec1[0])*(vec4[1]+vec1[1]);

  normalise(normal);

}

void normalise(GLdouble *vec)
{
  GLdouble length = 0.0;
  int i;

  for (i=0;i<3;i++){
     length += vec[i]*vec[i]; 
  }
  length= (GLdouble) sqrt((double)length); 

  for (i=0;i<3;i++){
     vec[i] = vec[i]/length; 
  }
}

void draw_table( int player) {
	
	//glRotatef((2.0f * acos(quat[0]) * 180.0f / 3.14f), quat[1], quat[2], quat[3]);
	//glColor3f(0, 0, 1);
	//drawBox(g.dimension.x,g.dimension.y,5, 1, 1);

	glPushMatrix();
	//glRotatef((2.0f * acos(quat[0]) * 180.0f / 3.14f), quat[1],
	//quat[2], quat[3]);

	glTranslated(g.dimension.x/10/2-g.dimension.x/2,
	g.dimension.y/10/2-g.dimension.y/2, 0);

	int i, j, colorSwitch = 0;

	for(i = 0; i < 10; i++){
			for(j = 0; j < 10; j++){
					glPushMatrix();
							if (colorSwitch == 0){
											colorSwitch = 1;
											glColor3f(1,0,player);
									}
							else{
									colorSwitch = 0;
									glColor3f(player, 1, 0);
							}
							glTranslated(i*g.dimension.x/10,j*g.dimension.x/10, 0.0);
							drawBox(g.dimension.x/10, g.dimension.x/10, 5.0, 1, 1);

					glPopMatrix();
			}
			if (colorSwitch == 0)
					colorSwitch = 1;
			else
					colorSwitch = 0;
	}
	glPopMatrix();

	/*
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(0, 0, 0);
		glTexCoord2f(8.0,0.0); glVertex3d(150, 0,  0);
		glTexCoord2f(8.0,8.0); glVertex3d(150, 0,  -300);
		glTexCoord2f(0.0,8.0);  glVertex3d(0, 0, -300);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	*/

	if (player==1) {
		glTranslatef(g.goal.pos.x-(g.dimension.x/2), g.goal.pos.y-(g.dimension.y/2), 3);
		glColor3f(0, 1, 0);
		gluDisk(glQ,g.sphere.radius,g.sphere.radius+5,21,21);
		glColor3f(0, 0, 0);
		gluDisk(glQ,0,g.sphere.radius,21,21);

		glColor3f(0, 1, 1);
		for (int i = 0; i < curo; i++)
		{
			glLoadIdentity();
			glMultMatrixf( view_rotate );
			glTranslatef(g.ob[i].x-(g.dimension.x/2),g.ob[i].y-(g.dimension.y/2),g.sphere.radius);
			//gluCylinder(glQ,g.sphere.radius/2,g.sphere.radius/2,20,11,11);
			gluCylinder_closed(glQ,g.sphere.radius/2,g.sphere.radius/2,20,11,11,true,true);
		} 
	} else if (player==2) {
		glTranslatef(g.goal2.pos.x-(g.dimension.x/2), g.goal2.pos.y-(g.dimension.y/2), 3);
		glColor3f(0, 1, 0);
		gluDisk(glQ,g.sphere2.radius,g.sphere2.radius+5,21,21);
		glColor3f(0, 0, 0);
		gluDisk(glQ,0,g.sphere2.radius,21,21);

		glColor3f(0, 1, 1);
		for (int i = 0; i < curo; i++)
		{
			glLoadIdentity();
			glMultMatrixf( view_rotate );
			glTranslatef(g.ob2[i].x-(g.dimension.x/2),g.ob2[i].y-(g.dimension.y/2),g.sphere2.radius);
			//gluCylinder(glQ,g.sphere2.radius/2,g.sphere2.radius/2,20,11,11);
			gluCylinder_closed(glQ,g.sphere2.radius/2,g.sphere2.radius/2,20,11,11,true,true);
		} 
	}
	
	
}

void draw_sphere( int player ) {
	
	/*float x = quat[1], y = quat[2], z = quat[3], w = quat[0];

	//float *angle = acos(w) * 2.0f;
	float scale = sqrt(x * x + y * y + z * z);

	float axis[3] = {  x / scale,  y / scale,  z / scale } ;*/
	
	/*if (axis[0]>0) {
		g.sphere.pos.x ++;
	} else if (axis[0]<0) {
		g.sphere.pos.x --;
	}

	if (axis[1]>0) {
		g.sphere.pos.y ++;
	} else if (axis[1]<0) {
		g.sphere.pos.y --;
	}*/
	if (player==1) {
		glLoadIdentity();
	
		if (anglex>360||anglex<-360) anglex=0;
		if (angley>360||angley<-360) angley=0;

		glMultMatrixf( view_rotate );
		glTranslatef(g.sphere.pos.x-(g.dimension.x/2), g.sphere.pos.y-(g.dimension.y/2), 2+g.sphere.radius);
		//glColor3f(0, 1, 1);
		//gluSphere(glQ, g.sphere.radius, 21, 21);

		if (g.acx>0) {
			glRotatef(anglex,0.0,1.0,0.0);
			anglex=anglex+g.acx*3;
		}

		if (g.acx<0) {
			glRotatef(anglex,0.0,-1.0,0.0);
			anglex=anglex-g.acx*3;
		}

		if (g.acy>0) {
			glRotatef(angley,1.0,0.0,0.0);
			angley=angley-g.acy*3;
		}

		if (g.acy<0) {
			glRotatef(angley,-1.0,0.0,0.0);
			angley=angley+g.acy*3;
		}

		//gluDisk(glQ,0,g.sphere.radius*2,11,11);

		glScalef(g.sphere.radius,g.sphere.radius,g.sphere.radius);
		glColor3f(0.7, 0.7, 0.7);
		drawmodel_box();
	} else if (player==2) {
		glLoadIdentity();
	
		if (anglex2>360||anglex2<-360) anglex2=0;
		if (angley2>360||angley2<-360) angley2=0;

		glMultMatrixf( view_rotate );
		glTranslatef(g.sphere2.pos.x-(g.dimension.x/2), g.sphere2.pos.y-(g.dimension.y/2), 2+g.sphere2.radius);
		//glColor3f(0, 1, 1);
		//gluSphere(glQ, g.sphere.radius, 21, 21);

		if (g.acx2>0) {
			glRotatef(anglex2,0.0,1.0,0.0);
			anglex2=anglex2+g.acx2*3;
		}

		if (g.acx2<0) {
			glRotatef(anglex2,0.0,-1.0,0.0);
			anglex2=anglex2-g.acx2*3;
		}

		if (g.acy2>0) {
			glRotatef(angley2,1.0,0.0,0.0);
			angley2=angley2-g.acy2*3;
		}

		if (g.acy2<0) {
			glRotatef(angley2,-1.0,0.0,0.0);
			angley2=angley2+g.acy2*3;
		}

		//gluDisk(glQ,0,g.sphere.radius*2,11,11);

		glScalef(g.sphere2.radius,g.sphere2.radius,g.sphere2.radius);
		glColor3f(0.7, 0.7, 0.7);
		drawmodel_box();
	}
	
}

/*
game
*/
void init_game(float dimx, float dimy, float radius, float mindist, int player, int ob) 
{
	int limx = dimx-(2*radius+5);
	int limy = dimy-(2*radius+5);
	int actualmin = radius* + mindist;
	curo = ob;
	g.dimension.x = dimx;
	g.dimension.y = dimy;
	g.mindist = mindist;
	g.acxp=0;
	g.acyp=0;
	g.sphere.radius = g.sphere2.radius = radius;
	nob=ob;
	g.obst=nob;

	/*g.ob.reserve(ob);
	g.ob.resize(ob);
	g.ob2.reserve(ob);
	g.ob2.resize(ob);*/

	if (player==1) {
		g.acx=0;
		g.acy=0;

		srand (time(NULL));
		g.sphere.init.x = g.sphere.pos.x = g.sphere.prev.x = (rand() % limx + radius);
		g.sphere.init.y = g.sphere.pos.y = g.sphere.prev.y = (rand() % limy + radius);

		int safe=0;
		do {
			g.goal.pos.x = (rand() % (limx-5) + radius+5);
			safe++;
			if (safe>10) {
				actualmin--;
				safe=0;
			}
		} while ( (g.goal.pos.x > (g.sphere.init.x-actualmin) ) && (g.goal.pos.x < (g.sphere.init.x+actualmin) ) );
		safe=0;
		do {
			g.goal.pos.y = (rand() % (limy-5) + radius+5);
			safe++;
			if (safe>10) {
				actualmin--;
				safe=0;
			}
		} while ( (g.goal.pos.y > (g.sphere.init.y-actualmin) ) && (g.goal.pos.y < (g.sphere.init.y+actualmin) ) );

		int c=0;
		pair np;
		while (c<ob) {
			c++;
			int safe=0;
			bool ok=true;

			do {
				np.x=(rand() % (limx) + radius);
				safe++;
				if (safe>10) {
					ob--;
					ok=false;
					break;
				}
			} while ( ( (np.x > g.sphere.init.x-g.sphere.radius-5) && (np.x < g.sphere.init.x+g.sphere.radius+5) ) ||
				( (np.x > g.goal.pos.x - g.sphere.radius-5) && ((np.x < g.goal.pos.x + g.sphere.radius+5)) ) );

			do {
				np.y=(rand() % (limy) + radius);
				safe++;
				if (safe>10) {
					ob--;
					ok=false;
					break;
				}
			} while ( ( (np.y > g.sphere.init.y-g.sphere.radius-5) && (np.y < g.sphere.init.y+g.sphere.radius+5) ) ||
				( (np.y > g.goal.pos.y - g.sphere.radius-5) && ((np.y < g.goal.pos.y + g.sphere.radius+5)) ) );

			if (ok)	g.ob[c-1]=np;
			/*std::cout << "\n" << std::flush;
			std::cout << np.x << "," << np.y << "\n" << std::flush;
			std::cout << ok << "," << c << "\n" << std::flush;
			std::cout << g.ob[c].x << "," << g.ob[c].y << "\n" << std::flush;
			std::cout << "\n" << std::flush;*/
			
		}
	} else if (player==2) {
		g.acx2=0;
		g.acy2=0;

		srand (time(NULL));
		g.sphere2.init.x = g.sphere2.pos.x = g.sphere2.prev.x = (rand() % limx + radius);
		g.sphere2.init.y = g.sphere2.pos.y = g.sphere2.prev.y = (rand() % limy + radius);

		int safe=0;
		do {
			g.goal2.pos.x = (rand() % (limx-5) + radius+5);
			safe++;
			if (safe>10) {
				actualmin--;
				safe=0;
			}
		} while ( (g.goal2.pos.x > (g.sphere2.init.x-actualmin) ) && (g.goal2.pos.x < (g.sphere2.init.x+actualmin) ) );
		safe=0;
		do {
			g.goal2.pos.y = (rand() % (limy-5) + radius+5);
			safe++;
			if (safe>10) {
				actualmin--;
				safe=0;
			}
		} while ( (g.goal2.pos.y > (g.sphere2.init.y-actualmin) ) && (g.goal2.pos.y < (g.sphere2.init.y+actualmin) ) );

		int c=0;
		pair np;

		while (c<ob) {
			c++;
			int safe=0;
			bool ok=true;

			do {
				np.x=(rand() % (limx) + radius);
				safe++;
				if (safe>10) {
					ob--;
					ok=false;
					break;
				}
			} while ( ( (np.x > g.sphere2.init.x-g.sphere2.radius-5) && (np.x < g.sphere2.init.x+g.sphere2.radius+5) ) ||
				( (np.x > g.goal2.pos.x - g.sphere2.radius-5) && ((np.x < g.goal2.pos.x + g.sphere2.radius+5)) ) );

			do {
				np.y=(rand() % (limy) + radius);
				safe++;
				if (safe>10) {
					ob--;
					ok=false;
					break;
				}
			} while ( ( (np.y > g.sphere2.init.y-g.sphere2.radius-5) && (np.y < g.sphere2.init.y+g.sphere2.radius+5) ) ||
				( (np.y > g.goal2.pos.y - g.sphere2.radius-5) && ((np.y < g.goal2.pos.y + g.sphere2.radius+5)) ) );

			if (ok)	g.ob2[c-1]=np;
			/*std::cout << "\n" << std::flush;
			std::cout << np.x << "," << np.y << "\n" << std::flush;
			std::cout << ok << "," << c << "\n" << std::flush;
			std::cout << g.ob2[c].x << "," << g.ob2[c].y << "\n" << std::flush;
			std::cout << "\n" << std::flush;*/
		}
	}
	
	return;
}

bool checkcollision (int player) {
	if (player==1) {
		for (int i = 0; i < nob; i++)
		{
			if ( (g.sphere.pos.x>g.ob[i].x-13) && ((g.sphere.pos.x<g.ob[i].x+13)) ) {
				if ( (g.sphere.pos.y>g.ob[i].y-13) && (g.sphere.pos.y<g.ob[i].y+13) ){
					if (g.acx>0) {
						g.sphere.pos.x--;
					} else {
						g.sphere.pos.x++;
					}
					if (g.acy>0) {
						g.sphere.pos.y--;
					} else {
						g.sphere.pos.y++;
					}
					checkcollision(player);
					return true;
				}	
			}
		} 

	}

	if (player==2) {

		for (int i = 0; i < nob; i++)
		{
			if ( (g.sphere2.pos.x>g.ob2[i].x-13) && ((g.sphere2.pos.x<g.ob2[i].x+13)) ) {
				if ( (g.sphere2.pos.y>g.ob2[i].y-13) && (g.sphere2.pos.y<g.ob2[i].y+13) ){
					if (g.acx2>0) {
						g.sphere2.pos.x--;
					} else {
						g.sphere2.pos.x++;
					}
					if (g.acy2>0) {
						g.sphere2.pos.y--;
					} else {
						g.sphere2.pos.y++;
					}
					checkcollision(player);
					return true;
				}	
			}
		} 

	}

	return false;
}

bool checkgoal(int player) {

	if (player==1) {

		if ((g.sphere.pos.x>g.goal.pos.x-g.sphere.radius) && (g.sphere.pos.x<g.goal.pos.x+g.sphere.radius)){
			if ((g.sphere.pos.y>g.goal.pos.y-g.sphere.radius) && (g.sphere.pos.y<g.goal.pos.y+g.sphere.radius)){
				return true;
			}	
		}

	}

	if (player==2) {

		if ((g.sphere2.pos.x>g.goal2.pos.x-g.sphere2.radius) && (g.sphere2.pos.x<g.goal2.pos.x+g.sphere2.radius)){
			if ((g.sphere2.pos.y>g.goal2.pos.y-g.sphere2.radius) && (g.sphere2.pos.y<g.goal2.pos.y+g.sphere2.radius)){
				return true;
			}	
		}

	}

	//if ((g.sphere.pos.x==g.goal.pos.x) && (g.sphere.pos.y==g.goal.pos.y)){}

	return false;
}

bool checkbounds(int player) {

	if (player==1) {

		if (g.sphere.pos.x>g.dimension.x) return true;
		if (g.sphere.pos.x<0) return true;
		if (g.sphere.pos.y>g.dimension.y) return true;
		if (g.sphere.pos.y<0) return true;

	}

	if (player==2) {

		if (g.sphere2.pos.x>g.dimension.x) return true;
		if (g.sphere2.pos.x<0) return true;
		if (g.sphere2.pos.y>g.dimension.y) return true;
		if (g.sphere2.pos.y<0) return true;

	}

	//if ((g.sphere.pos.x==g.goal.pos.x) && (g.sphere.pos.y==g.goal.pos.y)){}

	return false;

}

int get_angle( double trans[3][4], double *wa, double *wb, double *wc )
{
    double      a, b, c;
    double      sina, cosa, sinb, cosb, sinc, cosc;

    if( trans[2][2] > 1.0 ) {
        printf("cos(beta) = %f\n", trans[2][2]);
        trans[2][2] = -1.0;
    }
    else if( trans[2][2] < -1.0 ) {
        printf("cos(beta) = %f\n", trans[2][2]);
        trans[2][2] = -1.0;
    }
    cosb = trans[2][2];
    b = acos( cosb );
    sinb = sin( b );
    if( b >= 0.000001 || b <= -0.000001) {
        cosa = trans[0][2] / sinb;
        sina = trans[1][2] / sinb;
        if( cosa > 1.0 ) {
            printf("cos(alph) = %f\n", cosa);
            cosa = 1.0;
            sina = 0.0;
        }
        if( cosa < -1.0 ) {
            printf("cos(alph) = %f\n", cosa);
            cosa = -1.0;
            sina =  0.0;
        }
        if( sina > 1.0 ) {
            printf("sin(alph) = %f\n", sina);
            sina = 1.0;
            cosa = 0.0;
        }
        if( sina < -1.0 ) {
            printf("sin(alph) = %f\n", sina);
            sina = -1.0;
            cosa =  0.0;
        }
        a = acos( cosa );
        if( sina < 0 ) a = -a;

        sinc =  (trans[2][1]*trans[0][2]-trans[2][0]*trans[1][2])
              / (trans[0][2]*trans[0][2]+trans[1][2]*trans[1][2]);
        cosc =  -(trans[0][2]*trans[2][0]+trans[1][2]*trans[2][1])
               / (trans[0][2]*trans[0][2]+trans[1][2]*trans[1][2]);
        if( cosc > 1.0 ) {
            printf("cos(r) = %f\n", cosc);
            cosc = 1.0;
            sinc = 0.0;
        }
        if( cosc < -1.0 ) {
            printf("cos(r) = %f\n", cosc);
            cosc = -1.0;
            sinc =  0.0;
        }
        if( sinc > 1.0 ) {
            printf("sin(r) = %f\n", sinc);
            sinc = 1.0;
            cosc = 0.0;
        }
        if( sinc < -1.0 ) {
            printf("sin(r) = %f\n", sinc);
            sinc = -1.0;
            cosc =  0.0;
        }
        c = acos( cosc );
        if( sinc < 0 ) c = -c;
    }
    else {
        a = b = 0.0;
        cosa = cosb = 1.0;
        sina = sinb = 0.0;
        cosc = trans[0][0];
        sinc = trans[1][0];
        if( cosc > 1.0 ) {
            printf("cos(r) = %f\n", cosc);
            cosc = 1.0;
            sinc = 0.0;
        }
        if( cosc < -1.0 ) {
            printf("cos(r) = %f\n", cosc);
            cosc = -1.0;
            sinc =  0.0;
        }
        if( sinc > 1.0 ) {
            printf("sin(r) = %f\n", sinc);
            sinc = 1.0;
            cosc = 0.0;
        }
        if( sinc < -1.0 ) {
            printf("sin(r) = %f\n", sinc);
            sinc = -1.0;
            cosc =  0.0;
        }
        c = acos( cosc );
        if( sinc < 0 ) c = -c;
    }

    *wa = a;
    *wb = b;
    *wc = c;

    return(0);
}

int check_incline( double card_trans[3][4], double base_trans[3][4], double *angle )
{
    ARMat   *mat_a, *mat_b, *mat_c;
    double  a, b, c;
    int     i, j;

    mat_a = arMatrixAlloc( 4, 4 );
    mat_b = arMatrixAlloc( 4, 4 );
    mat_c = arMatrixAlloc( 4, 4 );
    for( j = 0; j < 3; j++ ) {
        for( i = 0; i < 4; i++ ) {
            mat_b->m[j*4+i] = base_trans[j][i];
        }
    }
    mat_b->m[3*4+0] = 0.0;
    mat_b->m[3*4+1] = 0.0;
    mat_b->m[3*4+2] = 0.0;
    mat_b->m[3*4+3] = 1.0;

    for( j = 0; j < 3; j++ ) {
        for( i = 0; i < 4; i++ ) {
            mat_a->m[j*4+i] = card_trans[j][i];
        }
    }
    mat_a->m[3*4+0] = 0.0;
    mat_a->m[3*4+1] = 0.0;
    mat_a->m[3*4+2] = 0.0;
    mat_a->m[3*4+3] = 1.0;
    arMatrixSelfInv( mat_a );
    arMatrixMul( mat_c, mat_a, mat_b );

    get_angle( (double (*)[4])(mat_c->m), &a, &b, &c );

    arMatrixFree( mat_a );
    arMatrixFree( mat_b );
    arMatrixFree( mat_c );

    if( b > 0.4 ) {
      *angle = a + 3.141592;
      return 1;
    }

    return 0;
}

/*
MAIN
*/

void callback(int id){
	switch (id){
		case 1: 
			for (int f=0;f!=16;f++)
				view_rotate[f]=iden_matrix[f];
			if (modo_jogo==0) {

				dimx=spin1->get_float_val();
				dimy=spin2->get_float_val();
				mindis=spin3->get_float_val();
				nob=spin4->get_float_val();

				if (dimx<50) {
					dimx=50;
				} else if (dimx>500) {
					dimx=500;
				}
				if (dimy<50) {
					dimy=50;
				} else if (dimy>500) {
					dimy=500;
				}
				if (mindis<1) {
					mindis=1;
				} else if (mindis>500) {
					mindis=500;
				}
				if (nob<0) {
					nob=0;
				} else if (nob>50) {
					nob=50;
				}

				g.dimension.x=dimx;
				g.dimension.y=dimy;
				g.mindist=mindis;
				g.obst=nob;

				init_game(g.dimension.x,g.dimension.y,10.0,g.mindist,1,g.obst);
				init_game(g.dimension.x,g.dimension.y,10.0,g.mindist,2,g.obst);

				g.p1=0;
				g.p2=0;
			} else if (modo_jogo==1) {
				init_game(100.0,100.0,10.0,10.0,1,0);
				init_game(100.0,100.0,10.0,10.0,2,0);
			} else if (modo_jogo==2) {
				init_game(150.0,150.0,10.0,20.0,1,5);
				init_game(150.0,150.0,10.0,20.0,2,5);
			} else if (modo_jogo==3) {
				init_game(200.0,200.0,10.0,50.0,1,15);
				init_game(200.0,200.0,10.0,50.0,2,15);
			} else {
				init_game(g.dimension.x,g.dimension.y,10.0,g.mindist,1,g.obst);
				init_game(g.dimension.x,g.dimension.y,10.0,g.mindist,2,g.obst);
			}
			break;
		case 2: 
			g.sphere.prev.x=g.sphere.pos.x;
			g.sphere.prev.y=g.sphere.pos.y;
			g.acxp=g.acx;
			g.acyp=g.acy;
			break;
		case 3:
			g.sphere.pos.x=g.sphere.prev.x;
			g.sphere.pos.y=g.sphere.prev.y;
			g.acx=g.acxp;
			g.acy=g.acyp;
			break;
		case 4:
			g.sphere.pos.x=g.sphere.init.x;
			g.sphere.pos.y=g.sphere.init.y;
			g.acx=0;
			g.acy=0;
			break;
		case 5:
			cleanup();
			exit(0);
			break;
		case 6:
			if (modo_jogo==0) {

			} else if (modo_jogo==1) {
				spin1->set_float_val(100);
				spin2->set_float_val(100);
				spin3->set_float_val(10);
				spin4->set_float_val(0);
			} else if (modo_jogo==2) {
				spin1->set_float_val(150);
				spin2->set_float_val(150);
				spin3->set_float_val(20);
				spin4->set_float_val(5);
			} else if (modo_jogo==3) {
				spin1->set_float_val(200);
				spin2->set_float_val(200);
				spin3->set_float_val(50);
				spin4->set_float_val(15);
			} else {
				
			}
			break;
		case 7:
			if (dimx<50) {
				dimx=50;
			}
			break;
		case 8:
			if (dimy<50) {
				dimy=50;
			}
			break;
		case 9:
			if (mindis<1) {
				mindis=1;
			}
			break;
		case 10:
			for (int f=0;f!=16;f++)	view_rotate[f]=iden_matrix[f];
			break;
		case 11:
			if (nob>50) {
				nob=50;
			} else if (nob<0) {
				nob=0;
			}
			break;
		default:
			break;
	}
}

void myGlutReshape(int w, int h)
{
	int tx, ty, tw, th;

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
	xy_aspect = (float)tw / (float)th;

	//GLUI_Master.auto_set_viewport();
	//glutSetWindow(1);

	glutPostRedisplay();

}

void myGlutIdle( void ) {

	glutSetWindow(1);
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	/*pixmap.readBMPFile("feup.bmp");
	pixmap.setTexture(1);*/
		
    if( init() < 0 ) exit(0);

	GLUI_Master.set_glutReshapeFunc( myGlutReshape );

	init_game(150.0,150.0,10.0,10.0,1,5);
	init_game(150.0,150.0,10.0,10.0,2,5);
	g.p1=0;
	g.p2=0;
	g.acxp=0;
	g.acyp=0;
	g.obst=5;

    arVideoCapStart();

    glutMotionFunc( motionEvent );

    argMainLoop( mouseEvent, keyEvent, mainLoop );

	return (0);
}

/* set up the application parameters - read in from command line*/
static int init(void)
{
    char     cparaname[256];
    char     pattname[256];
	char     pattname2[256];
    ARParam  wparam;

    strcpy( cparaname, "Data/camera_para.dat" );
    strcpy( pattname,  "Data/patt.hiro" );
	strcpy( pattname2, "Data/patt.kanji" );
    
    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparaname, 1, &wparam) < 0 ) {
       printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 2, 1, 0 );

    if( (target_id = arLoadPatt(pattname)) < 0 ) {
        printf("Target pattern load error!!\n");
        exit(0);
    }

	if( (target_id2 = arLoadPatt(pattname2)) < 0 ) {
        printf("Target pattern load error!!\n");
        exit(0);
    }

    arDebug = 1;

	glQ = gluNewQuadric();

    return 0;
}

static void mainLoop(void)
{
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k, q;

    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    argDrawMode2D();
    argDispImage( dataPtr, 0, 0 );

    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }
    arVideoCapNext();

    /* if the debug mode is on draw squares 
       around the detected squares in the video image */
    if( arDebug ) {
        if( arImageProcMode == AR_IMAGE_PROC_IN_HALF )
            argDispHalfImage( arImage, 2, 1 );
        else
            argDispImage( arImage, 2, 1);
    }

	if (glon) {
		glon=false;
		argDrawMode3D();
		//argDraw3dCamera( 1, 1 );
		
		glui = GLUI_Master.create_glui_subwindow (1, GLUI_SUBWINDOW_RIGHT );

		view_rot = glui->add_rotation( "Rotacao", view_rotate );
		view_rot->set_spin( 1.0 );
		GLUI_Button *resrot_button = glui->add_button("RESET CAMERA", 10,callback);
    
		/*glui->add_separator();
		trans_z = glui->add_translation( "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
		trans_z->set_speed( 1 );*/

		glui->add_separator();
		//glui->add_spinner_to_panel(panel_main,"Eixos");

		GLUI_Rollout *opcoes = glui->add_rollout("Game Menu");
		GLUI_Button *new_game_button = glui->add_button_to_panel(opcoes,"NEW GAME", 1,callback);

		GLUI_Rollout *def = glui->add_rollout_to_panel(opcoes,"Mode");
		modo = glui->add_radiogroup_to_panel(def,&modo_jogo,6,callback);
		glui->add_radiobutton_to_group( modo, "CUSTOM");
		glui->add_radiobutton_to_group( modo, "EASY");
		glui->add_radiobutton_to_group( modo, "NORMAL");
		glui->add_radiobutton_to_group( modo, "HARD");

		save_button = glui->add_button_to_panel(opcoes,"SAVE", 2,callback);
		undo_button= glui->add_button_to_panel(opcoes,"UNDO", 3,callback);
		reset_button = glui->add_button_to_panel(opcoes,"RESET", 4,callback);
		GLUI_Button *exit_button = glui->add_button_to_panel(opcoes,"EXIT",5,callback);

		GLUI_Rollout *config = glui->add_rollout("Configure");
		spin1 = glui->add_spinner_to_panel(config,"X Dimension",3,&dimx,7,callback);
		spin2 = glui->add_spinner_to_panel(config,"Y Dimension",3,&dimy,8,callback);
		spin3 = glui->add_spinner_to_panel(config,"Min. Distance",3,&mindis,9,callback);
		spin4 = glui->add_spinner_to_panel(config,"Nº Obstacles",2,&nob,11,callback);
		
		GLUI_Rollout *inst = glui->add_rollout("Instructions");
		glui->add_statictext_to_panel(inst,"Player Hiro");
		glui->add_statictext_to_panel(inst,"W-UP S-DOWN");
		glui->add_statictext_to_panel(inst,"A-LEFT D-RIGHT");
		glui->add_statictext_to_panel(inst,"Player Kanji");
		glui->add_statictext_to_panel(inst,"I-UP K-DOWN");
		glui->add_statictext_to_panel(inst,"J-LEFT L-RIGHT");

		GLUI_Rollout *creditos = glui->add_rollout("Credits",0);
		glui->add_statictext_to_panel(creditos,"Vitor Ribeiro");
		glui->add_statictext_to_panel(creditos,"ei07038");
		glui->add_statictext_to_panel(creditos,"Tiago Borba");
		glui->add_statictext_to_panel(creditos,"ei06049");
		
		GLUI_Master.set_glutKeyboardFunc( keyEvent );
		GLUI_Master.set_glutMouseFunc( mouseEvent );
		glutMotionFunc( motionEvent );
		GLUI_Master.set_glutIdleFunc( myGlutIdle );
		GLUI_Master.set_glutSpecialFunc( NULL );
		//GLUI_Master.sync_live_all();
		
		glui->set_main_gfx_window( 1 );

		argDrawMode2D();	
		
	}

    /* check for object visibility */
    k = -1;
	q = -1;

    for( j = 0; j < marker_num; j++ ) {

        if (  marker_info[j].id == target_id  ){
            if( k == -1 ) k = j;
            else {
                if( marker_info[k].cf < marker_info[j].cf ) k = j;
            }
        }

		if (marker_info[j].id == target_id2 ){
            if( q == -1 ) q = j;
            else {
                if( marker_info[q].cf < marker_info[j].cf ) q = j;
            }
        }

    }

    if( k != -1 ) {
        glDisable(GL_DEPTH_TEST);
        getResult( &marker_info[k] , 1);
    }

	twop=false;
	if ( q!=-1 ) {
		glDisable(GL_DEPTH_TEST);
		getResult( &marker_info[q] , 2);
		save_button->disable();
		undo_button->disable();
		reset_button->disable();
		twop=true;
	}

	draw_exview( a, b, r, target_trans, 1, 1 );

	if (!twop) {
		save_button->enable();
		undo_button->enable();
		reset_button->enable();
	} else {
		sprintf(string, "Score: %i : %i \n", g.p1,g.p2);
		print_string( string );
	}
	

	//Render objects in the same coordinate space of AR marker 	
	//argConvGlpara(target_trans, myScene->markerTrans);
	/*argDrawMode3D();
    argDraw3dCamera( 0, 0 );
	myOpenGL->RenderMarkerObjs(myScene);
	argDrawMode2D();*/

	//goal = checkgoal(player);
	//outofbounds = checkbounds(player);
	if (checkcollision(1)) {
		g.acx=-g.acx/2;
		g.acy=-g.acy/2;
	}

	if (checkcollision(2)) {
		g.acx2=-g.acx2/2;
		g.acy2=-g.acy2/2;
	}


	if (checkgoal(1)) {
		//for (int f=0;f!=16;f++)	view_rotate[f]=iden_matrix[f];
		init_game(g.dimension.x,g.dimension.y,10.0,g.mindist,1,g.obst);
		if (twop) g.p1++;
	}

	if (checkgoal(2)) {
		//for (int f=0;f!=16;f++)	view_rotate[f]=iden_matrix[f];
		init_game(g.dimension.x,g.dimension.y,10.0,g.mindist,2,g.obst);
		g.p2++;
	}

	if (checkbounds(1)) {
		g.sphere.pos.x=g.sphere.init.x;
		g.sphere.pos.y=g.sphere.init.y;
		g.acx=0;
		g.acy=0;
	}

	if (checkbounds(2)) {
		g.sphere2.pos.x=g.sphere2.init.x;
		g.sphere2.pos.y=g.sphere2.init.y;
		g.acx2=0;
		g.acy2=0;
	}

	g.sphere.pos.x=g.sphere.pos.x+g.acx;
	g.sphere.pos.y=g.sphere.pos.y+g.acy;
	g.sphere2.pos.x=g.sphere2.pos.x+g.acx2;
	g.sphere2.pos.y=g.sphere2.pos.y+g.acy2;

	glutPostRedisplay();

	

	argSwapBuffers();
}

static void getResult( ARMarkerInfo *marker_info , int player)
{
    /*char        string1[256];
    char        string2[256];
	char        string3[256];
	char        string4[256];
	char        string5[256];
	double angle;
	double Xpos;
	double Ypos;
	double Zpos;
	double range;*/

    if( arGetTransMat(marker_info, target_center, target_width, target_trans) < 0 ) return;
    if( arUtilMatInv(target_trans, cam_trans) < 0 ) return;
    if( arUtilMat2QuatPos(cam_trans, quat, pos) < 0 ) return;

	/*
	//only the first time, it takes trasformation matrix of initial position of marker
	//and stores it in fixed_trans
	if(firstTime == true)
	{
		firstTime = false;
		if( arGetTransMat(marker_info, target_center, target_width, fixed_trans) < 0 ) return;
		argConvGlpara(fixed_trans, myScene->fixedTrans);
	}

	//take trasformation of marker
	if( arGetTransMat(marker_info, target_center, target_width, target_trans) < 0 ) return;
	
	//convert marker transformation in ODE matrix
	if( arUtilMatInv(target_trans, cam_trans) < 0 ) return;

	//passa la matrice di trasformazione del marker, per 
	//la visualizzazione OpenGL del piano mobile
	argConvGlpara(target_trans, myScene->markerTrans);

	double wmat1[3][4], wmat2[3][4];
	arUtilMatInv(fixed_trans, wmat1);
    arUtilMatMul(wmat1,target_trans, wmat2);

	double trans[16];
	argConvGlpara(wmat2, trans);

	dMatrix3 matrix;
	dRSetIdentity(matrix);

	matrix[0]=trans[0]; matrix[4]=trans[1]; matrix[8]= trans[2]; 
	matrix[1]=trans[4]; matrix[5]=trans[5]; matrix[9]= trans[6]; 
	matrix[2]=trans[8]; matrix[6]=trans[9]; matrix[10]=trans[10]; 
	matrix[3]=trans[12]; matrix[7]=trans[13]; matrix[11]=trans[14]; 
	
	dGeomID plane_mesh;
	dGeomSetRotation(plane_mesh,matrix);
	dGeomSetPosition(plane_mesh,matrix[3],matrix[7],matrix[11]);
	

	Xpos = target_trans[0][3];
	Ypos = target_trans[1][3];
	Zpos = target_trans[2][3];
	range = sqrt(Xpos*Xpos+Ypos*Ypos+Zpos*Zpos);

	check_incline(target_trans,cam_trans,&angle);*/

    /*sprintf(string1," QUAT: Pos x: %3.1f  y: %3.1f  z: %3.1f \n",
            pos[0], pos[1], pos[2]);
    sprintf(string2, "      Quat qx: %3.2f qy: %3.2f qz: %3.2f qw: %3.2f \n",
            quat[0], quat[1], quat[2], quat[3]);
	sprintf(string3, "Sphere X: %f , Y: %f  \n", g.sphere.pos.x,g.sphere.pos.y);
	sprintf(string4, "Hole X: %f , Y: %f  \n", g.goal.pos.x,g.goal.pos.y);
	
	sprintf(string1," %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n",
		view_rotate[0], view_rotate[1],view_rotate[2],view_rotate[3],view_rotate[4],view_rotate[5],view_rotate[6],
		view_rotate[7],view_rotate[8],view_rotate[9],view_rotate[10],view_rotate[11],view_rotate[12],view_rotate[13],
		view_rotate[14],view_rotate[15]);*/

    //strcat( string1, string2 );
	//strcat( string1, string3 );
	//strcat( string1, string4 );

    draw( "target", target_trans, 0, 0 , player);
    //draw_exview( a, b, r, target_trans, 1, 1 );

    //print_string( string1 );

    return;
}


/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

static void keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        cleanup();
        exit(0);
    }

    /* change the threshold value when 't' key pressed */
    /*if( key == 't' ) {
        printf("Enter new threshold value (default = 100): ");
        scanf("%d",&thresh); while( getchar()!='\n' );
        printf("\n");
    }*/

    /* turn on and off the debug mode with right mouse */
    /*if( key == 'd' ) {
        arDebug = 1 - arDebug;
        if( arDebug == 0 ) {
            glClearColor( 0.0, 0.0, 0.0, 0.0 );
            glClear(GL_COLOR_BUFFER_BIT);
            argSwapBuffers();
            glClear(GL_COLOR_BUFFER_BIT);
            argSwapBuffers();
        }
    }

    /*if(key == 'o') {
        outputMode = (outputMode + 1) % 2;
    }*/

    /*if(key == 'c' ) {
        disp_mode = 1 - disp_mode;
    }*/

	if( key == 'd' ) {
		g.acx++;
    }

	if( key == 'a' ) {
		g.acx--;
    }

	if( key == 's' ) {
		g.acy--;
    }

	if( key == 'w' ) {
		g.acy++;
    }

	if( key == 'l' ) {
		g.acx2++;
    }

	if( key == 'j' ) {
		g.acx2--;
    }

	if( key == 'k' ) {
		g.acy2--;
    }

	if( key == 'i' ) {
		g.acy2++;
    }

	//glutPostRedisplay();

}

static void motionEvent( int x, int y )
{
    if( mouse_st == 1 ) {
        a += ((double)x - mouse_ox) / 2.0;
        b -= ((double)y - mouse_oy) / 2.0;
        if( a <   0.0 ) a += 360.0;
        if( a > 360.0 ) a -= 360.0;
        if( b < -90.0 ) b =  -90.0;
        if( b >   0.0 ) b =    0.0;
    }
    else if( mouse_st == 2 ) {
        r *= (1.0 + ((double)y - mouse_oy)*0.01);
        if( r < 10.0 ) r = 10.0;
    }

    mouse_ox = x;
    mouse_oy = y;

	//glutPostRedisplay();
}

static void mouseEvent(int button, int state, int x, int y)
{
    if( state == GLUT_UP ) {
        mouse_st = 0;
    }
    else if( state == GLUT_DOWN ) {
        if( button == GLUT_LEFT_BUTTON ) {
            mouse_st = 1;
            mouse_ox = x;
            mouse_oy = y;
        }
        /*else if( button == GLUT_MIDDLE_BUTTON ) {
            disp_mode = 1 - disp_mode;
        }*/
        else if( button == GLUT_RIGHT_BUTTON ) {
            mouse_st = 2;
            mouse_ox = x;
            mouse_oy = y;
        }
    }

	//glutPostRedisplay();
}
