#include "GameScene.h"
#include "GameOverScene.h"
#include "PauseScene.h"
#include "TutorialScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {

    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, -1000));
    // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); // debug only

    auto layer = GameScene::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!LayerGradient::initWithColor(Color4B::WHITE, Color4B(0, 127, 255, 255), Vec2(0, -1))) {

        return false;
    }

    tool.setUserValue("CURRENT COINS", 0);
    tool.setUserValue("CURRENT SCORE", 0);

    tool.initMeasurement();

    gui.init(this, &tool, true);
    gui.generate("main menu");

    ball.init(this, &seed, &tool);
    obs.init(this, &seed, &tool, &gui);

    obs_backgroundScreenSaver(0.0);
    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_backgroundScreenSaver), 2.0f);

    // event listeners
    delayTouch(0.1);
    touching();
    onKeyReleasing();

    return true;
}

// obstacles //

void GameScene::obs_paddleHit(int member) {

    obs.paddleHit(member);

    member == 0 ?
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_paddleStopHit_left), 0.2f):
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_paddleStopHit_right), 0.2f);
}

void GameScene::obs_paddleMove(int member, int valSign) {

    obs.paddleMove(member, valSign);

    member == 0 ?
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_paddleStopMove_left), 0.1f):
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_paddleStopMove_right), 0.1f);
}

void GameScene::obs_paddleStopHit_left(float dt) {

    if (obs.get_isFuncPaddleStopHit(0) == false) {

        obs.paddleStop(0, true);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_paddleStopHit_left), 0.2f);
    }
    else obs.setDefault_isPaddleStopped(0);
}

void GameScene::obs_paddleStopHit_right(float dt) {

    if (obs.get_isFuncPaddleStopHit(1) == false) {

        obs.paddleStop(1, true);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_paddleStopHit_right), 0.2f);
    }
    else obs.setDefault_isPaddleStopped(1);
}

void GameScene::obs_paddleStopMove_left(float dt)  {obs.paddleStop(0, false);}
void GameScene::obs_paddleStopMove_right(float dt) {obs.paddleStop(1, false);}

void GameScene::obs_objectsUpdate(float dt) {
    
    obs.brickTailDetection();
    obs.fallObjectsBackToOrigin();

    std::string detectedObject = obs.objectsCollision(ball.getBall(), ball.getCurrentSelected());

    if      (detectedObject == "coin")   onDetectedCoin();
    else if (detectedObject == "magnet") onDetectedMagnet();
    else if (detectedObject == "shield") onDetectedShield();
    else if (

        detectedObject == "laser"       ||
        detectedObject == "left thorn"  ||
        detectedObject == "right thorn" ||
        detectedObject == "full thorn"
    ) {
        ball.explode();
    }
    else if (detectedObject == "container") {

        std::string detectedObject_container;
        bool isFlipX;
        auto ballPB = ball.getBall()->getPhysicsBody();

        if (ballPB->getVelocity().x < 0) isFlipX = false;
        else isFlipX = true;

        detectedObject_container = obs.itemAttracted(ball.getReflection(), obs.containerAnimate(isFlipX));

        if      (detectedObject_container == "coin")   onDetectedCoin();
        else if (detectedObject_container == "magnet") onDetectedMagnet();
        else if (detectedObject_container == "shield") onDetectedShield();

        ballPB->setVelocity(ballPB->getVelocity() / 2);
    }
}

void GameScene::obs_backgroundScrollDown(float dt)  {obs.backgroundScrollDown();}
void GameScene::obs_backgroundScreenSaver(float dt) {obs.backgroundScreenSaver();}

void GameScene::obs_laserAnimate(float dt)  {obs.laserAnimate();}
void GameScene::obs_coinAnimate(float dt)   {obs.coinAnimate();}
void GameScene::obs_magnetAnimate(float dt) {obs.magnetAnimate();}
void GameScene::obs_shieldAnimate(float dt) {obs.shieldAnimate();}

void GameScene::obs_coinsAttracted(float dt) {

    int count = obs.coinsAttracted(ball.getReflection());
    for (int i = 1; i <= count; i++) onDetectedCoin();
}

void GameScene::obs_coinsAttracted_unschedule(float dt) {

    this->unschedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_coinsAttracted));
}

