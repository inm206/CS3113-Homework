#include "SpaceInvaders.h"

using namespace std;

enum GameState { STATE_MENU=1, STATE_GAME=2, STATE_GAME_OVER=3, STATE_WIN=4};

GLuint LoadTexture(const char *image_path){
    SDL_Surface *surface = IMG_Load(image_path);
    if (!surface){ return -1; }
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void PrintText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a){
    
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float texture_size = 1.0 / 16.0f;
    vector<float> vertexData;
    vector<float> texCoordData;
    vector<float> colorData;
    for (GLuint i = 0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
        vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f * size), 0.5f * size, ((size + spacing) * i) +
            (-0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), -0.5f * size, ((size + spacing) * i) + (0.5f * size), 0.5f
            * size });
        texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
            texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
    }
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4);
    glDisableClientState(GL_COLOR_ARRAY);
}

SpaceInvaders::SpaceInvaders() : keys(SDL_GetKeyboardState(NULL))
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    fontTexture = LoadTexture("pixel_font.png");
    bg = LoadTexture("bg.jpg");
    spriteSheet = LoadTexture("sprites.png");
  
    ship = new GameApp(spriteSheet, 0.0f, -0.9f, 0.0f, 0.0f, 0.0f, 0.0f / 256.0f, 156.0f / 512.0f, 112.0f / 256.0f, 75.0f / 512.0f);
    
    background = new GameApp(bg, -0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
    
    Init();
    
    lastFrameTicks = 0.0f;
    
}



SpaceInvaders::~SpaceInvaders(){
    delete ship;
    for (GLuint i = 0; i < playerBullets.size(); i++) { delete playerBullets[i]; }
    for (GLuint i = 0; i < enemyBullets.size(); i++) { delete enemyBullets[i]; }
    for (GLuint i = 0; i < enemies.size(); i++) { delete enemies[i]; }
    Mix_FreeChunk(winSound);
    Mix_FreeChunk(shootSound);
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(injurySound);
    Mix_FreeChunk(enemySound);
    Mix_FreeChunk(loseSound);
    Mix_FreeMusic(music);
    SDL_Quit();
}

void SpaceInvaders::Init(){
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    for (GLuint i = 0; i < playerBullets.size(); i++) { delete playerBullets[i]; }
    for (GLuint i = 0; i < enemyBullets.size(); i++) { delete enemyBullets[i]; }
    for (GLuint i = 0; i < enemies.size(); i++) { delete enemies[i]; }
    playerBullets.clear();
    enemyBullets.clear();
    enemies.clear();
    
    for (float i = 0.3f; i < 1.0f; i+=0.3f){
        enemies.push_back(new GameApp(spriteSheet, -0.8f, i, 0.2f, -0.05f, 0.0f, 114.0f / 256.0f, 0.0f / 512.0f, 93.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, -0.4f, i, 0.2f, -0.05f, 0.0f, 0.0f / 256.0f, 311.0f / 512.0f, 104.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, 0.0f, i, 0.2f, -0.05f, 0.0f, 0.0f / 256.0f, 397.0f / 512.0f, 103.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, 0.4f, i, 0.2f, -0.05f, 0.0f, 114.0f / 256.0f, 86.0f / 512.0f, 82.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, 0.8f, i, 0.2f, -0.05f, 0.0f, 106.0f / 256.0f, 311.0f / 512.0f, 97.0f / 256.0f, 84.0f / 512.0f));
    }
    state = 1;
    level = 1;
    score = 0;
    speeder = 1;
    alive = true;
    damage = 0;
    timePassed = 0.0f;
    
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    shootSound = Mix_LoadWAV("shoot.wav");
    winSound = Mix_LoadWAV("win.wav");
    hitSound = Mix_LoadWAV("hitSound.wav");
    injurySound = Mix_LoadWAV("injurySound.wav");
    enemySound = Mix_LoadWAV("enemySound.wav");
    loseSound = Mix_LoadWAV("loseSound.wav");
    music = Mix_LoadMUS("track.mp3");
    Mix_PlayMusic(music, -1);
}

