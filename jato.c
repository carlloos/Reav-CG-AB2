#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "png_texture.h"


#define TEXTURA_DO_PLANO "mcz.png"
#define TEXTURA_DO_AVIAO "textura_jato.png"
#define PI 3.1415


GLfloat obs[3]={0.0,7.0,0.0}; //posição do observador
GLfloat look[3]={0.0,3.0,0.0}; //ponto de vista do obs

GLuint  textura_plano; //id das texturas
GLuint  textura_aviao;


GLfloat tetaxz=0; //angulo de rotação do ponto de observação
GLfloat raioxz=6; //distancia entre obs e img
GLuint  jato; //id p/ lista de apresentação do jato


GLfloat cta[4][2]={ //para haver repetição da textura no corpo do jato
  {-1.0,-1.0},
  {+1.0,-1.0},
  {+1.0,+1.0},
  {-1.0,+1.0}
};


void reshape(int width, int height){
  width= 800;
  height = 600;
  glViewport(0,0,(GLint)width,(GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0,width/(float)height,0.1,30.0);
  glMatrixMode(GL_MODELVIEW);
}

void desenha_jato(void){
  GLUquadricObj *quadric; //quadrica (cones, cilindros e esferas)

  GLfloat asa[][3]={ //coords dos poligonos
    {-2.5,0.0,0.0},
    {+2.5,0.0,0.0},
    {0.0,0.0,3.0}
  };

  GLfloat cauda[][3]={
    {0.0,0.0,0.0},
    {0.0,1.5,-1.0},
    {0.0,1.5,0.0},
    {0.0,0.0,1.5}
  };


  //jato

  jato = glGenLists(1); //cria as listas de apresentação vazias

  glNewList(jato, GL_COMPILE); //nova lista


  glBegin(GL_TRIANGLES); //asa
  glTexCoord2fv(cta[0]); glVertex3fv(asa[0]);
  glTexCoord2fv(cta[1]); glVertex3fv(asa[1]);
  glTexCoord2fv(cta[3]); glVertex3fv(asa[2]);
  glDisable(GL_TEXTURE_2D);
  glEnd();

 
  quadric = gluNewQuadric(); //corpo (cilindro)
  gluQuadricTexture(quadric, GL_TRUE); //habilita o mapeamento de textura
  gluCylinder(quadric, 0.3, 0.3, 4, 12, 3);
  
 
  quadric = gluNewQuadric(); //frente
  gluQuadricTexture(quadric, GL_TRUE);
  glPushMatrix();
  glTranslatef(0,0,4);
  gluCylinder(quadric, 0.3, 0.0, 1.5, 12, 3);
  glPopMatrix();


  glBegin(GL_POLYGON); //cauda
  glTexCoord2fv(cta[0]); glVertex3fv(cauda[0]);
  glTexCoord2fv(cta[1]); glVertex3fv(cauda[1]);
  glTexCoord2fv(cta[2]); glVertex3fv(cauda[2]);
  glTexCoord2fv(cta[3]); glVertex3fv(cauda[3]);
  glEnd();

  glTranslatef(0,0.3,3.0); //cabine
  glPushMatrix();
  glScalef(0.4,0.4,2.0);
  quadric=gluNewQuadric();
  glColor4f(1,1.0,1,1.0);
  glDisable(GL_TEXTURE_2D);
  gluSphere(quadric,0.5,12,12);
  glPopMatrix();


  glEndList(); //fim da lista de apresentação do jato
}

void display(void){
  glEnable(GL_DEPTH_TEST);

  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  

  glPushMatrix();

  // calcula a posicao do observador 
  obs[0]=raioxz*cos(2*PI*tetaxz/360);
  obs[2]=raioxz*sin(2*PI*tetaxz/360);
  gluLookAt(obs[0],obs[1],obs[2],look[0],look[1],look[2],0.0,1.0,0.0);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);//parametros de textura

  // grava a transformacao atua

  glPushMatrix();
  glColor4f(1.0, 1.0, 1.0, 1.0); //cor do plano
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  
  glBindTexture(GL_TEXTURE_2D,textura_plano);
  glBegin(GL_QUADS);
  //colocando a textura de mcz no plano
  glTexCoord2f(0, 1); glVertex3f( -10, 0, 15);
  glTexCoord2f(0, 0); glVertex3f( 10,  0, 15);
  glTexCoord2f(1, 1); glVertex3f( 10,  0,  -15);
  glTexCoord2f(1, 0); glVertex3f( -10, 0,  -15);
  glEnd();

  glTranslatef(0.0,2.0,-3.0);
  glColor4f(1.0, 1.0, 1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D,textura_aviao);
  glCallList(jato);
  glPopMatrix();
  // volta para a ultima transformacao

  
  glPopMatrix();
  glutSwapBuffers();
}


void special(int key, int x, int y){
  switch (key) {
  case GLUT_KEY_UP:
    obs[1]=obs[1]+1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_DOWN:
    obs[1] =obs[1]-1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_LEFT:
    tetaxz=tetaxz+2;
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    tetaxz=tetaxz-2;
    glutPostRedisplay();
    break;
  }
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 27:
    exit(0);
    break;
  case 'r':
    raioxz=raioxz+1;
    glutPostRedisplay();
    break;
  case 'R':
    raioxz=raioxz-1;
    if(raioxz==0){
      raioxz=1;
    }
    glutPostRedisplay();
    break;
  }
}

void carregar_texturas(void){
  textura_plano = png_texture_load(TEXTURA_DO_PLANO, NULL, NULL);
  textura_aviao = png_texture_load(TEXTURA_DO_AVIAO, NULL, NULL);
}

void init(){
  glEnable(GL_DEPTH_TEST); //profundidade
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  carregar_texturas();
  desenha_jato();
  glEnable(GL_TEXTURE_2D);

  //iluminação
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

int main(int argc,char **argv){
  glutInitWindowPosition(0,0);
  glutInitWindowSize(800,600);
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_ALPHA);

  glutCreateWindow("REAV CG AB2");


  init();
  
  //glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();

  return(0);
}