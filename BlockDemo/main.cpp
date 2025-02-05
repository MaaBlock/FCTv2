#include "../FCT/headers.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>
#include <cmath>
#include <map>
using namespace FCT;
#include "App.h"

int main()
{
    App app;
    app.init();
    app.run();
}