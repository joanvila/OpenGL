#include <GL/glew.h>
#include "MyGLWidget.h"

#include <iostream>

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

  // Desactivem el VAO
  glBindVertexArray(0);

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

  //Pintem la primera vegada
  int varLoc = glGetUniformLocation(program->programId(), "val");
  glUniform1f(varLoc, 0.5);

}

void MyGLWidget::keyPressEvent(QKeyEvent *e){
  switch (e->key()){
    case Qt::Key_Escape:
      exit(0);
    case Qt::Key_S:
      varLoc = glGetUniformLocation(program->programId(), "val");
      scl += 0.1;
      glUniform1f(varLoc, scl);
      updateGL();
      break;
    case Qt::Key_D:
      varLoc = glGetUniformLocation(program->programId(), "val");
      scl -= 0.1;
      glUniform1f(varLoc, scl);
      updateGL();
      break;
    default: e->ignore();
  }
}

