#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/transform.h"
#include "stb/stb_image.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


Tema2::Tema2()
{
    nrPines = 50;
    nrBoxes = 2;
    nrEnemys = 15;
    dronePosition = glm::vec3(0, 0, 0);
    propellerSpeed = 25.0f;
    propellerRotation = 0;
    propellerPositions = {
        glm::vec3(0.55f, 0.08f, 0.f),
        glm::vec3(-0.55f, 0.08f, 0.f)
    };
    droneRotation = 0;
    showPointer = false;
    firstSecondCamera = false;
    hit = false;
    hitRotation = 0;
    hitFall = 0;
    droneSpeed = 4;
    droneAcceleration = false;
    droneShiftAcc = 1;
    droneAngleMovement = glm::vec2(0, 0);
    droneMove = false;
    count = 0;
    minimapCamera = new gfxc::Camera();
    cameraOffset = glm::vec3(0, 2, 5);
    minimapCamera = new gfxc::Camera();
    minimapCamera->SetPosition(glm::vec3(0, 20, 0));
    timer = 0;
    gameTime = 0.5f;
    TimeColor = glm::vec3(0.0f, 1.0f, 0.0f);
    startRace = false;
    resetGame = false;
    nrRings = 20;
    ringTaken = false;
    movement = true;
    startRandomCircles = true;
    newGame = true;
	rotation = 0;

}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

    string texturePath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "textures");
    string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders");
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "textures");
    srand(static_cast<unsigned int>(time(0)));
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ground3.jpg").c_str(), GL_REPEAT);
        mapTextures["ground"] = texture;
    }


    {
        Mesh* mesh = terrain::CreateTerrain("ground", 80, 80);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = objects::CreatePine("pine", 1, 2, glm::vec3(0, 0.5f, 0));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = objects::CreateCilinder("cilinder", 1, 8, glm::vec3(0.4f, 0.3f, 0));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = drone::CreateParallelepiped("droneBody", glm::vec3(0.6f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f));
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = drone::CreateParallelepiped("droneBody2", glm::vec3(0.6f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = drone::CreateParallelepiped("dronePropeller", glm::vec3(0.2f, 0.01f, 0.01f), glm::vec3(0, 0, 0));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = drone::CreateParallelepiped("droneSides", glm::vec3(0.03f, 0.03f, 0.03f), glm::vec3(0.5f, 0.5f, 0.5f));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = drone::CreateParallelepiped("box", glm::vec3(1, 1, 1), glm::vec3(0.5f, 0.5f, 0.5f));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = objects::CreateRectangle("rectangle", glm::vec3(0.8f, 0, 0));
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = objects::CreateDisk("disk", 1, glm::vec3(0, 0.5f, 0));
        meshes[mesh->GetMeshID()] = mesh;
    }


    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = objects::CreateTimeBar("timeBar", glm::vec3(0, 0, 0), 0.1f, glm::vec3(0, 1, 0), true);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = objects::CreateTimeBarContur("timeBarContour", glm::vec3(0, 0, 0), 0.1f, glm::vec3(TimeColor.x, TimeColor.y, 0), true);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("ring");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "Ring.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("mainRing");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "Ring.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("arrow");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "models"), "arrow.fbx");
        meshes[mesh->GetMeshID()] = mesh;
    }


    // Create a shader program for standard rendering
    {
        Shader* shader = new Shader("ShaderNormal");
        shader->AddShader(PATH_JOIN(shaderPath, "NormalVS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "NormalFS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    cubeMapTextureID = UploadCubeMapTexture(
        PATH_JOIN(texturePath, "posx.png"),
        PATH_JOIN(texturePath, "posy.png"),
        PATH_JOIN(texturePath, "posz.png"),
        PATH_JOIN(texturePath, "negx.png"),
        PATH_JOIN(texturePath, "negy.png"),
        PATH_JOIN(texturePath, "negz.png"));


    for (int i = 0; i < nrBoxes; i++) {
        float x = static_cast<float>(rand() % 70 - 35);
        float z = static_cast<float>(rand() % 70 - 35);
        float y = static_cast<float>(rand() % 10);
        boxPosition.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < nrPines; ++i) {
        generatePines == true;
        // Generate random positions for the pine tree
        float randomX = static_cast<float>(rand() % 60 - 30);
        float randomZ = static_cast<float>(rand() % 60 - 30);
 /*       for (int i = 0; i < nrBoxes; i++) {
            if (abs(randomX - boxPosition[i].x) < 5 && abs(randomZ - boxPosition[i].y) < 5) {
                i--;
                generatePines = false;
                break;
            }
        }*/
        if (randomX > 1 || randomX < -1 || randomZ > 1 || randomZ < -1 && generatePines == true) {
            pinePositions.push_back(glm::vec2(randomX, randomZ));
        }
        else {
            if (randomX < 1 && randomX > -1) {
                randomX += 2;

            }
            else {
                randomX -= 2;
            }
            pinePositions.push_back(glm::vec2(randomX, randomZ));
        }

    }



    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* LightShader = new Shader("ColorShader");
        LightShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "ColorVertexShader.glsl"), GL_VERTEX_SHADER);
        LightShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "ColorFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        LightShader->CreateAndLink();
        shaders[LightShader->GetName()] = LightShader;
    }

    count = 5;

}


