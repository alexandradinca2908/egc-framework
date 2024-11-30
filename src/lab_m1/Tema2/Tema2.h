#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/transform3D.h"
#include "lab_m1/Tema2/camera/camera.h"
#include "lab_m1/Tema2/drone/drone.h"
#include "lab_m1/Tema2/rotor/rotor.h"

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

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        Camera* camera;
        Drone* drone;
        Rotor* rotors[4];

        glm::ivec2 startResolution;
        glm::mat4 projectionMatrix;

        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;
    };
}   // namespace m1
