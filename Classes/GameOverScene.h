#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"
#include "Tool.h"
#include "GUI.h"

class GameOverScene : public cocos2d::Layer {
    
    public:

        // there's no 'id' in cpp, so we recommend returning the class instance pointer
        static cocos2d::Scene* createScene(cocos2d::RenderTexture *renTex);

        // here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();
        
        // implement the "static create()" method manually
        CREATE_FUNC(GameOverScene);

    private:

        // METHODS //

        void touching();
        void delayTouch(float dt);
        void set_isTouchable(float dt);
        void onKeyReleasing();

        // scene replacement //

        void goToMainMenuScene(float dt);
        void set_isAvailableToLeave(float dt);

        // OBJECTS //

        Tool tool;
        GUI gui;

        // VARIABLES //

        bool
            isTouchable = false,
            isTouched = false,
            isAvailableToLeave = false;

        float
            xBegan, yBegan,
            xEnded, yEnded;
};

#endif // __GAME_OVER_SCENE_H__