void Tema2::FrameStart()
{
    if (startRandomCircles == true && startRace == true) {
        ringPosition.clear();
        for (int i = 0; i < nrRings; i++) {
            float x = static_cast<float>(rand() % 60 - 30);
            float y = static_cast<float>(rand() % 16 + 1);
            float z = static_cast<float>(rand() % 60 - 30);
            float w = static_cast<float>(rand() % 60 - 30);
            ringPosition.push_back(glm::vec4(x, y, z, w));
        }

        for (int i = 0; i < nrEnemys; i++) {
            float x = static_cast<float>(rand() % 60 - 30);
            float z = static_cast<float>(rand() % 60 - 30);
            float y = static_cast<float>(rand() % 10 + 1);
            for (int i = 0; i < nrBoxes; i++) {
                if (abs(x - boxPosition[i].x) < 5 && abs(z - boxPosition[i].y) < 5) {
                    i--;
                    break;
                }
            }
            enemys.push_back(glm::vec3(x, y, z));
        }
        startRandomCircles = false;
    }
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderMinimap(float deltaTimeSeconds) {
    glm::ivec2 resolution = window->GetResolution();
    glViewport(resolution.x - 200, resolution.y - 200, 200, 200);
    glClear(GL_DEPTH_BUFFER_BIT);


    GetSceneCamera()->SetPosition(dronePosition + glm::vec3(0, -dronePosition.y + 10, 0));
    GetSceneCamera()->SetRotation(glm::vec3(-M_PI / 2, droneRotation, 0));
    GetSceneCamera()->Update();

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-40, 0.2f, -40));
    RenderSimpleMesh(meshes["ground"], shaders["LabShader"], modelMatrix, mapTextures["ground"]);

    Shader* shader = shaders["VertexColor"];

    for (const auto& position : pinePositions) {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, 0.5f, position.y)); // Position of the pine disk
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f)); // Disk size
        RenderSimpleMesh(meshes["disk"], shader, modelMatrix);
    }

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(dronePosition.x, 0.5f, dronePosition.z));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    RenderSimpleMesh(meshes["rectangle"], shader, modelMatrix);

    glViewport(0, 0, resolution.x, resolution.y);
}



