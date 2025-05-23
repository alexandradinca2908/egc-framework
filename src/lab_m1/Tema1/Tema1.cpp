#include "lab_m1/Tema1/Tema1.h"

#include <iostream>

#include "lab_m1/Tema1/transform2D.h"

using namespace std;
using namespace m1;


Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    // Create the VAO and bind it
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO = 0;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Send data to GPU vertex shader program

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void Tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices,
    unsigned int& VBO)
{
    // Create the VAO and bind it
    unsigned int VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO = 0;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Send data to GPU vertex shader program

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    //  Initialize variables
    startResolution = resolution;
    elevate = 0.0f;
    scale = 0.0f;
    drag = 0.0f;

    //  Lanscape mesh
    landscape = new m1::Landscape(resolution.x, resolution.y);
    CreateMesh("landscape", landscape->getVertices(), landscape->getIndices());

    //  Terrain mesh
    terrain = new m1::Terrain(resolution.x, resolution.y);
    CreateMesh("terrain", terrain->getVertices(), terrain->getIndices(), terrain->getVBO());

    //  Tank meshes
    tank1 = new m1::Tank(resolution.x, resolution.y, 1,
        terrain->spawnTank(resolution.x, 1));
    CreateMesh("tank1", tank1->getVertices(), tank1->getIndices());

    tank2 = new m1::Tank(resolution.x, resolution.y, 2,
        terrain->spawnTank(resolution.x, 2));
    CreateMesh("tank2", tank2->getVertices(), tank2->getIndices());

    //  Turret meshes
    turret1 = new m1::Turret(resolution.x, resolution.y);
    CreateMesh("turret1", turret1->getVertices(), turret1->getIndices());

    turret2 = new m1::Turret(resolution.x, resolution.y);
    CreateMesh("turret2", turret2->getVertices(), turret2->getIndices());

    //  Health meshes
    health1 = new m1::Healthbar(resolution.x, resolution.y, 1, tank1->getPosition());
    CreateMesh("health1Full", health1->getVerticesFull(), health1->getIndicesFull(), health1->getVBOFull());
    CreateMesh("health1Empty", health1->getVerticesEmpty(), health1->getIndicesEmpty());

    health2 = new m1::Healthbar(resolution.x, resolution.y, 2, tank2->getPosition());
    CreateMesh("health2Full", health2->getVerticesFull(), health2->getIndicesFull(), health2->getVBOFull());
    CreateMesh("health2Empty", health2->getVerticesEmpty(), health2->getIndicesEmpty());

    //  Projectile mesh
    projectile = new m1::Projectile(resolution.x, resolution.y);
    CreateMesh("projectile", projectile->getVertices(), projectile->getIndices());

    //  Hitboxes
    hitbox1 = new m1::Hitbox(resolution.x, resolution.y, tank1->getPosition(), tank1->getAngle());
    CreateMesh("hitbox1", hitbox1->getVertices(), hitbox1->getIndices());

    hitbox2 = new m1::Hitbox(resolution.x, resolution.y, tank2->getPosition(), tank2->getAngle());
    CreateMesh("hitbox2", hitbox2->getVertices(), hitbox2->getIndices());

    //  Guideline
    guide = new m1::Guideline(resolution.x, resolution.y);
    CreateMesh("guide", guide->getVertices(), guide->getIndices());

    //  Alien
    alien = new m1::Alien(resolution.x, resolution.y, glm::vec3(resolution.x / 2.0f, resolution.y / 1.1f, 0));
    CreateMesh("alien", alien->getVertices(), alien->getIndices());
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    //  TERRAIN
    {
        //  Level terrain if necessary
        terrain->levelTerrain(startResolution.x, startResolution.y, deltaTimeSeconds);

        //  Update mesh
        glBindBuffer(GL_ARRAY_BUFFER, terrain->getVBO());
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexFormat) * terrain->getVertices().size(),
            &(terrain->getVertices()[0]));

        //  Render terrain
        RenderMesh(meshes["terrain"], shaders["VertexNormal"], glm::vec3(0, 0, 0), glm::vec3(1));
    }

    //  TANKS
    {
        //  Tank follows terrain leveling
        tank1->setPosition(terrain->updateTankHeight(startResolution.x, tank1->getPosition()));
        tank1->setAngle(terrain->updateTankAngle(startResolution.x, tank1->getPosition()));
        tank2->setPosition(terrain->updateTankHeight(startResolution.x, tank2->getPosition()));
        tank2->setAngle(terrain->updateTankAngle(startResolution.x, tank2->getPosition()));

        //  Set tank1 position
        modelMatrix = glm::mat3(1);
        if (game_over && tank2_wins) {
            modelMatrix *= transform2D::Translate(tank1->getPosition().x - drag, tank1->getPosition().y + elevate);
            modelMatrix += transform2D::Scale(scale, scale);
        }
        else {
            modelMatrix *= transform2D::Translate(tank1->getPosition().x, tank1->getPosition().y);
        }
       
        //  Set tank1 angle
        modelMatrix *= transform2D::Rotate(tank1->getAngle());

        //  Render tank1
        RenderMesh2D(meshes["tank1"], shaders["VertexNormal"], modelMatrix);

        //  Set tank2 position
        modelMatrix = glm::mat3(1);
        if (game_over && !tank2_wins && tank1_wins) {
            modelMatrix *= transform2D::Translate(tank2->getPosition().x - drag, tank2->getPosition().y + elevate);
            modelMatrix += transform2D::Scale(scale, scale);
        }
        else {
            modelMatrix *= transform2D::Translate(tank2->getPosition().x, tank2->getPosition().y);
        }

        //  Set tank2 angle
        modelMatrix *= transform2D::Rotate(tank2->getAngle());

        //  Render tank2
        RenderMesh2D(meshes["tank2"], shaders["VertexNormal"], modelMatrix);

        //  TURRETS
        //  Turret follows tank
        turret1->setPosition(glm::vec3(tank1->getPosition().x, tank1->getPosition().y, 0));
        turret2->setPosition(glm::vec3(tank2->getPosition().x, tank2->getPosition().y, 0));

        //  Set turret1 position
        modelMatrix = glm::mat3(1);
        if (game_over && tank2_wins) {
            modelMatrix *= transform2D::Translate(turret1->getPosition().x - drag, turret1->getPosition().y + elevate);
            modelMatrix += transform2D::Scale(scale, scale);
        }
        else {
            modelMatrix *= transform2D::Translate(turret1->getPosition().x, turret1->getPosition().y);
        }
        
        modelMatrix *= transform2D::Rotate(tank1->getAngle());

        //  Set turret1 angle
        float tX = turret1->initialStart(startResolution.y)[0];
        float tY = turret1->initialStart(startResolution.y)[1];
        modelMatrix *= transform2D::Translate(tX, tY);
        modelMatrix *= transform2D::Rotate(glm::radians(turret1->getAngle()));
        modelMatrix *= transform2D::Translate(-tX, -tY);

        //  Render turret1
        RenderMesh2D(meshes["turret1"], shaders["VertexNormal"], modelMatrix);

        //  Set turret2 position
        modelMatrix = glm::mat3(1);
        if (game_over && !tank2_wins && tank1_wins) {
            modelMatrix *= transform2D::Translate(turret2->getPosition().x - drag, turret2->getPosition().y + elevate);
            modelMatrix += transform2D::Scale(scale, scale);
        }
        else {
            modelMatrix *= transform2D::Translate(turret2->getPosition().x, turret2->getPosition().y);
        }
        
        modelMatrix *= transform2D::Rotate(tank2->getAngle());

        //  Set turret2 angle
        tX = turret2->initialStart(startResolution.y)[0];
        tY = turret2->initialStart(startResolution.y)[1];
        modelMatrix *= transform2D::Translate(tX, tY);
        modelMatrix *= transform2D::Rotate(glm::radians(turret2->getAngle()));
        modelMatrix *= transform2D::Translate(-tX, -tY);

        //  Render turret2
        RenderMesh2D(meshes["turret2"], shaders["VertexNormal"], modelMatrix);
    }

    //  HEALTHBARS
    {
        //  Healthbar follows tank
        health1->setPosition(glm::vec3(tank1->getPosition().x, tank1->getPosition().y, 0));
        health2->setPosition(glm::vec3(tank2->getPosition().x, tank2->getPosition().y, 0));

        //  Set health1 position
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(health1->getPosition().x, health1->getPosition().y);

        //  Render health1
        if (!tank2_wins){
            RenderMesh2D(meshes["health1Full"], shaders["VertexNormal"], modelMatrix);
            RenderMesh2D(meshes["health1Empty"], shaders["VertexNormal"], modelMatrix);
        }

        //  Set health2 position
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(health2->getPosition().x, health2->getPosition().y);

        //  Render health2
        if (!tank1_wins) {
            RenderMesh2D(meshes["health2Full"], shaders["VertexNormal"], modelMatrix);
            RenderMesh2D(meshes["health2Empty"], shaders["VertexNormal"], modelMatrix);
        }
    }

    //  HITBOXES
    //  Hitbox follows tank
    {
        hitbox1->setPosition(tank1->getPosition());
        hitbox2->setPosition(tank2->getPosition());
        hitbox1->setAngle(tank1->getAngle());
        hitbox2->setAngle(tank2->getAngle());
    }

    //  PROJECTILES
    {
        //  Update positions and render
        for (Projectile* p : projectiles) {
            p->setPosition(p->getPosition() + p->getDirection() * deltaTimeSeconds);

            glm::vec3 v = p->getDirection();
            v.y -= G * deltaTimeSeconds;
            p->setDirection(v);

            //  Set projectile position
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(p->getPosition().x, p->getPosition().y);
            modelMatrix *= transform2D::Rotate(p->getAngle());

            if (p->getPosition().x > 0 && p->getPosition().x < startResolution.x) {
                //  Check for collisions
                bool hitTank1 = hitbox1->projectileCollision(p->getPosition().x, p->getPosition().y,
                    startResolution.x, startResolution.y);
                bool hitTank2 = hitbox2->projectileCollision(p->getPosition().x, p->getPosition().y,
                    startResolution.x, startResolution.y);

                if (hitTank1) {
                    int collisions = p->getCollisions() + 1;
                    p->setCollisions(collisions);
                }

                if (hitTank2) {
                    int collisions = p->getCollisions() + 1;
                    p->setCollisions(collisions);
                }

                //  Only check for terrain collision if there are no tank collisions
                bool hitTerrain = false;
                if (!hitTank1 && !hitTank2) {
                    hitTerrain = terrain->projectileCollision(p->getPosition().x, p->getPosition().y,
                        startResolution.x, startResolution.y);
                }

                //  Ignore "collision" when projectile is first launched
                //  Render only if there are no collisions and projectile is still in frame
                if (p->getCollisions() < 2 && !hitTerrain) {
                    RenderMesh2D(meshes["projectile"], shaders["VertexNormal"], modelMatrix);
                }
                else {
                    //  Shift healthbar
                    if (hitTank1) {
                        vector<VertexFormat>& vertices = health1->getVerticesFull();

                        vertices[2].position.x -= health1->barLength(startResolution.x) / 10;
                        vertices[3].position.x -= health1->barLength(startResolution.x) / 10;

                        tank1->setHits(tank1->getHits() + 1);

                        //  Update mesh
                        if (!game_over) {
                            glBindBuffer(GL_ARRAY_BUFFER, health1->getVBOFull());
                            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexFormat) * vertices.size(), &vertices[0]);
                        }

                        //  Check for endgame
                        if (tank1->getHits() == 10) {
                            game_over = true;
                            tank2_wins = true;
                        }

                    }
                    if (hitTank2) {
                        vector<VertexFormat>& vertices = health2->getVerticesFull();

                        vertices[2].position.x -= health2->barLength(startResolution.x) / 10;
                        vertices[3].position.x -= health2->barLength(startResolution.x) / 10;

                        tank2->setHits(tank2->getHits() + 1);

                        //  Update mesh
                        if (!game_over) {
                            glBindBuffer(GL_ARRAY_BUFFER, health2->getVBOFull());
                            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexFormat) * vertices.size(), &vertices[0]);
                        }

                        //  Check for endgame
                        if (tank2->getHits() == 10) {
                            game_over = true;
                            tank1_wins = true;
                        }

                    }
                    p->setPosition(glm::vec3(0, 0, 0));
                }
            }
            else {
                p->setPosition(glm::vec3(0, 0, 0));
            }
        }

        //  Clear space by removing used projectiles
        for (int i = 0; i < projectiles.size(); i++) {
            Projectile* p = projectiles[i];

            if (p->getPosition().x <= 0 || p->getPosition().x >= startResolution.x) {
                projectiles.erase(projectiles.begin() + i);
            }
        }
    }

    //  GUIDELINES
    if (!game_over) {
        //  Guideline for tank1
        float turretAngle = glm::radians(turret1->getAngle());
        float tankAngle = tank1->getAngle();
        float angle = turretAngle + tankAngle;

        //  Set data
        guide->setAngle(tankAngle);
        guide->setPosition(glm::vec3(turret1->getPosition().x,
            turret1->getPosition().y, 0));
        guide->setDirection(glm::vec3(cos(angle) * MAGNITUDE,
            sin(angle) * MAGNITUDE, 0));

        //  Render until it's out of the screen
        do {
            //  Set guideline position
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(guide->getPosition().x, guide->getPosition().y);
            modelMatrix *= transform2D::Rotate(guide->getAngle());

            //  Render
            RenderMesh2D(meshes["guide"], shaders["VertexNormal"], modelMatrix);

            //  Update position
            guide->setPosition(guide->getPosition() + (guide->getDirection() * deltaTimeSeconds) / 3.5f);

            //  Update direction
            glm::vec3 v = guide->getDirection();
            v.y -= G * deltaTimeSeconds / 3.5f;
            guide->setDirection(v);

        } while (guide->inScreen(startResolution.x, startResolution.y));

        //  Guideline for tank1
        turretAngle = glm::radians(turret2->getAngle());
        tankAngle = tank2->getAngle();
        angle = turretAngle + tankAngle;

        //  Set data
        guide->setAngle(tankAngle);
        guide->setPosition(glm::vec3(turret2->getPosition().x,
            turret2->getPosition().y, 0));
        guide->setDirection(glm::vec3(cos(angle)* MAGNITUDE,
            sin(angle)* MAGNITUDE, 0));

        //  Render until it's out of the screen
        do {
            //  Set guideline position
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(guide->getPosition().x, guide->getPosition().y);
            modelMatrix *= transform2D::Rotate(guide->getAngle());

            //  Render
            RenderMesh2D(meshes["guide"], shaders["VertexNormal"], modelMatrix);

            //  Update position
            guide->setPosition(guide->getPosition() + (guide->getDirection() * deltaTimeSeconds) / 3.5f);

            //  Update direction
            glm::vec3 v = guide->getDirection();
            v.y -= G * deltaTimeSeconds / 3.5f;
            guide->setDirection(v);

        } while (guide->inScreen(startResolution.x, startResolution.y));
    }

    //  ALIEN
    //  Until game is over, alien is idle
    if (!game_over) {
        //  Set alien position
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(alien->getPosition().x, alien->getPosition().y);

        //  Calculate alien angle
        //  Cap at abs(20) degrees then switch direction
        float angle = alien->getAngle();
        if (alien->getLeft()) {
            angle -= 8 * deltaTimeSeconds;
        }
        else {
            angle += 8 * deltaTimeSeconds;
        }

        if (angle >= 20.0f) {
            angle = 20.0f;
            alien->setLeft(true);
        }
        else if (angle <= -20.0f) {
            angle = -20.0f;
            alien->setLeft(false);
        }

        alien->setAngle(angle);

        //  Set alien angle
        modelMatrix *= transform2D::Rotate(glm::radians(alien->getAngle()));

        //  Render alien
        RenderMesh2D(meshes["alien"], shaders["VertexNormal"], modelMatrix);
    }
    //  Alien begins death animation
    else {
        //  Step 1: alien returns to 0 degrees
        if (alien->getAngle() != 0.0f) {
            //  Set alien position
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(alien->getPosition().x, alien->getPosition().y);

            //  Bring alien angle to 0 degrees
            float angle = alien->getAngle();
            if (angle < 0) {
                angle += 10 * deltaTimeSeconds;
            }
            else if (angle > 0) {
                angle -= 10 * deltaTimeSeconds;
            }

            //  Aproximate if needed
            if (abs(angle) <= 5.0f) {
                angle = 0.0f;
            }

            alien->setAngle(angle);

            //  Set alien angle
            modelMatrix *= transform2D::Rotate(glm::radians(alien->getAngle()));

            //  Render alien
            RenderMesh2D(meshes["alien"], shaders["VertexNormal"], modelMatrix);
        }

        //  Step 2: go to the position of the loser tank
        else {
            //  Get x position
            glm::vec3 loserTankPos;
            if (tank2_wins) {
                loserTankPos = tank1->getPosition();
            }
            else {
                loserTankPos = tank2->getPosition();
            }

            //  Bring the alien above the tank with delta time step
            if (alien->getPosition().x != loserTankPos.x && !alien->getGrabbed()) {
                float speed = 120.0f;

                if (alien->getPosition().x > loserTankPos.x) {
                    alien->setPosition(alien->getPosition() - glm::vec3(deltaTimeSeconds * speed, 0, 0));
                }
                else {
                    alien->setPosition(alien->getPosition() + glm::vec3(deltaTimeSeconds * speed, 0, 0));
                }

                if (abs(alien->getPosition().x - loserTankPos.x) <= 10.0f) {
                    alien->setPosition(glm::vec3(loserTankPos.x, alien->getPosition().y, 0));
                    alien->setGrabbed(true);
                }

                //  Set alien position
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(alien->getPosition().x, alien->getPosition().y);

                //  Render alien
                RenderMesh2D(meshes["alien"], shaders["VertexNormal"], modelMatrix);
            }

            //  Step 3: pull and shrink the tank until it disappears
            else if (alien->getPosition().y - loserTankPos.y - elevate > startResolution.y / 7.0f) {
                float speed = 150.0f;
                elevate += speed * deltaTimeSeconds;

                //  Slowly shrink the tank
                if (scale > -0.5f) {
                    scale -= deltaTimeSeconds / 2.0f;
                }

                //  Set alien position
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(alien->getPosition().x, alien->getPosition().y);

                //  Render alien
                RenderMesh2D(meshes["alien"], shaders["VertexNormal"], modelMatrix);
            }

            //  Step 4: leave the screen
            else if (alien->getPosition().x - drag != 0) {
                float speed = 400.0f;
                drag += deltaTimeSeconds * speed;

                //  Set alien position
                float position = alien->getPosition().x - drag;
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(position, alien->getPosition().y);

                //  Render alien
                RenderMesh2D(meshes["alien"], shaders["VertexNormal"], modelMatrix);
            }
        }
    }

    //  LANDSCAPE
    //  Render landscape
    RenderMesh(meshes["landscape"], shaders["VertexNormal"], glm::vec3(0, 0, 0), glm::vec3(1));
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 120.0f;
    float turretSpeed = 90.0f;

    //  Tank1
    if (window->KeyHold(GLFW_KEY_A) == true && !game_over) {
        float x = tank1->getPosition().x - deltaTime * speed;

        if (x < 0) {
            tank1->setPosition(glm::vec3(0, 0, 0));
        }
        else {
            tank1->setPosition(glm::vec3(x, 0, 0));
        }
    }
    if (window->KeyHold(GLFW_KEY_D) == true && !game_over) {
        float x = tank1->getPosition().x + deltaTime * speed;

        if (x > startResolution.x) {
            tank1->setPosition(glm::vec3(startResolution.x, 0, 0));
        }
        else {
            tank1->setPosition(glm::vec3(x, 0, 0));
        }
    }

    //  Tank2
    if (window->KeyHold(GLFW_KEY_LEFT) == true && !game_over) {
        float x = tank2->getPosition().x - deltaTime * speed;

        if (x < 0) {
            tank2->setPosition(glm::vec3(0, 0, 0));
        }
        else {
            tank2->setPosition(glm::vec3(x, 0, 0));
        }
    }
    if (window->KeyHold(GLFW_KEY_RIGHT) == true && !game_over) {
        float x = tank2->getPosition().x + deltaTime * speed;

        if (x > startResolution.x) {
            tank2->setPosition(glm::vec3(startResolution.x, 0, 0));
        }
        else {
            tank2->setPosition(glm::vec3(x, 0, 0));
        }
    }

    //  Turret1
    if (window->KeyHold(GLFW_KEY_W) == true && !game_over) {
        float angle = turret1->getAngle() + 2 * deltaTime * turretSpeed;

        if (angle > 360) {
            turret1->setAngle(360 - angle);
        }
        else {
            turret1->setAngle(angle);
        }
    }
    if (window->KeyHold(GLFW_KEY_S) == true && !game_over) {
        float angle = turret1->getAngle() - 2 * deltaTime * turretSpeed;

        if (angle < 0) {
            turret1->setAngle(360 + angle);
        }
        else {
            turret1->setAngle(angle);
        }
    }
    //  Turret2
    if (window->KeyHold(GLFW_KEY_UP) == true && !game_over) {
        float angle = turret2->getAngle() + 2 * deltaTime * turretSpeed;

        if (angle > 360) {
            turret2->setAngle(360 - angle);
        }
        else {
            turret2->setAngle(angle);
        }
    }
    if (window->KeyHold(GLFW_KEY_DOWN) == true && !game_over) {
        float angle = turret2->getAngle() - 2 * deltaTime * turretSpeed;

        if (angle < 0) {
            turret2->setAngle(360 + angle);
        }
        else {
            turret2->setAngle(angle);
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE && !game_over) {
        //  Spawn projectile
        Projectile* newProjectile = new m1::Projectile();

        float turretAngle = glm::radians(turret1->getAngle());
        float tankAngle = tank1->getAngle();
        float angle = turretAngle + tankAngle;

        //  Set data
        newProjectile->setAngle(tank1->getAngle());
        newProjectile->setPosition(glm::vec3(turret1->getPosition().x,
            turret1->getPosition().y, 0));
        newProjectile->setDirection(glm::vec3(cos(angle) * MAGNITUDE,
            sin(angle) * MAGNITUDE, 0));

        //  Add to projectile list
        projectiles.push_back(newProjectile);
    }

    if (key == GLFW_KEY_ENTER && !game_over) {
        //  Spawn projectile
        Projectile* newProjectile = new m1::Projectile();

        float turretAngle = glm::radians(turret2->getAngle());
        float tankAngle = tank2->getAngle();
        float angle = turretAngle + tankAngle;

        //  Set data
        newProjectile->setAngle(tank2->getAngle());
        newProjectile->setPosition(glm::vec3(turret2->getPosition().x,
            turret2->getPosition().y, 0));
        newProjectile->setDirection(glm::vec3(cos(angle) * MAGNITUDE,
            sin(angle) * MAGNITUDE, 0));

        //  Add to projectile list
        projectiles.push_back(newProjectile);
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}