#ifndef __OBSTACLES_H__
#define __OBSTACLES_H__

#include "cocos2d.h"
#include "Seed.h"
#include "Tool.h"
#include "GUI.h"

class Obstacles {

    public :

        void init(cocos2d::Layer *layer_in, Seed *seed_in, Tool *tool_in, GUI *gui_in);
        void brickInit();

        void brickTailDetection();
        void fallObjectsBackToOrigin();
        int getObjectFramesCount(std::string name);
        std::string objectsCollision(cocos2d::Sprite *ball, int currentBallSelected);

        void laserAnimate();
        void coinAnimate();
        void magnetAnimate();
        void shieldAnimate();

        cocos2d::Sprite *containerAnimate(bool isFlipX);

        std::string itemAttracted(cocos2d::Sprite *ballReflection, cocos2d::Sprite *item);
        int coinsAttracted(cocos2d::Sprite *ballReflection);

        void paddleHit(int member);
        void paddleMove(int member, int valSign);
        void paddleStop(int member, bool isHit);
        void setDefault_isPaddleStopped(int member);
        void set_isPaddleMoving_false();
        
        void backgroundScreenSaver();
        void backgroundScrollDown();

        bool get_isFuncPaddleStopHit(int member);
        bool get_isPaddleStopped(int member);
        bool get_isPaddleMoved(int member);
        bool get_isPaddleMoving(int member);
        
    private:

        void brickSpawn();
        void setBricksPos(int member = -1);
        void setBrickBodyProperties(bool isVisible, int categoryBitmask, int collisionBitmask);
        void updateVelocity();

        void sparkEffect(cocos2d::Node *ball, cocos2d::Node *target, cocos2d::Color3B color);
        void sparkEffect(cocos2d::Node *target, cocos2d::Color3B color);

        void setFallObjectCondition(
            cocos2d::Sprite **sprites,
            cocos2d::PhysicsBody *body,
            cocos2d::Vec2 position,
            cocos2d::Vec2 velocity,
            int count);

        void setFallObjectCondition(
            cocos2d::Sprite *sprite,
            cocos2d::PhysicsBody *body,
            cocos2d::Vec2 position,
            cocos2d::Vec2 velocity);

        void holdersMove(int member, int valSign, bool isHit);
        bool isAtVisible(cocos2d::Sprite *sprite);

        static const int
            edgesCount = 3,
            paddlesCount = 2,
            holdersCount = 4,
            jointsCount = 4,
            bricksCount = 30,
            leftThornsCount = 3,
            rightThornsCount = 3,
            fullThornsCount = 3,
            lasersCount = 12,
            coinsCount = 24 + 2,
            magnetsCount = 3 + 2,
            shieldsCount = 3 + 2,
            sparksCount = 16,
            containersCount = 12,
            containerAnimationsCount = 2,
            backgroundsCount = 4;

        static const int
            laserFramesCount = 4,
            coinFramesCount = 8,
            magnetFramesCount = 16,
            shieldFramesCount = 16,
            sparkFramesCount = 6,
            containerAnimationFramesCount = 7;

        cocos2d::Node *edgeNode[edgesCount];

        cocos2d::Sprite
            *paddle[paddlesCount],
            *holder[holdersCount],
            *joint[jointsCount],
            *brick[bricksCount],
            *leftThorn[leftThornsCount],
            *rightThorn[rightThornsCount],
            *fullThorn[fullThornsCount],
            *laser[lasersCount][laserFramesCount],
            *coin[coinsCount][coinFramesCount],
            *magnet[magnetsCount][magnetFramesCount],
            *shield[shieldsCount][shieldFramesCount],
            *spark[sparksCount][sparkFramesCount],
            *sparkSub[sparksCount][sparkFramesCount],
            *container[containersCount],
            *containerAnimation[containerAnimationsCount][containerAnimationFramesCount],
            *background[backgroundsCount];
        
        cocos2d::PhysicsBody
            *edgeBody[edgesCount],
            *paddleBody[paddlesCount],
            *holderBody[holdersCount],
            *brickBody[bricksCount],
            *leftThornBody[leftThornsCount],
            *rightThornBody[rightThornsCount],
            *fullThornBody[fullThornsCount],
            *laserBody[lasersCount],
            *coinBody[coinsCount],
            *magnetBody[magnetsCount],
            *shieldBody[shieldsCount],
            *containerBody[containersCount];

        cocos2d::Vec2
            paddleBuffPos[paddlesCount],
            holderBuffPos[holdersCount],
            jointBuffPos[jointsCount],
            brickPos[bricksCount / 3];

        // determined balls color
        cocos2d::Color3B ballColor[5] = {

            cocos2d::Color3B(255, 199,   0),
            cocos2d::Color3B( 93, 200,  43),
            cocos2d::Color3B(  0, 242, 255),
            cocos2d::Color3B(255,   1,  46),
            cocos2d::Color3B( 11,   0, 255)
        };

        cocos2d::Layer *layer;

        Seed *seed;
        Tool *tool;
        GUI *gui;
        
        const float paddleRot = 45.0, holderRot = paddleRot / 2;

        int
            currentBrick = 0, brickTailTag, MMbgEffect_valSign = 1,
            currentLeftThorn = 0,
            currentRightThorn = 0,
            currentFullThorn = 0,
            currentLaser = 0,
            currentCoin = 0,
            currentMagnet = 0,
            currentShield = 0,
            currentSpark = 0,
            currentContainer = 0,
            currentContainerAnimation = 0,
            currentBackground = 0,
            currentCollidedContainer;

        float BGHeight;

        bool
            isPaddleStopped[2] = {true, true},
            isPaddleMoved[2] = {false, false},
            isPaddleMoving[2] = {false, false},
            isFuncPaddleStopHit[2] = {false, false},
            isPlayedSFX_hit_laser[lasersCount],
            isPlayedSFX_hit_leftThorn[leftThornsCount],
            isPlayedSFX_hit_rightThorn[rightThornsCount],
            isPlayedSFX_hit_fullThorn[fullThornsCount],
            isPlayedSFX_hit_container[containersCount],
            isPlayedSFX_hit_brick[bricksCount],
            isPlayedSFX_hit_paddle[paddlesCount],
            isPlayedSFX_hit_holder[holdersCount],
            isPlayedSFX_hit_edge = false;
};

#endif // __OBSTACLES_H__