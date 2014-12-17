#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "GameApp.h"
#include <vector>
#include <string>
#include <SDL_mixer.h>


class SpaceInvaders{
    
public:
    SpaceInvaders();
    ~SpaceInvaders();
    
    void Init();
    void Init2();
    bool UpdateAndRender();
    void Render();
    void Update(float elapsed);
    
private:
    int state;
    int score;
    int damage;
    int level;
    int speeder;
    bool alive;
    bool done;
    float lastFrameTicks;
    float timePassed;
    SDL_Window* displayWindow;

    
    GLuint fontTexture;
    GLuint spriteSheet;
    GLuint bg;
    GameApp* ship;
    GameApp* background;
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
    void RenderOver();
    
    Mix_Chunk *winSound, *shootSound, *hitSound, *injurySound, *enemySound, *loseSound;
    Mix_Music *music;
    
    const Uint8 *keys;
};