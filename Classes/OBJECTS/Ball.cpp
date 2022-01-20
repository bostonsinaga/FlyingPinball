#include "OBJECTS/Ball.h"

USING_NS_CC;

void Ball::init(cocos2d::Layer *layer_in, Seed *seed_in, Tool *tool_in) {

    layer = layer_in;
    seed = seed_in;
    tool = tool_in;

    currentSelected = tool->getUDef()->getIntegerForKey("CURRENT SELECTED", 0);

    int isPurchased[ballsCount];
    char str[32];

    for (int i = 0; i < ballsCount; i++) {
        
        std::sprintf(str, "IS BALL %d PURCHASED", i + 1);
        isPurchased[i] = tool->getUDef()->getIntegerForKey(str, 0);
    }

    Vec2 ballPos = Point(tool->pos(0.0, 0.0, "bw", 3.39, "bh", 1.98));

    int k = 0;

    for (int i = 0; i < ballsCount; i++) {

        switch (i) {

            case 0: tool->mySpriteFrames(ball[i], ballFramesCount, "BALLS/sun_%d.png", 4, layer); break;
            case 1: tool->mySpriteFrames(ball[i], ballFramesCount, "BALLS/plant_%d.png", 4, layer); break;
            case 2: tool->mySpriteFrames(ball[i], ballFramesCount, "BALLS/water_%d.png", 4, layer); break;
            case 3: tool->mySpriteFrames(ball[i], ballFramesCount, "BALLS/cell_%d.png", 4, layer); break;
            case 4: tool->mySpriteFrames(ball[i], ballFramesCount, "BALLS/earth_%d.png", 4, layer); break;
        }

        tool->mySpriteAnimation(
            
            ball[i],
            ballFramesCount,
            i == 0 ? 0.09 : (i == 4 ? 0.09 : 0.1),
            i == 4 ? false : true
        );

        bool isInvisible = false;

        for (int j = 0; j < ballFramesCount; j++) {

            if (i == currentSelected && isPurchased[i] == true) ball[i][j]->setVisible(true);
            else {
                
                ball[i][j]->setVisible(false);
                if (j == 0) isInvisible = true;
            }

            if (j == 0) ball[i][j]->setPosition(ballPos);
            else ball[i][j]->setPosition(ball[i][j]->getContentSize() / 2);
        }

        if (isInvisible) k++;
    }

    if (k == ballsCount) {
        
        for (int j = 0; j < ballFramesCount; j++) ball[0][j]->setVisible(true);
        tool->setUserValue("CURRENT SELECTED", 0, &currentSelected);
    }

    reflection = tool->mySprite("GENERAL/reflection.png");
    reflection->setPosition(ballPos);
    layer->addChild(reflection, 5);

    // effects //

    for (int i = 0; i < smokeCounts; i++)
        tool->mySpriteFrames(smoke[i], smokeFramesCount, "GENERAL/smoke_%d.png", 5, layer);
}

void Ball::swap(int valSign) {
    
    if ((currentSelected != ballsCount - 1 && valSign == 1)
        || (currentSelected != 0 && valSign == -1)) {

        for (int i = 0; i < ballFramesCount; i++) {

            ball[currentSelected][i]->setVisible(false);
            ball[currentSelected + valSign][i]->setVisible(true);
        }

        tool->setUserValue("CURRENT SELECTED", currentSelected + valSign, &currentSelected);
    }
}

void Ball::attachPhysicsBody() {
        
    ballBody = PhysicsBody::createCircle(ball[currentSelected][0]->getContentSize().width / 2);
    ballBody->setDynamic(true);
    ballBody->setCategoryBitmask(seed->getCatMask("ball"));
    ballBody->setCollisionBitmask(seed->getColMask("ball"));
    ballBody->setContactTestBitmask(seed->getColMask("ball"));
    ball[currentSelected][0]->setPhysicsBody(ballBody);

    reflectionBody = PhysicsBody::createCircle(ball[currentSelected][0]->getContentSize().width / 2);
    reflectionBody->setDynamic(false);
    reflectionBody->setCategoryBitmask(seed->getCatMask("reflection"));
    reflectionBody->setCollisionBitmask(seed->getColMask("reflection"));
    reflectionBody->setContactTestBitmask(seed->getColMask("reflection"));
    reflection->setPhysicsBody(reflectionBody);
}

