
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

void DrawSprite(GLint texture, float x, float y, float rotation) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    GLfloat quad[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}

GLuint LoadTexture(const char *image_path) {
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    //glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
    
	bool done = false;
	
	SDL_Event event;
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    
    float lastFrameTicks = 0.0f;
    float emojiAngle = 0.0f;
    
    GLint crack = LoadTexture("crack.jpg");
    GLint flower = LoadTexture("flower.jpg");
    //GLint grout = LoadTexture("grout.jpg");
    
    
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        
        lastFrameTicks = ticks;
        emojiAngle += elapsed;
        
        glClearColor(0.4f, 0.2f, 0.4f, 1.0f); //sets color to clear (REMOVES NOISE THAT WAS THERE BEFORE)
        glClear(GL_COLOR_BUFFER_BIT); //clears screen using above color
        
        glMatrixMode(GL_MODELVIEW);
        
        
        GLfloat triangle[] = {-1.0f, 0.5f, -1.5f, -0.5f, -0.5f, -0.5f}; //defines points of vertices of triangle
        glVertexPointer(2, GL_FLOAT, 0, triangle);
        glEnableClientState(GL_VERTEX_ARRAY);
    
        GLfloat triangleColors[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0};
        glColorPointer(3, GL_FLOAT, 0, triangleColors);
        glEnableClientState(GL_COLOR_ARRAY); //This enables the code above that gets the colored vertexes working
        
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glDisableClientState(GL_COLOR_ARRAY);
        
        //Triangle2
        GLfloat triangle2[] = {1.0f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f}; //defines points of vertices of triangle
        glVertexPointer(2, GL_FLOAT, 0, triangle2);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        
        //Square with paint crack
        DrawSprite(crack, 0.0, 0.0, emojiAngle*30);
        
        GLfloat quad[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        
        //Square with flower
        DrawSprite(flower, 0.0, 0.0, 0.0);
        
        GLfloat quad2[] = {0.75f, 0.0f, 0.75f, 0.5f, 1.0f, 0.0f, 1.0f, 0.5f};
        glVertexPointer(2, GL_FLOAT, 0, quad2);
        glEnableClientState(GL_VERTEX_ARRAY);
        /*
        //Square with grout
        DrawSprite(grout, 0.0, 0.0, 0.0);
        
        GLfloat quad3[] = {-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f};
        glVertexPointer(2, GL_FLOAT, 0, quad3);
        glEnableClientState(GL_VERTEX_ARRAY);
        */
        
        
		SDL_GL_SwapWindow(displayWindow);
	}
    
	SDL_Quit();
	return 0;
}