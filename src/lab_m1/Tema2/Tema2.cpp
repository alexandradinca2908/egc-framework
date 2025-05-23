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

    //  Create shadow shader
    {
        Shader* shader = new Shader("ShadowShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shadow-shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shadow-shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //  Create delivery shader
    {
        Shader* shader = new Shader("DeliveryShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "delivery", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "delivery", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    //  Initialize variables
    startResolution = window->GetResolution();

    //  Camera object
    {
        cameraMain = new Camera();
        cameraMain->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        cameraMain->RotateThirdPerson_OY(glm::radians(220.0f));
        cameraMain->TranslateForward(6.0f);
        cameraMain->TranslateRight(-2.0f);
        cameraMain->TranslateUpward(3.0f);

        //  Perspective camera
        fov = RADIANS(60);
        aspectRatio = window->props.aspectRatio;
        nearPlane = 0.01f;
        farPlane = 200.0f;

        //  Projection matrix
        projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

        cameraMini = new Camera();
        cameraMini->Set(glm::vec3(GRIDLENGTH / 2.0f, 25.0f, GRIDLENGTH / 2.0f), glm::vec3(GRIDLENGTH / 2.0f, 0, GRIDLENGTH / 2.0f), glm::vec3(0, 0, -1));

        //  Ortho camera
        left = -50.0f;
        right = 50.0f;
        bottom = -50.0f;
        top = 50.0f;
        zNear = nearPlane;
        zFar = farPlane;
    }

    //  Animation variables
    {
        cameraSpeedMove = 0.0f;
        isSlidingForward = false;
        isSlidingBackward = false;
        collision = false;
        wHold = false;
        sHold = false;
        pickUp = false;
        received = false;
        timer = 0.0f;
        deliveredPackets = 0;
        stopwatch = TIMELEFT;
    }

    //  Grid tracker
    {
        for (int i = 1; i < GRIDLENGTH; i++) {
            usedX[i] = false;
            usedZ[i] = false;
        }
        //  Drone spawn point is busy
        usedX[5] = true;
        usedZ[5] = true;
    }

    //  Minimap
    miniViewportArea = ViewportArea(startResolution.x / 1.35f, startResolution.y / 1.35f, startResolution.x / 4.f, startResolution.y / 4.f);
    
    //  Ground
    {
        ground = new Ground(startResolution.x, startResolution.y);
        CreateMesh("ground", ground->getVertices(), ground->getIndices());
    }

    //  Drone
    {
        drone = new Drone(startResolution.x, startResolution.y, cameraMain->GetTargetPosition());
        CreateMesh("drone", drone->getVertices(), drone->getIndices());
    }

    //  Rotors
    {
        for (int i = 0; i < 4; i++) {
            rotors[i] = new Rotor(startResolution.x, startResolution.y,
                drone->calculateRotorOffset(startResolution.x, startResolution.y, i));
        }
        CreateMesh("rotor", rotors[0]->getVertices(), rotors[0]->getIndices());
    }

    //  Trees
    {
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

                    float y = ground->calculateHeight((float)x, (float)z);
                    position = glm::vec3(x, y, z);

                    ok = true;
                    usedX[x] = true;
                    usedZ[z] = true;
                }
            }

            trees[i] = new Tree(startResolution.x, startResolution.y, position);
            trees[i]->setScale(float(rand() % 5) / 10 + 1.0f);
        }
        CreateMesh("tree", trees[0]->getVertices(), trees[0]->getIndices());
        CreateMesh("tree2D", trees[0]->generateVertices2D(startResolution.x, startResolution.y), trees[0]->generateIndices2D());
    }

    //  Packet
    {
        packet = new Packet(startResolution.x, startResolution.y);
        //  Generate a position
        {
            bool ok = false;
            glm::vec3 position = glm::vec3(0, 0, 0);

            while (!ok) {
                int x = rand() % GRIDLENGTH + GRIDMARGINS;
                int z = rand() % GRIDLENGTH + GRIDMARGINS;

                if ((x < GRIDLENGTH - GRIDMARGINS) && (z < GRIDLENGTH - GRIDMARGINS)
                    && (!usedX[x] || !usedZ[z])) {

                    float y = ground->calculateHeight((float)x, (float)z);
                    position = glm::vec3(x, y, z);

                    packet->setPosition(position);
                    packet->setStartPosition(position);
                    packet->setScale(float(rand() % 5) / 10 + 1.0f);

                    ok = true;
                    usedX[x] = true;
                    usedZ[z] = true;
                }
            }
        }

        CreateMesh("packet", packet->getVertices(), packet->getIndices());
        CreateMesh("packet2D", packet->generateVertices2D(startResolution.x, startResolution.y), packet->generateIndices2D());
    }

    //  Delivery area
    {
        delivery = new Delivery(startResolution.x, startResolution.y);
        //  Generate a position
        {
            bool ok = false;
            glm::vec3 position = glm::vec3(0, 0, 0);

            while (!ok) {
                int x = rand() % GRIDLENGTH + GRIDMARGINS;
                int z = rand() % GRIDLENGTH + GRIDMARGINS;

                if ((x < GRIDLENGTH - GRIDMARGINS) && (z < GRIDLENGTH - GRIDMARGINS)
                    && (!usedX[x] || !usedZ[z])) {

                    float y = ground->calculateHeight((float)x, (float)z) + 0.5f;
                    position = glm::vec3(x, y, z);

                    delivery->setPosition(position);

                    ok = true;
                    usedX[x] = true;
                    usedZ[z] = true;
                }
            }
        }

        CreateMesh("delivery", delivery->getVertices(), delivery->getIndices());
    }

    //  Guide arrow
    {
        arrow = new Arrow(startResolution.x, startResolution.y, glm::vec3(0, 0, 0));
        CreateMesh("arrow", arrow->getVertices(), arrow->getIndices());
    }

    //  Text
    {
        textRenderer = new gfxc::TextRenderer(window->props.selfDir, startResolution.x, startResolution.y);
        textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 30);
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

void Tema2::RenderScenePerspective(float deltaTimeSeconds) {
    //  TREES
    {
        for (int i = 0; i < TREES; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= Translate(trees[i]->getPosition().x, trees[i]->getPosition().y,
                trees[i]->getPosition().z);
            modelMatrix *= Scale(trees[i]->getScale(), trees[i]->getScale(), trees[i]->getScale());

            RenderMesh(meshes["tree"], shaders["VertexNormal"], modelMatrix);
            RenderMesh(meshes["tree"], shaders["ShadowShader"], modelMatrix);
        }
    }

    //  GROUND
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMesh(meshes["ground"], shaders["GroundShader"], modelMatrix);
    }

    //  DRONE
    {
        //  Check for collisions

        //  Out of bounds collision
        if (!collision && (drone->getPosition().x < 0 || drone->getPosition().x > GRIDLENGTH)) {
            collision = true;
        }

        if (!collision && (drone->getPosition().z < 0 || drone->getPosition().z > GRIDLENGTH)) {
            collision = true;
        }

        if (!collision && drone->getPosition().y > GRIDLENGTH / 4.0f) {
            collision = true;
        }

        //  Ground collision
        for (float alpha = 0; alpha < 360.0f && !collision; alpha += 45.0f) {
            float x = drone->getPosition().x + drone->HITBOX * cos(alpha);
            float z = drone->getPosition().z + drone->HITBOX * sin(alpha);

            float height = ground->calculateHeight(x, z);

            if (height >= drone->getPosition().y) {
                collision = true;
                break;
            }
        }

        //  Tree collision
        for (int i = 0; i < TREES && !collision; i++) {
            for (float alpha = 0; alpha < 360.0f && !collision; alpha += 45.0f) {
                float x = drone->getPosition().x + drone->HITBOX * cos(alpha);
                float z = drone->getPosition().z + drone->HITBOX * sin(alpha);

                glm::vec3 droneHitbox = glm::vec3(x, drone->getPosition().y, z);

                collision = trees[i]->checkCollision(droneHitbox);
            }
        }

        //  Packet collision
        for (float alpha = 0; alpha < 360.0f && !pickUp && !received; alpha += 45.0f) {
            float x = drone->getPosition().x + drone->HITBOX * cos(alpha);
            float z = drone->getPosition().z + drone->HITBOX * sin(alpha);

            glm::vec3 droneHitbox = glm::vec3(x, drone->getPosition().y, z);

            pickUp = packet->checkCollision(droneHitbox);
        }

        //  Delivery collision
        for (float alpha = 0; alpha < 360.0f && pickUp && !received; alpha += 45.0f) {
            float x = drone->getPosition().x + drone->HITBOX * cos(alpha);
            float z = drone->getPosition().z + drone->HITBOX * sin(alpha);

            glm::vec3 droneHitbox = glm::vec3(x, drone->getPosition().y, z);

            received = delivery->checkCollision(droneHitbox);

            //  Increase score
            if (received) {
                deliveredPackets++;
            }
        }

        //  Momentum animation
        if (cameraSpeedMove > 0.0f && (isSlidingForward || isSlidingBackward)) {
            if (collision) {
                cameraSpeedMove -= deltaTimeSeconds * 1.5f;
            }
            else {
                cameraSpeedMove -= deltaTimeSeconds / 1.5f;
            }

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

            cameraMain->TranslateUpward(upward);
            cameraMain->MoveForward(forward);
            cameraMain->TranslateRight(right);
        }

        if (cameraSpeedMove <= 0.0f) {
            collision = false;
        }

        //  Update position
        drone->setPosition(cameraMain->GetTargetPosition());

        //  Set matrix for mesh
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(drone->getPosition().x, drone->getPosition().y, drone->getPosition().z);
        modelMatrix *= RotateOY(glm::radians(-45.0f));
        modelMatrix *= RotateOY(glm::radians(drone->getAngleOy()));
        modelMatrix *= RotateOZ(glm::radians(drone->getAngleOz()));
        modelMatrix *= RotateOX(glm::radians(drone->getAngleOx()));
        modelMatrix *= RotateOY(glm::radians(45.0f));
        RenderMesh(meshes["drone"], shaders["VertexNormal"], modelMatrix);
        RenderMesh(meshes["drone"], shaders["ShadowShader"], modelMatrix);
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
            RenderMesh(meshes["rotor"], shaders["ShadowShader"], modelMatrix);
        }
    }

    //  PACKET
    {
        if (!pickUp) {
            //  When idle, packet is not moving
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= Translate(packet->getPosition().x, packet->getPosition().y, packet->getPosition().z);
            modelMatrix *= Scale(packet->getScale(), packet->getScale(), packet->getScale());

            RenderMesh(meshes["packet"], shaders["VertexNormal"], modelMatrix);
            RenderMesh(meshes["packet"], shaders["ShadowShader"], modelMatrix);
        }
        else {
            //  When picked up, packet follows drone movement
            //  But it must be below the drone
            float offset = 0.6f;

            //  Set matrix for mesh
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= Translate(drone->getPosition().x, drone->getPosition().y - offset, drone->getPosition().z);
            modelMatrix *= RotateOY(glm::radians(drone->getAngleOy()));
            modelMatrix *= RotateOY(glm::radians(-45.0f));
            modelMatrix *= RotateOZ(glm::radians(drone->getAngleOz()));
            modelMatrix *= RotateOX(glm::radians(drone->getAngleOx()));
            modelMatrix *= RotateOY(glm::radians(45.0f));
            modelMatrix *= Translate(0, -offset, 0);
            RenderMesh(meshes["packet"], shaders["VertexNormal"], modelMatrix);
            RenderMesh(meshes["packet"], shaders["ShadowShader"], modelMatrix);
        }

        if (received) {
            packet->setPosition(delivery->getPosition());
            pickUp = false;
        }
    }

    //  DELIVERY
    if (pickUp || received) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(delivery->getPosition().x, delivery->getPosition().y,
            delivery->getPosition().z);

        RenderMesh(meshes["delivery"], shaders["DeliveryShader"], modelMatrix);
    }

    //  DELIVERY COMPLETE - TRANSITION
    if (received) {
        timer += deltaTimeSeconds;

        if (timer >= 3.0f) {
            received = false;
            timer = 0.0f;

            //  Clear old positions
            usedX[(int)packet->getStartPosition().x] = false;
            usedZ[(int)packet->getStartPosition().z] = false;

            usedX[(int)delivery->getPosition().x] = false;
            usedZ[(int)delivery->getPosition().z] = false;

            //  Generate new positions
            for (int i = 0; i <= 1; i++) {
                bool ok = false;
                glm::vec3 position = glm::vec3(0, 0, 0);

                while (!ok) {
                    int x = rand() % GRIDLENGTH + GRIDMARGINS;
                    int z = rand() % GRIDLENGTH + GRIDMARGINS;

                    if ((x < GRIDLENGTH - GRIDMARGINS) && (z < GRIDLENGTH - GRIDMARGINS)
                        && (!usedX[x] || !usedZ[z])) {

                        float y = ground->calculateHeight((float)x, (float)z);
                        position = glm::vec3(x, y, z);

                        if (i == 0) {
                            packet->setPosition(position);
                            packet->setStartPosition(position);
                            packet->setScale(float(rand() % 5) / 10 + 1.0f);
                        }
                        else {
                            delivery->setPosition(position);
                        }

                        ok = true;
                        usedX[x] = true;
                        usedZ[z] = true;
                    }
                }
            }
        }
    }

    //  GUIDE ARROW
    {
        //  Update position
        arrow->setPosition(drone->getPosition() - glm::vec3(0, 0.55f, 0));

        //  Calculate the pointing direction
        if (!pickUp) {
            //  Point towards the packet
            float dX = drone->getPosition().x - packet->getPosition().x;
            float dZ = drone->getPosition().z - packet->getPosition().z;

            float angle = glm::degrees(atan2(dX, dZ));

            arrow->setAngleOy(angle);
        }
        else {
            //  Point towards the destination
            float dX = drone->getPosition().x - delivery->getPosition().x;
            float dZ = drone->getPosition().z - delivery->getPosition().z;

            float angle = glm::degrees(atan2(dX, dZ));

            arrow->setAngleOy(angle);
        }

        //  Set matrix for mesh
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(arrow->getPosition().x, arrow->getPosition().y, arrow->getPosition().z);
        modelMatrix *= RotateOY(glm::radians(arrow->getAngleOy()));
        RenderMesh(meshes["arrow"], shaders["VertexNormal"], modelMatrix);
    }

    //  TEXT
}

