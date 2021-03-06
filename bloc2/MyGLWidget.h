#define GLM_FORCE_RADIANS
#include <QGLWidget>
#include <QGLShader>
#include <QGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model/Model.h"

class MyGLWidget : public QGLWidget {
  Q_OBJECT

  public:
    MyGLWidget (QGLFormat &f, QWidget *parent=0);

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    void projectTransform ();
		void viewTransform ();
		void terraAlInici ();
    void buscaExtremsICentreCaixa ();
		void recalcularParamsVisio ();
		void imprimirInfo ();

    // attribute locations
    GLuint vertexLoc, colorLoc, projLoc, viewLoc;
    // uniform locations
    GLuint transLoc;

    // VAO i VBO names
    GLuint VAO_Casa, VBO_CasaPos, VBO_CasaCol;
    GLuint VAO_Terra, VBO_TerraPos, VBO_TerraCol;
		GLuint VAO_model, VBO_modelPos, VBO_modelCol;
    // Program
    QGLShaderProgram *program;

		glm::vec3 OBS;

    // Internal vars
    float scale;
		float rotateHomer;
    glm::vec3 pos;
		double raV;
		double FOVini;
		double FOV;

		//Caixa contenidora
		glm::vec3 maxVert;
		glm::vec3 minVert;
		glm::vec3 centreCaixaInicial;
		glm::vec3 centreCaixa;
		double radiEsfera;
		double d;
		double zNear;
		double zFar;
		

		Model m;
};

