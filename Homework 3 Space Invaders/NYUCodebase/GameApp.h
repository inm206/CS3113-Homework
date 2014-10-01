//
//  GameApp.h
//  NYUCodebase
//
//  Created by Isaac Mbira on 9/28/14.
//  Copyright (c) 2014 Isaac Mbira. All rights reserved.
//

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class GameApp{

public:

    GameApp(int texture, float posX, float posY, float rot, float w, float h);
    GameApp(int spriteSheet, float posX, float posY, float dX, float dY, float rot, float uSprite, float vSprite, float w, float h);
    ~GameApp();
    
    void Draw();
    void DrawSprite(float scale);
    
    float x;
    float y;
    float rotation;
    int textureID;
    float u;
    float v;
    float width;
    float height;
    
    float speed;
    float direction_x;
    float direction_y;
};