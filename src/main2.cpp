// // Incluir las bibliotecas necesarias de OpenGL
// #include <iostream>
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// // Definir las dimensiones de la ventana
// const GLuint WIDTH = 800, HEIGHT = 600;

// // Vertex shader
// const GLchar* vertexShaderSource = "#version 330 core\n"
//     "layout (location = 0) in vec3 aPos;\n"
//     "uniform mat4 model;\n"
//     "uniform mat4 view;\n"
//     "uniform mat4 projection;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
//     "}\0";

// // Fragment shader
// const GLchar* fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(0.7f, 0.9f, 0.4f, 1.0f);\n"
//     "}\n\0";

// int main() 
// {
//     // Inicializar GLFW
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//     // Crear una ventana
//     GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Paisaje Rural", nullptr, nullptr);
//     if (window == nullptr) {
//         std::cout << "Error al crear la ventana de GLFW" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);

//     // Cargar las funciones de OpenGL con glad
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//         std::cout << "Error al inicializar GLAD" << std::endl;
//         glfwTerminate();
//         return -1;
//     }

//     // Crear el vertex shader
//     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//     glCompileShader(vertexShader);

//     // Crear el fragment shader
//     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//     glCompileShader(fragmentShader);

//     // Crear el programa de shaders
//     GLuint shaderProgram = glCreateProgram();
//     glAttachShader(shaderProgram, vertexShader);
//     glAttachShader(shaderProgram, fragmentShader);
//     glLinkProgram(shaderProgram);
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);

//     // Definir los vértices del paisaje
//     GLfloat vertices[] = {
//         // Suelo
//         -10.0f, 0.0f, -10.0f,
//         10.0f, 0.0f, -10.0f,
//         10.0f, 0.0f, 10.0f,
//         -10.0f, 0.0f, 10.0f,
//         // Montaña
//         -5.0f, 0.0f, -5.0f,
//         0.0f, 5.0f, -5.0f,
//         5.0f, 0.0f, -5.0f
//     };

//     // Crear los objetos de OpenGL necesarios para dibujar el paisaje
//     GLuint VBO, VAO, EBO;
//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     // Enlazar el VAO y el VBO
//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//     glEnableVertexAttribArray(0);

//     // Enlazar el EBO
//     GLuint indices[] = {
//         0, 1, 2,
//         0, 2, 3,
//         0, 4, 1,
//         1, 4, 2,
//         2, 4, 3,
//         3, 4, 0,
//         1, 5, 2,
//         2, 5, 3
//     };

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     // Desenlazar el VAO, el VBO y el EBO
//     glBindBuffer(GL_ARRAY_BUFFER, 0);
//     glBindVertexArray(0);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//     // Bucle principal de dibujado de OpenGL
//     while (!glfwWindowShouldClose(window))
//     {
//         // Gestionar eventos y entradas
//         glfwPollEvents();

//         // Limpiar la pantalla de dibujado
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         // Usar el programa de shaders creado
//         glUseProgram(shaderProgram);

//         // Definir las matrices de transformación
//         glm::mat4 model, view, projection;
//         model = glm::mat4(1.0f);
//         view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
//         projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

//         // Obtener la ubicación de las matrices de transformación en los shaders
//         GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
//         GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
//         GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

//         // Enviar las matrices de transformación al vertex shader
//         glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//         glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//         glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

//         // Dibujar el paisaje
//         glBindVertexArray(VAO);
//         glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
//         glBindVertexArray(0);

//         // Intercambiar los buffers de OpenGL
//         glfwSwapBuffers(window);
//     }

//     // Limpiar los objetos de OpenGL
//     glDeleteVertexArrays(1, &VAO);
//     glDeleteBuffers(1, &VBO);
//     glDeleteBuffers(1, &EBO);
//     glDeleteProgram(shaderProgram);

//     // Terminar GLFW
//     glfwTerminate(); 
//     return 0;
// }