#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

char *fileReader(char *fileName);
void createVS(GLuint* vs, const char *vss);
void createFS(GLuint* fs, const char *fss);
void createSP(GLuint* vs, GLuint* fs, GLuint* sp);
void createTEX(GLuint* tex, char *imgName, int textureSlot);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

char *fileReader(char *fileName) {
	FILE* filePointer = fopen(fileName, "r");
	fseek(filePointer, 0, SEEK_END);
	long fileSize = ftell(filePointer);
	rewind(filePointer);
	char* buffer = malloc(fileSize + 1);
	fread(buffer, 1, fileSize, filePointer);
	buffer[fileSize] = '\0';
	fclose(filePointer);
	return buffer;
}

void createVS(GLuint* vs, const char* vss) {
	*vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vs, 1, &vss, NULL);
	glCompileShader(*vs);
}

void createFS(GLuint* fs, const char* fss) {
	*fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*fs, 1, &fss, NULL);
	glCompileShader(*fs);
}

void createSP(GLuint* vs, GLuint* fs, GLuint* sp) {
	*sp = glCreateProgram();
	glAttachShader(*sp, *vs);
	glAttachShader(*sp, *fs);
	glLinkProgram(*sp);
}

void createTEX(GLuint* tex, char *imageName, int textureSlot) {
	stbi_set_flip_vertically_on_load(true);
	int iw, ih, nch;
	unsigned char *imgData = stbi_load(imageName, &iw, &ih, &nch, 4);
	glGenTextures(1, tex);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iw, ih, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	else if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		force = 0.05;
	}
}
