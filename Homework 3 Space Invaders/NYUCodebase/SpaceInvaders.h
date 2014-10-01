//
//  SpaceInvaders.h
//  NYUCodebase
//
//  Created by Isaac Mbira on 9/28/14.
//  Copyright (c) 2014 Isaac Mbira. All rights reserved.
//

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "GameApp.h"
#include <vector>
#include <string>


class SpaceInvaders{
    
public:
    SpaceInvaders();
    ~SpaceInvaders();
    
    void Init();
    bool UpdateAndRender();
    void Render();
    void Update(float elapsed);
    
private:
    int state;
    int score;
    int damage;
    bool alive;
    bool done;
    float lastFrameTicks;
    float timePassed;
    SDL_Window* displayWindow;
    
    GLuint fontTexture;
    GLuint spriteSheet;
    GameApp* ship;
    std::vector<GameApp*> enemies;
    std::vector<GameApp*> playerBullets;
    std::vector<GameApp*> enemyBullets;
    
    void UpdateBullets(float elapsed, std::vector<GameApp*>& bullets);
    void UpdateEnemiesAndScore(float elapsed, std::vector<GameApp*>& enemies, std::vector<GameApp*>& bullets);
    void UpdateDamage(float elapsed, std::vector<GameApp*>& bullets);
    
    void UpdateMenu();
    void UpdateGame();
    
    void RenderMenu();
    void RenderGame();
    void RenderWin();
    void RenderLoss();
};