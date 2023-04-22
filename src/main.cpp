#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"
#include "learnopengl/camera.h"
#include "Cylinder.h"
#include "Sphere.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(std::vector<std::string> faces);
unsigned int cylinderVAO, cylinderVBO, cylinderEBO;
unsigned int pyramidVAO, pyramidVBO;
// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Plantilla5 del Sabado 22-04-2023, Prof. Jesus Becerra, Comp. Grafica LUZ-IUTA 2023", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("2.1.basic_lighting.vs", "2.1.basic_lighting.fs");
    Shader lightCubeShader("2.1.light_cube.vs", "2.1.light_cube.fs");
    Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float verticesGeneral[] = {
        //Positions           //Normals            //Colors

        //Cubo: cara de atras (0 - 5):
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        //Cubo: cara de adelante (6 - 11):
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

        //Cubo: cara de la izquierda (12 - 17):
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        //Cubo: cara de la derecha (18 - 23):
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        //Cubo: cara de abajo (24 - 29):
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

        //Cubo: cara de arriba (30 - 35):
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0,  0.0f,  0.0f, 1.0f,

        //Plano marron levantado (hay que acostarlo para usarlo como suelo) (36 - 39):
        -1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.58f,  0.32f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.58f,  0.32f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.58f,  0.32f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.58f,  0.32f, 0.0f
    };

    float verticesPiramide[] = {
        //Piramide (piso de abajo) (0 - 3):
        -0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, 0.0f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.0f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,

        //Piramide (pared de la izquierda) (4 - 6):
        -0.5f, 0.0f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,
         0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,

        //Piramide (pared de la derecha) (7 - 9):
         0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,
         0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,

        //Piramide (pared de atras) (10 - 12):
        -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, 0.0f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  1.0f, 0.0f,

        //Piramide (pared de adelante) (13 - 15):
        -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
         0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
    };


