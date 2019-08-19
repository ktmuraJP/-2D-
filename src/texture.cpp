#include "src.h" 
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
unsigned int ScreenToTexture(int width, int height)
{
    unsigned int texture;
    unsigned char *data;
    GLenum format;
    glGenTextures(1, &texture);
    format = GL_RGB;
    
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = ( GLubyte* )malloc( width * height * 3 );

    glReadBuffer( GL_BACK );
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadPixels(0,0,width,height,format,GL_UNSIGNED_BYTE,data);

    glTexImage2D(GL_TEXTURE_2D, 0, 
            format, width, height, 0, 
            format, GL_UNSIGNED_BYTE, data);  
    glGenerateMipmap(GL_TEXTURE_2D);
    free(data);
    return texture;
}
unsigned int SetupTexture(const char* imgpath, 
        int *img_width, int *img_height,
        bool flip=false)
{
    unsigned int texture;
    unsigned char *data;
    int width, height, nrChannels;
    GLenum format;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    stbi_set_flip_vertically_on_load(flip);

    data = stbi_load(imgpath,
            &width, &height, &nrChannels, 0);
    *img_width = width;
    *img_height = height;
    
    if (data){
        if(nrChannels == 1)
            format = GL_RED;
        else if(nrChannels == 3)
            format = GL_RGB;
        else if(nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, 
                format, width, height, 0, 
                format, GL_UNSIGNED_BYTE, data);  
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    }else{
        //printf("%s\n",imgpath);
        printf("texture load error\n");
    }
    
    return texture;
}