void GameScene::obs_shieldOff(float dt) {

    ball.shieldOn(false);
}

void GameScene::onDetectedCoin() {

    int currentCoins = tool.getUDef()->getIntegerForKey("CURRENT COINS");
    tool.setUserValue("CURRENT COINS", currentCoins + 1);

    gui.setBoardString(
        
        0, std::to_string(currentCoins + 1),
        Size(tool.getOSz("bw") * 2/5, tool.getOSz("bh") * 2/3),
        tool.pos(1.0, 1.0) - gui.getBoardSize(std::to_string(currentCoins)) / 2 -tool.getOSz('b') / 2
    );

    scoreAddition(50);
    tool.playSFX("coin");
}

void GameScene::onDetectedMagnet() {

    gui.durationBar_on("magnet");

    this->unschedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_coinsAttracted));
    this->unschedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_coinsAttracted_unschedule));
    
    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_coinsAttracted), 0.0f);
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_coinsAttracted_unschedule), 15.0f);

    scoreAddition(100);
    tool.playSFX("power up");
}

void GameScene::onDetectedShield() {

    ball.shieldOn(true);
    gui.durationBar_on("shield");

    this->unschedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_shieldOff));
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::obs_shieldOff), 15.0f);

    scoreAddition(100);
    tool.playSFX("power up");
}

// ball //

void GameScene::ball_reflectionOffset(float dt) {

    ball.reflectionOffset();

    if (ball.isFall(&isExplode_receiver)) {

        this->unschedule(static_cast<SEL_SCHEDULE>(&GameScene::ball_reflectionOffset));
        this->unschedule(static_cast<SEL_SCHEDULE>(&GameScene::scoreAddition));
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::goToGameOverScene),
            isExplode_receiver == true ? 0.65f : 0.0f);
    }
}

// gui //

void GameScene::gui_noticeBoardChange(
            
    std::string designationBefore,
    std::string designationNext,
    float dt
) {

    gui_noticeBoardDesignation = designationNext;
    gui.showNoticeBoard(false, designationBefore);

    delayTouch(dt);
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::gui_showNoticeBoard), dt);
}

void GameScene::gui_showNoticeBoard(float dt) {gui.showNoticeBoard(true, gui_noticeBoardDesignation);}

// SCENES //

void GameScene::startGame() {
    // data
    int timeNow = std::time(0);
    tool.setUserValue("START TIME", timeNow);
    isGameStarted = true;

    // inits //
    
    ball.spawn();
    obs.brickInit();

    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_objectsUpdate), 0.0f);

    this->unschedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_backgroundScreenSaver));
    obs_backgroundScrollDown(0.0);
    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_backgroundScrollDown), 10.0f);

    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_laserAnimate),  0.1 * obs.getObjectFramesCount("laser"));
    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_coinAnimate),   0.1 * obs.getObjectFramesCount("coin"));
    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_magnetAnimate), 0.1 * obs.getObjectFramesCount("magnet"));
    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::obs_shieldAnimate), 0.1 * obs.getObjectFramesCount("shield"));

    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::ball_reflectionOffset), 0.0f);

    this->schedule(static_cast<SEL_SCHEDULE>(&GameScene::scoreAddition), 0.1f);

    checkingTutorialNeeds();
}

void GameScene::goToPauseScene() {

    auto scene = PauseScene::createScene(getRenTex());
    Director::getInstance()->pushScene(scene);
}

void GameScene::goToGameOverScene(float dt) {

    ball.hide();
    gui.hideDurationBars();
    gui.remove("game");

    auto scene = GameOverScene::createScene(getRenTex());
    Director::getInstance()->pushScene(scene);
}

cocos2d::RenderTexture *GameScene::getRenTex() {

    // previously take a screenshot //

    auto renTex = RenderTexture::create(tool.getVSz().width, tool.getVSz().height);
    renTex->setPosition(tool.ctrPos());

    renTex->begin();
    this->getParent()->visit();
    renTex->end();

    return renTex;
}

// scoring //

