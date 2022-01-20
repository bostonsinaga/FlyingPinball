#ifndef _TOOL_H_
#define _TOOL_H_

#include "cocos2d.h"
#include "AudioEngine.h"

USING_NS_CC;

class Tool {

    public:

        Sprite *mySprite(std::string filePath) {

            auto sprite = Sprite::create(filePath);
            sprite->setContentSize(sprite->getContentSize() * getUDef()->getFloatForKey("SCALE RATE"));
            return sprite;
        }

        Label *myLabel(std::string str, float height = 0.0) {

            if (height == 0.0) height = getOSz("bh");
            auto label = Label::createWithSystemFont(str, "", height);
            return label;
        }

        void mySpriteFrames(
            
            Sprite **sprite,
            int count,
            const char *format,
            int z_order,
            Node *parent
        ) {

            char name[100];

            for (int i = 0; i < count; i++) {
                
                std::sprintf(name, format, i + 1);
                sprite[i] = mySprite(name);

                if (i == 0) {

                    sprite[i]->setPosition(pos(0.5, 2.0));
                    parent->addChild(sprite[i], z_order);
                }
                else {
                    
                    sprite[i]->setOpacity(0);
                    sprite[i]->setPosition(sprite[0]->getContentSize() / 2);
                    sprite[0]->addChild(sprite[i]);
                }
            }
        }

        // suitable for fall object
        void mySpriteAnimation(
            
            Sprite **sprite,
            int count,
            float dt = 0.1,
            Color3B color = Color3B::WHITE
        ) {

            for (int i = 0; i < count; i++) {

                if (color != Color3B::WHITE) sprite[i]->setColor(color);
                sprite[i]->runAction(Sequence::create(

                    DelayTime::create(i * dt), 
                    FadeIn::create(0),
                    DelayTime::create(dt),
                    FadeOut::create(0),
                    nullptr
                ));
            }
        }

        // suitable for ball
        void mySpriteAnimation(Sprite **sprite, int count, float dt, bool isToFro) {

            for (int i = 0; i < count; i++) {

                if (isToFro) {
                
                    sprite[i]->runAction(RepeatForever::create(

                        i != count / 2 ?
                        Sequence::create(

                            DelayTime::create(i * dt),
                            FadeIn::create(0),
                            DelayTime::create(dt),
                            FadeOut::create(0),
                            DelayTime::create((count - i) * dt),
                            nullptr
                        ) :
                        Sequence::create(

                            DelayTime::create(i * dt),
                            FadeIn::create(0),
                            DelayTime::create(dt),
                            FadeOut::create(0),
                            DelayTime::create(dt),
                            FadeIn::create(0),
                            DelayTime::create(dt),
                            FadeOut::create(0),
                            nullptr
                        )
                    ));
                }
                else {
                    
                    sprite[i]->runAction(RepeatForever::create(

                        Sequence::create(

                            DelayTime::create(i * dt),
                            FadeIn::create(0),
                            DelayTime::create(dt),
                            FadeOut::create(0),
                            nullptr
                        )
                    ));
                }
            }
        }

        void setUserValue(

            const char *dataName,
            int value,
            int *valueName = nullptr,
            int defaultValue = 0
        ) {

            uDef->setIntegerForKey(dataName, value);
            uDef->flush();
            if (valueName != nullptr) *valueName = uDef->getIntegerForKey(dataName, defaultValue);
        }

        void setUserValue(

            const char *dataName,
            bool value,
            bool *valueName = nullptr,
            bool defaultValue = false
        ) {

            uDef->setIntegerForKey(dataName, value);
            uDef->flush();
            if (valueName != nullptr) *valueName = uDef->getIntegerForKey(dataName, defaultValue);
        }

        void initMeasurement() {

            Sprite *measureObj;
            
            measureObj = mySprite("GENERAL/brick.png");
            brickWidth = measureObj->getContentSize().width;
            brickHeight = measureObj->getContentSize().height;
            measureObj->runAction(RemoveSelf::create());

            measureObj = mySprite("GENERAL/paddle.png");
            paddleWidth = measureObj->getContentSize().width;
            paddleHeight = measureObj->getContentSize().height;
            measureObj->runAction(RemoveSelf::create());
        }

        void currentObjectCounter(int *currentTag, int total) {

            int tag = *currentTag;
            tag++;
            *currentTag = tag;
            if (*currentTag >= total) *currentTag = 0;
        }

        void changeObjectParent(Node *child, Node *parent, int z_order) {

            child->retain();
            child->removeFromParent();
            parent->addChild(child, z_order);
            child->release();
        }

        Size getOSz(char dataTag) {

            if      (dataTag == 'b') return Size(brickWidth, brickHeight);
            else if (dataTag == 'p') return Size(paddleWidth, paddleHeight);
            else return Size::ZERO;
        }

        float getOSz(std::string dataName) {

            if      (dataName == "bw") {return brickWidth;} // brick
            else if (dataName == "bh") {return brickHeight;}
            else if (dataName == "pw") {return paddleWidth;} // paddle
            else if (dataName == "ph") {return paddleHeight;}
            else return 0.0;
        }