// Vector de vértices para la pirámide
float pyramidVertices[] = {
    // Piso
    -0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f,  0.0f,
     0.5f, 0.0f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f,  0.0f,
     0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f,  1.0f,
    -0.5f, 0.0f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f,  1.0f,
    // Pared izquierda
    -0.5f, 0.0f, -0.5f,   0.0f,  1.0f, -1.0f,   0.0f,  0.0f,
    -0.5f, 0.0f,  0.5f,   0.0f,  1.0f, -1.0f,   1.0f,  0.0f,
     0.5f, 0.0f,  0.5f,   0.0f,  1.0f, -1.0f,   1.0f,  1.0f,
    // Pared derecha
     0.5f, 0.0f, -0.5f,   0.0f,  1.0f,  1.0f,   0.0f,  0.0f,
     0.5f, 0.0f,  0.5f,   0.0f,  1.0f,  1.0f,   1.0f,  0.0f,
    -0.5f, 0.0f,  0.5f,   0.0f,  1.0f,  1.0f,   1.0f,  1.0f,
    // Pared atrás
    -0.5f, 0.0f, -0.5f,  -1.0f,  1.0f,  0.0f,   0.0f,  0.0f,
     0.5f, 0.0f, -0.5f,  -1.0f,  1.0f,  0.0f,   1.0f,  0.0f,
     0.5f, 0.0f,  0.5f,  -1.0f,  1.0f,  0.0f,   1.0f,  1.0f,
    -0.5f, 0.0f,  0.5f,  -1.0f,  1.0f,  0.0f,   0.0f,  1.0f,
    // Pared adelante
    -0.5f, 0.0f, -0.5f,   1.0f,  1.0f,  0.0f,   0.0f,  0.0f,
    -0.5f, 0.0f,  0.5f,   1.0f,  1.0f,  0.0f,   1.0f,  0.0f,
     0.5f, 0.0f,  0.5f,   1.0f,  1.0f,  0.0f,   1.0f,  1.0f,
     0.5f, 0.0f, -0.5f,   1.0f,  1.0f,  0.0f,   0.0f,  1.0f
};
    Cylinder cylinder1(0.5f, 0.5f, 1.0f, 36, 8, true, 2); // baseRadius, topRadius, height, sectors, stacks, flat shading, up-axis=Y(2)
    //Cylinder cylinder1(0.01f, 0.6f, 1.2f, 36, 8, true, 2);  // baseRadius, topRadius, height, sectors, stacks, smooth(default), up-axis=Y(2)
    //cylinder1.printSelf();

    unsigned int indicesCilindro[(unsigned int)cylinder1.indices.size()];
    for (int i = 0; i < (unsigned int)cylinder1.indices.size(); i++)
    {
        indicesCilindro[i] = cylinder1.indices[i];
    }
    
    unsigned int cantVertCylinder = (unsigned int)cylinder1.interleavedVertices.size();
    cantVertCylinder = cantVertCylinder + cantVertCylinder / 8;
    float cylinderData[cantVertCylinder];
    for (int i = 0, k = 0; k < (unsigned int)cylinder1.vertices.size(); i+=9, k+=3)
    {
        cylinderData[i] = cylinder1.vertices[k];
        cylinderData[i+1] = cylinder1.vertices[k+1];
        cylinderData[i+2] = cylinder1.vertices[k+2];
        cylinderData[i+3] = cylinder1.normals[k];
        cylinderData[i+4] = cylinder1.normals[k+1];
        cylinderData[i+5] = cylinder1.normals[k+2];
        cylinderData[i+6] = 1.0;
        cylinderData[i+7] = 0.5;
        cylinderData[i+8] = 0.31;
    }

    Sphere sphere1(1.0f, 36, 18, true, 2);    // radius, sectors, stacks, non-smooth (flat) shading, Y-up
    //sphere1.printSelf();

    unsigned int indicesSphere[(unsigned int)sphere1.indices.size()];
    for (int i = 0; i < (unsigned int)sphere1.indices.size(); i++)
    {
        indicesSphere[i] = sphere1.indices[i];
    }
    
    unsigned int cantVertSphere = (unsigned int)sphere1.interleavedVertices.size();
    cantVertSphere = cantVertSphere + cantVertSphere / 8;
    float sphereData[cantVertSphere];
    for (int i = 0, k = 0; k < (unsigned int)sphere1.vertices.size(); i+=9, k+=3)
    {
        sphereData[i] = sphere1.vertices[k];
        sphereData[i+1] = sphere1.vertices[k+1];
        sphereData[i+2] = sphere1.vertices[k+2];
        sphereData[i+3] = sphere1.normals[k];
        sphereData[i+4] = sphere1.normals[k+1];
        sphereData[i+5] = sphere1.normals[k+2];
        sphereData[i+6] = 1.0;
        sphereData[i+7] = 1.0;
        sphereData[i+8] = 0;
    }

    Cylinder cone1(0.5f, 0.001f, 1.0f, 36, 8, true, 2); // baseRadius, topRadius, height, sectors, stacks, flat shading, up-axis=Y(2)

    unsigned int indicesCono[(unsigned int)cone1.indices.size()];
    for (int i = 0; i < (unsigned int)cone1.indices.size(); i++)
    {
        indicesCono[i] = cone1.indices[i];
    }
    
    unsigned int cantVertCono = (unsigned int)cone1.interleavedVertices.size();
    cantVertCono = cantVertCono + cantVertCono / 8;
    float coneData[cantVertCono];
    for (int i = 0, k = 0; k < (unsigned int)cone1.vertices.size(); i+=9, k+=3)
    {
        coneData[i] = cone1.vertices[k];
        coneData[i+1] = cone1.vertices[k+1];
        coneData[i+2] = cone1.vertices[k+2];
        coneData[i+3] = cone1.normals[k];
        coneData[i+4] = cone1.normals[k+1];
        coneData[i+5] = cone1.normals[k+2];
        coneData[i+6] = 1.0;
        coneData[i+7] = 0.5;
        coneData[i+8] = 0.31;
    }

 float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };





    // first, configure the cube's VAO (and VBO)
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGeneral), verticesGeneral, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the verticesGeneral are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);






    unsigned int piramidVBO, pyramidVAO;
    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &piramidVBO);

    glBindBuffer(GL_ARRAY_BUFFER, piramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPiramide), verticesPiramide, GL_STATIC_DRAW);

    glBindVertexArray(pyramidVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);






    unsigned int cylinderVBO, cylinderVAO, cylinderEBO;
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glGenBuffers(1, &cylinderEBO);

    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinderData), cylinderData, GL_STATIC_DRAW);

    glBindVertexArray(cylinderVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCilindro), indicesCilindro, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    ////glBindBuffer(GL_ARRAY_BUFFER, 0); ***
    ////glBindVertexArray(0); ***







    unsigned int sphereVBO, sphereVAO, sphereEBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphereData), sphereData, GL_STATIC_DRAW);

    glBindVertexArray(sphereVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSphere), indicesSphere, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);




    unsigned int coneVBO, coneVAO, coneEBO;
    glGenVertexArrays(1, &coneVAO);
    glGenBuffers(1, &coneVBO);
    glGenBuffers(1, &coneEBO);

    glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coneData), coneData, GL_STATIC_DRAW);

    glBindVertexArray(coneVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coneEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCono), indicesCono, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);





    //skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);






    // // load textures
    // // -------------
    std::string cadena1 = "resources/textures/skybox/right.JPG";
    std::string cadena2 = "resources/textures/skybox/left.JPG";
    std::string cadena3 = "resources/textures/skybox/up.JPG";
    std::string cadena4 = "resources/textures/skybox/down.JPG";
    std::string cadena5 = "resources/textures/skybox/front.JPG";
    std::string cadena6 = "resources/textures/skybox/back.JPG";
    std::vector<std::string> faces
    {
        cadena1,
        cadena2,
        cadena3,
        cadena4,
        cadena5,
        cadena6
    };
    unsigned int cubemapTexture = loadCubemap(faces);




    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        ////////////////////lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //INICIO DEL ESCENARIO
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//--------------------------------------------
        // RENDERIZANDO EL SUELO DEL AMBIENTE (PARED MARRON LEVANTADA ORIGINALMENTE)
        lightingShader.setVec3("objectColor", 0.9f,  0.9f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(20.0f, 1.0f, 100.0f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 36, 4);
//--------------------------------------------


//--------------------------------------------
        // RENDERIZANDO EL CUBO
        lightingShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // RENDERIZANDO OTRO CUBO
        lightingShader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // RENDERIZANDO OTRO CUBO
        lightingShader.setVec3("objectColor", 1.0f, 1.0f, 0.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0, 1.0, 0.0));
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
//--------------------------------------------

