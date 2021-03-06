#include <QGLWidget>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <QGLShader>
#include <QGLShaderProgram>
#include <QKeyEvent>

#define GLM_FORCE_RADIANS

class MyGLWidget : public QGLWidget 
{
  Q_OBJECT

  public:
    MyGLWidget (QGLFormat &f, QWidget *parent=0);
  
  protected:
    // initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resizeGL() - Es cridat quan canvi la mida del widget
    virtual void resizeGL (int width, int height);

    virtual void keyPressEvent(QKeyEvent *e);

  private:
    void createBuffers ();
    void initShaders ();
    void modelTransform();

    QGLShader *fs, *vs;
    QGLShaderProgram *program;

    int varLoc;
    GLuint transLoc;

    float scl;

    glm::mat4 TG;

    GLuint VAO, VBO, VBOc;
};