void Tema2::Update(float deltaTimeSeconds)
{


    // cursor to center
    if (showPointer == false) {
        window->CenterPointer();
        window->HidePointer();
    }
    else {
        window->ShowPointer();
    }

    for (auto pos : pinePositions) {
        float pipeHeight = 1, pipeRadius = 0.5f;
        for (float i = 0; i < pipeHeight; i += 0.1f) {
            glm::vec3 centerPine = glm::vec3(pos.x, i, pos.y);
            if (glm::length(dronePosition - centerPine) < pipeRadius) {
                hit = true;
                count--;
                glm::vec3 direction = glm::normalize(dronePosition - centerPine);
                dronePosition += direction * 0.2f;
            }
        }

        float pineCrownRadius = 1.3f;
        float pineCrownHeight = pipeHeight + 2.f;
        for (float i = pipeHeight + 0.5f; i < pineCrownHeight; i += 0.1f) {
            glm::vec3 centerPine = glm::vec3(pos.x, i, pos.y);

            if (i > pineCrownHeight / 2) {
                pineCrownRadius -= 0.07f;
            }

            if (glm::length(dronePosition - centerPine) < pineCrownRadius) {
                hit = true;
                glm::vec3 direction = glm::normalize(dronePosition - centerPine);
                dronePosition += direction * 0.2f;
                count--;
            }
        }
    }

    {
        for (auto pos : enemys) {
            if (glm::length(dronePosition - pos) < 1.8f) {
                hit = true;
                count--;
                glm::vec3 direction = glm::normalize(dronePosition - pos);
                dronePosition += direction * 0.2f;
            }
        }
    }
    /* for (auto pos : boxPosition) {
         float x = pos.x;
         float z = pos.z;
         float y = pos.y;
         float boxHeight = 8;
         float boxRadius = 4.4f;
         for (float i = 0; i < boxHeight; i += 0.1f) {
             glm::vec3 centerBox = glm::vec3(x, i, z);
             if (glm::length(dronePosition - centerBox) < boxRadius) {
                 hit = true;
                 count--;
                 glm::vec3 direction = glm::normalize(dronePosition - centerBox);
                 dronePosition += direction * 0.2f;
             }
         }

     }*/

    if (droneAcceleration == true) {
        droneSpeed += 25 * deltaTimeSeconds;
    }

    if (count == 0) {
        cout << "GAME OVER" << endl;
    }

    if (count == 0) {
		movement = false;
    }



    // start race
    if (startRace == true) {
        TimeColor.x += deltaTimeSeconds / 10;
        gameTime -= deltaTimeSeconds / 20;
        if (TimeColor.x > 0.9f) {
            TimeColor.x = 1;
        }
        TimeColor.y -= deltaTimeSeconds / 10;
        if (TimeColor.y < 0.1f) {
            TimeColor.y = 0;
        }

        if (gameTime < 0) {
            gameTime = 0;
        }
    }
    // reset race
    if (resetGame == true) {
        TimeColor.x = 0;
        TimeColor.y = 1;
        resetGame = false;
        startRace = false;
        gameTime = 0.5f;
        newGame = true;
		movement = true;
    }

    if (droneSpeed > 10) {
        droneAcceleration = false;
        droneSpeed = 4;
    }

    if (hit == true) {
        hitRotation += 10.f * deltaTimeSeconds;
        hitFall -= 5.f * deltaTimeSeconds;
    }


    if (dronePosition.y + hitFall < 0.3f) {
        dronePosition.y = 0.3f;
        hit = false;
        hitRotation = 0;
        hitFall = 0.f;
    }

    if (firstSecondCamera == false) {
        cameraOffset = glm::vec3(0, 3.f, 4.f);
    }
    else {
        cameraOffset = glm::vec3(0, 0.2f, -0.16f);
    }

    // drone move
    if (droneMove == false) {

        if (droneAngleMovement.x < 0) {
            droneAngleMovement.x += deltaTimeSeconds;
        }
        else if (droneAngleMovement.x > 0) {
            droneAngleMovement.x -= deltaTimeSeconds;
        }

        if (droneAngleMovement.y > 0) {
            droneAngleMovement.y -= deltaTimeSeconds;
        }
        else if (droneAngleMovement.y < 0) {
            droneAngleMovement.y += deltaTimeSeconds;
        }
    }

    // camera
    glm::mat4 droneRotationMatrix = glm::rotate(glm::mat4(1), droneRotation, glm::vec3(0, 1, 0));
    glm::vec3 rotatedOffset = glm::vec3(droneRotationMatrix * glm::vec4(cameraOffset, 1.0f));
    glm::vec3 cameraPosition = dronePosition + rotatedOffset;
    {
        GetSceneCamera()->SetPosition(cameraPosition + glm::vec3(0, hitFall, 0));
        GetSceneCamera()->SetRotation(glm::vec3(-0.5f, droneRotation, 0));
        GetSceneCamera()->Update();
    }

    {
        // timeBar
        Shader* shader = shaders["ColorShader"];
        glUseProgram(shader->GetProgramID());
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2.95f, 0));
        modelMatrix = glm::translate(modelMatrix, dronePosition);
        modelMatrix = glm::rotate(modelMatrix, droneRotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-30.f), glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.06f, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(gameTime, 0.08f, 1));

        glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(TimeColor));
        RenderSimpleMesh(meshes["timeBar"], shader, modelMatrix);

    }

    {   // main ring
        Shader* shader = shaders["ColorShader"];
        glm::vec4 mainRingPos = glm::vec4(0, -2, 0, 0);
        if (ringPosition.size() != 0) {
            mainRingPos = ringPosition.back();
        }

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(mainRingPos.x, mainRingPos.y, mainRingPos.z));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(mainRingPos.w), glm::vec3(0, 1, 0));

        modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));

        glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(1.f, 0.2f, 0.0f)));
        if (ringPosition.size() > 0) {
            RenderSimpleMesh(meshes["mainRing"], shader, modelMatrix);
        }

        if (glm::length(glm::vec3(mainRingPos) - dronePosition) < 1.f) {
            ringTaken = true;
            mainRingPos = glm::vec4(0, -2, 0, mainRingPos.w);
        }


        if (ringTaken == true && ringPosition.size() > 0) {
            mainRingPos = ringPosition.back();
            ringPosition.pop_back();
            ringTaken = false;

            TimeColor.x = 0;
            TimeColor.y = 1;
            gameTime = 0.5f;
        }

        if (ringPosition.size() == 0 && gameTime != 0 && startRace == true) {
            cout << "You Win";
            newGame == true;
            TimeColor.x = 0;
            TimeColor.y = 1;
            gameTime = 0.5f;
        }


        if (gameTime == 0 && ringPosition.size() > 0) {
            movement = false;
            hit = true;
            newGame = true;
        }
    }


    {
        // rings
        Shader* shader = shaders["ColorShader"];
        glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.7f, 0.5f, 0.0f)));

        for (int i = 0; i < ringPosition.size(); i++) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(ringPosition[i].x, ringPosition[i].y, ringPosition[i].z));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(ringPosition[i].w), glm::vec3(0, 1, 0));

            modelMatrix = glm::rotate(modelMatrix, glm::radians(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.02f));
            RenderSimpleMesh(meshes["ring"], shader, modelMatrix);
        }


    }

      //{
      //    // box
      //    for (int i = 0; i < nrBoxes; i++) {
      //        Shader* shader = shaders["ColorShader"];
      //        glUseProgram(shader->GetProgramID());
      //        glm::mat4 modelMatrix = glm::mat4(1);
      //        modelMatrix = glm::translate(modelMatrix, glm::vec3(boxPosition[i].x, boxPosition[i].y, boxPosition[i].z));
      //        modelMatrix = glm::scale(modelMatrix, glm::vec3(3.f, 8.f, 3.f));
      //        glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 1.f)));
      //        RenderSimpleMesh(meshes["box"], shader, modelMatrix);
      //    }
      //    
      //}

    {   // ground

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-40, 0, -40));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.05f));
        RenderSimpleMesh(meshes["ground"], shaders["LabShader"], modelMatrix, mapTextures["ground"]);
    }





    {
        Shader* shader = shaders["ColorShader"];
        glUseProgram(shader->GetProgramID());
        // pine
        for (int i = 0; i < nrPines; i++) {

            glm::vec2 position = pinePositions[i];

            glm::mat4 modelMatrix = glm::mat4(1);
            glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.5f, 0.0f)));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, 0, position.y));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["pine"], shader, modelMatrix);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.6f, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
            glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.6f, 0.0f)));
            RenderSimpleMesh(meshes["pine"], shader, modelMatrix);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
            glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.0f, 0.7f, 0.0f)));
            RenderSimpleMesh(meshes["pine"], shader, modelMatrix);

            // trunk
            glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.4f, 0.3f, 0.f)));
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, 0, position.y));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5f, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));
            RenderSimpleMesh(meshes["cilinder"], shader, modelMatrix);
        }

    }

    {
        // drone
        Shader* shader = shaders["ColorShader"];
        glUseProgram(shader->GetProgramID());


        glm::mat4 modelMatrix = glm::mat4(1);

        modelMatrix = glm::translate(modelMatrix, dronePosition + glm::vec3(0, hitFall, 0));
        modelMatrix = glm::rotate(modelMatrix, droneRotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, hitRotation, glm::vec3(0, 0, 1));
        // drone angle movement
        modelMatrix = glm::rotate(modelMatrix, droneAngleMovement.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, droneAngleMovement.y, glm::vec3(0, 0, 1));
        // ----------------
        modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0));
        glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.3f, 0.3f, 0.3f)));
        RenderSimpleMesh(meshes["droneBody"], shader, modelMatrix);
        for (int i = 0; i < 2; i++) {
            glm::mat4 propellerMatrix = modelMatrix;
            glm::mat4 sidesParts = modelMatrix;
            sidesParts = glm::translate(propellerMatrix, propellerPositions[i]);
            RenderSimpleMesh(meshes["droneSides"], shader, sidesParts);
            propellerMatrix = glm::translate(propellerMatrix, propellerPositions[i]);
            propellerMatrix = glm::rotate(propellerMatrix, propellerRotation, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], propellerMatrix);
        }
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, dronePosition + glm::vec3(0, hitFall, 0));
        modelMatrix = glm::rotate(modelMatrix, droneRotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, hitRotation, glm::vec3(0, 0, 1));

        // drone angle movement
        modelMatrix = glm::rotate(modelMatrix, droneAngleMovement.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, droneAngleMovement.y, glm::vec3(0, 0, 1));
        // ----------------

        modelMatrix = glm::rotate(modelMatrix, glm::radians(135.0f), glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["droneBody2"], shader, modelMatrix);
        for (int i = 0; i < 2; i++) {
            glm::mat4 propellerMatrix = modelMatrix;
            glm::mat4 sidesParts = modelMatrix;
            sidesParts = glm::translate(propellerMatrix, propellerPositions[i]);
            RenderSimpleMesh(meshes["droneSides"], shader, sidesParts);
            propellerMatrix = glm::translate(propellerMatrix, propellerPositions[i]);
            propellerMatrix = glm::rotate(propellerMatrix, propellerRotation, glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], propellerMatrix);
        }
        propellerRotation += propellerSpeed * deltaTimeSeconds;
    }

    {

        // enemys
        for (auto enemy : enemys) {
            Shader* shader = shaders["ColorShader"];
            glUseProgram(shader->GetProgramID());
            glm::vec3 direction = dronePosition - enemy;
            direction.y = 0;
            float angle = -atan2(direction.x, -direction.z);

            glm::mat4 modelMatrix = glm::mat4(1);
			rotation += 0.05f * deltaTimeSeconds;
            modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, enemy);
            modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f));

            glm::vec3 worldPosition = glm::vec3(modelMatrix * glm::vec4(enemy, 1.0f));

            glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.5f)));
            RenderSimpleMesh(meshes["droneBody"], shader, modelMatrix);
            for (int i = 0; i < 2; i++) {
                glm::mat4 propellerMatrix = modelMatrix;
                glm::mat4 sidesParts = modelMatrix;
                sidesParts = glm::translate(propellerMatrix, propellerPositions[i]);
                RenderSimpleMesh(meshes["droneSides"], shader, sidesParts);
                propellerMatrix = glm::translate(propellerMatrix, propellerPositions[i]);
                propellerMatrix = glm::rotate(propellerMatrix, propellerRotation, glm::vec3(0, 1, 0));
                RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], propellerMatrix);
            }

            modelMatrix = glm::mat4(1);

            modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
            modelMatrix = glm::translate(modelMatrix, enemy);
            modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(135.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f));

            glUniform3fv(glGetUniformLocation(shader->GetProgramID(), "objectColor"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.5f)));
            RenderSimpleMesh(meshes["droneBody2"], shader, modelMatrix);
            for (int i = 0; i < 2; i++) {
                glm::mat4 propellerMatrix = modelMatrix;
                glm::mat4 sidesParts = modelMatrix;
                sidesParts = glm::translate(propellerMatrix, propellerPositions[i]);
                RenderSimpleMesh(meshes["droneSides"], shader, sidesParts);
                propellerMatrix = glm::translate(propellerMatrix, propellerPositions[i]);
                propellerMatrix = glm::rotate(propellerMatrix, propellerRotation, glm::vec3(0, 1, 0));
                RenderSimpleMesh(meshes["dronePropeller"], shaders["VertexColor"], propellerMatrix);
            }
        }
    }



    {
        Shader* shader = shaders["ShaderNormal"];
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(80));
        auto camera = GetSceneCamera();
        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        int loc_texture = shader->GetUniformLocation("texture_cubemap");
        glUniform1i(loc_texture, 0);

        RenderSimpleMesh(meshes["cube"], shader, modelMatrix);

    }



    RenderMinimap(deltaTimeSeconds);




}


