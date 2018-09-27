#define GLM_FORCE_RADIANS

#include "Engine.h"




Engine::Engine() {}

bool Engine::initialize() {
    return initialize(1024, 576, "Parostatkiem w piękny rejs");
}

bool Engine::initialize(int windowWidth, int windowHeight, const string &windowTitle) {
    width=windowWidth;
    height=windowHeight;
    aspectRatio = static_cast<float>(windowWidth) / windowHeight;
    lights[0] = glm::vec3(100,100,100);
    maxShipX = 8.f;
    maxShipZ = 12.f;
    shipPos = 0;
    waterPos = 0;

    P = glm::perspective(50.f * 3.14f / 180.f, aspectRatio, 0.01f, 50.0f);
    if (!glfwInit()) {
        fprintf(stderr, "Cannot initialize GLFW library\n");
        return false;
    }
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Cannot create window\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Cannot initialize GLEW library\n");
        exit(EXIT_FAILURE);
    }
    glfwSetTime(0);
    initializeOpenGL();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    return true;
}

void Engine::finalize() {
    glfwDestroyWindow(window);
    glfwTerminate();
    for(auto model: models) {
        if(model != nullptr)
            delete model;
    }
}

const vector<Model *> &Engine::getModels() const {
    return models;
}

void Engine::initializeOpenGL() {
    glClearColor(0.2f, 0.8f, 1.f, 1);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}

void Engine::drawModel(Model & model, glm::mat4 V, glm::vec3 cameraPos) {
    GLfloat range = 8.f;
    GLint useTextures = 0;
    if(model.getTexture() != nullptr) {
        useTextures = 1;
    }
    glm::mat4 M = model.getM();

    model.getShader()->use();
    glUniformMatrix4fv(model.getShader()->getUniformLocation("P"),1, GL_FALSE, glm::value_ptr(P));
    glUniformMatrix4fv(model.getShader()->getUniformLocation("V"),1, GL_FALSE, glm::value_ptr(V));
    glUniformMatrix4fv(model.getShader()->getUniformLocation("M"),1, GL_FALSE, glm::value_ptr(M));
    glUniform3fv(model.getShader()->getUniformLocation("playerPosition"),1, glm::value_ptr(cameraPos));
    glUniform1fv(model.getShader()->getUniformLocation("lightRange"),1, &range);
    glUniform3fv(model.getShader()->getUniformLocation("lightPosition"),1, glm::value_ptr(lights[0]));
    glUniform1i(model.getShader()->getUniformLocation("colorTextureUnit"),0);
	glUniform1i(model.getShader()->getUniformLocation("specularTextureUnit"),1);
    glUniform1i(model.getShader()->getUniformLocation("useTextures"), useTextures);

    if(useTextures != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,model.getTexture()->getColorTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,model.getTexture()->getSpecularTexture());
    }

    glBindVertexArray(model.getVao());
    glDrawArrays(GL_TRIANGLES,0,model.getVertices().size());
    glBindVertexArray(0);
}

int Engine::getWidth() const {
    return width;
}

void Engine::setWidth(int width) {
    Engine::width = width;
}

int Engine::getHeight() const {
    return height;
}

void Engine::setHeight(int height) {
    Engine::height = height;
}

GLFWwindow *Engine::getWindow() const {
    return window;
}

void Engine::setWindow(GLFWwindow *window) {
    Engine::window = window;
}

float Engine::getAspectRatio() const {
    return aspectRatio;
}

void Engine::setAspectRatio(float aspectRatio) {
    Engine::aspectRatio = aspectRatio;
    if(aspectRatio != 0) {
        P = glm::perspective(50.f * 3.14f / 180.f, aspectRatio, 0.01f, 50.0f);
    }
}

void Engine::drawModels(glm::mat4 V, glm::vec3 cameraPos) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    for(Model *m : models) {
        drawModel(*m, V, cameraPos);
    }
    glfwSwapBuffers(window);
}

void Engine::addModel(Model *model) {
    models.push_back(model);
}

void Engine::setShip(Model * newShip) {
    ship = newShip;
}

void Engine::setWater(Model * newWater) {
    water = newWater;
}

void Engine::setShipPositionAndRotation(float deltaTime) {
    shipPos += deltaTime/4*0.3;
    if(shipPos > 2*PI)
        shipPos -= 2*PI;
    ship->setPosition(glm::vec3(maxShipX * cos(shipPos), -0.5f, maxShipZ * sin(shipPos)));
    ship->setRotation(glm::vec3(0.f,-shipPos,0.f));
    ship->recalculateM();
}

void Engine::setWaterPosition(float deltaTime) {
    waterPos += deltaTime/2;
    if(waterPos > 2*PI)
        waterPos -= 2*PI;
    water->setPosition(glm::vec3(0.f, 0.5*sin(waterPos)-0.3, 0.f));
    water->recalculateM();
}
