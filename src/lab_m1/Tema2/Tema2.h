#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/camera/camera.h"
#include "lab_m1/Tema2/drone/drone.h"
#include "lab_m1/Tema2/rotor/rotor.h"
#include "lab_m1/Tema2/ground/ground.h"
#include"lab_m1/Tema2/tree/tree.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        static const int TREES = 25;
        static const int GRIDLENGTH = 49;
        static const int GRIDMARGINS = 3;

        Camera* camera;
        Ground* ground;
        Drone* drone;
        Rotor* rotors[4];
        Tree* trees[TREES];

        glm::ivec2 startResolution;
        glm::mat4 projectionMatrix;

        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;
        bool usedX[GRIDLENGTH];
        bool usedZ[GRIDLENGTH];

        float cameraSpeedMove;
        bool isSlidingForward;
        bool isSlidingBackward;
    };
}   // namespace m1