void Tema2::RenderSceneOrtho() {
    //  TREES
    {
        for (int i = 0; i < TREES; i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= Translate(trees[i]->getPosition().x, trees[i]->getPosition().y,
                trees[i]->getPosition().z);
            modelMatrix *= Scale(trees[i]->getScale(), trees[i]->getScale(), trees[i]->getScale());

            RenderMeshMini(meshes["tree2D"], shaders["VertexNormal"], modelMatrix);
        }
    }

    //  GROUND
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMeshMini(meshes["ground"], shaders["GroundShader"], modelMatrix);
    }

    //  DRONE
    {
        //  Set matrix for mesh
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(drone->getPosition().x, drone->getPosition().y, drone->getPosition().z);
        modelMatrix *= Scale(6.0f, 6.0f, 6.0f);
        modelMatrix *= RotateOY(glm::radians(-45.0f));
        modelMatrix *= RotateOY(glm::radians(drone->getAngleOy()));
        modelMatrix *= RotateOZ(glm::radians(drone->getAngleOz()));
        modelMatrix *= RotateOX(glm::radians(drone->getAngleOx()));
        modelMatrix *= RotateOY(glm::radians(45.0f));
        RenderMeshMini(meshes["drone"], shaders["VertexNormal"], modelMatrix);
    }

    //  ROTORS
    {
        for (int i = 0; i < 4; i++) {
            //  Update position
            glm::vec3 offset = drone->calculateRotorOffset(startResolution.x, startResolution.y, i);

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

            RenderMeshMini(meshes["rotor"], shaders["VertexNormal"], modelMatrix);
        }
    }

    //  PACKET
    {
        if (!pickUp) {
            //  When idle, packet is not moving
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= Translate(packet->getPosition().x, packet->getPosition().y, packet->getPosition().z);
            modelMatrix *= Scale(packet->getScale(), packet->getScale(), packet->getScale());
            modelMatrix *= Scale(3.0f, 3.0f, 3.0f);

            RenderMeshMini(meshes["packet2D"], shaders["VertexNormal"], modelMatrix);
        }
        else {
            //  When picked up, packet follows drone movement
            //  But it must be below the drone
            float offset = 0.8f;

            //  Set matrix for mesh
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix *= Translate(drone->getPosition().x, drone->getPosition().y - offset, drone->getPosition().z);
            modelMatrix *= Scale(3.0f, 3.0f, 3.0f);
            modelMatrix *= RotateOY(glm::radians(drone->getAngleOy()));
            modelMatrix *= RotateOY(glm::radians(-45.0f));
            modelMatrix *= RotateOZ(glm::radians(drone->getAngleOz()));
            modelMatrix *= RotateOX(glm::radians(drone->getAngleOx()));
            modelMatrix *= RotateOY(glm::radians(45.0f));
            modelMatrix *= Translate(0, -offset, 0);
            RenderMeshMini(meshes["packet"], shaders["VertexNormal"], modelMatrix);
        }
    }

    //  DELIVERY
    if (pickUp || received) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= Translate(delivery->getPosition().x, delivery->getPosition().y,
            delivery->getPosition().z);

        RenderMeshMini(meshes["delivery"], shaders["DeliveryShader"], modelMatrix);
    }
}

