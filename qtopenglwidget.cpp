#include "qtopenglwidget.h"
#include <QDebug>
#include <QTimer>
#include <variables.h>
#include<iostream>
using namespace std;

QtOpenglWidget::QtOpenglWidget(QWidget *parent) : QOpenGLWidget (parent),
    VBO(QOpenGLBuffer::VertexBuffer)
{
    camera = std::make_unique<Camera>(QVector3D(5.0f, 0.0f, 10.0f));
    m_bLeftPressed = false;
    geometryengine = new GeometryEngine;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, [=]{
        m_nTimeValue += 1;
        update();
    });
    m_pTimer->start(40);
}

QtOpenglWidget::~QtOpenglWidget(){
    makeCurrent();
    delete  geometryengine;
    VBO.destroy();
    VAO.destroy();


    doneCurrent();
}

void QtOpenglWidget::initializeGL(){
    this->initializeOpenGLFunctions();

    bool success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/modelloading.vert");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram.log();
        return;
    }

    success = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/modelloading.frag");
    if (!success) {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shaderProgram.log();
        return;
    }

    success = shaderProgram.link();
    if(!success) {
        qDebug() << "shaderProgram link failed!" << shaderProgram.log();
    }

    success = Lightshader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/lamp.vert");
    if (!success) {
        qDebug() << "Lightshader addShaderFromSourceFile failed!" << Lightshader.log();
        return;
    }

    success = Lightshader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/lamp.frag");
    if (!success) {
        qDebug() << "Lightshader addShaderFromSourceFile failed!" << Lightshader.log();
        return;
    }

    success = Lightshader.link();
    if(!success) {
        qDebug() << "Lightshader link failed!" << Lightshader.log();
    }


    //VAO，VBO data
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.1f,  0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.1f, -0.1f, -0.1f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.1f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.1f,  0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.1f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.1f, -0.1f, -0.1f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.1f, -0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.1f,  0.1f,  0.1f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.1f,  0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.1f, -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.1f, -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.1f,  0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.1f, -0.1f,  0.1f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.1f, -0.1f, -0.1f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.1f,  0.1f,  0.1f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.1f,  0.1f, -0.1f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    QOpenGLVertexArrayObject::Binder vaoBind(&VAO);

    VBO.create();
    VBO.bind();
    VBO.allocate(vertices, sizeof(vertices));

    // position attribute
    Lightshader.bind();   // don't forget to activate/use the shader before setting uniforms!
    int attr = -1;
    attr = Lightshader.attributeLocation("aPos");
    Lightshader.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 8);
    Lightshader.enableAttributeArray(attr);
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    VBO.release();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
}

void QtOpenglWidget::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
}