void Ball::spawn() {

    for (int i = 0; i < ballsCount; i++) {

        if (i != currentSelected)
            for (int j = 0; j < ballFramesCount; j++) ball[i][j]->runAction(RemoveSelf::create());
    }

    randomPosSet();
    attachPhysicsBody();

    shield = DrawNode::create();
    shield->setPosition(ball[currentSelected][0]->getContentSize() / 2);
    shield->drawSolidCircle(Vec2::ZERO, ball[currentSelected][0]->getContentSize().width / 2, 0.0, 360, Color4F::WHITE);
    shield->setVisible(false);
    ball[currentSelected][0]->addChild(shield);
}

void Ball::hide() {

    ball[currentSelected][0]->setPosition(tool->pos(0.0, 2.0));
    ballBody->setDynamic(false);

    reflectionBody->setVelocity(Vec2::ZERO);
    reflection->setPosition(ball[currentSelected][0]->getPosition());
}

void Ball::hideExplode(bool isHiding) {

    std::string bitmaskName[2];
    bool isBallVisible;

    if (isHiding) {

        bitmaskName[0] = "blank";
        bitmaskName[1] = "blank";
        isBallVisible = false;
    }
    else {

        bitmaskName[0] = "ball";
        bitmaskName[1] = "reflection";
        isBallVisible = true;
    }

    ballBody->setCategoryBitmask(seed->getCatMask(bitmaskName[0]));
    ballBody->setCollisionBitmask(seed->getColMask(bitmaskName[0]));
    ballBody->setContactTestBitmask(seed->getColMask(bitmaskName[0]));

    reflectionBody->setCategoryBitmask(seed->getCatMask(bitmaskName[1]));
    reflectionBody->setCollisionBitmask(seed->getColMask(bitmaskName[1]));
    reflectionBody->setContactTestBitmask(seed->getColMask(bitmaskName[1]));

    for (int i = 0; i < ballFramesCount; i++) {
        
        ball[currentSelected][0]->setVisible(isBallVisible);
        reflection->setVisible(isBallVisible);
    }

    if (isHiding) {
        
        int valSign;

        if (ball[currentSelected][0]->getPosition().x > tool->getVSz('x') / 2) valSign = 1;
        else valSign = -1;
        
        ballBody->setVelocity(Vec2(valSign * 5000, -5000));
    }
}

void Ball::explode() {

    if (isShieldOn == false) {
        
        isExplode_sender = true;
        smokeEffect();
        hideExplode(true);
    }
}

void Ball::shieldOn(bool isOn) {

    if (isOn) {

        isShieldOn = true;
        shield->setVisible(true);
        shield->runAction(Repeat::create(
            
            Sequence::create(
                
                FadeOut::create(0.25),
                FadeIn::create(0.25),
                nullptr
            ),  30
        ));
    }
    else {

        isShieldOn = false;
        shield->setVisible(false);
    }
}

void Ball::reflectionOffset() {

    reflectionBody->setVelocity(ballBody->getVelocity());
    reflection->setPosition(ball[currentSelected][0]->getPosition());
}

void Ball::randomPosSet() {

    int valSign = std::rand() % 2 == 0 ? -1 : 1;
    ball[currentSelected][0]->setPosition(tool->pos(0.5, 0.5, "pw", valSign * 1.5, "bh", 3.0));
    smokeEffect();
}

void Ball::smokeEffect() {

    smoke[currentSmoke][0]->setPosition(ball[currentSelected][0]->getPosition());
    tool->mySpriteAnimation(smoke[currentSmoke], smokeFramesCount, 0.05);
    tool->currentObjectCounter(&currentSmoke, smokeCounts);
    tool->playSFX("explode");
}

void Ball::set_isShieldOn(bool isOn) {isShieldOn = isOn;} // call on tutorial

int Ball::getCurrentSelected() {return currentSelected;}

bool Ball::isFall(bool *isExplode_receiver) {

    if (ball[currentSelected][0]->getPosition().y +
        ball[currentSelected][0]->getContentSize().height / 2 < tool->pos(0.5, 0.0).y) {

        if (isShieldOn) {
            
            hideExplode(false);
            ballBody->setVelocity(Vec2::ZERO);
            randomPosSet();
        }
        else {

            *isExplode_receiver = isExplode_sender;
            return true;
        }
    }
    
    return false;
}

// node necessary
cocos2d::Sprite *Ball::getBall() {return ball[currentSelected][0];}
cocos2d::Sprite *Ball::getReflection() {return reflection;}