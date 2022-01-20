#ifndef __TUTORIAL_SCENE_H__
#define __TUTORIAL_SCENE_H__

#include "cocos2d.h"
#include "Tool.h"

class TutorialScene : public cocos2d::Layer {
    
    public:

        // there's no 'id' in cpp, so we recommend returning the class instance pointer
        static cocos2d::Scene* createScene(cocos2d::RenderTexture *renTex, int tutorialTag_in);

        // here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();
        
        // implement the "static create()" method manually
        CREATE_FUNC(TutorialScene);

    private:

        // METHODS //

        void touching();
        void delayTouch(float dt);
        void set_isTouchable(float dt);
        void onKeyReleasing();

        void action_tut_sub1(float dt);
        void action_tut_sub2(float dt);
        void action_tut1_sub1(float dt);
        void action_tut1_sub2(float dt);
        void action_tut2_sub1(float dt);
        void action_tut2_sub2(float dt);

        void passNotice(float dt);
        void passAction(float dt);

        void popGameScene();

        // OBJECTS //

        cocos2d::EventListenerTouchOneByOne *touchListener;
        cocos2d::EventListenerKeyboard *backButtonListener;

        cocos2d::DrawNode *darkScreen;
        cocos2d::Sprite *finger[2];
        cocos2d::Label *label[2], *passLabel;

        Tool tool;

        // VARIABLES //

        bool isTouchable = false;
        int pausedStartTime = std::time(0);
};

#endif // __TUTORIAL_SCENE_H__