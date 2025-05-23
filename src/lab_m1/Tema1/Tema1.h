 
#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/terrain/terrain.h"
#include "lab_m1/Tema1/tank/tank.h"
#include "lab_m1/Tema1/turret/turret.h"
#include "lab_m1/Tema1/landscape/landscape.h"
#include "lab_m1/Tema1/projectile/projectile.h"
#include "lab_m1/Tema1/healthbar/healthbar.h"
#include "lab_m1/Tema1/hitbox/hitbox.h"
#include "lab_m1/Tema1/guideline/guideline.h"
#include "lab_m1/Tema1/alien/alien.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices, unsigned int& VBO);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        const float G = 1200.0f;
        const float MAGNITUDE = 800.0f;

        bool tank1_wins = false;
        bool tank2_wins = false;
        bool game_over = false;

        float elevate;
        float scale;
        float drag;

        glm::ivec2 startResolution;
        glm::mat3 modelMatrix;

        Landscape* landscape;
        Terrain* terrain;
        Tank* tank1;
        Tank* tank2;
        Turret* turret1;
        Turret* turret2;
        Healthbar* health1;
        Healthbar* health2;
        Hitbox* hitbox1;
        Hitbox* hitbox2;
        Guideline* guide;
        Projectile* projectile;
        vector<Projectile*> projectiles;

        Alien* alien;
    };
}   // namespace m1
