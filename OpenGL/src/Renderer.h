#pragma once
#include <GL\glew.h>
#include <iostream>

#if DEV_MODE_LOGGING == true
#define LOG(x) std::cout << x << std::endl
#elif DEV_MODE_LOGGING == false
#define LOG(x)
#endif


#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
