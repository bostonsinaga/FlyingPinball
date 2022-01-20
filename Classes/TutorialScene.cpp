#include "TutorialScene.h"
#include "GameScene.h"

USING_NS_CC;

Node *screenShot_ts;
int tutorialTag;

Scene* TutorialScene::createScene(cocos2d::RenderTexture *renTex, int tutorialTag_in) {

    tutorialTag = tutorialTag_in;
    screenShot_ts = renTex;

    auto scene = Scene::create();
    auto layer = TutorialScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool TutorialScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {

        return false;
    }

    tool.initMeasurement();

    screenShot_ts->setPosition(tool.ctrPos());
    this->addChild(screenShot_ts);

    ////////////// stuff /////////////////

    // dark screen //

    darkScreen = DrawNode::create();
    darkScreen->setPosition(tool.ctrPos());
    darkScreen->drawSolidRect(

        Vec2(-tool.getVSz().width / 2, -tool.getVSz().height / 2),
        Vec2(tool.getVSz().width / 2, tool.getVSz().height / 2),
        Color4F(0, 0, 0, 100)
    );
    this->addChild(darkScreen);

    //// 0 = left ::: 1 = right ////

    for (int i = 0; i < 2; i++) {

        float xPosRate = i == 0 ? 0.25 : 0.75;
        
        // label //

        label[i] = tool.myLabel("");
        label[i]->setPosition(tool.pos(xPosRate, 0.8));
        label[i]->enableBold();
        label[i]->setOpacity(i == 0 ? 100 : 0);
        this->addChild(label[i]);

        // finger //

        finger[i] = tool.mySprite("GUI/finger_print.png");
        finger[i]->setPosition(tool.pos(xPosRate, 0.5));
        finger[i]->setScale(0.5);
        finger[i]->setOpacity(i == 0 ? 100 : 0);
        this->addChild(finger[i]);
    }

    ///////////////// directing section /////////////////

    //// tapping tutorial ////

    if (tutorialTag == 1) {

        label[0]->setString("LEFT HIT");
        label[1]->setString("RIGHT HIT");

        action_tut1_sub1(0.0);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut1_sub2), 1.0f);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::passNotice), 2.0f);

        delayTouch(2.01);
    }
    else if (tutorialTag == 2) {

        label[0]->setString("LEFT SWIPE");
        label[1]->setString("RIGHT SWIPE");

        action_tut2_sub1(0.0);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut2_sub2), 1.5f);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::passNotice), 3.0f);

        delayTouch(3.01);
    }

    action_tut_sub1(0.0);
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut_sub2), tutorialTag == 1 ? 1.0f : 1.5f);

    touching();
    onKeyReleasing();

    return true;
}

void TutorialScene::action_tut_sub1(float dt) {

    label[0]->runAction(Sequence::create(
        
        FadeTo::create(tutorialTag == 1 ? 0.5 : 0.75, 255),
        FadeTo::create(tutorialTag == 1 ? 0.5 : 0.75, 100),
        FadeOut::create(0),
        nullptr
    ));

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut_sub2), tutorialTag == 1 ? 1.0f : 1.5f);
}

void TutorialScene::action_tut_sub2(float dt) {

    label[1]->runAction(Sequence::create(
        
        FadeTo::create(tutorialTag == 1 ? 0.5 : 0.75, 255),
        FadeTo::create(tutorialTag == 1 ? 0.5 : 0.75, 100),
        FadeOut::create(0),
        nullptr
    ));

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut_sub1), tutorialTag == 1 ? 1.0f : 1.5f);
}

void TutorialScene::action_tut1_sub1(float dt) {
        
    finger[0]->runAction(Sequence::create(

        Spawn::create(

            ScaleTo::create(0.5, 1.0),
            FadeTo::create(0.5, 255),
            nullptr
        ),
        Spawn::create(

            ScaleTo::create(0.5, 0.5),
            FadeTo::create(0.5, 100),
            nullptr
        ),
        FadeOut::create(0),
        nullptr
    ));

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut1_sub2), 1.0f);
}

void TutorialScene::action_tut1_sub2(float dt) {

    finger[1]->runAction(Sequence::create(

        Spawn::create(

            ScaleTo::create(0.5, 1.0),
            FadeTo::create(0.5, 255),
            nullptr
        ),
        Spawn::create(

            ScaleTo::create(0.5, 0.5),
            FadeTo::create(0.5, 100),
            nullptr
        ),
        FadeOut::create(0),
        nullptr
    ));

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut1_sub1), 1.0f);
}