//--------------------------------------------
        // RENDERIZANDO LA PIRAMIDE
        lightingShader.setVec3("objectColor", 1.0f, 0.0f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, -0.5f, 0.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(pyramidVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //Piramide (piso)
        glDrawArrays(GL_TRIANGLE_FAN, 4, 3); //Piramide (pared izquierda)
        glDrawArrays(GL_TRIANGLE_FAN, 7, 3); //Piramide (pared derecha)
        glDrawArrays(GL_TRIANGLE_FAN, 10, 3); //Piramide (pared atras)
        glDrawArrays(GL_TRIANGLE_FAN, 13, 3); //Piramide (pared adelante)

        // RENDERIZANDO OTRA PIRAMIDE
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(pyramidVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //Piramide (piso)
        glDrawArrays(GL_TRIANGLE_FAN, 4, 3); //Piramide (pared izquierda)
        glDrawArrays(GL_TRIANGLE_FAN, 7, 3); //Piramide (pared derecha)
        glDrawArrays(GL_TRIANGLE_FAN, 10, 3); //Piramide (pared atras)
        glDrawArrays(GL_TRIANGLE_FAN, 13, 3); //Piramide (pared adelante)
//--------------------------------------------

//--------------------------------------------
        // RENDERIZANDO EL CILINDRO
        lightingShader.setVec3("objectColor", 0.0f, 1.0f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, sizeof(indicesCilindro), GL_UNSIGNED_INT, 0);

        // RENDERIZANDO OTRO CILINDRO
        //lightingShader.setVec3("objectColor", 0.0f, 1.0f, 1.0f); //Lo comento para que use el color del cilindro anterior
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 1.5f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        lightingShader.setMat4("model", model);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, sizeof(indicesCilindro), GL_UNSIGNED_INT, 0);

        // RENDERIZANDO OTRO CILINDRO
        //lightingShader.setVec3("objectColor", 0.0f, 1.0f, 1.0f); //Lo comento para que use el color del cilindro anterior
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 3.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        lightingShader.setMat4("model", model);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, sizeof(indicesCilindro), GL_UNSIGNED_INT, 0);
//--------------------------------------------
/* Simbolo play*/
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.5f, 5.3f, 0.0f));
        model = glm::scale(model, glm::vec3(0.15f, 2.5f, 1.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0, 0.0, 0.0));
        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
