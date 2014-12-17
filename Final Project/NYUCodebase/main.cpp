#include "SpaceInvaders.h"

int main(int argc, char *argv[]){
    
    SpaceInvaders invader;
    while (!invader.UpdateAndRender()) {
    }
    return 0;
}