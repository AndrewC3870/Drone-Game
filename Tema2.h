#pragma once

#include <string>
#include <unordered_map>
#include "terrain.h"
#include "objects.h"
#include "drone.h"


#include "components/simple_scene.h"
#include "components/transform.h"


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

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void RenderMinimap(float deltaTimeSeconds);
        unsigned int UploadCubeMapTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z);
        std::unordered_map<std::string, Texture2D*> mapTextures;
        std::vector<glm::vec2> pinePositions;
        int nrPines;
        int nrBoxes;
        float propellerSpeed;
        glm::vec3 dronePosition;
        float droneSpeed;
        float droneRotation;
        std::vector<glm::vec3> propellerPositions;
        float propellerRotation;
        glm::vec3 cameraOffset;
        gfxc::Camera* camera;
        bool showPointer;
        bool firstSecondCamera;
        bool hit;
        float hitRotation;
        float hitFall;
        bool droneAcceleration;
        float droneShiftAcc;
        glm::vec2 droneAngleMovement;
        bool droneMove;

        int count;
        gfxc::Camera* minimapCamera;
        int cubeMapTextureID;
        int cubeMapTextureID2;

        float timer;
        float gameTime;
        glm::vec3 TimeColor;
        bool startRace;
        bool resetGame;
        std::vector<glm::vec4> ringPosition;
        int nrRings;
        bool ringTaken;
        bool movement;
        bool startRandomCircles;
        bool newGame;
        bool generatePines;
        std::vector<glm::vec3> boxPosition;
        std::vector<glm::vec3> enemys;
        int nrEnemys;
        float rotation;
		bool renderGame;

    };
}   // namespace m1
