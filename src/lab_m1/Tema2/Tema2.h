#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/camera/camera.h"
#include "lab_m1/Tema2/drone/drone.h"
#include "lab_m1/Tema2/rotor/rotor.h"
#include "lab_m1/Tema2/ground/ground.h"
#include "lab_m1/Tema2/tree/tree.h"
#include "lab_m1/Tema2/packet/packet.h"
#include "lab_m1/Tema2/delivery/delivery.h"
#include "lab_m1/Tema2/arrow/arrow.h"
#include <ft2build.h>
#include FT_FREETYPE_H

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
        void RenderMeshMini(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void RenderScenePerspective(float deltaTimeSeconds);
        void RenderSceneOrtho();

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
        const double TIMELEFT = 100.0;

        //  Game objects
        Camera* cameraMain;
        Camera* cameraMini;
        Ground* ground;
        Drone* drone;
        Rotor* rotors[4];
        Tree* trees[TREES];
        Packet* packet;
        Delivery* delivery;
        Arrow* arrow;
        ViewportArea miniViewportArea;

        gfxc::TextRenderer* textRenderer;
        glm::ivec2 startResolution;
        
        //  Perspective camera
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
        double stopwatch;
        int deliveredPackets;
    };
}   // namespace m1
