#include "GameOverScene.h"
#include "GameScene.h"

USING_NS_CC;

Node *screenShot_gos;

Scene* GameOverScene::createScene(cocos2d::RenderTexture *renTex) {

    screenShot_gos = renTex;

    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {

        return false;
    }

    delayTouch(0.1);
    touching();
    onKeyReleasing();

    tool.initMeasurement();
    gui.init(this, &tool, false);
    gui.generate("game over");
    gui.showNoticeBoard(true, "game over");

    screenShot_gos->setPosition(tool.ctrPos());
    this->addChild(screenShot_gos);

    float timeToLeave = gui.gameOverSequence(
        
        tool.getUDef()->getIntegerForKey("CURRENT COINS"),
        tool.getUDef()->getIntegerForKey("CURRENT SCORE")
    );
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameOverScene::goToMainMenuScene), timeToLeave);
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameOverScene::set_isAvailableToLeave), timeToLeave / 2);

    return true;
}

void GameOverScene::touching() {

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);

    touchListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        if (isTouchable) {
            
            xBegan = -1.0;
            yBegan = -1.0;

            if (isTouched == false) {

                isTouched = true;

                xBegan = touch->getLocation().x;
                yBegan = touch->getLocation().y;

                // touch began //

                std::string str = gui.buttonDetection('b', Vec2(xBegan, yBegan));
                gui.press(str, 'b');

                // touch began //
            }
        }

        return true;
    };
    touchListener->onTouchEnded = [=] (Touch *touch, Event *event) {

        if (isTouchable) {
            
            isTouched = false;

            xEnded = touch->getLocation().x;
            yEnded = touch->getLocation().y;

            // touch ended //

            std::string str = gui.buttonDetection('e', Vec2(xBegan, yBegan), Vec2(xEnded, yEnded));
            gui.press(str, 'e');

            if (str == "home") {

                this->unschedule(static_cast<SEL_SCHEDULE>(&GameOverScene::goToMainMenuScene));
                goToMainMenuScene(0.0);
            }

            // touch ended //

            xBegan = -1.0;
            yBegan = -1.0;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

// main menu //

void GameOverScene::goToMainMenuScene(float dt) {

    tool.setUserValue(

        "TOTAL COINS",
        tool.getUDef()->getIntegerForKey("TOTAL COINS", 0) +
            tool.getUDef()->getIntegerForKey("CURRENT COINS")
    );

    int currentScore = tool.getUDef()->getIntegerForKey("CURRENT SCORE");

    if (currentScore > tool.getUDef()->getIntegerForKey("TOTAL SCORE", 0))
        tool.setUserValue("TOTAL SCORE", currentScore);

    screenShot_gos->runAction(RemoveSelf::create());

    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.25, scene));
}

void GameOverScene::set_isAvailableToLeave(float dt) {isAvailableToLeave = true;}

// touch and contact pacifier //

void GameOverScene::delayTouch(float dt) {

    isTouchable = false;
    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&GameOverScene::set_isTouchable), dt);
}

void GameOverScene::set_isTouchable(float dt) {isTouchable = true;}

// back button ANDROID
void GameOverScene::onKeyReleasing() {

    auto backButtonListener = EventListenerKeyboard::create();
    backButtonListener->onKeyReleased = [=](cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

        if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {

            if (isAvailableToLeave) goToMainMenuScene(0.0);
            tool.playSFX("select off");
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(backButtonListener, this);
}