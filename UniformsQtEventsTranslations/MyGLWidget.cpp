#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>
#include <math.h> 

MyGLWidget::MyGLWidget (QGLFormat &f, QWidget* parent) : QGLWidget(f, parent)
{
  setFocusPolicy(Qt::ClickFocus);  // per rebre events de teclat
}

void MyGLWidget::initShaders ()
{
  fs = new QGLShader(QGLShader::Fragment, this);
  fs->compileSourceFile("./fragshad.frag");
  vs = new QGLShader(QGLShader::Vertex, this);
  vs->compileSourceFile("./vertshad.vert");

  program = new QGLShaderProgram(this);
  program->addShader(fs);
  program->addShader(vs);
  program->link();
  program->bind();
}

void MyGLWidget::initializeGL ()
{
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL

  glClearColor (0.0, 0.0, 0.0, 1.0); // defineix color de fons (d'esborrat)

  initShaders();

  createBuffers();

  //Pintem la primera vegada  
  sclV.x = 1.0;
  sclV.y = 1.0;
  sclV.z = 1.0; 

  transV.x = 0.0;
  transV.y = 0.0;
  transV.z = 0.0;
  rotDegrees = 0.0;
  transLoc = glGetUniformLocation(program->programId(), "TG");
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  TG = glm::mat4(1.0);

  firstTime = true;
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO);

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(0.0, 1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(-1.0, -1.0, 0.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels vèrtexs
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
  int posV = glGetAttribLocation(program->programId(), "vertex");	
  glVertexAttribPointer(posV, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posV);

  //Colors
  glm::vec3 Colores[3];
  Colores[0] = glm::vec3(0.0, 1.0, 0.0);
  Colores[1] = glm::vec3(1.0, 0.0, 0.0);
  Colores[2] = glm::vec3(0.0, 0.0, 1.0);

  glBindVertexArray(VAO);

  // Creació del buffer amb les dades dels colors
  glGenBuffers(1, &VBOc);
  glBindBuffer(GL_ARRAY_BUFFER, VBOc);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colores), Colores, GL_STATIC_DRAW);

  int posC = glGetAttribLocation(program->programId(), "color");	
  glVertexAttribPointer(posC, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(posC);

	// Desactivem el VAO
  glBindVertexArray(0);

}

void MyGLWidget::keyPressEvent(QKeyEvent *e){
  switch (e->key()){
    case Qt::Key_Escape:
      exit(0);
    case Qt::Key_S:
      TG = glm::scale(TG, glm::vec3(1.1,1.1,1.1));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
      updateGL();
      break;
    case Qt::Key_D:
      TG = glm::scale(TG, glm::vec3(0.9,0.9,0.9));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
      updateGL();
      break;
    case Qt::Key_Space:
      TG = glm::rotate(TG, (float)20.0, glm::vec3(0.0,0.0,1.0));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
      updateGL();
      break;
    case Qt::Key_Left:
      TG = glm::translate(TG, glm::vec3(-0.1,0.0,0.0));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
      updateGL();
      break;
    case Qt::Key_Right:
      TG = glm::translate(TG, glm::vec3(0.1,0.0,0.0));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
      updateGL();
      break;
    case Qt::Key_Up:
      TG = glm::translate(TG, glm::vec3(0.0,0.1,0.0));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
      updateGL();
      break;
    case Qt::Key_Down:
      TG = glm::translate(TG, glm::vec3(0.0,-0.1,0.0));
      glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
      updateGL();
      break;
    default: e->ignore();
  }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e){
  if (firstTime){
    lastX = e->x();
    lastY = e->y();
    firstTime = false;
  }
  else{
    int currentX = e->x();
    int currentY = e->y();
    int incrementX = abs(lastX - currentX);
    int incrementY = abs(lastY - currentY);

	  if (incrementY > incrementX){
	    if (lastY < currentY){
	    	sclV.y = 1.1;
	    }
	    else{
	      sclV.y = 0.9;
	    }
	  }
	  else {
	    if (lastX < currentX){
	    	sclV.x = 1.0;
	    }
	    else{
	      sclV.x = 0.9;
	    }
	  }
    lastX = currentX;
    lastY = currentY;
  }
  
  TG = glm::scale(TG, sclV);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
  updateGL();
}

