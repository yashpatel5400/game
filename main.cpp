/*******************************************************************************
 * Filename     :   main.cpp
 * Content      :   Main game loop
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel
 * Language     :   C++17
*******************************************************************************/

#include <iostream>
#include <filesystem>
#include <mutex>
#include <thread>
#include <folly/Format.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "util/shader.h"

DEFINE_string(path, "", "path to shaders directory");

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(int argc, char** argv) {
    FLAGS_stderrthreshold = 0;
    FLAGS_logtostderr = 0;

    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    CHECK(window != NULL) << "Failed to create GLFW window";
    
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    CHECK(GLEW_OK == err) << "Failed to initialize GLEW";

    glEnable(GL_DEPTH_TEST);

    auto vsPath = std::filesystem::path(FLAGS_path) / "sdf.vs";
    auto fsPath = std::filesystem::path(FLAGS_path) / "sdf.fs";
    std::atomic_bool needsRecompile = false;
    GLuint program = gl_helper::compilePaths(vsPath, fsPath);

    std::map<std::filesystem::path, std::filesystem::file_time_type> shaderModified;
    shaderModified[vsPath] = std::filesystem::last_write_time(vsPath);
    shaderModified[fsPath] = std::filesystem::last_write_time(fsPath);

    std::thread recompile([&]() {
        while (true) {
            for (const auto& entry : shaderModified) {
                const std::filesystem::path shaderPath = entry.first;
                if (std::filesystem::last_write_time(shaderPath) > shaderModified[shaderPath]) {
                    shaderModified[shaderPath] = std::filesystem::last_write_time(shaderPath);
                    needsRecompile = true;
                }
            }
               
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    });

    float vertices[] = {
        2, 0,
        0, 2,
        0, 0
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        if (needsRecompile) {
            LOG(INFO) << folly::sformat("Shaders modified! Recompiling...");
            needsRecompile = false;
            program = gl_helper::compilePaths(vsPath, fsPath);
        }

        glUseProgram(program);
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
