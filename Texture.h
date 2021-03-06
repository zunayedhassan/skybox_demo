#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <fstream>

#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

using std::string;
using std::vector;
using std::ifstream;

struct TGA_Header
{
    GLubyte     ID_Length;
    GLubyte     ColorMapType;
    GLubyte     ImageType;
    GLubyte     ColorMapSpecification[5];
    GLshort     xOrigin;
    GLshort     yOrigin;
    GLshort     ImageWidth;
    GLshort     ImageHeight;
    GLubyte     PixelDepth;
};

class Texture
{
    // Member functions
    public:
        Texture(string in_filename, string in_name = "");
        ~Texture();

    // Variables
    public:
        unsigned char* imageData;
        unsigned int   bpp;
        unsigned int   width;
        unsigned int   height;
        unsigned int   texID;
        string         name;

        static vector<Texture*> textures;

    private:
        bool loadTGA(string filename);
        bool createTexture(unsigned char* imageData, int width, int height, int type);

};

#endif // TEXTURE_H
