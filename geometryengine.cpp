#include <QOpenGLExtraFunctions>
#include "geometryengine.h"
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <iostream>


GeometryEngine::GeometryEngine():textureid(0)
{

}

GeometryEngine::~GeometryEngine()
{

}

bool GeometryEngine::loadObj(string path, Model*& pModel)
{
    if(mapModel.find(path) != mapModel.end())
    {
        return true;
    }

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString() ;
        return false;
    }
     string directory = path.substr(0, path.find_last_of('/'));

    processNode(path, scene->mRootNode, scene,directory);
    pModel = &mapModel[path];
    return true;
}

void GeometryEngine::drawObj(string path,QOpenGLShaderProgram* program,bool bTess)
{
    Model* pModel;
    if(mapModel.find(path) == mapModel.end()) //如果不在已加载库中存在，加载并绘制
    {
        if(!loadObj(path, pModel))
        {
            return;
        }
    }

    auto vecMesh = mapModel[path];
    for(size_t i = 0;i < vecMesh.Count();i++)
    {
        //auto meshBuffer = vecMesh.GetMeshBuffer(i);
       // auto textures = vecMesh.GetMeshTexture(i);
        auto mesh = vecMesh.GetMesh(i);
        drawObj(mesh,program, bTess);
    }
}

void GeometryEngine::drawObj(Mesh* mesh, QOpenGLShaderProgram* program,bool bTess)
{
    mesh->buffer->bind();

    auto gl = QOpenGLContext::currentContext()->extraFunctions();

    //int offset = 0;
    int vertexLocation = program->attributeLocation("aPos");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

    int normalLocation = program->attributeLocation("aNormal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offsetof(VertexData, normal), 3, sizeof(VertexData));

    int texcoordLocation = program->attributeLocation("aTexCoords");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offsetof(VertexData, texcoord), 2, sizeof(VertexData));

    //offset += sizeof(QVector3D);

    int tangentLocation = program->attributeLocation("a_Tangent");
    program->enableAttributeArray(tangentLocation);
    program->setAttributeBuffer(tangentLocation, GL_FLOAT, offsetof(VertexData, tangent), 3, sizeof(VertexData));

   // offset += sizeof(QVector3D);

    //offset += sizeof(QVector3D);

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < mesh->textures.size(); i++)
    {
        string number;
        string name = mesh->textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to stream
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to stream
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to stream

        // now set the sampler to the correct texture unit
        //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        //改为加入灯光设置的显示
        program->setUniformValue(("material." + name + number).c_str(), i);  
        // and finally bind the texture
        mesh->textures[i].albedo->bind(i);
    }
    if(bTess)
    {
        gl->glPatchParameteri(GL_PATCH_VERTICES, 3);
        gl->glDrawElements(GL_PATCHES, mesh->buffer->indiceNum, GL_UNSIGNED_SHORT, nullptr);
    }
    else
    {
        gl->glDrawElements(GL_TRIANGLES,  mesh->buffer->indiceNum, GL_UNSIGNED_SHORT, nullptr);
    }
    mesh->buffer->release();
    for (unsigned int i = 0; i < mesh->textures.size(); i++)
    {
        mesh->textures[i].albedo->release(i);
    }
}



void GeometryEngine::processMesh(vector<VertexData>& vertices, vector<GLushort>& indices, vector<Texture>& textures, QOpenGLTexture*& albedo, aiMesh *mesh, const aiScene *scene,const string& directory)
{
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData vertex;
        if(mesh->mVertices)
        {
            vertex.position = QVector3D(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z);
        }
        if(mesh->mNormals)
        {
            vertex.normal = QVector3D(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z);
            //vertex.normal.normalized(); //y有问题
        }
        if(mesh->mTextureCoords[0])
        {
            vertex.texcoord = QVector2D(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y);

        }
        if(mesh->mTangents)
        {
            vertex.tangent = QVector3D(mesh->mTangents[i].x,mesh->mTangents[i].y,mesh->mTangents[i].z);
        }
        if(mesh->mBitangents)
        {
            vertex.bitangent = QVector3D(mesh->mBitangents[i].x,mesh->mBitangents[i].y,mesh->mBitangents[i].z);
        }
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(static_cast<GLushort>(face.mIndices[j]));
        }
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//    for(unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
//    {
//        aiString str;
//        material->GetTexture(aiTextureType_DIFFUSE, i, &str);
//        albedo = CResourceInfo::Inst()->CreateTexture(str.C_Str());
//    }
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse",directory);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular",directory);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal",directory);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height",directory);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
}

vector<Texture> GeometryEngine::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName , const string& directory)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.id = textureid++;
                texture.albedo = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);// store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
}

void GeometryEngine::processNode(const string& path, aiNode *node, const aiScene *scene,const string& directory)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        vector<VertexData> vertices;
        vector<GLushort> indices;
        vector<Texture> textures;
        QOpenGLTexture* albedo = nullptr;
        aiMesh *aimesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(vertices, indices, textures, albedo, aimesh, scene,directory);

        MeshBuffer* meshBuffer = new MeshBuffer();
//        for(size_t i = 0;i < indices.size() / 3; i++)
//        {
//            CalTangent(vertices[indices[3 * i]],vertices[indices[3 * i + 1]],vertices[indices[3 * i + 2]]);
//        }

        meshBuffer->Init(vertices.data(),static_cast<int>(vertices.size()));
        meshBuffer->Init(indices.data(),static_cast<int>(indices.size()));

        Mesh* mesh = new Mesh();
        mesh->name = node->mName.C_Str();
        mesh->buffer = meshBuffer;
        mesh->albedo = albedo;
        mesh->vertices = vertices;
        mesh->indices = indices;
        mesh->textures = textures;
        mapModel[path].Push(mesh);
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(path, node->mChildren[i], scene, directory);
    }
}


void GeometryEngine::CalTangent(VertexData& vertex0, VertexData& vertex1, VertexData& vertex2)
{
    float u0 = vertex0.texcoord.x();
    float v0 = vertex0.texcoord.y();

    float u1 = vertex1.texcoord.x();
    float v1 = vertex1.texcoord.y();

    float u2 = vertex2.texcoord.x();
    float v2 = vertex2.texcoord.y();

    float t1 = u1 - u0;
    float b1 = v1 - v0;

    float t2 = u2 - u0;
    float b2 = v2 - v0;

    QVector3D e0 = vertex1.position - vertex0.position;
    QVector3D e1 = vertex2.position - vertex0.position;

    float k = t1 * b2 - b1 * t2;

    QVector3D tangent;
    tangent = k * QVector3D(b2 * e0.x() - b1 * e1.x(),b2 * e0.y() - b1 * e1.y(),b2 * e0.z() - b1 * e1.z());

    QVector<VertexData*> vertexArr = { &vertex0, &vertex1, &vertex2};
    QVector<int> adjoinPlane;
    adjoinPlane.resize(vertexArr.size());
    for(int i = 0;i < vertexArr.size();i++)
    {
        adjoinPlane[i]++;
        float ratio = 1.0f / adjoinPlane[i];
        vertexArr[i]->tangent = vertexArr[i]->tangent * (1 - ratio) + tangent * ratio;
        vertexArr[i]->tangent.normalize();
    }
}
QOpenGLTexture* GeometryEngine:: TextureFromFile(const char *path, const string &directory)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    QOpenGLTexture* texture =new QOpenGLTexture(QImage(filename.c_str()).mirrored(), QOpenGLTexture::GenerateMipMaps);
    if (texture->isCreated())
    {
        texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);// 等于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);   //等价于glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;

    }
    return texture;
}