unsigned int Tema2::UploadCubeMapTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z)
{
    int width, height, chn;

    unsigned char* data_pos_x = stbi_load(pos_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_y = stbi_load(pos_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_z = stbi_load(pos_z.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_x = stbi_load(neg_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_y = stbi_load(neg_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_z = stbi_load(neg_z.c_str(), &width, &height, &chn, 0);

    unsigned int textureID = 0;
    // TODO(student): Create the texture

    glGenTextures(1, &textureID);
    // TODO(student): Bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (GLEW_EXT_texture_filter_anisotropic) {
        float maxAnisotropy;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // TODO(student): Load texture information for each face
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_z);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_z);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
    }

    // Free memory
    SAFE_FREE(data_pos_x);
    SAFE_FREE(data_pos_y);
    SAFE_FREE(data_pos_z);
    SAFE_FREE(data_neg_x);
    SAFE_FREE(data_neg_y);
    SAFE_FREE(data_neg_z);

    return textureID;
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glUniform1f(glGetUniformLocation(shader->program, "noiseFrequency"), 0.1f);
    glUniform1f(glGetUniformLocation(shader->program, "noiseAmplitude"), 0.25f);
    if (texture1)
    {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    }

    if (texture2)
    {

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);

    }



    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}



void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    glm::vec3 forward = glm::vec3(0, 0, -1);
    glm::vec3 right = glm::vec3(1, 0, 0);

    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), droneRotation, glm::vec3(0, 1, 0));
    forward = glm::vec3(rotationMatrix * glm::vec4(forward, 0.0f));
    right = glm::vec3(rotationMatrix * glm::vec4(right, 0.0f));
    if (movement == true) {
        if (hit == false) {

            if (window->KeyHold(GLFW_KEY_W)) {
                droneMove = true;

                dronePosition += forward * deltaTime * 3.0f * droneSpeed;
                droneAngleMovement.x -= deltaTime;
                if (droneAngleMovement.x < -glm::radians(10.f)) {
                    droneAngleMovement.x = -glm::radians(10.f);
                }
            }

            if (window->KeyHold(GLFW_KEY_S)) {
                droneMove = true;

                dronePosition -= forward * deltaTime * 2.0f * droneSpeed;
                droneAngleMovement.x += deltaTime;
                if (droneAngleMovement.x > glm::radians(10.f)) {
                    droneAngleMovement.x = glm::radians(10.f);
                }
            }

            if (window->KeyHold(GLFW_KEY_A)) {
                droneMove = true;

                dronePosition -= right * deltaTime * 2.f * droneSpeed;

                droneAngleMovement.y += deltaTime;
                if (droneAngleMovement.y > glm::radians(10.f)) {
                    droneAngleMovement.y = glm::radians(10.f);
                }
            }

            if (window->KeyHold(GLFW_KEY_D)) {
                droneMove = true;

                dronePosition += right * deltaTime * 2.0f * droneSpeed;

                droneAngleMovement.y -= deltaTime;
                if (droneAngleMovement.y < -glm::radians(10.f)) {
                    droneAngleMovement.y = -glm::radians(10.f);
                }
            }

            if (window->KeyHold(GLFW_KEY_Q)) {
                dronePosition.y -= deltaTime * 2.0f * droneSpeed;
            }
            if (window->KeyHold(GLFW_KEY_E)) {
                dronePosition.y += deltaTime * 2.0f * droneSpeed;
            }
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_P) {
        showPointer = !showPointer;
    }

    if (key == GLFW_KEY_V) {
        firstSecondCamera = !firstSecondCamera;
    }

    if (key == GLFW_KEY_SPACE) {
        droneAcceleration = true;
    }

    if (key == GLFW_KEY_ENTER && newGame == true) {
        startRace = true;
        startRandomCircles = true;
        newGame = false;
    }

    if (key == GLFW_KEY_R) {
        resetGame = true;
    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
	if (key == GLFW_KEY_W) {
        droneMove = false;
	}
    if (key == GLFW_KEY_S) {
        droneMove = false;
    }
    if (key == GLFW_KEY_D) {
        droneMove = false;
    }
    if (key == GLFW_KEY_A) {
        droneMove = false;
    }

}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    // Add mouse move event
    const float sensitivity = 0.003f;

    // Update drone rotation based on horizontal mouse movement
    droneRotation -= deltaX * sensitivity;


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