void SpaceInvaders::Init2(){
    
    for (GLuint i = 0; i < playerBullets.size(); i++) { delete playerBullets[i]; }
    for (GLuint i = 0; i < enemyBullets.size(); i++) { delete enemyBullets[i]; }
    for (GLuint i = 0; i < enemies.size(); i++) { delete enemies[i]; }
    playerBullets.clear();
    enemyBullets.clear();
    enemies.clear();
    
    for (float i = 0.3f; i < 1.0f; i+=0.3f){
        enemies.push_back(new GameApp(spriteSheet, -0.8f, i, 0.2f, -0.05f, 0.0f, 114.0f / 256.0f, 0.0f / 512.0f, 93.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, -0.4f, i, 0.2f, -0.05f, 0.0f, 0.0f / 256.0f, 311.0f / 512.0f, 104.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, 0.0f, i, 0.2f, -0.05f, 0.0f, 0.0f / 256.0f, 397.0f / 512.0f, 103.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, 0.4f, i, 0.2f, -0.05f, 0.0f, 114.0f / 256.0f, 86.0f / 512.0f, 82.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new GameApp(spriteSheet, 0.8f, i, 0.2f, -0.05f, 0.0f, 106.0f / 256.0f, 311.0f / 512.0f, 97.0f / 256.0f, 84.0f / 512.0f));
    }
    state = 2;
    /*score = 0;
    alive = true;
    damage = 0;
    timePassed = 0.0f;
    
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    shootSound = Mix_LoadWAV("shoot.wav");
    winSound = Mix_LoadWAV("win.wav");
    hitSound = Mix_LoadWAV("hitSound.wav");
    injurySound = Mix_LoadWAV("injurySound.wav");
    enemySound = Mix_LoadWAV("enemySound.wav");
    loseSound = Mix_LoadWAV("loseSound.wav");
    music = Mix_LoadMUS("track.mp3");
    //Mix_PlayMusic(music, -1);*/
}

bool SpaceInvaders::UpdateAndRender()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    Update(elapsed);
    Render();
    return done;
}

void SpaceInvaders::UpdateBullets(float elapsed, vector<GameApp*>& bullets){
    for (GLuint i = 0; i < bullets.size(); i++) {
        if (bullets[i]->y > 1.1 || bullets[i]->y < -1.1){
            if (bullets[i]){
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
            }
        }
        else{
            bullets[i]->y += bullets[i]->direction_y*bullets[i]->speed*elapsed;
        }
        
    }
}

void SpaceInvaders::UpdateEnemiesAndScore(float elapsed, vector<GameApp*>& enemies, vector<GameApp*>& bullets){
    timePassed += elapsed;
    for (GLuint i = 0; i < enemies.size(); i++) {
        enemies[i]->x += enemies[i]->direction_x*enemies[i]->speed*elapsed;
        if (enemies[i]->y < -0.75f){ state = 3; alive = false; }
        if (enemies[i]->x>1.2f||enemies[i]->x<-1.2f){
            for (GLuint k = 0; k < enemies.size(); k++) {
                enemies[k]->direction_x= -(enemies[k]->direction_x);
                enemies[k]->x += enemies[k]->direction_x*enemies[k]->speed*elapsed;
                enemies[k]->y += enemies[k]->direction_y;
                enemies[k]->speed += (0.5*speeder);
            }
        }
        if (enemies[i]->x > ship->x-0.01f && enemies[i]->x< ship->x+0.01f&&timePassed>0.2f){
            enemyBullets.push_back(new GameApp(spriteSheet, enemies[i]->x, enemies[i]->y, 0.0f, -1.0f, 0.0f, 173.0f / 256.0f, 211.0f / 512.0f, 13.0f / 256.0f, 57.0f / 512.0f));
            timePassed = 0.0f;
        }
        for (GLuint j = 0; j < bullets.size(); j++) {
            if ((enemies[i]->x + (enemies[i]->width*0.3f) > bullets[j]->x) &&
                (enemies[i]->x - (enemies[i]->width*0.3f) < bullets[j]->x) &&
                (enemies[i]->y + (enemies[i]->height*0.3f) > bullets[j]->y) &&
                (enemies[i]->y - (enemies[i]->height*0.3f) < bullets[j]->y)){
                if (bullets[j]){
                    delete bullets[j];
                    bullets.erase(bullets.begin() + j);
                }
                if (enemies[i]){
                    delete enemies[i];
                    enemies.erase(enemies.begin() + i);
                    score += 100;
                    Mix_PlayChannel( -1, hitSound, 0);
                }
                
                if (!enemies.size()) state = 3;
                break;
            }
        }
    }
}

void SpaceInvaders::UpdateDamage(float elapsed, vector<GameApp*>& bullets){
    for (GLuint j = 0; j < bullets.size(); j++) {
        if ((ship->x + (ship->width*0.3f) > bullets[j]->x) &&
            (ship->x - (ship->width*0.3f) < bullets[j]->x) &&
            (ship->y + (ship->height*0.3f) > bullets[j]->y) &&
            (ship->y - (ship->height*0.3f) < bullets[j]->y)){
            damage++;
            Mix_PlayChannel( -1, injurySound, 0);
            if (bullets[j]){
                delete bullets[j];
                bullets.erase(bullets.begin() + j);
            }
        }
    }
    if (damage > 3){
        alive = false;
        state = 3;
    }
}


