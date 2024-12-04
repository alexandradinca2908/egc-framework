#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>

using namespace std;
using namespace m1;
using namespace transform3D;

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
    //  Create ground shader
    {
        Shader* shader = new Shader("GroundShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "ground", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "ground", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //  Initialize variables
    startResolution = window->GetResolution();
    fov = RADIANS(60);
    aspectRatio = window->props.aspectRatio;
    nearPlane = 0.01f;
    farPlane = 200.0f;

    cameraSpeedMove = 0.0f;
    isSlidingForward = false;
    isSlidingBackward = false;
    collision = false;
    wHold = false;
    sHold = false;

    for (int i = 1; i < GRIDLENGTH; i++) {
        usedX[i] = false;
        usedZ[i] = false;
    }
    //  Drone spawn point is busy
    usedX[5] = true;
    usedZ[5] = true;

    //  Projection matrix
    projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

    //  Camera object
    camera = new Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    camera->RotateThirdPerson_OY(glm::radians(220.0f));
    camera->TranslateForward(6.0f);
    camera->TranslateRight(-2.0f);
    camera->TranslateUpward(3.0f);
    
    //  Ground
    ground = new Ground(startResolution.x, startResolution.y);
    CreateMesh("ground", ground->getVertices(), ground->getIndices());

    //  Drone
    drone = new Drone(startResolution.x, startResolution.y, camera->GetTargetPosition());
    CreateMesh("drone", drone->getVertices(), drone->getIndices());

    //  Rotors
    for (int i = 0; i < 4; i++) {
        rotors[i] = new Rotor(startResolution.x, startResolution.y, 
            drone->calculateRotorOffset(startResolution.x, startResolution.y, i));
    }
    CreateMesh("rotor", rotors[0]->getVertices(), rotors[0]->getIndices());

    //  Trees
    srand(time(NULL));
    for (int i = 0; i < TREES; i++) {
        //  Generate a position
        bool ok = false;
        glm::vec3 position = glm::vec3(0, 0, 0);

        while (!ok) {
            int x = rand() % GRIDLENGTH + GRIDMARGINS;
            int z = rand() % GRIDLENGTH + GRIDMARGINS;

            if ((x < GRIDLENGTH - GRIDMARGINS) && (z < GRIDLENGTH - GRIDMARGINS)
                && (!usedX[x] || !usedZ[z])) {

                printf("%d %d\n", x, z);

                float y = ground->calculateHeight((float) x, (float) z);
                position = glm::vec3(x, y, z);
                
                ok = true;
                usedX[x] = true;
                usedZ[z] = true;
            }
        }

        trees[i] = new Tree(startResolution.x, startResolution.y, position);
        trees[i]->setScale(float(rand() % 5) / 10 + 1.0f);

        CreateMesh("tree", trees[i]->getVertices(), trees[i]->getIndices());
    }
}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.235f, 0.616f, 0.678f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    //  TREES
    {
        for (int i = 0; i < TREES; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= Translate(trees[i]->getPosition().x, trees[i]->getPosition().y,
                trees[i]->getPosition().z);
            modelMatrix *= Scale(trees[i]->getScale(), trees[i]->getScale(), trees[i]->getScale());

            RenderMesh(meshes["tree"], shaders["VertexNormal"], modelMatrix);
        }  
    }

    //  GROUND
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["ground"], shaders["GroundShader"], modelMatrix);
    }

    //  DRONE
    {
        //  Check for collisions
        if (!collision) {
            float err = 0.1f;
            

            for (float alpha = 0; alpha < 360.0f; alpha += 45.0f) {
                float x = drone->getPosition().x + drone->HITBOX * cos(alpha);
                float z = drone->getPosition().z + drone->HITBOX * sin(alpha);

                float height = ground->calculateHeight(x, z);

                if (height >= drone->getPosition().y - err) {
                    collision = true;
                    break;
                }
            }
        }

        //  Momentum animation
        if (cameraSpeedMove > 0.0f && (isSlidingForward || isSlidingBackward)) {
            cameraSpeedMove -= deltaTimeSeconds / 1.5f;

            float upward = (15.0f - abs(drone->getAngleOx())) * (15.0f - abs(drone->getAngleOz()))
                * cameraSpeedMove * deltaTimeSeconds;
            upward /= 10.0f;
            
            float forward = drone->getAngleOx() * cameraSpeedMove * deltaTimeSeconds;
            float right = drone->getAngleOz() * cameraSpeedMove * deltaTimeSeconds;

            if ((isSlidingBackward && !collision) || (isSlidingForward && collision)) {
                upward *= -1;
                forward *= -1;
                right *= -1;
            }

            camera->TranslateUpward(upward);           
            camera->MoveForward(forward);
            camera->TranslateRight(right);
        }

        if (cameraSpeedMove <= 0.0f) {
            collision = false;
        }
        
        //  Update position
        drone->setPosition(camera->GetTargetPosition());

        //  Set matrix for mesh
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(drone->getPosition().x, drone->getPosition().y, drone->getPosition().z);
        modelMatrix *= RotateOY(glm::radians(-45.0f));
        modelMatrix *= RotateOY(glm::radians(drone->getAngleOy()));
        modelMatrix *= RotateOZ(glm::radians(drone->getAngleOz()));
        modelMatrix *= RotateOX(glm::radians(drone->getAngleOx()));
        modelMatrix *= RotateOY(glm::radians(45.0f));
        RenderMesh(meshes["drone"], shaders["VertexNormal"], modelMatrix);
    }

    //  ROTORS
    {
        //  Calculate angle once; all rotors share it
        float angle = rotors[0]->getAngleOy();
        float speed = 500.0f;

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
            modelMatrix *= Translate(rotors[i]->getPosition().x, rotors[i]->getPosition().y, rotors[i]->getPosition().z);
            modelMatrix *= Translate(-offset.x, -offset.y, -offset.z);
            modelMatrix *= RotateOY(glm::radians(-45.0f));
            modelMatrix *= RotateOY(glm::radians(drone->getAngleOy()));
            modelMatrix *= RotateOZ(glm::radians(drone->getAngleOz()));
            modelMatrix *= RotateOX(glm::radians(drone->getAngleOx()));
            modelMatrix *= RotateOY(glm::radians(45.0f));
            modelMatrix *= Translate(offset.x, offset.y, offset.z);
            modelMatrix *= RotateOY(glm::radians(rotors[i]->getAngleOy()));

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

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    //  Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    //  Get shader location for uniform mat4 "Model"
    int location = glGetUniformLocation(shader->program, "Model");

    //  Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    //  Get shader location for uniform mat4 "View"
    location = glGetUniformLocation(shader->program, "View");

    //  Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    //  Get shader location for uniform mat4 "Projection"
    location = glGetUniformLocation(shader->program, "Projection");

    //  Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    //  Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    
    float cameraSpeedRotate = 250.0f;

    //  THRUST
    if (window->KeyHold(GLFW_KEY_W) && !collision) {
        //  Mark key as pressed
        if (!wHold) {
            wHold = true;
        }

        //  Lose momentum
        if (isSlidingBackward) {
            cameraSpeedMove = 0.0f;
        }
        isSlidingForward = false;
        isSlidingBackward = false;

        //  Accelerate
        cameraSpeedMove += deltaTime;
        if (cameraSpeedMove > 0.8f) {
            cameraSpeedMove = 0.8f;
        }

        //  Translate the camera upward
        //  Take into account drone angles
        float upward = (15.0f - abs(drone->getAngleOx())) * (15.0f - abs(drone->getAngleOz()))
            * cameraSpeedMove * deltaTime;
        upward /= 10.0f;
        camera->TranslateUpward(upward);

        float forward = drone->getAngleOx() * cameraSpeedMove * deltaTime;
        camera->MoveForward(forward);

        float right = drone->getAngleOz() * cameraSpeedMove * deltaTime;
        camera->TranslateRight(right);
    }
    else if (wHold) {
        isSlidingForward = true;
        wHold = false;
    }

    if (window->KeyHold(GLFW_KEY_S) && !collision) {
        //  Mark key as pressed
        if (!sHold) {
            sHold = true;
        }
        
        //  Lose momentum
        if (isSlidingForward) {
            cameraSpeedMove = 0.0f;
        }
        isSlidingForward = false;
        isSlidingBackward = false;

        //  Accelerate
        cameraSpeedMove += deltaTime;
        if (cameraSpeedMove > 0.8f) {
            cameraSpeedMove = 0.8f;
        }

        //  Translate the camera downward
        //  Translate the camera upward
        //  Take into account drone angles
        float upward = (15.0f - abs(drone->getAngleOx())) * (15.0f - abs(drone->getAngleOz()))
            * cameraSpeedMove * deltaTime;
        upward /= 10.0f;
        camera->TranslateUpward(-upward);

        float forward = drone->getAngleOx() * cameraSpeedMove * deltaTime;
        camera->MoveForward(-forward);

        float right = drone->getAngleOz() * cameraSpeedMove * deltaTime;
        camera->TranslateRight(-right);
    }
    else if (sHold) {
        isSlidingBackward = true;
        sHold = false;
    }

    //  YAW
    if (window->KeyHold(GLFW_KEY_A)) {
        //  Rotate drone to the left
        float angle = drone->getAngleOy() + cameraSpeedRotate * deltaTime;
        float diff = angle - drone->getAngleOy();

        if (angle > 360.0f) {
            angle = angle - 360.0f;
        }

        drone->setAngleOy(angle);

        //  Rotate the camera with it
        camera->RotateThirdPerson_OY(glm::radians(diff));
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        //  Rotate drone to the right
        float angle = drone->getAngleOy() - cameraSpeedRotate * deltaTime;
        float diff = angle - drone->getAngleOy();

        if (angle < 0.0f) {
            angle = 360.0f + angle;
        }

        drone->setAngleOy(angle);
        
        //  Rotate the camera with it
        camera->RotateThirdPerson_OY(glm::radians(diff));
    }

    //  PITCH
    if (window->KeyHold(GLFW_KEY_UP)) {
        float angle = drone->getAngleOx() + cameraSpeedRotate * deltaTime;

        if (angle > 15.0f) {
            angle = 15.0f;
        }

        drone->setAngleOx(angle);
    }

    if (window->KeyHold(GLFW_KEY_DOWN)) {
        float angle = drone->getAngleOx() - cameraSpeedRotate * deltaTime;

        if (angle < -15.0f) {
            angle = -15.0f;
        }

        drone->setAngleOx(angle);
    }

    //  ROLL
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        float angle = drone->getAngleOz() + cameraSpeedRotate * deltaTime;

        if (angle > 15.0f) {
            angle = 15.0f;
        }

        drone->setAngleOz(angle);
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        float angle = drone->getAngleOz() - cameraSpeedRotate * deltaTime;

        if (angle < -15.0f) {
            angle = -15.0f;
        }

        drone->setAngleOz(angle);
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_TAB) {
        printf("Drone: %f %f %f\n", floor(drone->getPosition().x), drone->getPosition().y, floor(drone->getPosition().z));
        float y = ground->calculateHeight(floor(drone->getPosition().x), floor(drone->getPosition().z));
        printf("Terrain height: %f\n", y);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    //float sensivityOX = 0.001f;
    //float sensivityOY = 0.001f;

    ////  Rotate camera in third-person
    //camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
    //camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
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
