#ifndef CPPTEST_ENGINE_H
#define CPPTEST_ENGINE_H


#include "Model.h"
#include<iostream>

using namespace std;

class Engine {
private:
    int width;
    int height;
    float maxShipX;
    float maxShipZ;
    float shipPos;
    float waterPos;
    GLFWwindow * window;
    float aspectRatio;
    void initializeOpenGL();
    vector<Model*> models;
    glm::mat4 P;
    glm::vec3 lights[4];
    Model * ship;
    Model * water;

public:
    GLFWwindow *getWindow() const;

    void setWindow(GLFWwindow *window);

    float getAspectRatio() const;

    void setAspectRatio(float aspectRatio);

    Engine();

    bool initialize();

    bool initialize(int windowWidth, int windowHeight, const string &windowTitle);

    void finalize();

    void drawModel(Model &model, glm::mat4 V, glm::vec3 cameraPos);

    void drawModels(glm::mat4 V, glm::vec3 cameraPos);

    void addModel(Model * model);

    int getWidth() const;

    void setWidth(int width);

    int getHeight() const;

    void setHeight(int height);

    const vector<Model *> &getModels() const;

    void setShip(Model * newShip);

    void setWater(Model * newWater);

    void setShipPositionAndRotation(float deltaTime);

    void setWaterPosition(float deltaTime);
};

#endif //CPPTEST_ENGINE
