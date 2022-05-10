#include <raylib.h>
#include <stdio.h>
#include <math.h>
#include "animacao.h"
#include "personagem.h"
#include "mapas.h"

#define NUM_FRAMES  3



int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitAudioDevice();
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic screen manager");
  
    if(!IsWindowFullscreen()) ToggleFullscreen();

    Mapa maps;
    objPersonagem personagem1;
    objPersonagem personagem2;
    Rectangle chao = (Rectangle) {0, GetScreenHeight()-ALTURA_CHAO, GetScreenWidth(), 1};

    Texture2D button = LoadTexture("assets/images/background/button.png");
    //Texture2D back = LoadTexture("assets/images/background/vida2.png");

    carregarItensMapa(&maps);
    iniciarPersonagem(&personagem1, 1, 60);
    iniciarPersonagem(&personagem2, 0, GetScreenWidth() - 60 - LARGURA_PERSONAGEM);

    int framesCounter = 0;
    int framesSpeed = 8;
    int currentFrame = 0;
    int numeroFrames = 0;
    int parteDoJogo = 2;

    float frameHeight = (float)button.height/NUM_FRAMES;
    Rectangle sourceRec = { GetScreenWidth()/2-button.width/2, GetScreenHeight()-button.height-150, (float)button.width, (float)button.height };

    // Define button bounds on screen
    Rectangle btnBounds = { screenWidth/2.0f - button.width/2.0f, screenHeight/2.0f - button.height/NUM_FRAMES/2.0f, (float)button.width,(float)button.height };

    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED
    bool btnAction = false;         // Button action should be activated

    Vector2 mousePoint = { 0.0f, 0.0f };


    SetTargetFPS(60);     


    float deltaTime;

    //SetExitKey(KEY_NULL);
    // Main game loop
    while (!WindowShouldClose()){

        framesCounter++;

        if(parteDoJogo == 0){

            if (framesCounter >= (60/framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;
            }
            
            // Update  
            deltaTime = GetFrameTime();      
            if(personagem1.vida > 0) atualizarPersonagem(&personagem1, chao, deltaTime);
            if(personagem2.vida > 0) atualizarPersonagem(&personagem2, chao, deltaTime);
            checarParede(&personagem1);
            checarParede(&personagem2);
            if(personagem1.atk == 1 && personagem1.defendendo == 0){
                ataque(&personagem1,&personagem2);
            }
            if(personagem2.atk == 1 && personagem2.defendendo == 0 ){
                ataque(&personagem2, &personagem1);
            }
        }else if(parteDoJogo == 2){

            mousePoint = GetMousePosition();
            btnAction = false;

            // Check button state
            if (CheckCollisionPointRec(mousePoint, sourceRec))
            {   

                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) parteDoJogo = 0;
                //else btnState = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
            }
            else btnState = 0;
        }




        // Draw
        BeginDrawing();
            //Menu
            if(parteDoJogo == 2){

                ClearBackground(WHITE);

                DrawTextureRec(button, sourceRec, (Vector2){ GetScreenWidth()/2-button.width/2, GetScreenHeight()-button.height -100}, WHITE);
            }
            //Jogo
            else if(parteDoJogo==0){
                ClearBackground(WHITE);
                DrawTexture(maps.background,0,0,WHITE);
                DrawTexture(maps.midground,0,0,WHITE);
                DrawTexture(maps.foreground,0,0,WHITE);


                DrawRectangleRec((Rectangle) {115 + 8*(100 - personagem1.vida), 100, 8*personagem1.vida, 50}, GREEN);
                DrawRectangleRec((Rectangle) {GetScreenWidth()/2+60, 100, 8*personagem2.vida, 50}, RED);


                DrawTexture(maps.barraVida,0,0,WHITE);
                animacaoPersonagem(&personagem1,framesCounter,&parteDoJogo);
                animacaoPersonagem(&personagem2, framesCounter,&parteDoJogo);
                DrawTexture(maps.chao,0,GetScreenHeight()-ALTURA_CHAO-10,WHITE);

                
                

                

            }
            //Final
            else if (parteDoJogo ==1){
                ClearBackground(WHITE);
                DrawText("Final", 250, 20, 20, DARKGRAY);

            }
            

        EndDrawing();


        }
    
    descarregarItensMapa(&maps);
    terminarAnimacao(&personagem1);
    terminarAnimacao(&personagem2);
    encerrarSons(&personagem1);
    encerrarSons(&personagem2);
    UnloadTexture(button); 
    CloseAudioDevice();
    CloseWindow(); 

    return 0;
}
