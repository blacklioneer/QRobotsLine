#pragma once
#include <vector>
#include <QOpenGLShaderProgram>
using namespace std;


struct ObjectData{
    string name;
    string path;//文件路径
    string parent_type;//父类
    string higher_control;//上级控制对象
    bool isLight;
    QVector3D primary_position; //位置
    float primary_angle_x;
    float primary_angle_y;
    float primary_angle_z;
    QVector3D move_position; //位置
    float move_angle_x;
    float move_angle_y;
    float move_angle_z;

};

struct PointLights {
    string name;
    /** 点光源位置*/
    QVector3D position;
        /** 环境光分量*/
    QVector3D ambient;
        /** 漫反射分量*/
    QVector3D diffuse;
        /** 镜面高光(Specular Highlight)*/
    QVector3D specular;
    /** 常数项通常保持为1.0，它的主要作用是保证分母永远不会比1小，否则的话在某些距离上它反而会增加强度，这肯定不是我们想要的效果。**/
    float constant;
    /** 一次项会与距离值相乘，以线性的方式减少强度。*/
    float linear;
    /** 二次项会与距离的平方相乘，让光源以二次递减的方式减少强度。二次项在距离比较小的时候影响会比一次项小很多，但当距离值比较大的时候它就会比一次项更大了。 */
    float quadratic;
    /** 说明见 https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/ */
};


struct DirLights {
    QVector3D direction; // 环境平行光光照方向
    QVector3D ambient;  // 环境分量
    QVector3D diffuse;  // 漫反射分量
    QVector3D specular;  //镜面高光(Specular Highlight)
};


struct OjFind
{
    string findname;
    OjFind(string findID) :findname(findID) {}
    bool operator()(const ObjectData&t1)
    {
        return t1.name == findname;
    }
};

struct PLFind
{
    string findname;
    PLFind(string findID) :findname(findID) {}
    bool operator()(const PointLights&t1)
    {
        return t1.name == findname;
    }
};

//struct StringType{
//    string type;
//};

extern vector<string> g_parent_type; //绘制单元
/** 绘制单元组合*/
extern vector<ObjectData> g_objects;
/** 点光源组合*/
extern vector<PointLights> g_pointlights;
/** 全局环境光设置，需要一个输入组件*/
extern DirLights dirlights;

/** 初始化光源 光源窗口界面函数构造用 */
extern PointLights InitLight ;
/** 初始化绘制对象 模型界面窗口界面函数构造用 */
extern ObjectData InitObject ;