void GameScene::scoreAddition(int addition) {

    int currentScore = tool.getUDef()->getIntegerForKey("CURRENT SCORE");
    tool.setUserValue("CURRENT SCORE", currentScore + addition);
    
    gui.setBoardString(
            
        1, std::to_string(currentScore),
        Size(tool.getOSz("bw") * 2/5, tool.getOSz("bh") * 2/3),
        tool.pos(1.0, 1.0) - Vec2(gui.getBoardSize(std::to_string(currentScore)).width / 2,
            gui.getBoardSize(std::to_string(currentScore)).height * 7/4) - tool.getOSz('b') / 2
    );
}

void GameScene::scoreAddition(float dt) {scoreAddition();}

// tutorial needs //

void GameScene::checkingTutorialNeeds() {

    if (tool.getUDef()->getBoolForKey("IS TUTORIAL NEEDED", true)) {

        ball.set_isShieldOn(true);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::tut_tut1), 0.5f);
        this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::tut_tut2), 3.0f);
    }
}

void GameScene::tut_tut1(float dt) {

    auto scene = TutorialScene::createScene(getRenTex(), 1);
    Director::getInstance()->pushScene(scene);
    scene->runAction(RemoveSelf::create());
}

void GameScene::tut_tut2(float dt) {

    auto scene = TutorialScene::createScene(getRenTex(), 2);
    Director::getInstance()->pushScene(scene);
    scene->runAction(RemoveSelf::create());

    tool.setUserValue("IS TUTORIAL NEEDED", false);

    auto node = Node::create();
    this->addChild(node);
    node->runAction(Sequence::create(

        DelayTime::create(1.0),
        CallFunc::create([=]() {ball.set_isShieldOn(false);}),
        RemoveSelf::create(),
        nullptr
    ));
}

// EVENT LISTENERS //

