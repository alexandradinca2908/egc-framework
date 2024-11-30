#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

void Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
}

void Tema2::Init()
{
    //  Initialize variables
    startResolution = window->GetResolution();
    fov = RADIANS(60);
    aspectRatio = window->props.aspectRatio;
    nearPlane = 0.01f;
    farPlane = 200.0f;

    //  Camera object
    camera = new Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    
    //  Drone
    drone = new Drone(startResolution.x, startResolution.y, camera->GetTargetPosition());
    CreateMesh("drone", drone->getVertices(), drone->getIndices());

    //  Rotors
    for (int i = 0; i < 4; i++) {
        rotors[i] = new Rotor(startResolution.x, startResolution.y, 
            drone->calculateRotorOffset(startResolution.x, startResolution.y, i));
    }
    CreateMesh("rotor", rotors[0]->getVertices(), rotors[0]->getIndices());

    //  Projection matrix
    projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    //  DRONE
    {
        //  Update position
        drone->setPosition(camera->GetTargetPosition());

        //  Set matrix for mesh
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, drone->getPosition());
        modelMatrix = glm::rotate(modelMatrix, drone->getAngleOy(), glm::vec3(0, 1, 0));

        RenderMesh(meshes["drone"], shaders["VertexNormal"], modelMatrix);
    }

    //  ROTORS
    {
        //  Calculate angle once; all rotors share it
        float angle = rotors[0]->getAngleOy();
        float speed = 5.0f;

        angle += speed * deltaTimeSeconds;

        if (angle > 360.0f) {
            angle = angle - 360.0f;
        }

        for (int i = 0; i < 4; i++) {
            //  Update position
            glm::vec3 offset = drone->calculateRotorOffset(startResolution.x, startResolution.y, i);
            rotors[i]->setPosition(drone->getPosition() + offset);

            //  Update angle
            rotors[i]->setAngleOy(angle);

            //  Set matrix for mesh
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, rotors[i]->getPosition());
            modelMatrix = glm::translate(modelMatrix, -offset);
            modelMatrix = glm::rotate(modelMatrix, drone->getAngleOy(), glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, offset);
            modelMatrix = glm::rotate(modelMatrix, rotors[i]->getAngleOy(), glm::vec3(0, 1, 0));

            RenderMesh(meshes["rotor"], shaders["VertexNormal"], modelMatrix);
        }
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    
    float cameraSpeed = 4.0f;
    float droneSpeed = 4.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        //  Translate the camera forward
        camera->TranslateForward(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        //  Rotate drone + camera to the left
        camera->TranslateRight(-cameraSpeed * deltaTime);

        float angle = drone->getAngleOy() + droneSpeed * deltaTime;

        if (angle > 360) {
            angle = 360 - angle;
        }

        drone->setAngleOy(angle);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        //  Translate the camera backward
        camera->TranslateForward(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        //  Rotate drone + camera to the right
        camera->TranslateRight(cameraSpeed * deltaTime);

        float angle = drone->getAngleOy() - droneSpeed * deltaTime;

        if (angle < 0) {
            angle = 360 + angle;
        }

        drone->setAngleOy(angle);
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        //  Translate the camera downward
        camera->TranslateUpward(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        //  Translate the camera upward
        camera->TranslateUpward(cameraSpeed * deltaTime);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    //  Rotate camera in third-person
    camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
    camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
