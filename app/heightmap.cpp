/*******************************************************************************
 * Filename     :   main.cpp
 * Content      :   Main game loop
 * Created      :   June 21, 2020
 * Authors      :   Yash Patel
 * Language     :   C++17
*******************************************************************************/

#include <atomic>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <map>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../util/shader.h"

DEFINE_string(path, "", "path to shaders directory");
DEFINE_string(shader, "", "name of shader (assumed [shader].vs and [shader].fs files)");
DEFINE_int32(depth, 10, "length of heightmap");
DEFINE_int32(width, 10, "width of heightmap");
DEFINE_double(square, 0.10, "heightmap square size (smaller -> higher tesselation resolution)");

const unsigned int SCR_SIZE = 1000;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// camera code pulled from https://learnopengl.com/Getting-started/Camera
glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

bool wireframe = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window)
{
    constexpr float kStepSize = 0.05;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } 
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraFront * kStepSize;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraFront * kStepSize;
    }
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        wireframe = !wireframe;
        wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) 
                  : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

int main(int argc, char** argv) {
    FLAGS_stderrthreshold = 0;
    FLAGS_logtostderr = 0;

    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_SIZE, SCR_SIZE, "SDF", NULL, NULL);
    CHECK(window != NULL) << "Failed to create GLFW window";

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    GLenum err = glewInit();
    CHECK(GLEW_OK == err) << "Failed to initialize GLEW";

    glEnable(GL_DEPTH_TEST);

    auto vsPath = std::filesystem::path(FLAGS_path) / (FLAGS_shader + ".vs");
    auto fsPath = std::filesystem::path(FLAGS_path) / (FLAGS_shader + ".fs");
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

    const float kSquareSize = FLAGS_square;
    std::vector<float> vertices;

    for (int rowIdx = 0; rowIdx < FLAGS_depth; rowIdx++) {
        const float rowDepth = rowIdx * kSquareSize;
        for (int squareIdx = 0; squareIdx <= FLAGS_width; squareIdx++) {
            vertices.insert(vertices.end(), {
                squareIdx * kSquareSize, 0.0, rowDepth,
                squareIdx * kSquareSize, 0.0, rowDepth - kSquareSize,
            });
        }

        // degenerate vertices to avoid spurious triangles in heightmap
        if (rowIdx != FLAGS_depth - 1) {
            auto insertDegenerate = [&](const glm::vec3& point) {
                vertices.insert(vertices.end(), {
                    point.x, point.y, point.z,
                    point.x, point.y, point.z,
                    point.x, point.y, point.z
                });
            };

            insertDegenerate(glm::vec3(FLAGS_width * kSquareSize, 0.0, rowDepth + kSquareSize));
            insertDegenerate(glm::vec3(kSquareSize, 0.0, rowDepth + kSquareSize));
        }
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        float currentTime = glfwGetTime();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (needsRecompile) {
            LOG(INFO) << "Shaders modified! Recompiling...";
            needsRecompile = false;
            program = gl_helper::compilePaths(vsPath, fsPath);
        }

        glUseProgram(program);
        gl_helper::setFloat(program, "time", currentTime);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_SIZE / (float)SCR_SIZE, 0.1f, 100.0f);
        gl_helper::setMat4(program, "projection", projection);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0), cameraUp);
        gl_helper::setMat4(program, "view", view);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}