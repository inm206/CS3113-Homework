//
//  Entity.h
//  NYUCodebase
//
//  Created by Isaac Mbira on 9/17/14.
//  Copyright (c) 2014 Ivan Safrin. All rights reserved.
//

#ifndef NYUCodebase_Entity_h
#define NYUCodebase_Entity_h

class Entity {
    public:
        Entity();
        Entity(int texture, float posX, float posY, float rot, float w, float h);
        ~Entity();
    
        void Draw();
    
        float x;
        float y;
        float rotation;
    
        int textureID;
    
        float width;
        float height;
    
        float speed;
        float direction_x;
        float direction_y;
};

#endif
