#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Seed.h"
#include "Tool.h"
#include "GUI.h"
#include "OBJECTS/Ball.h"
#include "OBJECTS/Obstacles.h"

class GameScene : public cocos2d::LayerGradient {
    
    public:

        // there's no 'id' in cpp, so we recommend returning the class instance pointer
        static cocos2d::Scene* createScene();

        // here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();
        
        // implement the "static create()" method manually
        CREATE_FUNC(GameScene);

    private:

        // METHODS //

        // beginning //

        void setPhysicsWorld(cocos2d::PhysicsWorld *world) {sceneWorld = world;}
        void touching();
        void setCurrentTouch(cocos2d::Vec2 loc);
        void delayTouch(float dt);
        void set_isTouchable(float dt);
        void onKeyReleasing();

        // obstacle //

        void obs_paddleHit(int member);
        void obs_paddleMove(int member, int valSign);

        void obs_paddleStopHit_left(float dt);
        void obs_paddleStopHit_right(float dt);
        void obs_paddleStopMove_left(float dt);
        void obs_paddleStopMove_right(float dt);

        void obs_objectsUpdate(float dt);
        void obs_backgroundScrollDown(float dt);
        void obs_backgroundScreenSaver(float dt);

        void obs_laserAnimate(float dt);
        void obs_coinAnimate(float dt);
        void obs_magnetAnimate(float dt);
        void obs_shieldAnimate(float dt);

        void obs_coinsAttracted(float dt);
        void obs_coinsAttracted_unschedule(float dt);
        void obs_shieldOff(float dt);

        void onDetectedCoin();
        void onDetectedMagnet();
        void onDetectedShield();

        // ball //

        void ball_reflectionOffset(float dt);

        // gui //

        void gui_noticeBoardChange(
            
            std::string designationBefore,
            std::string designationNext,
            float dt
        );

        void gui_showNoticeBoard(float dt);

        // scenes //

        void startGame();
        void goToPauseScene();
        void goToGameOverScene(float dt);
        cocos2d::RenderTexture *getRenTex();

        // scoring //

        void scoreAddition(int addition = 1);
        void scoreAddition(float dt);

        // tutorial needs //

        void checkingTutorialNeeds();
        void tut_tut1(float dt);
        void tut_tut2(float dt);

        // OBJECTS //

        cocos2d::PhysicsWorld *sceneWorld;

        Seed seed;
        Tool tool;
        GUI gui;
        Ball ball;
        Obstacles obs;

        // VARIABLES //

        std::string gui_noticeBoardDesignation;

        int curTc;

        bool
            isTouchable = false,
            isTouched = false,
            isGameStarted = false,
            isExplode_receiver = false;

        cocos2d::Vec2 touchBegan, touchMoved, touchEnded;
};

#endif // __GAME_SCENE_H__