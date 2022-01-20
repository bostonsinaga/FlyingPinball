#ifndef __BALL_H__
#define __BALL_H__

#include "cocos2d.h"
#include "Seed.h"
#include "Tool.h"

class Ball {

    public :

        void init(cocos2d::Layer *layer_in, Seed *seed_in, Tool *tool_in);
        void swap(int valSign);
        void spawn();
        void hide();
        void hideExplode(bool isHiding);
        void explode();
        void shieldOn(bool isOn);
        void reflectionOffset();
        void set_isShieldOn(bool isOn);
        
        int getCurrentSelected();
        bool isFall(bool *isExplode_receiver);

        cocos2d::Sprite *getBall();
        cocos2d::Sprite *getReflection();
        
    private:

        void attachPhysicsBody();
        void randomPosSet();
        void smokeEffect();

        // OBJECTS //

        static const int
            ballsCount = 5,
            smokeCounts = 2;

        static const int
            ballFramesCount = 3,
            smokeFramesCount = 8;

        cocos2d::Sprite
            *ball[ballsCount][ballFramesCount],
            *smoke[smokeCounts][smokeFramesCount],
            *reflection;
        
        cocos2d::DrawNode *shield;
        cocos2d::PhysicsBody *ballBody, *reflectionBody;
        cocos2d::Layer *layer;

        Seed *seed;
        Tool *tool;
        
        // VARIABLES //

        int
            currentSelected,
            currentSmoke = 0;

        bool isShieldOn = false, isExplode_sender = false;
};

#endif // __BALL_H__