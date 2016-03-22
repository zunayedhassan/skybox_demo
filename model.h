#ifndef MODEL_H
#define MODEL_H

#include "CommonTools.h"
#include "Texture.h"
#include "vertex.h"

class Model
{
private:
    vector<Vertex*> *vertices = nullptr;

    Vertex textureCoords[6] = {
        Vertex(0, 0),
        Vertex(0, 1),
        Vertex(1, 1),
        Vertex(1, 1),
        Vertex(1, 0),
        Vertex(0, 0)
    };

    bool isNormalEnable = true;

public:
    Texture *Textures = nullptr;

    Model(string fileNamePrefix, string modelName = "", bool isNormalEnable = true);
    void Draw();
    float* GetNormal(float *coord1, float *coord2, float *coord3);
    ~Model();
};

#endif // MODEL_H