static QVector3D cubePositions[] = {
  QVector3D(0.7f,  0.2f,  2.0f),
  QVector3D(2.3f, -3.3f, -4.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-4.0f,  2.0f, -12.0f),
  QVector3D( 2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
  QVector3D( 1.3f, -2.0f, -2.5f),
  QVector3D( 1.5f,  2.0f, -2.5f),
  QVector3D( 1.5f,  0.2f, -1.5f),
  QVector3D(-1.3f,  1.0f, -1.5f)
};

void QtOpenglWidget::paintGL(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    camera->processInput(1.0f);
     shaderProgram.bind();

    // bind textures on corresponding texture units
    shaderProgram.setUniformValue("dirLight.direction",QVector3D(-0.2f, -1.0f, -0.3f));
    shaderProgram.setUniformValue("dirLight.ambient", QVector3D(0.05f, 0.05f, 0.05f));
    shaderProgram.setUniformValue("dirLight.diffuse",QVector3D(0.4f, 0.4f, 0.4f));
    shaderProgram.setUniformValue("dirLight.specular", QVector3D(0.1f, 0.1f, 0.1f));
    //新增3.23为实现光影反射而增加

    // point light 1
    shaderProgram.setUniformValue("pointLights[0].position", cubePositions[0]);
    shaderProgram.setUniformValue("pointLights[0].ambient", QVector3D(0.05f, 0.05f, 0.05f));
    shaderProgram.setUniformValue("pointLights[0].diffuse", QVector3D(0.8f, 0.8f, 0.8f));
    shaderProgram.setUniformValue("pointLights[0].specular", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniformValue("pointLights[0].constant", 1.0f);
    shaderProgram.setUniformValue("pointLights[0].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[0].quadratic", 0.032f);
    // point light 2
    shaderProgram.setUniformValue("pointLights[1].position", cubePositions[1]);
    shaderProgram.setUniformValue("pointLights[1].ambient", QVector3D(0.05f, 0.05f, 0.05f));
    shaderProgram.setUniformValue("pointLights[1].diffuse", QVector3D(0.8f, 0.8f, 0.8f));
    shaderProgram.setUniformValue("pointLights[1].specular", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniformValue("pointLights[1].constant", 1.0f);
    shaderProgram.setUniformValue("pointLights[1].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[1].quadratic", 0.032f);
    // point light 3
    shaderProgram.setUniformValue("pointLights[2].position", cubePositions[2]);
    shaderProgram.setUniformValue("pointLights[2].ambient", QVector3D(0.05f, 0.05f, 0.05f));
    shaderProgram.setUniformValue("pointLights[2].diffuse", QVector3D(0.8f, 0.8f, 0.8f));
    shaderProgram.setUniformValue("pointLights[2].specular", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniformValue("pointLights[2].constant", 1.0f);
    shaderProgram.setUniformValue("pointLights[2].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[2].quadratic", 0.032f);
    // point light 4
    shaderProgram.setUniformValue("pointLights[3].position", cubePositions[3]);
    shaderProgram.setUniformValue("pointLights[3].ambient", QVector3D(0.05f, 0.05f, 0.05f));
    shaderProgram.setUniformValue("pointLights[3].diffuse", QVector3D(0.8f, 0.8f, 0.8f));
    shaderProgram.setUniformValue("pointLights[3].specular", QVector3D(1.0f, 1.0f, 1.0f));
    shaderProgram.setUniformValue("pointLights[3].constant", 1.0f);
    shaderProgram.setUniformValue("pointLights[3].linear", 0.09f);
    shaderProgram.setUniformValue("pointLights[3].quadratic", 0.032f);

    // spotLight
//    ourShader.setVec3("spotLight.position", camera->position);
//    ourShader.setVec3("spotLight.direction", camera->front);
//    ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//    ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
//    ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//    ourShader.setFloat("spotLight.constant", 1.0f);
//    ourShader.setFloat("spotLight.linear", 0.09);
//    ourShader.setFloat("spotLight.quadratic", 0.032);
//    ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//    ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    // material properties
    shaderProgram.setUniformValue("material.shininess", 64.0f);
    // view/projection transformations
//    QMatrix4x4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    QMatrix4x4 projection;
    projection.perspective(camera->zoom, 1.0f * width() / height(), 0.1f, 100.f);
    shaderProgram.setUniformValue("projection", projection);
    shaderProgram.setUniformValue("view", camera->getViewMatrix());

    // render the loaded model
    QMatrix4x4 model ;
//    model.translate(QVector3D(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
//    model.scale(0.2f);	// it's a bit too big for our scene, so scale it down
//    shaderProgram.setUniformValue("model", model);
    for(vector<ObjectData>::iterator it=g_objects.begin();it!=g_objects.end();it++){
        model = QMatrix4x4() ;
        model.translate((*it).primary_position);
        model.rotate((*it).primary_angle_x,QVector3D(1.0f,0.0f,0.0f));
        model.rotate((*it).primary_angle_y,QVector3D(0.0f,1.0f,0.0f));
        model.rotate((*it).primary_angle_z,QVector3D(0.0f,0.0f,1.0f));
       // model.translate(QVector3D(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        model.scale(0.2f);	// it's a bit too big for our scene, so scale it down
        shaderProgram.setUniformValue("model", model);
        geometryengine->drawObj((*it).path,&shaderProgram,false);
    }
//    geometryengine->drawObj("C:/QTwindows/QTOGtest/objects/nanosuit/nanosuit.obj",
//                            &shaderProgram,false);
//    geometryengine->drawObj("C:/QTwindows/QTOGtest/objects/45 ACP Smith and Wesson Hand Gun/Handgun_obj.obj",
//                            &shaderProgram,false);
    shaderProgram.release();

    {// render light
        Lightshader.bind();
        Lightshader.setUniformValue("projection", projection);
        Lightshader.setUniformValue("view", camera->getViewMatrix());
        QOpenGLVertexArrayObject::Binder vaoBind(&VAO);
        for (unsigned int i = 0; i < 4; i++)
            {
                    model = QMatrix4x4() ;
                   if (i == 0) {
                       model.translate(QVector3D(0.0f, 0.0f, 0.0f));
                       model.rotate(0, QVector3D(0.0f, 1.0f, 0.0f));
                   }
                model.translate(cubePositions[i]);
                float angle = (i + 1.0f) * m_nTimeValue;
                model.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));

                model.scale(0.2f); // Make it a smaller cube
                Lightshader.setUniformValue("model", model);;
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
    }


    Lightshader.release();
}

void QtOpenglWidget::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        camera->keys[key] = true;
}

void QtOpenglWidget::keyReleaseEvent(QKeyEvent *event)
{
    int key = event->key();
    if (key >= 0 && key < 1024)
        camera->keys[key] = false;
}

void QtOpenglWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_bLeftPressed = true;
        m_lastPos = event->pos();
    }
}

void QtOpenglWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    m_bLeftPressed = false;
}

void QtOpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bLeftPressed) {
        int xpos = event->pos().x();
        int ypos = event->pos().y();

        int xoffset = xpos - m_lastPos.x();
        int yoffset = m_lastPos.y() - ypos;
        m_lastPos = event->pos();
        camera->processMouseMovement(xoffset, yoffset);
    }
}

void QtOpenglWidget::wheelEvent(QWheelEvent *event)
{
    QPoint offset = event->angleDelta();
    camera->processMouseScroll(offset.y()/20.0f);
}
