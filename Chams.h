#ifndef CHAMSCDP_H
#define CHAMSCDP_H
#include <GLES2/gl2.h>
#include <dlfcn.h>
#include <dlfcn.h>
#include <And64InlineHook/And64InlineHook.hpp>

//github chams by: @ghostmodzofc

#include <stdbool.h>

static void *handle;
static const char* shaderName;

static bool enableWallhack;
static bool enableWallhackS;
static bool enableWallhackW;
static bool enableWallhackG;
static bool enableWallhackO;
static bool enableRainbow;
static bool enableRainbow1;

static float r = 0.0f;
static float g = 0.0f;
static float b = 0.0f;
static int w = 1;
static int a = 255;

static float red = 0.0f;
static float gren = 0.0f;
static float blue = 0.0f;
static float mi = 0.0f;

void setShader(const char* s) {
    shaderName = s;
}

const char* getShader() {
    return shaderName;
}

void SetWallhack(bool enable) {
    enableWallhack = enable;
}

void SetWallhackS(bool enable) {
    enableWallhackS = enable;
}

void SetWallhackW(bool enable) {
    enableWallhackW = enable;
}

void SetWallhackG(bool enable) {
    enableWallhackG = enable;
}

void SetWallhackO(bool enable) {
    enableWallhackO = enable;
}

bool getWallhackEnabled() {
    return enableWallhack;
}

bool getShadingEnabled() {
    return enableWallhackS;
}

bool getWireframeEnabled() {
    return enableWallhackW;
}

bool getGlowEnabled() {
    return enableWallhackG;
}

bool getOutlineEnabled() {
    return enableWallhackO;
}

void SetRainbow(bool enable) {
    enableRainbow = enable;
}

void SetRainbow1(bool enable) {
    enableRainbow1 = enable;
}

void SetR(int set) {
    r = set;
}

void SetG(int set) {
    g = set;
}

void SetB(int set) {
    b = set;
}

void SetW(int set) {
    w = set;
}

int (*old_glGetUniformLocation)(GLuint, const GLchar *);

GLint new_glGetUniformLocation(GLuint program, const GLchar *name) {
    return old_glGetUniformLocation(program, name);
}

bool isCurrentShader(const char *shader) {
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return old_glGetUniformLocation(currentProgram, shader) != -1;
}

void (*old_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void *indices);
void new_glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    old_glDrawElements(mode, count, type, indices);
    
    if (mode != GL_TRIANGLES || count < 1000) {
        return;
    }

    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    const GLchar *shaderName = getShader();
    GLint uniformLocation = old_glGetUniformLocation(currentProgram, shaderName);
    
    if (uniformLocation == -1) {
        printf("Uniform location for shader '%s' not found in the current program.\n", shaderName);
        return;
    }

glColorMask(1, 1, 1, 1);
glDepthRangef(0.5, 1);

int red = 255, gren = 0, blue = 0, mi = 0;

if (getWireframeEnabled()) {
    if (enableWallhackW) {
        glDepthRangef(1, 0.5);
    } else {
        glDepthRangef(0.5, 1);
    }
    glBlendColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
    glLineWidth(w);
    old_glDrawElements(GL_LINE_LOOP, count, type, indices);
}

if (getWallhackEnabled()) {
    glBlendColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    glColorMask(r, g, b, 255);
    glEnable(GL_BLEND);
    glBlendFunc(GL_CONSTANT_ALPHA, GL_CONSTANT_COLOR);
}

if (getShadingEnabled()) {
    glDepthRangef(1, 0.5);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_CONSTANT_COLOR);
    glBlendEquation(GL_FUNC_ADD);
    glBlendColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    glDepthFunc(GL_ALWAYS);
    old_glDrawElements(GL_TRIANGLES, count, type, indices);
    glColorMask(r, g, b, 255);
    glBlendFunc(GL_DST_COLOR, GL_ONE);
    glDepthFunc(GL_LESS);
    glBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
}

if (getGlowEnabled()) {
    glEnable(GL_BLEND);
    glBlendColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    glColorMask(1, 1, 1, 1);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
    glLineWidth(w);

    glDepthRangef(0.5, 1);
    old_glDrawElements(GL_LINES, count, type, indices);
    glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDepthRangef(1, 0.5);
    old_glDrawElements(GL_TRIANGLES, count, type, indices);
}

if (getOutlineEnabled()) {
    glDepthRangef(1, 0.5);
    glLineWidth(w);
    glEnable(GL_BLEND);
    glColorMask(1, 1, 1, 1);
    glBlendFuncSeparate(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
    glBlendColor(0, 0, 0, 1);
    old_glDrawElements(GL_TRIANGLES, count, type, indices);
    glBlendColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    old_glDrawElements(GL_LINES, count, type, indices);
}

old_glDrawElements(mode, count, type, indices);

glDepthRangef(0.5, 1);
glColorMask(1, 1, 1, 1);
glDisable(GL_BLEND);
}

bool mlovinit(){
    handle = NULL;
    handle = dlopen("libGLESv2.so", RTLD_LAZY);
    if(!handle){
        return false;
    }
    return true;
}

void LogShaders(){
    auto p_glGetUniformLocation = (const void*(*)(...))dlsym(handle, "glGetUniformLocation");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        return;
    }else{
        A64HookFunction(reinterpret_cast<void*>(p_glGetUniformLocation), reinterpret_cast<void*>(new_glGetUniformLocation), reinterpret_cast<void**>(&old_glGetUniformLocation));
    }
}

void Wallhack(){
    auto p_glDrawElements = (const void*(*)(...))dlsym(handle, "glDrawElements");
    const char *dlsym_error = dlerror();
    if(dlsym_error){
        return;
    }else{
        A64HookFunction(reinterpret_cast<void*>(p_glDrawElements), reinterpret_cast<void*>(new_glDrawElements), reinterpret_cast<void**>(&old_glDrawElements));
    }
}

#endif


