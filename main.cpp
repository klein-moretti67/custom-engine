#define ENGINE_IMPLEMENTATION
#include "engine.h"
#include <chrono>
#include <iostream>
#include <vector>


// Globals required by engine.h
float force = 0.0f;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Rect {
  float x, y, w, h;
  float r, g, b;
};

bool checkCollision(const Rect &a, const Rect &b) {
  return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h &&
         a.y + a.h > b.y;
}

int main() {
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Jump Square", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  glfwSetKeyCallback(window, key_callback);

  // Load Shaders
  char *vsSource = fileReader("vertex_shader.glsl");
  char *fsSource = fileReader("fragment_shader.glsl");
  if (!vsSource || !fsSource) {
    std::cerr << "Failed to read shaders" << std::endl;
    return -1;
  }

  GLuint vs, fs, sp;
  createVS(&vs, vsSource);
  createFS(&fs, fsSource);
  createSP(&vs, &fs, &sp);

  free(vsSource);
  free(fsSource);

  // Setup Geometry (a simple quad)
  float vertices[] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f};
  unsigned int indices[] = {0, 1, 2, 0, 2, 3};

  GLuint vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Game Objects
  Rect player = {100, 100, 50, 50, 0.2f, 0.8f, 0.2f};
  float playerVelocityY = 0.0f;
  const float gravity = -0.001f;
  const float jumpImpulse = 0.5f;

  std::vector<Rect> obstacles;
  float obstacleSpeed = 0.3f;
  float obstacleTimer = 0.0f;

  auto lastTime = std::chrono::high_resolution_clock::now();

  while (!glfwWindowShouldClose(window)) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime =
        std::chrono::duration<float, std::chrono::milliseconds::period>(
            currentTime - lastTime)
            .count();
    lastTime = currentTime;

    // Input Handling (via force global from engine.h)
    if (force > 0 && player.y <= 0) {
      playerVelocityY = jumpImpulse;
      force = 0.0f;
    }

    // Physics
    playerVelocityY += gravity * deltaTime;
    player.y += playerVelocityY * deltaTime;

    if (player.y < 0) {
      player.y = 0;
      playerVelocityY = 0;
    }

    obstacleTimer += deltaTime;
    if (obstacleTimer > 2000.0f) {
      obstacles.push_back({(float)SCREEN_WIDTH, 0, 40,
                           (float)(50 + rand() % 100), 0.8f, 0.2f, 0.2f});
      obstacleTimer = 0;
    }

    for (auto it = obstacles.begin(); it != obstacles.end();) {
      it->x -= obstacleSpeed * deltaTime;
      if (checkCollision(player, *it)) {
        // Reset Game
        player.y = 0;
        playerVelocityY = 0;
        obstacles.clear();
        break;
      }
      if (it->x + it->w < 0)
        it = obstacles.erase(it);
      else
        ++it;
    }

    // Rendering
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(sp);

    // Simple Ortho Projection (could be improved)
    float projection[16] = {2.0f / SCREEN_WIDTH,
                            0,
                            0,
                            0,
                            0,
                            2.0f / SCREEN_HEIGHT,
                            0,
                            0,
                            0,
                            0,
                            1,
                            0,
                            -1,
                            -1,
                            0,
                            1};
    glUniformMatrix4fv(glGetUniformLocation(sp, "projection"), 1, GL_FALSE,
                       projection);

    auto drawRect = [&](const Rect &r) {
      float model[16] = {r.w, 0, 0, 0, 0,   r.h, 0, 0,
                         0,   0, 1, 0, r.x, r.y, 0, 1};
      glUniformMatrix4fv(glGetUniformLocation(sp, "model"), 1, GL_FALSE, model);
      glUniform3f(glGetUniformLocation(sp, "color"), r.r, r.g, r.b);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

    glBindVertexArray(vao);
    drawRect(player);
    for (const auto &o : obstacles)
      drawRect(o);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