/**/

    // Draw the first floor
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            lightingShader.setVec3("objectColor", 1.0f, 1.0f, 0.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(i * 5.0f, 2.0f, j * 2.0f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // Draw the second floor
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            lightingShader.setVec3("objectColor", 1.0f, 1.0f, 0.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(i * 5.0f, 4.0f, j * 2.0f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // Draw the third floor
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            lightingShader.setVec3("objectColor", 1.0f, 1.0f, 0.0f);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(i * 5.0f, 6.0f, j * 2.0f));
            lightingShader.setMat4("model", model);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }


           // RENDERIZANDO LA PIRÁMIDE
        lightingShader.setVec3("objectColor", 0.0f, 0.0f, 0.3f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 5.5f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(pyramidVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //Piramide (piso)
        glDrawArrays(GL_TRIANGLE_FAN, 4, 3); //Piramide (pared izquierda)
        glDrawArrays(GL_TRIANGLE_FAN, 7, 3); //Piramide (pared derecha)
        glDrawArrays(GL_TRIANGLE_FAN, 10, 3); //Piramide (pared atras)
        glDrawArrays(GL_TRIANGLE_FAN, 13, 3); //Piramide (pared adelante)
//--------------------------------------------
        // RENDERIZANDO LA ESFERA
        lightingShader.setVec3("objectColor", 0.5f, 0.5f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.5f, 0.0f, 0.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sizeof(indicesSphere), GL_UNSIGNED_INT, 0);

        // RENDERIZANDO OTRA ESFERA
        //lightingShader.setVec3("objectColor", 0.5f, 0.5f, 1.0f); //Lo comento para que use el color de la esfera anterior
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-6.5f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sizeof(indicesSphere), GL_UNSIGNED_INT, 0);
//--------------------------------------------

//--------------------------------------------
        // RENDERIZANDO EL CONO
        lightingShader.setVec3("objectColor", 1.0f, 0.2f, 0.69f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(8.0f, 2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        lightingShader.setMat4("model", model);

        glBindVertexArray(coneVAO);
        glDrawElements(GL_TRIANGLES, sizeof(indicesCono), GL_UNSIGNED_INT, 0);
//--------------------------------------------


//--------------------------------------------
        // RENDERIZANDO LA LAMPARA CUADRADA, ES DECIR, EL CUBO BLANCO DE DONDE SALE LA LUZ
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
//--------------------------------------------


//--------------------------------------------
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
//--------------------------------------------
        
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //FIN DEL ESCENARIO
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &pyramidVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteVertexArrays(1, &coneVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &piramidVBO);
    glDeleteBuffers(1, &cylinderEBO);
    glDeleteBuffers(1, &sphereEBO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &coneEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos.y += camera.MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos.y -= camera.MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos.x += camera.MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos.x -= camera.MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        lightPos.z += camera.MovementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        lightPos.z -= camera.MovementSpeed * deltaTime;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
