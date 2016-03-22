#include "model.h"

Model::Model(string fileNamePrefix, string modelName, bool isNormalEnable)
{
    this->Textures = new Texture(fileNamePrefix + ".tga", modelName);
    this->vertices = new vector<Vertex*>;

    this->isNormalEnable = isNormalEnable;

    CommonTools *commonTools = new CommonTools();
    fstream *verticesFile = commonTools->GetFileStream(fileNamePrefix + ".txt");

    string currentLine = "";

    while (!verticesFile->eof())
    {
        getline(*verticesFile, currentLine);

        vector<string> *currentPointTexts = commonTools->GetSplittedStrings(currentLine, ' ');

        float x = commonTools->GetNumberFromString(commonTools->Trim(currentPointTexts->at(0)));
        float y = commonTools->GetNumberFromString(commonTools->Trim(currentPointTexts->at(1)));
        float z = commonTools->GetNumberFromString(commonTools->Trim(currentPointTexts->at(2)));

        Vertex *currentPoint = new Vertex(x, y, z);

        this->vertices->push_back(currentPoint);
    }

    verticesFile->close();
}

void Model::Draw()
{
    glBindTexture(GL_TEXTURE_2D, this->Textures->texID);

    glPushMatrix();

    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLES);

    int trianglePoint = 0;
    int textureCoordIndex = 0;
    Vertex currentTriangle[3] = NULL;

    for (vector<Vertex*>::iterator vertex = this->vertices->begin(); vertex < this->vertices->end(); ++vertex)
    {
        currentTriangle[++trianglePoint - 1] = *(*vertex);

        if (trianglePoint == 3)
        {
            trianglePoint = 0;

            if (this->isNormalEnable)
            {
                float *normal = this->GetNormal(currentTriangle[0].GetArray(), currentTriangle[1].GetArray(), currentTriangle[2].GetArray());
                glNormal3f(-normal[0], -normal[1], -normal[2]);
            }

            for (int index = 0; index < 3; index++)
            {
                glTexCoord2f(this->textureCoords[textureCoordIndex].X, this->textureCoords[textureCoordIndex].Y);
                glVertex3f(currentTriangle[index].X, currentTriangle[index].Y, currentTriangle[index].Z);

                if (++textureCoordIndex == 6)
                {
                    textureCoordIndex = 0;
                }
            }
        }
    }

    glEnd();

    glPopMatrix();
}

float* Model::GetNormal(float *coord1, float *coord2, float *coord3)
{
    // calculate Vector1 and Vector2
    float va[3], vb[3], vr[3];

    va[0] = coord1[0] - coord2[0];
    va[1] = coord1[1] - coord2[1];
    va[2] = coord1[2] - coord2[2];

    vb[0] = coord1[0] - coord3[0];
    vb[1] = coord1[1] - coord3[1];
    vb[2] = coord1[2] - coord3[2];

    // cross product
    vr[0] = va[1] * vb[2] - vb[1] * va[2];
    vr[1] = vb[0] * va[2] - va[0] * vb[2];
    vr[2] = va[0] * vb[1] - vb[0] * va[1];

    // normalization factor
    float val = sqrt(pow(vr[0], 2) + pow(vr[1], 2) + pow(vr[2], 2));

    float norm[3];

    norm[0] = vr[0] / val;
    norm[1] = vr[1] / val;
    norm[2] = vr[2] / val;

    return norm;
}

Model::~Model()
{

}

