#include "shader.h"
#include "src.h"

int window_w = 800;
int window_h = 600;

class TextClass
{
public:
    FT_Library ft;
    FT_Face face;
     
    TextClass(std::string path)
    {
        if(FT_Init_FreeType(&ft))
            printf("freetype initialize fail\n");
        if(FT_New_Face(ft, path.c_str(), 0, &face))
            printf("freetype initialize fail\n");
        FT_Set_Pixel_Sizes(face, 0, 48);  

    }
};

class MovableObjectClass
{
public:
    
    int display_w,display_h;
    GLFWwindow *window;
    
    unsigned int texture;
    int width,height;
    
    unsigned int VBO,VAO,EBO;
    

    glm::vec3 position;

    ShaderClass shader{"shader/objectvs.glsl","shader/objectfs.glsl"};
    
    MovableObjectClass(std::string path)
    {
        float vertices[] = {
             1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
             1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f, // bottom left
            -1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f  // top left
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
                8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
                8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 
                8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        texture = SetupTexture(path.c_str(),&width,&height,true);
    }
    float k = 1.0f;
    float kx = 0.0f;
    float ky = 0.0f;
    float px = 1.0f;
    float py = 1.0f;
    void Draw()
    {
        float aspectratio = (float)display_w / (float)display_h;
        glm::mat4 projection = glm::ortho(
                -aspectratio/2, aspectratio/2, 
                -aspectratio/2, aspectratio/2);
        glm::mat4 model = glm::mat4(1.0f);
        printf("\rx = %f, y = %f    px = %f py = %f",kx,ky,px,py);
        
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            k+=0.01;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            k-=0.01;

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            kx+=0.01;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            kx-=0.01;
        
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            ky+=0.01;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            ky-=0.01;
        
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            px+=0.01;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            py+=0.01;
        
        model = glm::scale(model,glm::vec3(px,py*aspectratio,0.0f));
        model = glm::scale(model,glm::vec3(1.0f));
        model = glm::translate(model,glm::vec3(kx,ky,0.0f));
        model = glm::scale(model,glm::vec3(k));
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.use();
        shader.setMat4("projection",projection);
        shader.setMat4("model",model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};
void mainloop(GLFWwindow* window)
{
    int display_w,display_h;
    TextClass text{"./lib/fonts/Arial.ttf"};
    MovableObjectClass object{"container.jpg"};
    object.window = window;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.5f,0.5f,0.5f,1.0f);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

        }

        {
            object.display_w = display_w;
            object.display_h = display_h;
            object.Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int WindowContext()
{

    if (!glfwInit()){
        printf("glfw initialize fail\n");
        return 1;
    }
    if(!gl3wInit()){
        printf("gl3w initialize fail\n");
        return 1;
    }
    

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Decide GL+GLSL versions

    // GL 3.0 + GLSL 130
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // 3.0+ only

    GLFWwindow* window = glfwCreateWindow(window_w, 
            window_h, "test", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    mainloop(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

int main()
{
    // Setup window
    if(WindowContext() == 1)
        return 1;
    // Setup Dear ImGui context
    return 0;
}
