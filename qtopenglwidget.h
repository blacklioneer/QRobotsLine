#ifndef QTOPENGLWIDGET_H
#define QTOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include "tools/camera.h"
#include "geometryengine.h"

class QtOpenglWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    QtOpenglWidget(QWidget *parent = nullptr);
    ~QtOpenglWidget() Q_DECL_OVERRIDE;

protected:
    virtual void initializeGL() Q_DECL_OVERRIDE;
    virtual void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    virtual void paintGL() Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void pointLightSetting();

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLShaderProgram Lightshader;
    QOpenGLBuffer VBO;
    GeometryEngine* geometryengine;
    QOpenGLVertexArrayObject VAO;

    QTimer* m_pTimer = nullptr;
    int     m_nTimeValue = 0;

    // camera
    std::unique_ptr<Camera> camera;
//    std::unique_ptr<Model> ourmodel;
    Model* ourmodel;
    bool m_bLeftPressed;
    QPoint m_lastPos;
};

#endif // QTOPENGLWIDGET_H
