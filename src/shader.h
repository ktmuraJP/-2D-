#ifndef SHADERCLASS_H
#define SHADERCLASS_H
#include "src.h"
#include <vector>
#include <string>
class ShaderClass
{
public:
    unsigned int ID;

    ShaderClass(
            const char* vertexPath, 
            const char* fragmentPath, 
            const char* geometryPath = nullptr)
    {

        unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        
        int Result = GL_FALSE;
        int InfoLogLength;
        
        //printf("compiling vertex shader ... ");
        std::string vertexCode = loadfile(vertexPath);
        const char *VertexSourcePointer = vertexCode.c_str();
        glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
        glCompileShader(VertexShaderID);

        glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

        char VertexShaderErrorMessage[InfoLogLength];
        glGetShaderInfoLog(VertexShaderID, 
                InfoLogLength, 
                NULL, 
                VertexShaderErrorMessage);

        if (Result == GL_FALSE) {
            printf("\033[91mvertex compile error\033[0m\n");
            printf("\033%s\033[0m\n",vertexPath);
            printf("%s\n", VertexShaderErrorMessage);
            printf("\033[92m%s\033[0m", VertexSourcePointer);

        } else {
            //printf("success\n");
        }

        
        //printf("compiling fragment shader ... ");
        std::string fragmentCode = loadfile(fragmentPath);
        const char *FragmentSourcePointer = fragmentCode.c_str();
        glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
        glCompileShader(FragmentShaderID);

        glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
        glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

        char FragmentShaderErrorMessage[InfoLogLength];
        glGetShaderInfoLog(FragmentShaderID, 
                InfoLogLength, 
                NULL, 
                FragmentShaderErrorMessage);
        if (Result == GL_FALSE) {
            printf("\033[91mfragment compile error\033[0m\n");
            printf("\033%s\033[0m\n",fragmentPath);
            printf("%s\n", FragmentShaderErrorMessage);
            printf("\033[92m%s\033[0m", FragmentSourcePointer);
        } else {
            //printf("success\n");
        }

        unsigned int GeometryShaderID;
        if(geometryPath != NULL)
        {
            std::string geometryCode = loadfile(geometryPath);
            //printf("compiling geometry shader ... ");
            GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
            const char *GeometrySourcePointer = geometryCode.c_str();
            glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer , NULL);
            glCompileShader(GeometryShaderID);

            glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
            glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

            char GeometryShaderErrorMessage[InfoLogLength];
            glGetShaderInfoLog(GeometryShaderID, 
                    InfoLogLength, 
                    NULL, 
                    GeometryShaderErrorMessage);
            if (Result == GL_FALSE) {
                printf("\033[91mgeometry compile error\033[0m\n");
                printf("\033%s\033[0m\n",geometryPath);
                printf("%s\n", GeometryShaderErrorMessage);
            } else {
                //printf("success\n");
            }
            
        }

        //printf("linking program ... ");
        ID = glCreateProgram();
        glAttachShader(ID, VertexShaderID);
        glAttachShader(ID, FragmentShaderID);
        if(geometryPath != NULL)
            glAttachShader(ID, GeometryShaderID);

        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &Result);
        glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &InfoLogLength);

        char ProgramErrorMessage[(InfoLogLength>1)? InfoLogLength : 1];
        glGetProgramInfoLog(ID, 
                InfoLogLength, 
                NULL, 
                ProgramErrorMessage);
        if (Result == GL_FALSE) {
            printf("%s\n", ProgramErrorMessage);
        } else
            //printf("success\n");

        glDeleteShader(VertexShaderID);
        glDeleteShader(FragmentShaderID);
        if(geometryPath!=nullptr)
            glDeleteShader(GeometryShaderID);
    }

    void use() 
    { 
        glUseProgram(ID); 
    }

    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }

    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};

#endif
