// Local headers
#include "program.hpp"
#include <utilities/window.hpp>
#include <utilities/timeutils.h>
#include <utilities/shader.hpp>
#include <utilities/camera.hpp>
#include <particles/particleSystem.hpp>

// System headers
#include <glm/glm.hpp>

// Standard headers
#include <iostream>

Gloom::Camera* camera;
/**
 * @brief This function is called when we register a mouse event from the window
 * See https://www.glfw.org/docs/3.3/input_guide.html#cursor_pos
 */
void mouseCallback(GLFWwindow* window, double x, double y) {
    camera->handleCursorPosInput(x, y);
}

/**
 * @brief Function that is called when a mouse button event is registered
 * See https://www.glfw.org/docs/3.3/input_guide.html#input_mouse_button
 */
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    camera->handleMouseButtonInputs(button, action);
}

/**
 * @brief Function that is called when a keyboard button press is registered.
 * See https://www.glfw.org/docs/3.3/input_guide.html#input_key
 */
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    camera->handleKeyboardInputs(key, action);
}

void runProgram(GLFWwindow* window)
{
    // OpenGL settings: Really, none of these are required for the program to work
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Disable built-in dithering
    glDisable(GL_DITHER);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // User inputs
    // See https://www.glfw.org/docs/3.3/input_guide.html
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Callbacks setup for user input
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyboardCallback);
    
    camera = new Gloom::Camera();
    ParticleSystem particles (glm::vec3(-20.,-20.,-20.), glm::vec3(20.,20.,20.));

    getTimeDeltaSeconds();
    std::cout << "Ready! Use WASDEQ to move, hold left mouse button to change your view!" << std::endl;
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
	    // Clear colour and depth buffers
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the frame
        double timeDelta = getTimeDeltaSeconds();

        camera->updateCamera(timeDelta);
        particles.update();

        // Render the frame
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);

        particles.render(window, camera, true);

        // Process input events
        glfwPollEvents();

        // Flip buffers
        glfwSwapBuffers(window);
    }
}