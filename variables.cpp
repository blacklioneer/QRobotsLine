#include "variables.h"

#include <vector>
using namespace std;
vector<ObjectData> g_objects;
vector<string> g_parent_type;
vector<PointLights> g_pointlights;
DirLights dirlights = {QVector3D(-0.2f, -1.0f, -0.3f),
                       QVector3D(0.05f, 0.05f, 0.05f),
                       QVector3D(0.4f, 0.4f, 0.4f),
                       QVector3D(0.1f, 0.1f, 0.1f)
};
//PointLights pointlights = {\
//    QVector3D(1.0f, 1.0f,1.0f),\
//    QVector3D(0.05f, 0.05f, 0.05f),\
//    QVector3D(0.8f, 0.8f, 0.8f),\
//    QVector3D(1.0f, 1.0f, 1.0f),\
//    1.0f,\
//    0.09f,\
//    0.032f\
//};