void Tema2::Update(float deltaTimeSeconds)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    //  Render main scene in perspective mode
    projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
    RenderScenePerspective(deltaTimeSeconds);

    //  Draw text
    const float kTopY = 10.f;
    const float kRowHeight = 25.f;
    int rowIndex = 0;
    glm::vec3 kTextColor = glm::vec3(0.2f, 0.3f, 0.2f);

    //  Game is running
    if (stopwatch > 0.0) {
        textRenderer->RenderText("Delivered: " + std::to_string(deliveredPackets), 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("Time left: " + std::to_string((int)stopwatch), 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    }
    //  Game over
    else {
        rowIndex = 10;
        kTextColor = glm::vec3(0.8f, 0.2f, 0);
        textRenderer->RenderText("GAME OVER", startResolution.x / 2.7, kTopY + kRowHeight * rowIndex, 2.0f, kTextColor);

        rowIndex = 13;
        textRenderer->RenderText("Final score: " + std::to_string(deliveredPackets), startResolution.x / 3.3, kTopY + kRowHeight * rowIndex++, 2.0f, kTextColor);
    }

    //  Add to stopwatch
    stopwatch -= (double)deltaTimeSeconds;
    if (stopwatch <= 0.0) {
        stopwatch = 0.0;
    }

    //  Clear depth buffer for ortho view
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    //  Render the scene again, in the new viewport
    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    RenderSceneOrtho();
}

void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(cameraMain->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(cameraMain->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::RenderMeshMini(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(cameraMini->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    
    if (stopwatch > 0.0) {
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
            if (cameraSpeedMove > 0.7f) {
                cameraSpeedMove = 0.7f;
            }

            //  Translate the camera upward
            //  Take into account drone angles
            float upward = (15.0f - abs(drone->getAngleOx())) * (15.0f - abs(drone->getAngleOz()))
                * cameraSpeedMove * deltaTime;
            upward /= 10.0f;
            cameraMain->TranslateUpward(upward);

            float forward = drone->getAngleOx() * cameraSpeedMove * deltaTime;
            cameraMain->MoveForward(forward);

            float right = drone->getAngleOz() * cameraSpeedMove * deltaTime;
            cameraMain->TranslateRight(right);
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
            if (cameraSpeedMove > 0.7f) {
                cameraSpeedMove = 0.7f;
            }

            //  Translate the camera downward
            //  Translate the camera upward
            //  Take into account drone angles
            float upward = (15.0f - abs(drone->getAngleOx())) * (15.0f - abs(drone->getAngleOz()))
                * cameraSpeedMove * deltaTime;
            upward /= 10.0f;
            cameraMain->TranslateUpward(-upward);

            float forward = drone->getAngleOx() * cameraSpeedMove * deltaTime;
            cameraMain->MoveForward(-forward);

            float right = drone->getAngleOz() * cameraSpeedMove * deltaTime;
            cameraMain->TranslateRight(-right);
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
            cameraMain->RotateThirdPerson_OY(glm::radians(diff));
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
            cameraMain->RotateThirdPerson_OY(glm::radians(diff));
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
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_TAB) {
        printf("Drone: %f %f %f\n", drone->getPosition().x, drone->getPosition().y, drone->getPosition().z);
        printf("Tree 0: %f %f %f\n", trees[0]->getPosition().x, trees[0]->getPosition().y, trees[0]->getPosition().z);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
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
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x / 1.35f, resolution.y / 1.35f, resolution.x / 4.f, resolution.y / 4.f);
}