void SpaceInvaders::Update(float elapsed)
{
    UpdateBullets(elapsed, playerBullets);
    UpdateBullets(elapsed, enemyBullets);
    UpdateEnemiesAndScore(elapsed, enemies, playerBullets);
    UpdateDamage(elapsed, enemyBullets);
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if(keys[SDL_SCANCODE_ESCAPE]){
            done = true;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == 1){
                if (state == 2){
                    Mix_PlayChannel(-1, shootSound, 0);
                    playerBullets.push_back(new GameApp(spriteSheet, ship->x, -0.8f, 0.0f, 1.0f, 0.0f, 158.0f / 256.0f, 211.0f / 512.0f, 13.0f / 256.0f, 57.0f / 512.0f));
                }
                else if (level > 1){
                    Init2();
                    speeder += 1;
                }
                else if (state == 3){
                    Init();
                }
                else state = 2;
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            ship->x = (((float)event.motion.x / 800.0f) * 2.666f) - 1.333f;
            if (ship->x > 1.2f){ ship->x = 1.2f; }
            else if (ship->x < -1.2f){ ship->x = -1.2f; }
        }
    }
}


void SpaceInvaders::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    switch (state) {
        case STATE_MENU:
            RenderMenu();
            break;
        case STATE_GAME:
            RenderGame();
            break;
        case STATE_GAME_OVER:
            if (alive){
                RenderWin();
            }
            else {
                RenderLoss();
            }
            break;
    }
    SDL_GL_SwapWindow(displayWindow);
}

void SpaceInvaders::RenderMenu(){
    glLoadIdentity();
    glTranslatef(-0.8f, 0.5f, 0.0);
    PrintText(fontTexture, "Space Invaders",0.13f,0.0f,1.0f,1.0f,1.0f,1.0f);
    glLoadIdentity();
    glTranslatef(-0.3f, -0.5f, 0.0);
    PrintText(fontTexture, "Click To Begin", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
}

void SpaceInvaders::RenderGame(){
    background->DrawSprite(2.0f);
    glLoadIdentity();
    glTranslatef(-1.2f, 0.85f, 0.0);
    PrintText(fontTexture, to_string(score),0.05f,0.0f,1.0f,1.0f,1.0f,1.0f);
    glTranslatef(0.0f, 0.1f, 0.0);
    for (GLuint i = damage; i < 3; i++){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, spriteSheet);
        glMatrixMode(GL_MODELVIEW);
        GLfloat quad[] = { -112.0f / 256.0f * 0.1, 75.0f / 512.0f * 0.2, -112.0f / 256.0f * 0.1, -75.0f / 512.0f * 0.2,
            112.0f / 256.0f * 0.1, -75.0f / 512.0f * 0.2, 112.0f / 256.0f * 0.1, 75.0f / 512.0f * 0.2 };
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat quadUVs[] = { 0.0f / 256.0f, 156.0f / 512.0f, 0.0f / 256.0f, 156.0f / 512.0f + 75.0f / 512.0f, 0.0f / 256.0f + 112.0f / 256.0f, 156.0f / 512.0f + 75.0f / 512.0f, 0.0f / 256.0f + 112.0f / 256.0f, 156.0f / 512.0f };
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
        glTranslatef(0.1f, 0.0f, 0.0);
    }
    glLoadIdentity();
    
    ship->DrawSprite(0.3f);
    
    
    for (GLuint i = 0; i < enemies.size(); i++) {
        enemies[i]->DrawSprite(0.3f);
    }
    for (GLuint i = 0; i < playerBullets.size(); i++) {
        playerBullets[i]->DrawSprite(0.3f);
    }
    for (GLuint i = 0; i < enemyBullets.size(); i++) {
        enemyBullets[i]->DrawSprite(0.3f);
        //Mix_PlayChannel(1, enemySound, 0);
    }
}

void SpaceInvaders::RenderWin(){
    glLoadIdentity();
    glTranslatef(-0.85f, 0.5f, 0.0);
    PrintText(fontTexture, "You Won!", 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
    glTranslatef(0.35f, -1.0f, 0.0);
    PrintText(fontTexture, "Click To Enter Next Level", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    Mix_PlayChannel(-1, winSound, 0);
    //state = 4;
    level += 1;
}

void SpaceInvaders::RenderLoss(){
    glLoadIdentity();
    glTranslatef(-0.5f, 0.5f, 0.0);
    PrintText(fontTexture, "You Lost!", 0.13f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.25f, 0.0);
    PrintText(fontTexture, "Final score: ", 0.05f,0.0f,1.0f,1.0f,1.0f,1.0f);
    glTranslatef(0.7f, 0.0f, 0.0);
    PrintText(fontTexture, to_string(score),0.05f,0.0f,1.0f,1.0f,1.0f,1.0f);
    glTranslatef(-0.7f, -0.5f, 0.0);
    PrintText(fontTexture, "Click To Try Again", 0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    Mix_PlayChannel(-1, loseSound, 0);
    level = 1;
}