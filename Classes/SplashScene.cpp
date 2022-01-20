#include "SplashScene.h"
#include "TitleScene.h"
#include "AudioEngine.h"

USING_NS_CC;

Scene* SplashScene::createScene() {

    auto scene = Scene::create();
    auto layer = SplashScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B::WHITE)) {

        return false;
    }

    AudioEngine::preload("SOUNDS/stay_in_action.mp3");
    AudioEngine::preload("SOUNDS/coin.mp3");
    AudioEngine::preload("SOUNDS/explode.mp3");
    AudioEngine::preload("SOUNDS/hit.mp3");
    AudioEngine::preload("SOUNDS/laser_hit.mp3");
    AudioEngine::preload("SOUNDS/new_high_score.mp3");
    AudioEngine::preload("SOUNDS/notice.mp3");
    AudioEngine::preload("SOUNDS/purchased.mp3");
    AudioEngine::preload("SOUNDS/power_up.mp3");
    AudioEngine::preload("SOUNDS/select_off.mp3");
    AudioEngine::preload("SOUNDS/select_on.mp3");

    tool.initMeasurement();

    // data //

    tool.setUserValue("MUSIC ON", true);
    tool.setUserValue("SOUND ON", true);

    // view //

    auto splashScreen = tool.mySprite("BACKGROUND/developer_logo.png");
    splashScreen->setPosition(tool.ctrPos());
    this->addChild(splashScreen);

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&SplashScene::goToTitleScene), 2.0f);

    return true;
}

void SplashScene::goToTitleScene(float dt) {

    tool.setUserValue("ID BG MUSIC", AudioEngine::play2d("SOUNDS/stay_in_action.mp3", true, 1.0f));
    auto scene = TitleScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.25, scene));
}