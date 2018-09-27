#define GLM_FORCE_RADIANS

#include "Engine.h"
#include "Camera.h"
#include <GLFW/glfw3.h>

Engine *enginePointer;

using namespace std;

void windowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //Obraz ma być generowany w oknie o tej rozdzielczości
    enginePointer->setAspectRatio(float(width)/height);
    enginePointer->setWidth(width);
    enginePointer->setHeight(height);
}

void setupModels(Engine & engine) {
    auto *ship = new Model();
    auto *shipShader = new ShaderProgram();

    auto *water = new Model();
    auto *waterShader = new ShaderProgram();
    auto *waterTexture = new Texture();

    shipShader->load("../Shaders/shipShader.vert", nullptr, "../Shaders/shipShader.frag");
    waterShader->load("../Shaders/waterShader.vert", nullptr, "../Shaders/waterShader.frag");

    waterTexture->loadColorTexture("../Textures/waterTexture.png");
    waterTexture->loadSpecularTexture("../Textures/waterTexture.png");

    if(ship->loadFromOBJFile("../Models/Ship.obj")) {}
    ship->setShader(shipShader);
    ship->initializeAndLoadToGraphicsCard();
    ship->setPosition(glm::vec3(5.0,-0.5,5.0));
    ship->setScale(glm::vec3(0.05f,0.05f,0.05f));
    ship->setRotation(glm::vec3(0.f,0.f,0.f));
    ship->recalculateM();
    
    water->loadFromOBJFile("../Models/floor.obj");
    water->setShader(waterShader);
    water->initializeAndLoadToGraphicsCard();
    water->setPosition(glm::vec3(-10.f,0.f,-10.f));
    water->setScale(glm::vec3(10.f,1.f,10.f));
    water->setRotation(glm::vec3(0.f,0.f,0.f));
    water->recalculateM();
    water->setTexture(waterTexture);

    engine.addModel(ship);
    engine.addModel(water);
    engine.setShip(ship);
    engine.setWater(water);
}

int main()
{
    srand(time(0));
    Engine engine;
    Camera camera(engine);

    if(engine.initialize(1280, 720, "Parostatkiem w piękny rejs")) {
        setupModels(engine);
        glfwSetFramebufferSizeCallback(engine.getWindow(), windowResize);
        glfwSetTime(0);
        while (!(glfwGetKey(engine.getWindow(),GLFW_KEY_ESCAPE) == 1) || glfwWindowShouldClose(engine.getWindow())) {
            camera.changePosition(float(glfwGetTime()));
            engine.setShipPositionAndRotation(float(glfwGetTime()));
            engine.setWaterPosition(float(glfwGetTime()));
            glfwSetTime(0);
            engine.drawModels(camera.getV(), camera.getPosition());
            glfwPollEvents();
        }
    }
    engine.finalize();
    return 0;
}
