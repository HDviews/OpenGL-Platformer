#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>

#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

using namespace std;
using namespace glm;

GLFWwindow* window;

const int SCREEN_HEIGHT = 640;
const int SCREEN_WIDTH = 480;
const float SPEED = 3.0f;
const float MOUSESPEED = 0.005f;
glm::mat4 proj;
glm::mat4 view;
glm::mat4 model;
glm::mat4 scalar = glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));
glm::vec3 up = glm::vec3(0, 1, 0);

// position
glm::vec3 position = glm::vec3( 0, 0, 5);
glm::vec3 lookat_position = glm::vec3( 0, 0, -1);
glm::vec3 x_position = glm::vec3( 10, 0, 0 );
glm::vec3 y_position = glm::vec3( 0, 10, 0 );
glm::vec3 z_position = glm::vec3( 0, 0, 10 );

struct player {
 
glm::vec2 pos, size;
glm::vec3 color;
const char * texture_path;
GLuint VAO, VBO, EBO, texture;
int width, height, nrChannels;
glm::mat4 model = glm::mat4(1.0f);
GLubyte *data;
glm::vec3 position = glm::vec3(0.1f, 0.1f, 0.0f);
GLfloat container[];

//glfwGetTime is called only once, the first time this function is called
const double lastTime = glfwGetTime();

//Compute time difference between current and last frame
double currentTime = glfwGetTime();
float deltaTime = float(currentTime - lastTime);


void init(glm::vec3 color, const char * texture_path) {

    GLfloat vertices[] = {
        //vertex positions  colors                      texture coords
        -1.0f, -1.0f, 0.0f, color.x, color.y, color.z,  0.0f,  0.0f,
         1.0f, -1.0f, 0.0f, color.x, color.y, color.z,  1.0f,  0.0f,
         1.0f,  1.0f, 0.0f, color.x, color.y, color.z,  1.0f,  1.0f, 
        -1.0f,  1.0f, 0.0f, color.x, color.y, color.z,  0.0f,  1.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenTextures(1, &texture);
    glBindBuffer(GL_TEXTURE_2D, texture);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    stbi_set_flip_vertically_on_load(true); 
    data = stbi_load(texture_path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    model = glm::mat4(1.0f);
};

void move(int key_up, int key_down, int key_left, int key_right) {

    if (glfwGetKey(window,  key_up) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(0,  deltaTime + position.y, 0));
    }

    if (glfwGetKey(window, key_down) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3(0, deltaTime - position.y, 0));
    }

    if (glfwGetKey(window,  key_left) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3( deltaTime - position.x, 0, 0));
    }

    if (glfwGetKey(window,  key_right) == GLFW_PRESS) {
        model = glm::translate(model, glm::vec3( deltaTime + position.x, 0, 0));
    }
}
void draw() {

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(VAO);
    // set the texture wrapping parameters 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
};

void update() {
    glUniformMatrix4fv(3, 1, GL_FALSE, &model[0][0]);
};

void destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

GLfloat calculateVertices();
};

mat4 GetProjectionMatrix() {
    return proj;
}

mat4 GetViewMatrix() {
    return view;
}

void computeMatricesFromInputs() {

    //glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    //Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);



    if (glfwGetKey(window,  GLFW_KEY_UP) == GLFW_PRESS) {
        position -= y_position * deltaTime * SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        position += y_position * deltaTime * SPEED;
    }

    if (glfwGetKey(window,  GLFW_KEY_RIGHT) == GLFW_PRESS) {
        position -= x_position * deltaTime * SPEED;
    }

    if (glfwGetKey(window,  GLFW_KEY_LEFT) == GLFW_PRESS) {
        position += x_position * deltaTime * SPEED;
    }

    view = glm::lookAt (
        position,
        position + lookat_position,
        up
    );

    lastTime = currentTime;
}

unsigned int LoadShaders(const char * vertexFilePath, const char * fragmentFilePath) {
    //create shaders
    unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    
    //read vertex shader code
    string VertexShaderCode;
    ifstream VertexShaderStream(vertexFilePath);
    if (VertexShaderStream.is_open()) {
        stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    } else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertexFilePath);
		getchar();
		return 0;
	}

    //read fragment shader code
    string FragmentShaderCode;
    ifstream FragmentShaderStream(fragmentFilePath, ios::in);
    if (FragmentShaderStream.is_open()) {
        stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    } else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", fragmentFilePath);
		getchar();
		return 0;
	}
    
    int Result = false;
    int InfoLogLength;

    //compile vertex Shader
    printf("Compiling shader : %s\n", vertexFilePath);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    //Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s/n", &VertexShaderErrorMessage[0]);
    }

    //compile Fragment Shader
    printf("Compiling shader : %s\n", fragmentFilePath);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    //Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s/n", &FragmentShaderErrorMessage[0]);
    }

    //link the program
    printf("Linking program\n");
    unsigned int ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    //check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s/n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

int main() {

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow(SCREEN_HEIGHT, SCREEN_WIDTH, "Hello World", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if(!gladLoadGL()) 
        return -1;
    
    player square;
    square.init(glm::vec3(1.0f), "vesentry2.png" );
    
    GLuint ProgramID = LoadShaders("vertex.shader", "fragment.shader");
    glClearColor(0.2, 0.3, 0.4, 1.0);

    GLuint ProjectionID = glGetUniformLocation(ProgramID, "proj");
    GLuint ViewID = glGetUniformLocation(ProgramID, "view");
    GLuint ScalarID = glGetUniformLocation(ProgramID, "scalar");
    GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");

    proj = glm::ortho<float>(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT, -10.0f, 10.0f);
    view = glm::lookAt(
        glm::vec3(0,0,1), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)   
    );
    model = glm::mat4(1.0f);
    
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(ProgramID);

        computeMatricesFromInputs();
        proj = GetProjectionMatrix();
        view = GetViewMatrix();

        glUniformMatrix4fv(0, 1, GL_FALSE, &proj[0][0]);
        glUniformMatrix4fv(1, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, &scalar[0][0]);

        square.draw();
        square.move(GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D);
        square.update();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    square.destroy();

    /*
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
	glDeleteProgram(ProgramID);
    */
    glfwTerminate();
    return 0;
}