        Size getVSz() {

            return vSize;
        }

        float getVSz(char axis, float ratio = 1.0) {

            return axis == 'x' ?
                vSize.width * ratio + vOrigin.x:
                vSize.height * ratio + vOrigin.y;
        }

        bool isAtTouchPos(Node *node, char touchCondition, Vec2 began, Vec2 ended) {

            /*
                'b' for Began
                'e' for Began with Ended
            */
            if (touchCondition == 'b') {

                if (began.x >= node->getPosition().x - node->getContentSize().width / 2 && began.x <= node->getPosition().x + node->getContentSize().width / 2
                    && began.y >= node->getPosition().y - node->getContentSize().height / 2 && began.y <= node->getPosition().y + node->getContentSize().height / 2) {

                    return true;
                }
                else return false;
            }
            else if (touchCondition == 'e') {

                if (began.x >= node->getPosition().x - node->getContentSize().width / 2 && began.x <= node->getPosition().x + node->getContentSize().width / 2
                    && began.y >= node->getPosition().y - node->getContentSize().height / 2 && began.y <= node->getPosition().y + node->getContentSize().height / 2
                    && ended.x >= node->getPosition().x - node->getContentSize().width / 2 && ended.x <= node->getPosition().x + node->getContentSize().width / 2
                    && ended.y >= node->getPosition().y - node->getContentSize().height / 2 && ended.y <= node->getPosition().y + node->getContentSize().height / 2) {

                    return true;
                }
                else return false;
            }
            else return false;
        }

        bool isAtCollision(Node *node_1, Node *node_2) {

            Vec2 posDif = node_1->getPosition() - node_2->getPosition();

            if (std::abs(posDif.x) <= node_1->getContentSize().width / 2 + node_2->getContentSize().width / 2
                && std::abs(posDif.y) <= node_1->getContentSize().height / 2 + node_2->getContentSize().height / 2)
                return true;
            else return false;
        }

        Vec2 ctrPos() {

            return Point(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y);
        }

        Vec2 pos(float x, float y) {

            return Point(vSize.width * x + vOrigin.x, vSize.height * y + vOrigin.y);
        }

        Vec2 pos(float x, float y, std::string oSzTag, float add, char whatAdd) {

            return whatAdd == 'x' ?
                Point(vSize.width * x + vOrigin.x + getOSz(oSzTag) * add, vSize.height * y + vOrigin.y):
                Point(vSize.width * x + vOrigin.x, vSize.height * y + vOrigin.y + getOSz(oSzTag) * add);
        }

        Vec2 pos(float x, float y, std::string oSzTagX, float addX, std::string oSzTagY, float addY) {

            return Point(
                
                vSize.width * x + vOrigin.x + getOSz(oSzTagX) * addX,
                vSize.height * y + vOrigin.y + getOSz(oSzTagY) * addY
            );
        }

        UserDefault *getUDef() {return uDef;}

        // AUDIO ENGINE //

        void playBGMusic(bool isPlay) {

            if (isPlay) setUserValue("ID BG MUSIC", AudioEngine::play2d("SOUNDS/stay_in_action.mp3", true, 1.0f));
            else AudioEngine::stop(uDef->getIntegerForKey("ID BG MUSIC"));
        }

        void playSFX(std::string designation, float volRate = 1.0) {

            if (uDef->getBoolForKey("SOUND ON")) {
                
                if      (designation == "coin")           AudioEngine::play2d("SOUNDS/coin.mp3",           false, volRate);
                else if (designation == "collapse")       AudioEngine::play2d("SOUNDS/collapse.mp3",       false, volRate);
                else if (designation == "explode")        AudioEngine::play2d("SOUNDS/explode.mp3",        false, volRate);
                else if (designation == "hit")            AudioEngine::play2d("SOUNDS/hit.mp3",            false, volRate);
                else if (designation == "laser hit")      AudioEngine::play2d("SOUNDS/laser_hit.mp3",      false, volRate);
                else if (designation == "new high score") AudioEngine::play2d("SOUNDS/new_high_score.mp3", false, volRate);
                else if (designation == "notice")         AudioEngine::play2d("SOUNDS/notice.mp3",         false, volRate);
                else if (designation == "purchased")      AudioEngine::play2d("SOUNDS/purchased.mp3",      false, volRate);
                else if (designation == "power up")       AudioEngine::play2d("SOUNDS/power_up.mp3",       false, volRate);
                else if (designation == "select ball")    AudioEngine::play2d("SOUNDS/select_ball.mp3",    false, volRate);
                else if (designation == "select off")     AudioEngine::play2d("SOUNDS/select_off.mp3",     false, volRate);
                else if (designation == "select on")      AudioEngine::play2d("SOUNDS/select_on.mp3",      false, volRate);
            }
        }

    private:

        Size vSize = Director::getInstance()->getVisibleSize();
        Vec2 vOrigin = Director::getInstance()->getVisibleOrigin();
        UserDefault *uDef = UserDefault::getInstance();

        float
            brickWidth, brickHeight,
            paddleWidth, paddleHeight;
};

#endif // _TOOL_H_