void GameScene::touching() {
        
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        if (isTouchable) {

            touchBegan = Vec2(-1, -1);

            if (isTouched == false) {

                isTouched = true;
                touchBegan = touch->getLocation();

                std::string str = gui.buttonDetection('b', touchBegan);
                gui.press(str, 'b');
            }
        }

        return true;
    };
    touchListener->onTouchMoved = [=](Touch *touch, Event *event) {

        if (isTouchable && isGameStarted) {

            touchMoved = touch->getLocation();

            if (touchBegan.x < tool.getVSz('x', 0.5)
                && obs.get_isPaddleMoving(0) == false
                && obs.get_isPaddleStopped(0) == true) {

                if (touchMoved.x - touchBegan.x >= tool.getOSz("bw")) {

                    if (obs.get_isPaddleMoved(0) == false) {
                        
                        obs_paddleMove(0, 1);
                        touchBegan = touchMoved;
                    }
                }
                else if (touchMoved.x - touchBegan.x <= -tool.getOSz("bw")) {

                    if (obs.get_isPaddleMoved(0) == true) {
                        
                        obs_paddleMove(0, -1);
                        touchBegan = touchMoved;
                    }
                }
            }
            else if (touchBegan.x >= tool.getVSz('x', 0.5)
                    && obs.get_isPaddleMoving(1) == false
                    && obs.get_isPaddleStopped(1) == true) {
                
                if (touchMoved.x - touchBegan.x >= tool.getOSz("bw")) {

                    if (obs.get_isPaddleMoved(1) == true) {
                        
                        obs_paddleMove(1, 1);
                        touchBegan = touchMoved;
                    }
                }
                else if (touchMoved.x - touchBegan.x <= -tool.getOSz("bw")) {

                    if (obs.get_isPaddleMoved(1) == false) {
                        
                        obs_paddleMove(1, -1);
                        touchBegan = touchMoved;
                    }
                }
            }
        }
    };
    touchListener->onTouchEnded = [=](Touch *touch, Event *event) {

        if (isTouchable) {
            
            isTouched = false;
            touchEnded = touch->getLocation();

            std::string str = gui.buttonDetection('e', touchBegan, touchEnded);
            gui.press(str, 'e');

            if (isGameStarted) {
                    
                if (str == "pause") {

                    goToPauseScene();
                }
                else {

                    if (touchEnded.x <= tool.getVSz('x', 0.5) && touchBegan.x <= tool.getVSz('x', 0.5) && obs.get_isPaddleMoving(0) == false) {
                    
                        if (obs.get_isPaddleStopped(0)) obs_paddleHit(0);
                    }
                    else if (touchEnded.x > tool.getVSz('x', 0.5) && touchBegan.x > tool.getVSz('x', 0.5) && obs.get_isPaddleMoving(1) == false) {
                        
                        if (obs.get_isPaddleStopped(1)) obs_paddleHit(1);
                    }

                    obs.set_isPaddleMoving_false();
                }
            }
            else {

                // exclusive for GUI //

                auto pressedArrow = [=](int valSign) {

                    ball.swap(valSign);
                    gui.syncBallPrice();
                    
                    if (gui.get_ball_isPurchased()) { // unhide
                                        
                        gui.hidePrebuildButton(false, 0); // play button
                        gui.hideBoard(false, 0); // play board
                    }
                    else { // hide
                        
                        gui.hidePrebuildButton(true, 0); // play button
                        gui.hideBoard(true, 0); // play board
                    }
                };

                // touch //

                if (str == "play") {

                    startGame();
                    gui.remove("main menu");
                    gui.generate("game");
                }
                else if (str == "arrow left") {

                    pressedArrow(-1);
                }
                else if (str == "arrow right") {

                    pressedArrow(1);
                }
                else if (str == "buy") {

                    gui.showNoticeBoard(true, "payment");
                }
                else if (str == "payment buy") {

                    int change = tool.getUDef()->getIntegerForKey("TOTAL COINS", 0) - gui.getBallPrice();

                    if (change >= 0) {

                        gui_noticeBoardChange("payment", "ball purchased", 0.1);
                        tool.setUserValue("TOTAL COINS", change);
                        gui.set_ball_isPurchased();
                    }
                    else gui_noticeBoardChange("payment", "not enough", 0.1);
                }
                else if (str == "payment cancel") {

                    gui.showNoticeBoard(false, "payment");
                }
                else if (str == "exit yes") {

                    Director::getInstance()->end();
                }
                else if (str == "exit no") {

                    gui.showNoticeBoard(false, "exit");
                }
                else if (str == "not enough ok") {

                    gui.showNoticeBoard(false, "not enough");
                }
                else if (str == "ball purchased ok") {

                    gui.showNoticeBoard(false, "ball purchased");
                }
                else if (str == "settings") {

                    gui.showNoticeBoard(true, "settings");
                }
                else if (str == "settings music") {

                    bool isOn = tool.getUDef()->getBoolForKey("MUSIC ON");
                    tool.setUserValue("MUSIC ON", !tool.getUDef()->getBoolForKey("MUSIC ON"));
                    gui_noticeBoardChange("settings", "settings", 0.1);
                    tool.playBGMusic(!isOn);
                    if (isOn) tool.playSFX("select off");
                    else tool.playSFX("select on");
                }
                else if (str == "settings sound") {

                    bool isOn = tool.getUDef()->getBoolForKey("SOUND ON");
                    tool.setUserValue("SOUND ON", !isOn);
                    gui_noticeBoardChange("settings", "settings", 0.1);
                    if (isOn == false) tool.playSFX("select on");
                }
            }

            touchBegan = Vec2(-1, -1);
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::setCurrentTouch(cocos2d::Vec2 loc) {

    if (loc.x < tool.getVSz('x', 0.5)) curTc = 0;
    else curTc = 1;
}

void GameScene::delayTouch(float dt) {

    isTouchable = false;
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameScene::set_isTouchable), dt);
}

void GameScene::set_isTouchable(float dt) {isTouchable = true;}

// back button ANDROID
void GameScene::onKeyReleasing() {

    auto backButtonListener = EventListenerKeyboard::create();
    backButtonListener->onKeyReleased = [=](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

        if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {

            if (isGameStarted) {

                goToPauseScene();
            }
            else {
                
                if      (gui.getNoticeDesignation() == "payment")        gui.showNoticeBoard(false, "payment");
                else if (gui.getNoticeDesignation() == "exit")           gui.showNoticeBoard(false, "exit");
                else if (gui.getNoticeDesignation() == "not enough")     gui.showNoticeBoard(false, "not enough");
                else if (gui.getNoticeDesignation() == "ball purchased") gui.showNoticeBoard(false, "ball purchased");
                else if (gui.getNoticeDesignation() == "settings")       gui.showNoticeBoard(false, "settings");
                else gui.showNoticeBoard(true, "exit");
            }

            tool.playSFX("select off");
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(backButtonListener, this);
}