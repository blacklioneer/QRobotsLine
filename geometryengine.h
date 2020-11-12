
#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QOpenGLTexture>

using namespace std;
struct VertexData
{
    QVector3D position;
      // normal
      QVector3D normal;
      // texCoords
      QVector2D texcoord;
      // tangent
      QVector3D tangent;
      // bitangent
      QVector3D bitangent;
};

struct  Texture{
    unsigned int id;
    QOpenGLTexture* albedo;
    string type;
    string path;
};

struct MeshBuffer
{
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    int vertexNum;
    int indiceNum;

    MeshBuffer() : indexBuf(QOpenGLBuffer::IndexBuffer)
    {
        arrayBuf.create();
        indexBuf.create();
    }

    ~MeshBuffer()
    {
        arrayBuf.destroy();
        indexBuf.destroy();
    }

    void Init(VertexData* vertex, int num)
    {
        vertexNum = num;
        arrayBuf.bind();
        arrayBuf.allocate(vertex, vertexNum * static_cast<int>(sizeof(VertexData)));
    }

    void Init(GLushort* indice, int num)
    {
        indiceNum = num;
        indexBuf.bind();
        indexBuf.allocate(indice, indiceNum * static_cast<int>(sizeof(GLushort)));
    }

    void bind()
    {
        arrayBuf.bind();
        indexBuf.bind();
    }
    void release()
    {
        arrayBuf.release();
        indexBuf.release();
    }
};

struct Mesh
{
    string name;
    MeshBuffer* buffer = nullptr;
    vector<VertexData> vertices;
    vector<unsigned short> indices;
    vector<Texture> textures;
    QOpenGLTexture* albedo;
    //少了一个VAO不知后面是否需要 texture也发生了改变
};

class Model
{
private:
    vector<Mesh*> vecMesh;
    string directory;
public:
    void Push(Mesh* mesh)
    {
        vecMesh.emplace_back(mesh);
    }
    MeshBuffer* GetMeshBuffer(size_t idx) { return vecMesh[idx]->buffer;}
    vector<Texture> GetMeshTexture(size_t idx){return vecMesh[idx]->textures;}
    Mesh* GetMesh(size_t idx) { return vecMesh[idx];}
    size_t Count() { return vecMesh.size();}
};

class GeometryEngine
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    bool loadObj(string path, Model*& pModel);
    void drawObj(string path,QOpenGLShaderProgram* program,bool bTess = false);
//    void drawObj(MeshBuffer* meshBuffer, QOpenGLShaderProgram* program,bool bTess = false);
    void drawObj(Mesh* mesh, QOpenGLShaderProgram* program,bool bTess = false);

    void CalTangent(VertexData& vertex0, VertexData& vertex1, VertexData& vertex2);

private:
    void processNode(const string& path, aiNode *node, const aiScene *scene,const string& directory);
    void processMesh(vector<VertexData>& vertices, vector<GLushort>& indices,vector<Texture>& textures,QOpenGLTexture*& albedo, aiMesh *mesh, const aiScene *scen, const string& directory);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName,const string& directory);
    vector<Texture> textures_loaded;
    map<string, Model> mapModel;
    unsigned int textureid;
    QOpenGLTexture* TextureFromFile(const char *path, const string &directory);
};

#endif // GEOMETRYENGINE_H
