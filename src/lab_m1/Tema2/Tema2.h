#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/camera/camera.h"
#include "lab_m1/Tema2/drone/drone.h"
#include "lab_m1/Tema2/rotor/rotor.h"
#include "lab_m1/Tema2/ground/ground.h"
#include "lab_m1/Tema2/tree/tree.h"
#include "lab_m1/Tema2/packet/packet.h"
#include "lab_m1/Tema2/delivery/delivery.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

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
        void RenderScene(float deltaTimeSeconds);
        void DrawScene();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        //  Constants
        static const int TREES = 55;
        static const int GRIDLENGTH = 99;
        static const int GRIDMARGINS = 4;

        //  Game objects
        Camera* camera;
        Ground* ground;
        Drone* drone;
        Rotor* rotors[4];
        Tree* trees[TREES];
        Packet* packet;
        Delivery* delivery;
        ViewportArea miniViewportArea;

        glm::ivec2 startResolution;
        
        //  Perspective caera
        glm::mat4 projectionMatrix;
        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;

        //  Ortho camera
        float left;
        float right;
        float bottom;
        float top;
        float zNear;
        float zFar;

        bool usedX[GRIDLENGTH];
        bool usedZ[GRIDLENGTH];

        float cameraSpeedMove;
        bool isSlidingForward;
        bool isSlidingBackward;
        bool collision;
        bool wHold;
        bool sHold;
        bool pickUp;
        bool received;
        float timer;
    };
}   // namespace m1
