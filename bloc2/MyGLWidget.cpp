#include <GL/glew.h>
#include "MyGLWidget.h"
#include <iostream>

using namespace std;

MyGLWidget::MyGLWidget (QGLFormat &f, QWidget* parent) : QGLWidget(f, parent)
{
  setFocusPolicy(Qt::ClickFocus); // per rebre events de teclat
  scale = 1.0f;
}

void MyGLWidget::initializeGL () 
{
  // glew és necessari per cridar funcions de les darreres versions d'OpenGL
  glewExperimental = GL_TRUE;
  glewInit(); 
  glGetError();  // Reinicia la variable d'error d'OpenGL
	
  glEnable(GL_DEPTH_TEST);

	rotateHomer = 0.0f;
  glClearColor(0.7, 1.0, 0.7, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();

  m.load("./models/Patricio.obj");
	buscaExtremsICentreCaixa();

  createBuffers();
	modelTransform ();
	recalcularParamsVisio();
  projectTransform();
	viewTransform();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Pintem el model
	glBindVertexArray(VAO_model);
  glDrawArrays(GL_TRIANGLES, 0, m.faces().size()*3);
	glBindVertexArray(0);
/*
	terraAlInici();  

  // Activem el VAO per a pintar el terra
  glBindVertexArray (VAO_Terra);

  // pintem
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
*/
}

void MyGLWidget::buscaExtremsICentreCaixa () 
{
  // Busquem els extrems de la caixa contenidora
	maxVert.x = m.vertices()[0];
	maxVert.y = m.vertices()[1];
	maxVert.z = m.vertices()[2];

  minVert.x = m.vertices()[0];
	minVert.y = m.vertices()[1];
	minVert.z = m.vertices()[2];

	for (unsigned int i = 3; i < m.vertices().size(); i+= 3){
		if (m.vertices()[i] > maxVert.x) maxVert.x = m.vertices()[i];
		if (m.vertices()[i] < minVert.x) minVert.x = m.vertices()[i];

		if (m.vertices()[i+1] > maxVert.y) maxVert.y = m.vertices()[i+1];
		if (m.vertices()[i+1] < minVert.y) minVert.y = m.vertices()[i+1];

		if (m.vertices()[i+2] > maxVert.z) maxVert.z = m.vertices()[i+2];
		if (m.vertices()[i+2] < minVert.z) minVert.z = m.vertices()[i+2];
	}

	centreCaixaInicial.x = (minVert.x + maxVert.x)/2;
	centreCaixaInicial.y = (minVert.y + maxVert.y)/2;
	centreCaixaInicial.z = (minVert.z + maxVert.z)/2;

	double x = maxVert.x - centreCaixaInicial.x;
	double y = maxVert.y - centreCaixaInicial.y;
	double z = maxVert.z - centreCaixaInicial.x;

	radiEsfera = sqrt(pow(x, 2.0)+pow(y, 2.0)+pow(z, 2.0));

}

void MyGLWidget::recalcularParamsVisio()
{
	d = radiEsfera*2;
	OBS = glm::vec3(0.0,0.0,d);
	FOVini = 2*asin(radiEsfera/d);
	FOV = 2*asin(radiEsfera/d);
	zNear = d - radiEsfera;
	zFar = d + radiEsfera;
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
  transform = glm::rotate(transform, rotateHomer, glm::vec3(0.,1.,0.));
	transform = glm::translate(transform, -centreCaixaInicial);
	centreCaixa = centreCaixaInicial - centreCaixaInicial;
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::terraAlInici () 
{
  // Matriu de transformació de model
  glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
  transform = glm::rotate(transform, 0.0f, glm::vec3(0.,1.,0.));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::projectTransform()
{
	// Matriu de transformació de la projecció
	raV = double (width())/double (height());
	if (raV > 1){
		glm::mat4 Proj = glm::perspective(FOV, raV, zNear, zFar);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
	}
	else {
		FOV = 2*atan(tan(FOVini)/raV);
		glm::mat4 Proj = glm::perspective(FOV, raV, zNear, zFar);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
	}
}

void MyGLWidget::viewTransform()
{
	glm::mat4 View = glm::lookAt(OBS, glm::vec3(0,0,0), glm::vec3(0,1,0));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  glViewport(0, 0, w, h);
	projectTransform();
}

void MyGLWidget::imprimirInfo () 
{



}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  switch (event->key()) {
    case Qt::Key_Escape:
      exit(0);
    case Qt::Key_S: { // escalar a més gran
      scale += 0.05;
      modelTransform ();
      updateGL();
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      scale -= 0.05;
      modelTransform ();
      updateGL();
      break;
		}
		case Qt::Key_R: { // escalar a més petit
      rotateHomer += M_PI/4.0;
      modelTransform ();
      updateGL();
      break;
    }
    default: event->ignore(); break;
  }
}

void MyGLWidget::createBuffers () 
{
  // Dades del terra
  // Dos VBOs, un amb posició i l'altre amb color
  glm::vec3 posterra[4] = {
	glm::vec3(-1.0, -1.0, -1.0),
	glm::vec3(-1.0, -1.0, 1.0),
	glm::vec3(1.0, -1.0, -1.0),
	glm::vec3(1.0, -1.0, 1.0)
  }; 
  glm::vec3 colterra[4] = {
	glm::vec3(0.2,0.4,0.6),
	glm::vec3(0.2,0.4,0.6),
	glm::vec3(0.2,0.4,0.6),
	glm::vec3(0.2,0.4,0.6)
  };

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_Terra);
  glBindVertexArray(VAO_Terra);

  glGenBuffers(1, &VBO_TerraPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(posterra), posterra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glGenBuffers(1, &VBO_TerraCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_TerraCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colterra), colterra, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
  
	//
	//MODEL
	//
	glGenVertexArrays(1, &VAO_model);
	glBindVertexArray(VAO_model);
  
  glGenBuffers(1, &VBO_modelPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_modelPos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_vertices(), GL_STATIC_DRAW);

	// Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

	glGenBuffers(1, &VBO_modelCol);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_modelCol);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*m.faces().size()*3*3, m.VBO_matdiff(), GL_STATIC_DRAW);

	// Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

	glBindVertexArray(0);
}

void MyGLWidget::carregaShaders() 
{
  // Creem els shaders per al fragment shader i el vertex shader
  QGLShader fs (QGLShader::Fragment, this);
  QGLShader vs (QGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
}

