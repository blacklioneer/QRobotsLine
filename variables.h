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

struct OjFind
{
    string findname;
    OjFind(string findID) :findname(findID) {}
    bool operator()(const ObjectData&t1)
    {
        return t1.name == findname;
    }
};

//struct StringType{
//    string type;
//};

extern vector<string> g_parent_type;
extern vector<ObjectData> g_objects;
