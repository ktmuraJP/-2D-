#ifndef SRC_H
#define SRC_H
#include<GL/gl3w.h>
#include<GLFW/glfw3.h>
#include<stb_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
unsigned int SetupTexture(const char* imgpath,
        int *img_width, int *img_height,bool flip);
std::string loadfile(const char *path);
#endif