void TutorialScene::action_tut2_sub1(float dt) {

    Vec2 fingerPos = tool.pos(0.25, 0.5);
    
    finger[0]->runAction(Sequence::create(

        MoveTo::create(0, fingerPos - Vec2(tool.getOSz("bw") * 2, 0)),
        Spawn::create(
            
            ScaleTo::create(0.5, 1.0),
            FadeTo::create(0.5, 255),
            nullptr
        ),
        MoveTo::create(0.5, fingerPos + Vec2(tool.getOSz("bw") * 2, 0)),
        Spawn::create(
            
            ScaleTo::create(0.5, 0.5),
            FadeTo::create(0.5, 100),
            nullptr
        ),
        FadeOut::create(0),
        nullptr
    ));

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut2_sub2), 1.5f);
}

void TutorialScene::action_tut2_sub2(float dt) {

    Vec2 fingerPos = tool.pos(0.75, 0.5);
    
    finger[1]->runAction(Sequence::create(

        MoveTo::create(0, fingerPos + Vec2(tool.getOSz("bw") * 2, 0)),
        Spawn::create(
            
            ScaleTo::create(0.5, 1.0),
            FadeTo::create(0.5, 255),
            nullptr
        ),
        MoveTo::create(0.5, fingerPos - Vec2(tool.getOSz("bw") * 2, 0)),
        Spawn::create(
            
            ScaleTo::create(0.5, 0.5),
            FadeTo::create(0.5, 100),
            nullptr
        ),
        FadeOut::create(0),
        nullptr
    ));

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut2_sub1), 1.5f);
}

void TutorialScene::passNotice(float dt) {

    passLabel = tool.myLabel("OK!", tool.getOSz("bh") * 3/2);
    passLabel->setPosition(tool.ctrPos());
    passLabel->setScale(0.875);
    passLabel->setOpacity(100);
    this->addChild(passLabel);

    passAction(0.0);
    this->schedule(static_cast<SEL_SCHEDULE>(&TutorialScene::passAction), 1.0f);
}

void TutorialScene::passAction(float dt) {

    passLabel->runAction(Sequence::create(

        Spawn::create (
            
            ScaleTo::create(0.5, 1.0),
            FadeTo::create(0.5, 255),
            nullptr
        ),
        Spawn::create (
            
            ScaleTo::create(0.5, 0.875),
            FadeTo::create(0.5, 100),
            nullptr
        ),
        nullptr
    ));
}

void TutorialScene::touching() {

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        return true;
    };
    touchListener->onTouchEnded = [=] (Touch *touch, Event *event) {

        if (isTouchable) {

            isTouchable = false;
            
            _eventDispatcher->removeEventListener(touchListener);
            _eventDispatcher->removeEventListener(backButtonListener);
            popGameScene();
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void TutorialScene::popGameScene() {

    int recoveredTime = tool.getUDef()->getIntegerForKey("START TIME") + std::time(0) - pausedStartTime;
    tool.setUserValue("START TIME", recoveredTime);

    for (int i = 0; i < 2; i++) {

        if (i == 0) {
            
            this->unschedule(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut1_sub1));
            this->unschedule(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut1_sub2));
        }
        else {
            
            this->unschedule(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut2_sub1));
            this->unschedule(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut2_sub2));
        }
        
        finger[i]->runAction(RemoveSelf::create());
        label[i]->runAction(RemoveSelf::create());
    }

    this->unschedule(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut_sub1));
    this->unschedule(static_cast<SEL_SCHEDULE>(&TutorialScene::action_tut_sub2));
    this->unschedule(static_cast<SEL_SCHEDULE>(&TutorialScene::passAction));

    passLabel->runAction(RemoveSelf::create());
    darkScreen->runAction(RemoveSelf::create());
    screenShot_ts->runAction(RemoveSelf::create());

    Director::getInstance()->popScene();
}

// touch and contact pacifier //

void TutorialScene::delayTouch(float dt) {

    isTouchable = false;
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TutorialScene::set_isTouchable), dt);
}

void TutorialScene::set_isTouchable(float dt) {isTouchable = true;}

// back button ANDROID
void TutorialScene::onKeyReleasing() {

    backButtonListener = EventListenerKeyboard::create();
    backButtonListener->onKeyReleased = [=](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

        if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
                
            if (isTouchable) {
                
                _eventDispatcher->removeEventListener(touchListener);
                _eventDispatcher->removeEventListener(backButtonListener);
                popGameScene();
            }
            tool.playSFX("select on");
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(backButtonListener, this);
}