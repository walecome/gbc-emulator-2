#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <assert.h>
#include <glad/glad.h>
#include <iostream>

/**
 * Window utilites.
 */
namespace Window {

/**
 * Create main game window.
 */
bool createMainWindow(int width, int height, std::string title);

/**
 * Return main game window.
 */
GLFWwindow* mainWindow();

/**
 * Update main window.
 *
 * To be ran inside of game loop.
 */
void update();

/**
 * Return true if window is not set for closing.
 */
bool shouldRemainOpen();

/**
 * Destroy.
 */
void destroy();
}  // namespace Window
