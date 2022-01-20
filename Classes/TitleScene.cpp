#include "TitleScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* TitleScene::createScene() {

    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool TitleScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!LayerGradient::initWithColor(Color4B::WHITE, Color4B(0, 127, 255, 255), Vec2(0, -1))) {

        return false;
    }

    tool.initMeasurement();

    // view //

    auto bg = tool.mySprite("BACKGROUND/background_1.png");
    bg->setPosition(tool.ctrPos());
    this->addChild(bg);

    Label *title[2];

    for (int i = 0; i < 2; i++) {

        title[i] = tool.myLabel(i == 0 ? "FLYING" : "PINBALL", tool.getOSz("bh") * 2);
        title[i]->setPosition(tool.ctrPos() + Vec2(0,  tool.getOSz("bh") * (i == 0 ? 1.25 : -1.25)));
        title[i]->setColor(i == 0 ? Color3B::WHITE : Color3B(255, 153, 0));
        title[i]->enableOutline(i == 0 ? Color4B(0, 127, 255, 255) : Color4B(156, 42, 0, 255), 20);
        title[i]->setScale(0);
        this->addChild(title[i]);

        title[i]->runAction(Spawn::create(

            Sequence::create(

                RotateBy::create(0.375 ,  22.5),
                RotateBy::create(0.375 , -22.5),
                RotateBy::create(0.375 ,  22.5),
                RotateBy::create(0.375 , -22.5),
                nullptr
            ),
            Sequence::create(
                
                ScaleTo::create(0.375, 0.375, 0.375),
                ScaleTo::create(0.375, 0.000, 0.750),
                ScaleTo::create(0.375, 1.125, 1.125),
                ScaleTo::create(0.375, 1.000, 1.000),
                nullptr
            ),
            nullptr
        ));
    }

    this->scheduleOnce(static_cast<SEL_SCHEDULE>(&TitleScene::goToGameScene), 2.0f);

    return true;
}

void TitleScene::goToGameScene(float dt) {

    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.25, scene));
}