#ifndef ENGINE_H
#define ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/KHR/khrplatform.h"

/* Include standard GL headers via GLAD */
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

/* Forward declare stb_image function if needed in API, but it seems internal.
 */
/* If the user wants stb_image public API, they should include it themselves or
 * we expose it. */
/* For now, we only use it in implementation. */

/* API Declarations */
char *fileReader(const char *fileName);
void createVS(GLuint *vs, const char *vss);
void createFS(GLuint *fs, const char *fss);
void createSP(GLuint *vs, GLuint *fs, GLuint *sp);
void createTEX(GLuint *tex, const char *imgName, int textureSlot);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

/* External variables expected by the engine */
extern float force;

#ifdef __cplusplus
}
#endif

#ifdef ENGINE_IMPLEMENTATION

/* Implementation Section */

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#ifdef __cplusplus
extern "C" {
#endif

char *fileReader(const char *fileName) {
  FILE *filePointer = fopen(fileName, "r");
  if (!filePointer)
    return NULL;
  fseek(filePointer, 0, SEEK_END);
  long fileSize = ftell(filePointer);
  rewind(filePointer);
  char *buffer = (char *)malloc(fileSize + 1);
  if (!buffer) {
    fclose(filePointer);
    return NULL;
  }
  size_t readSize = fread(buffer, 1, fileSize, filePointer);
  buffer[readSize] = '\0'; /* use readSize just in case */
  fclose(filePointer);
  return buffer;
}

void createVS(GLuint *vs, const char *vss) {
  *vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(*vs, 1, &vss, NULL);
  glCompileShader(*vs);
}

void createFS(GLuint *fs, const char *fss) {
  *fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(*fs, 1, &fss, NULL);
  glCompileShader(*fs);
}

void createSP(GLuint *vs, GLuint *fs, GLuint *sp) {
  *sp = glCreateProgram();
  glAttachShader(*sp, *vs);
  glAttachShader(*sp, *fs);
  glLinkProgram(*sp);
}

void createTEX(GLuint *tex, const char *imageName, int textureSlot) {
  stbi_set_flip_vertically_on_load(true);
  int iw, ih, nch;
  unsigned char *imgData = stbi_load(imageName, &iw, &ih, &nch, 4);
  if (imgData) {
    glGenTextures(1, tex);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, *tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iw, ih, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imgData);
  } else {
    // Maybe log error?
    fprintf(stderr, "Failed to load texture: %s\n", imageName);
  }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  (void)scancode;
  (void)mods;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }

  else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    force = 0.05f; /* use float literal */
  }
}

#ifdef __cplusplus
}
#endif

#endif /* ENGINE_IMPLEMENTATION */

#endif /* ENGINE_H */
