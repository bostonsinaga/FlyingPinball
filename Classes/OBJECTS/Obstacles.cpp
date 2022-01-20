#include "OBJECTS/Obstacles.h"

USING_NS_CC;

void Obstacles::init(cocos2d::Layer *layer_in, Seed *seed_in, Tool *tool_in, GUI *gui_in) {

    layer = layer_in;
    seed = seed_in;
    tool = tool_in;
    gui = gui_in;

    setBricksPos();

    int valSign, trgMember;

    // background //

    for (int i = 0; i < backgroundsCount; i++) {
        
        char str[32];
        std::sprintf(str, "BACKGROUND/background_%d.png", i == backgroundsCount - 1 ? 2 : i + 1);
        background[i] = tool->mySprite(str);

        if (i == 0) {
            
            BGHeight = background[i]->getContentSize().height;
            background[i]->setPosition(tool->ctrPos());
        }
        else {

            background[i]->setPosition(tool->ctrPos() + Vec2(0, BGHeight * 1.5));
            background[i]->setScale(1.5, 1.25);
            background[i]->setOpacity(0);
        }

        layer->addChild(background[i]);
    }

    // edge //
    
    float edgePoint;

    for (int i = 0; i < edgesCount; i++) {

        if (i == 0) {
            
            valSign = 1;
            edgePoint = 0.0;
        }
        else {
            
            valSign = -1;
            edgePoint = 1.0;
        }
        
        edgeNode[i] = Node::create();
        edgeBody[i] = PhysicsBody::createBox(i != 2 ?

            Size(tool->getOSz("bw"), tool->getVSz('y', 1.5)):
            Size(tool->getVSz('x', 1.5), tool->getOSz("bh"))
        );
        edgeBody[i]->setDynamic(false);
        edgeBody[i]->setCategoryBitmask(seed->getCatMask("obstacle"));
        edgeBody[i]->setCollisionBitmask(seed->getColMask("obstacle"));
        edgeBody[i]->setContactTestBitmask(seed->getColMask("obstacle"));
        edgeNode[i]->setPhysicsBody(edgeBody[i]);
        edgeNode[i]->setPosition(i != 2 ?
        
            tool->pos(edgePoint, 0.5, "bw", -valSign * 0.5, 'x'):
            tool->pos(0.5, 1.0, "bh", 0.5, 'y')
        );
        layer->addChild(edgeNode[i]);
    }

    // paddle //
    
    for (int i = 0; i < paddlesCount; i++) {

        if (i == 0) valSign = 1;
        else valSign = -1;
        
        paddle[i] = tool->mySprite("GENERAL/paddle.png");
        paddle[i]->setPosition(tool->pos(0.5, 0.5, "pw", -valSign * 1.5, "bh", -3.0));
        if (i == 1) paddle[i]->setFlippedX(true);
        paddleBody[i] = PhysicsBody::createBox(paddle[i]->getContentSize());
        paddleBody[i]->setDynamic(false);
        paddleBody[i]->setCategoryBitmask(seed->getCatMask("obstacle"));
        paddleBody[i]->setCollisionBitmask(seed->getColMask("obstacle"));
        paddleBody[i]->setContactTestBitmask(seed->getColMask("obstacle"));
        paddle[i]->setPhysicsBody(paddleBody[i]);
        paddle[i]->setRotation(valSign * paddleRot);
        layer->addChild(paddle[i], 2);

        paddleBuffPos[i] = paddle[i]->getPosition();
    }

    // holder //

    for (int i = 0; i < holdersCount; i++) {

        if (i < 2) {

            valSign = 1;
            trgMember = 0;
        }
        else {

            valSign = -1;
            trgMember = 1;
        }

        holder[i] = tool->mySprite("GENERAL/paddle.png");

        if (i % 2 == 0) {
            
            holder[i]->setPosition(Point(
            
                paddle[trgMember]->getPosition().x
                    - valSign * std::abs(std::cos(paddle[trgMember]->getRotation() / 180 * M_PI) * (tool->getOSz("pw") / 2))
                    - valSign * tool->getOSz("pw") / 3,
                paddle[trgMember]->getPosition().y
                    + std::abs(std::sin(paddle[trgMember]->getRotation() / 180 * M_PI) * (tool->getOSz("pw") / 2))
                    - tool->getOSz("ph") / 3 + std::sin(holderRot / 180 * M_PI) * (tool->getOSz("pw") / 2)
            ));
        }
        else {

            holder[i]->setPosition(Point(
            
                holder[i - 1]->getPosition().x - valSign * tool->getOSz("pw") * 0.75,
                holder[i - 1]->getPosition().y + std::sin(holderRot / 180 * M_PI) * tool->getOSz("pw") * 0.81
            ));
        }

        if (i >= 2) holder[i]->setFlippedX(true);

        holderBody[i] = PhysicsBody::createBox(holder[i]->getContentSize());
        holderBody[i]->setDynamic(false);
        holderBody[i]->setCategoryBitmask(seed->getCatMask("obstacle"));
        holderBody[i]->setCollisionBitmask(seed->getColMask("obstacle"));
        holderBody[i]->setContactTestBitmask(seed->getColMask("obstacle"));
        holder[i]->setPhysicsBody(holderBody[i]);
        holder[i]->setRotation(valSign * holderRot);
        layer->addChild(holder[i], 1);

        joint[i] = tool->mySprite("GENERAL/joint.png");
        joint[i]->setPosition(Point(

            holder[i]->getPosition().x + valSign * (tool->getOSz("pw") / 2 - joint[i]->getContentSize().width * 0.575)
            - valSign * (tool->getOSz("pw") / 2) + valSign * std::cos(holderRot / 180 * M_PI) * (tool->getOSz("pw") / 2),
            holder[i]->getPosition().y - std::sin(holderRot / 180 * M_PI) * (tool->getOSz("pw") / 2) + joint[i]->getContentSize().height / 4
        ));
        layer->addChild(joint[i], 2);

        holderBuffPos[i] = holder[i]->getPosition();
        jointBuffPos[i] = joint[i]->getPosition();
    }

    // tag for played hit sound effect

    for (int i = 0; i < lasersCount; i++)      isPlayedSFX_hit_laser[i]      = false;
    for (int i = 0; i < leftThornsCount; i++)  isPlayedSFX_hit_leftThorn[i]  = false;
    for (int i = 0; i < rightThornsCount; i++) isPlayedSFX_hit_rightThorn[i] = false;
    for (int i = 0; i < fullThornsCount; i++)  isPlayedSFX_hit_fullThorn[i]  = false;
    for (int i = 0; i < containersCount; i++)  isPlayedSFX_hit_container[i]  = false;
    for (int i = 0; i < bricksCount; i++)      isPlayedSFX_hit_brick[i]      = false;
    for (int i = 0; i < paddlesCount; i++)     isPlayedSFX_hit_paddle[i]     = false;
}

// objects initialization //

void Obstacles::brickInit() {

    for (int i = 0; i < bricksCount; i++) {

        brick[i] = tool->mySprite("GENERAL/brick.png");
        brick[i]->setPosition(tool->pos(0.5, 2.0));
        brickBody[i] = PhysicsBody::createBox(brick[i]->getContentSize());
        brickBody[i]->setDynamic(false);
        brickBody[i]->setCategoryBitmask(seed->getCatMask("obstacle"));
        brickBody[i]->setCollisionBitmask(seed->getColMask("obstacle"));
        brickBody[i]->setContactTestBitmask(seed->getColMask("obstacle"));
        brick[i]->setPhysicsBody(brickBody[i]);
        layer->addChild(brick[i], 1);
    }

    for (int i = 0; i < leftThornsCount; i++) {

        leftThorn[i] = tool->mySprite("GENERAL/thorn_left.png");
        leftThorn[i]->setPosition(tool->pos(0.5, 2.0));
        leftThornBody[i] = PhysicsBody::createBox(leftThorn[i]->getContentSize());
        leftThornBody[i]->setDynamic(false);
        leftThornBody[i]->setCategoryBitmask(seed->getCatMask("left thorn"));
        leftThornBody[i]->setCollisionBitmask(seed->getColMask("left thorn"));
        leftThornBody[i]->setContactTestBitmask(seed->getColMask("left thorn"));
        leftThorn[i]->setPhysicsBody(leftThornBody[i]);
        layer->addChild(leftThorn[i], 1);
    }

    for (int i = 0; i < rightThornsCount; i++) {

        rightThorn[i] = tool->mySprite("GENERAL/thorn_right.png");
        rightThorn[i]->setPosition(tool->pos(0.5, 2.0));
        rightThornBody[i] = PhysicsBody::createBox(rightThorn[i]->getContentSize());
        rightThornBody[i]->setDynamic(false);
        rightThornBody[i]->setCategoryBitmask(seed->getCatMask("right thorn"));
        rightThornBody[i]->setCollisionBitmask(seed->getColMask("right thorn"));
        rightThornBody[i]->setContactTestBitmask(seed->getColMask("right thorn"));
        rightThorn[i]->setPhysicsBody(rightThornBody[i]);
        layer->addChild(rightThorn[i], 1);
    }

    for (int i = 0; i < fullThornsCount; i++) {

        fullThorn[i] = tool->mySprite("GENERAL/thorn_full.png");
        fullThorn[i]->setPosition(tool->pos(0.5, 2.0));
        fullThornBody[i] = PhysicsBody::createBox(fullThorn[i]->getContentSize());
        fullThornBody[i]->setDynamic(false);
        fullThornBody[i]->setCategoryBitmask(seed->getCatMask("full thorn"));
        fullThornBody[i]->setCollisionBitmask(seed->getColMask("full thorn"));
        fullThornBody[i]->setContactTestBitmask(seed->getColMask("full thorn"));
        fullThorn[i]->setPhysicsBody(fullThornBody[i]);
        layer->addChild(fullThorn[i], 1);
    }

    for (int i = 0; i < lasersCount; i++) {

        tool->mySpriteFrames(laser[i], laserFramesCount, "GENERAL/laser_%d.png", 1, layer);

        laserBody[i] = PhysicsBody::createBox(laser[i][0]->getContentSize());
        laserBody[i]->setDynamic(false);
        laserBody[i]->setCategoryBitmask(seed->getCatMask("laser"));
        laserBody[i]->setCollisionBitmask(seed->getColMask("laser"));
        laserBody[i]->setContactTestBitmask(seed->getColMask("laser"));
        laser[i][0]->setPhysicsBody(laserBody[i]);
    }

    for (int i = 0; i < coinsCount; i++) {

        tool->mySpriteFrames(coin[i], coinFramesCount, "ITEMS/coin_%d.png", 1, layer);

        coinBody[i] = PhysicsBody::createBox(coin[i][0]->getContentSize());
        coinBody[i]->setDynamic(false);
        coinBody[i]->setCategoryBitmask(seed->getCatMask("coin"));
        coinBody[i]->setCollisionBitmask(seed->getColMask("coin"));
        coinBody[i]->setContactTestBitmask(seed->getColMask("coin"));
        coin[i][0]->setPhysicsBody(coinBody[i]);

        coin[i][0]->setTag(1);
    }

    for (int i = 0; i < magnetsCount; i++) {

        tool->mySpriteFrames(magnet[i], magnetFramesCount, "ITEMS/magnet_%d.png", 1, layer);

        magnetBody[i] = PhysicsBody::createBox(magnet[i][0]->getContentSize());
        magnetBody[i]->setDynamic(false);
        magnetBody[i]->setCategoryBitmask(seed->getCatMask("magnet"));
        magnetBody[i]->setCollisionBitmask(seed->getColMask("magnet"));
        magnetBody[i]->setContactTestBitmask(seed->getColMask("magnet"));
        magnet[i][0]->setPhysicsBody(magnetBody[i]);

        magnet[i][0]->setTag(2);
    }

    for (int i = 0; i < shieldsCount; i++) {

        tool->mySpriteFrames(shield[i], shieldFramesCount, "ITEMS/shield_%d.png", 1, layer);

        shieldBody[i] = PhysicsBody::createBox(shield[i][0]->getContentSize());
        shieldBody[i]->setDynamic(false);
        shieldBody[i]->setCategoryBitmask(seed->getCatMask("shield"));
        shieldBody[i]->setCollisionBitmask(seed->getColMask("shield"));
        shieldBody[i]->setContactTestBitmask(seed->getColMask("shield"));
        shield[i][0]->setPhysicsBody(shieldBody[i]);

        shield[i][0]->setTag(3);
    }

    for (int i = 0; i < containersCount; i++) {

        container[i] = tool->mySprite("GENERAL/container.png");
        container[i]->setPosition(tool->pos(0.5, 2.0));
        containerBody[i] = PhysicsBody::createBox(container[i]->getContentSize());
        containerBody[i]->setDynamic(false);
        containerBody[i]->setCategoryBitmask(seed->getCatMask("blank"));
        containerBody[i]->setCollisionBitmask(seed->getColMask("blank"));
        containerBody[i]->setContactTestBitmask(seed->getColMask("blank"));
        container[i]->setPhysicsBody(containerBody[i]);
        layer->addChild(container[i], 1);
    }

    for (int i = 0; i < containerAnimationsCount; i++)
        tool->mySpriteFrames(containerAnimation[i], containerAnimationFramesCount, "GENERAL/container_%d.png", 1, layer);

    // effect //

    for (int i = 0; i < sparksCount; i++) {

        tool->mySpriteFrames(spark[i], sparkFramesCount, "GENERAL/spark_%d.png", 6, layer);
        tool->mySpriteFrames(sparkSub[i], sparkFramesCount, "GENERAL/spark_%d.png", 6, layer);

        for (int j = 0; j < sparkFramesCount; j++)
            sparkSub[i][j]->setContentSize(sparkSub[i][j]->getContentSize() * 0.95);
    }

    // start spawning
    brickSpawn();
}

void Obstacles::brickSpawn() {

    int brickTag;

    for (int i = 0; i < seed->getBricksCountPattern(); i++) {

        brickTag = seed->getBrickTag(i);

        if (seed->get_isVelocityChanged()) updateVelocity();

        brick[currentBrick]->setPosition(brickPos[i]);
        brickBody[currentBrick]->setVelocity(Vec2(0, seed->getVelY()));

        {if (brickTag != 1) setBrickBodyProperties(false, seed->getCatMask("blank"), seed->getColMask("blank"));}

        if (brickTag == 1) {

            setBrickBodyProperties(true, seed->getCatMask("obstacle"), seed->getColMask("obstacle"));
        }
        else if (brickTag == 2) {

            leftThorn[currentLeftThorn]->setPosition(brickPos[i] - Vec2(tool->getOSz("bw"), 0));
            leftThornBody[currentLeftThorn]->setVelocity(brickBody[currentBrick]->getVelocity());
            tool->currentObjectCounter(&currentLeftThorn, leftThornsCount);
        }
        else if (brickTag == 3) {

            rightThorn[currentRightThorn]->setPosition(brickPos[i] + Vec2(tool->getOSz("bw"), 0));
            rightThornBody[currentRightThorn]->setVelocity(brickBody[currentBrick]->getVelocity());
            tool->currentObjectCounter(&currentRightThorn, rightThornsCount);
        }
        else if (brickTag == 4) {

            fullThorn[currentFullThorn]->setPosition(brickPos[i]);
            fullThornBody[currentFullThorn]->setVelocity(brickBody[currentBrick]->getVelocity());
            tool->currentObjectCounter(&currentFullThorn, fullThornsCount);
        }
        else if (brickTag == 5) {

            setFallObjectCondition(

                laser[currentLaser],
                laserBody[currentLaser],
                brickPos[i],
                brickBody[currentBrick]->getVelocity(),
                laserFramesCount
            );

            tool->currentObjectCounter(&currentLaser, lasersCount);
        }
        else if (brickTag == 6) {

            setFallObjectCondition(

                coin[currentCoin],
                coinBody[currentCoin],
                brickPos[i],
                brickBody[currentBrick]->getVelocity(),
                coinFramesCount
            );

            tool->currentObjectCounter(&currentCoin, coinsCount);
        }
        else if (brickTag == 7) {

            setFallObjectCondition(

                magnet[currentMagnet],
                magnetBody[currentMagnet],
                brickPos[i],
                brickBody[currentBrick]->getVelocity(),
                magnetFramesCount
            );

            tool->currentObjectCounter(&currentMagnet, magnetsCount);
        }
        else if (brickTag == 8) {

            setFallObjectCondition(

                shield[currentShield],
                shieldBody[currentShield],
                brickPos[i],
                brickBody[currentBrick]->getVelocity(),
                shieldFramesCount
            );

            tool->currentObjectCounter(&currentShield, shieldsCount);
        }
        else if (brickTag == 9) {

            container[currentContainer]->setPosition(brickPos[i]);
            containerBody[currentContainer]->setVelocity(brickBody[currentBrick]->getVelocity());
            tool->currentObjectCounter(&currentContainer, containersCount);
        }

        if (i == seed->getBricksCountPattern() - 1) brickTailTag = currentBrick;

        tool->currentObjectCounter(&currentBrick, bricksCount);
    }
}

void Obstacles::setBricksPos(int member) {

    for (int i = 0; i < seed->getBricksCountPattern(); i++) {

        if (i == 0) {

            if (member == -1)
                brickPos[i] = tool->pos(0.5, 1.0, "bh", 1.5, 'y');
            else
                brickPos[i] = Point(tool->getVSz('x', 0.5), brick[member]->getPosition().y + tool->getOSz("bh"));
        }
        else
            brickPos[i] = Point(tool->getVSz('x', 0.5), brickPos[i - 1].y + tool->getOSz("bh"));
    }
}

void Obstacles::setBrickBodyProperties(bool isVisible, int categoryBitmask, int collisionBitmask) {

    brick[currentBrick]->setVisible(isVisible);
    brickBody[currentBrick]->setCategoryBitmask(categoryBitmask);
    brickBody[currentBrick]->setCollisionBitmask(collisionBitmask);
    brickBody[currentBrick]->setContactTestBitmask(collisionBitmask);
}

void Obstacles::brickTailDetection() {

    if (brick[brickTailTag]->getPosition().y + tool->getOSz("bh") / 2
        <= tool->pos(0.5, 1.0, "bh", 1.0, 'y').y) {

        setBricksPos(brickTailTag);
        brickSpawn();
    }
}

void Obstacles::laserAnimate() {

    for (int i = 0; i < lasersCount; i++) 
        if (isAtVisible(laser[i][0])) tool->mySpriteAnimation(laser[i], laserFramesCount);
}

void Obstacles::coinAnimate() {

    for (int i = 0; i < coinsCount; i++) 
        if (isAtVisible(coin[i][0])) tool->mySpriteAnimation(coin[i], coinFramesCount);
}

void Obstacles::magnetAnimate() {

    for (int i = 0; i < magnetsCount; i++) 
        if (isAtVisible(magnet[i][0])) tool->mySpriteAnimation(magnet[i], magnetFramesCount);
}

void Obstacles::shieldAnimate() {

    for (int i = 0; i < shieldsCount; i++) 
        if (isAtVisible(shield[i][0])) tool->mySpriteAnimation(shield[i], shieldFramesCount);
}

cocos2d::Sprite *Obstacles::containerAnimate(bool isFlipX) {

    Sprite *dropSprite;

    int valSign = isFlipX == false ? -1 : 1;

    std::srand(std::time(0));
    int randInt = std::rand() % 14 + 1;

    std::string dropName = "";

    if (randInt <= 2) {

        switch (randInt) {

            case 1: dropName = "magnet"; break;
            case 2: dropName = "shield"; break;
        }
    }
    else if (randInt <= 6) dropName = "coin";

    // drop item //

    if (dropName == "magnet") {

        dropSprite = magnet[currentMagnet + 1][0];

        setFallObjectCondition(

            magnet[currentMagnet + 1],
            magnetBody[currentMagnet + 1],
            container[currentCollidedContainer]->getPosition() +
                Vec2(valSign * container[currentCollidedContainer]->getContentSize().width / 2, 0),
            Vec2::ZERO,
            magnetFramesCount
        );

        magnetAnimate();
        dropSprite->runAction(MoveBy::create(0.25, Vec2(valSign * tool->getOSz("pw"), 0)));
        tool->currentObjectCounter(&currentMagnet + 1, magnetsCount);
    }
    else if (dropName == "shield") {

        dropSprite = shield[currentShield + 1][0];

        setFallObjectCondition(

            shield[currentShield + 1],
            shieldBody[currentShield + 1],
            container[currentCollidedContainer]->getPosition() +
                Vec2(valSign * container[currentCollidedContainer]->getContentSize().width / 2, 0),
            Vec2::ZERO,
            shieldFramesCount
        );

        shieldAnimate();
        dropSprite->runAction(MoveBy::create(0.25, Vec2(valSign * tool->getOSz("pw"), 0)));
        tool->currentObjectCounter(&currentShield + 1, shieldsCount);
    }
    else if (dropName == "coin") {

        dropSprite = coin[currentCoin + 1][0];

        setFallObjectCondition(

            coin[currentCoin + 1],
            coinBody[currentCoin + 1],
            container[currentCollidedContainer]->getPosition() +
                Vec2(valSign * container[currentCollidedContainer]->getContentSize().width / 2, 0),
            Vec2::ZERO,
            coinFramesCount
        );

        coinAnimate();
        dropSprite->runAction(MoveBy::create(0.25, Vec2(valSign * tool->getOSz("pw"), 0)));
        tool->currentObjectCounter(&currentCoin + 1, coinsCount);
    }
    else dropSprite = nullptr;

    // container animation //

    containerAnimation[currentContainerAnimation][0]->setFlippedX(isFlipX);
    containerAnimation[currentContainerAnimation][0]->setPosition(container[currentCollidedContainer]->getPosition());

    tool->mySpriteAnimation(containerAnimation[currentContainerAnimation], containerAnimationFramesCount, 0.06);
    tool->currentObjectCounter(&currentContainerAnimation, containerAnimationsCount);
    container[currentCollidedContainer]->setPosition(tool->pos(0.5, -0.75));

    return dropSprite;
}

// fall object commands (except brick) //

void Obstacles::fallObjectsBackToOrigin() {

    for (int i = 0; i < leftThornsCount; i++)
        if (leftThorn[i]->getPosition().y <= tool->getVSz('y', -0.5))
            setFallObjectCondition(leftThorn[i], leftThornBody[i], tool->pos(0.5, 2.0), Vec2::ZERO);
    
    for (int i = 0; i < rightThornsCount; i++)
        if (rightThorn[i]->getPosition().y <= tool->getVSz('y', -0.5))
            setFallObjectCondition(rightThorn[i], rightThornBody[i], tool->pos(0.5, 2.0), Vec2::ZERO);

    for (int i = 0; i < fullThornsCount; i++)
        if (fullThorn[i]->getPosition().y <= tool->getVSz('y', -0.5))
            setFallObjectCondition(fullThorn[i], fullThornBody[i], tool->pos(0.5, 2.0), Vec2::ZERO);

    for (int i = 0; i < lasersCount; i++)
        if (laser[i][0]->getPosition().y <= tool->getVSz('y', -0.5))
            setFallObjectCondition(laser[i], laserBody[i], tool->pos(0.5, 2.0), Vec2::ZERO, laserFramesCount);

    for (int i = 0; i < coinsCount; i++)
        if (coin[i][0]->getPosition().y <= tool->getVSz('y', -0.5))
            setFallObjectCondition(coin[i], coinBody[i], tool->pos(0.5, 2.0), Vec2::ZERO, coinFramesCount);

    for (int i = 0; i < magnetsCount; i++)
        if (magnet[i][0]->getPosition().y <= tool->getVSz('y', -0.5))
            setFallObjectCondition(magnet[i], magnetBody[i], tool->pos(0.5, 2.0), Vec2::ZERO, magnetFramesCount);

    for (int i = 0; i < shieldsCount; i++)
        if (shield[i][0]->getPosition().y <= tool->getVSz('y', -0.5))
            setFallObjectCondition(shield[i], shieldBody[i], tool->pos(0.5, 2.0), Vec2::ZERO, shieldFramesCount);
}

void Obstacles::updateVelocity() {

    Vec2 velocity = Vec2(0, seed->getVelY());

    for (int i = 0; i < bricksCount; i++)
        if (i != currentBrick && isAtVisible(brick[i])) brickBody[i]->setVelocity(velocity);

    for (int i = 0; i < leftThornsCount; i++)
        if (i != currentLeftThorn && isAtVisible(leftThorn[i])) leftThornBody[i]->setVelocity(velocity);

    for (int i = 0; i < rightThornsCount; i++)
        if (i != currentRightThorn && isAtVisible(rightThorn[i])) rightThornBody[i]->setVelocity(velocity);

    for (int i = 0; i < fullThornsCount; i++)
        if (i != currentFullThorn && isAtVisible(fullThorn[i])) fullThornBody[i]->setVelocity(velocity);

    for (int i = 0; i < lasersCount; i++)
        if (i != currentLaser && isAtVisible(laser[i][0])) laserBody[i]->setVelocity(velocity);

    for (int i = 0; i < coinsCount; i++)
        if (i != currentCoin && isAtVisible(coin[i][0])) coinBody[i]->setVelocity(velocity);

    for (int i = 0; i < magnetsCount; i++)
        if (i != currentMagnet && isAtVisible(magnet[i][0])) magnetBody[i]->setVelocity(velocity);

    for (int i = 0; i < shieldsCount; i++)
        if (i != currentShield && isAtVisible(shield[i][0])) shieldBody[i]->setVelocity(velocity);

    for (int i = 0; i < containersCount; i++)
        if (i != currentContainer && isAtVisible(container[i])) containerBody[i]->setVelocity(velocity);
}

void Obstacles::setFallObjectCondition(
    cocos2d::Sprite **sprites,
    cocos2d::PhysicsBody *body,
    cocos2d::Vec2 position,
    cocos2d::Vec2 velocity,
    int count) {

    for (int i = 0; i < count; i ++) sprites[i]->setOpacity(i == 0 ? 255 : 0);
    sprites[0]->setPosition(position);
    body->setVelocity(velocity);
}

void Obstacles::setFallObjectCondition(
    cocos2d::Sprite *sprite,
    cocos2d::PhysicsBody *body,
    cocos2d::Vec2 position,
    cocos2d::Vec2 velocity) {
        
    sprite->setPosition(position);
    body->setVelocity(velocity);
}

void Obstacles::sparkEffect(cocos2d::Node *ball, cocos2d::Node *target, cocos2d::Color3B color) {

    int randInt = std::rand() % 8;

    spark[currentSpark][0]->setPosition(Point(
        
        ball->getPosition().x,
        (ball->getPosition().y + target->getPosition().y) / 2
    ));
    spark[currentSpark][0]->setRotation(45.0 * randInt);

    sparkSub[currentSpark][0]->setPosition(spark[currentSpark][0]->getPosition());
    sparkSub[currentSpark][0]->setRotation(spark[currentSpark][0]->getRotation());

    tool->mySpriteAnimation(spark[currentSpark], sparkFramesCount, 0.05, color);
    tool->mySpriteAnimation(sparkSub[currentSpark], sparkFramesCount, 0.05);
    tool->currentObjectCounter(&currentSpark, sparksCount);
}

void Obstacles::sparkEffect(cocos2d::Node *target, cocos2d::Color3B color) {
        
    if (target->getPosition().y > tool->pos(0.5, -0.5).y
        && target->getPosition().y < tool->pos(0.5, 1.1).y) {

        spark[currentSpark][0]->setPosition(target->getPosition());
        sparkSub[currentSpark][0]->setPosition(target->getPosition());
        tool->mySpriteAnimation(spark[currentSpark], sparkFramesCount, 0.05, color);
        tool->mySpriteAnimation(sparkSub[currentSpark], sparkFramesCount, 0.05);
        tool->currentObjectCounter(&currentSpark, sparksCount);
    }
}

std::string Obstacles::itemAttracted(cocos2d::Sprite *ballReflection, cocos2d::Sprite *item) {

    std::string type;

    if (item != nullptr
        && ballReflection->getPosition().y + tool->getOSz("bh") / 2 <= tool->pos(0.5, 1.0).y
        && ballReflection->getPosition().y - tool->getOSz("bh") / 2 >= tool->pos(0.5, 0.0).y) {

        Color3B color;

        switch (item->getTag()) {

            case 1: {
                
                color = Color3B(255, 218, 24);
                type = "coin";
                break;
            }
            case 2: {
                
                color = Color3B::GRAY;
                type = "magnet";
                break;
            }
            case 3: {
                
                color = Color3B::GRAY;
                type = "shield";
                break;
            }
        }
        
        tool->changeObjectParent(item, ballReflection, -1);
                
        Vec2 centerPos = ballReflection->getContentSize() / 2;
        item->setPosition(centerPos + item->getPosition() - ballReflection->getPosition());

        item->runAction(Sequence::create(
            
            MoveTo::create(0.25, centerPos),
            CallFunc::create([=]() {
                
                sparkEffect(ballReflection, color);
                item->setPosition(tool->pos(0.5, -0.75));
                tool->changeObjectParent(item, layer, 1);
            }),
            nullptr
        ));
    }

    return type;
}

int Obstacles::coinsAttracted(cocos2d::Sprite *ballReflection) {

    int count = 0;

    for (int i = 0; i < coinsCount; i++) {

        if (coin[i][0]->getPosition().y + tool->getOSz("bh") / 2 <= tool->pos(0.5, 1.0).y
            && coin[i][0]->getPosition().y - tool->getOSz("bh") / 2 >= tool->pos(0.5, 0.0).y
            && std::abs(coin[i][0]->getPosition().x - tool->ctrPos().x) <= 0.0) {

            itemAttracted(ballReflection, coin[i][0]);
            count++;
        }
    }

    return count;
}

int Obstacles::getObjectFramesCount(std::string name) {

    if      (name == "laser")  return laserFramesCount;
    else if (name == "coin")   return coinFramesCount;
    else if (name == "magnet") return magnetFramesCount;
    else if (name == "shield") return shieldFramesCount;
    else return 0;
}

std::string Obstacles::objectsCollision(cocos2d::Sprite *ball, int currentBallSelected) {

    for (int i = 0; i < coinsCount; i++) {

        if (tool->isAtCollision(coin[i][0], ball)) {
            
            sparkEffect(coin[i][0], Color3B(255, 218, 24));
            coin[i][0]->setPosition(tool->pos(0.5, -0.75));
            tool->playSFX("coin");
            return "coin";
        }
    }

    for (int i = 0; i < magnetsCount; i++) {

        if (tool->isAtCollision(magnet[i][0], ball)) {
            
            sparkEffect(magnet[i][0], Color3B::GRAY);
            magnet[i][0]->setPosition(tool->pos(0.5, -0.75));
            tool->playSFX("power up");
            return "magnet";
        }
    }

    for (int i = 0; i < shieldsCount; i++) {

        if (tool->isAtCollision(shield[i][0], ball)) {
            
            sparkEffect(shield[i][0], Color3B::GRAY);
            shield[i][0]->setPosition(tool->pos(0.5, -0.75));
            tool->playSFX("power up");
            return "shield";
        }
    }
    
    for (int i = 0; i < lasersCount; i++) {

        if (tool->isAtCollision(laser[i][0], ball)) {

            if (isPlayedSFX_hit_laser[i] == false) {
                
                isPlayedSFX_hit_laser[i] = true;
                tool->playSFX("laser hit");
            }

            return "laser";
        }
        else isPlayedSFX_hit_laser[i] = false;
    }

    for (int i = 0; i < leftThornsCount; i++) {

        if (tool->isAtCollision(leftThorn[i], ball)
            && ball->getPosition().x < leftThorn[i]->getPosition().x
            && ball->getPosition().y < leftThorn[i]->getPosition().y) {

            if (isPlayedSFX_hit_leftThorn[i] == false) {
                
                isPlayedSFX_hit_leftThorn[i] = true;
                tool->playSFX("hit");
            }
                
            return "left thorn";
        }
        else isPlayedSFX_hit_leftThorn[i] = false;
    }

    for (int i = 0; i < rightThornsCount; i++) {

        if (tool->isAtCollision(rightThorn[i], ball)
            && ball->getPosition().x > rightThorn[i]->getPosition().x
            && ball->getPosition().y < rightThorn[i]->getPosition().y) {

            if (isPlayedSFX_hit_rightThorn[i] == false) {
                
                isPlayedSFX_hit_rightThorn[i] = true;
                tool->playSFX("hit");
            }
                
            return "right thorn";
        }
        else isPlayedSFX_hit_rightThorn[i] = false;
    }

    for (int i = 0; i < fullThornsCount; i++) {

        if (tool->isAtCollision(fullThorn[i], ball)
            && (ball->getPosition().x < fullThorn[i]->getPosition().x - tool->getOSz("bw") / 2
                || ball->getPosition().x > fullThorn[i]->getPosition().x + tool->getOSz("bw") / 2)
            && ball->getPosition().y < fullThorn[i]->getPosition().y) {

            if (isPlayedSFX_hit_fullThorn[i] == false) {
                
                isPlayedSFX_hit_fullThorn[i] = true;
                tool->playSFX("hit");
            }
                
            return "full thorn";
        }
        else isPlayedSFX_hit_fullThorn[i] = false;
    }

    for (int i = 0; i < containersCount; i++) {

        if (tool->isAtCollision(container[i], ball)) {
                
            currentCollidedContainer = i;
            tool->playSFX("collapse");
            
            if (isPlayedSFX_hit_container[i] == false) {
                
                isPlayedSFX_hit_container[i] = true;
                tool->playSFX("hit");
            }

            return "container";
        }
        else isPlayedSFX_hit_container[i] = false;
    }

    // not essential //    

    if (ball->getPosition().x - ball->getContentSize().width / 2 <= 0
        || ball->getPosition().x + ball->getContentSize().width / 2 >= tool->getVSz('x')
        || ball->getPosition().y + ball->getContentSize().height / 2 >= tool->getVSz('y')) {

        if (isPlayedSFX_hit_edge == false) {
            
            isPlayedSFX_hit_edge = true;
            tool->playSFX("hit");
        }

        return "";
    }
    else isPlayedSFX_hit_edge = false;

    for (int i = 0; i < bricksCount; i++) {

        if (tool->isAtCollision(brick[i], ball)
            && brick[i]->getPhysicsBody()->getCollisionBitmask() != seed->getColMask("blank")) {

            if (isPlayedSFX_hit_brick[i] == false) {
                
                isPlayedSFX_hit_brick[i] = true;
                tool->playSFX("hit");
            }

            return "";
        }
        else isPlayedSFX_hit_brick[i] = false;
    }

    for (int i = 0; i < paddlesCount; i++) {

        if (tool->isAtCollision(paddle[i], ball)) {

            sparkEffect(ball, paddle[i], ballColor[currentBallSelected]);

            if (isPlayedSFX_hit_paddle[i] == false) {
                
                isPlayedSFX_hit_paddle[i] = true;
                tool->playSFX("hit");
            }

            return "";
        }
        else isPlayedSFX_hit_paddle[i] = false;
    }

    for (int i = 0; i < holdersCount; i++) {

        if (tool->isAtCollision(holder[i], ball)) {

            if (isPlayedSFX_hit_holder[i] == false) {
                
                isPlayedSFX_hit_holder[i] = true;
                tool->playSFX("hit", 0.5);
            }

            return "";
        }
        else isPlayedSFX_hit_holder[i] = false;
    }

    return "";
}

bool Obstacles::isAtVisible(cocos2d::Sprite *sprite) {

    if (sprite->getPosition().y - tool->getOSz("bh") / 2 <= tool->pos(0.5, 1.5).y
        && sprite->getPosition().y + tool->getOSz("bh") / 2 >= tool->pos(0.5, 0.0).y)
        return true;
    else return false;
}

// background command //

void Obstacles::backgroundScreenSaver() {

    Vec2 background_0_posBuff = background[0]->getPosition();

    background[0]->runAction(Sequence::create(

        Spawn::create(

            MoveTo::create(1.0, Point(
                
                background[0]->getPosition().x,
                background[0]->getPosition().y + BGHeight * MMbgEffect_valSign * 0.025
            )),
            ScaleTo::create(1.0, 1.0 + MMbgEffect_valSign * 0.025, 1.0),
            nullptr
        ),
        Spawn::create(

            MoveTo::create(1.0, background_0_posBuff),
            ScaleTo::create(1.0, 1.0, 1.0),
            nullptr
        ),
        CallFunc::create([=]() {MMbgEffect_valSign = MMbgEffect_valSign == 1 ? -1 : 1;}),
        nullptr
    ));
}

void Obstacles::backgroundScrollDown() {

    int upperBackground = currentBackground + 1 >= backgroundsCount ? 0 : currentBackground + 1;

    background[upperBackground]->setScale(1.5, 1.25);
    background[upperBackground]->setPosition(tool->ctrPos() + Vec2(0, BGHeight * 1.5));
    background[upperBackground]->runAction(Spawn::create(

        FadeIn::create(0.25),
        MoveTo::create(10, tool->ctrPos()),
        ScaleTo::create(10, 1.0),
        nullptr
    ));

    background[currentBackground]->runAction(Spawn::create(

        MoveTo::create(10, tool->ctrPos() - Vec2(0, BGHeight * 1.5)),
        ScaleTo::create(10, 0.5, 0.75),
        Sequence::create(

            DelayTime::create(9.75),
            FadeOut::create(0.25),
            nullptr
        ),
        nullptr
    ));

    tool->currentObjectCounter(&currentBackground, backgroundsCount);
}

// paddle commands //

void Obstacles::paddleHit(int member) {

    paddle[member]->runAction(Sequence::create(

        DelayTime::create(0.2),
        Spawn::create(

            MoveTo::create(0.2, paddleBuffPos[member]),
            RotateTo::create(0.2, member == 0 ? 45 : -45),
            nullptr
        ),
        nullptr
    ));

    isPaddleStopped[member] = false;
    paddleBody[member]->setVelocity(Vec2(0, 500));
    paddleBody[member]->setAngularVelocity(member == 0 ? 3.75 : -3.75);

    if (isPaddleMoved[member]) holdersMove(member, member == 0 ? -1 : 1, true);
}

void Obstacles::paddleMove(int member, int valSign) {

    Action *moveToPaddle;
    isPaddleMoving[member] = true;
    holdersMove(member, valSign, false);
    
    if (isPaddleMoved[member]) {
        
        isPaddleMoved[member] = false;
        moveToPaddle = MoveTo::create(0, paddleBuffPos[member]);
    }
    else {
        
        isPaddleMoved[member] = true;
        moveToPaddle = MoveTo::create(0, paddleBuffPos[member] + valSign * Vec2(tool->getOSz("pw") / 2, 0));
    }

    paddle[member]->runAction(Sequence::create(
        
        DelayTime::create(0.1),
        moveToPaddle,
        nullptr
    ));

    paddleBody[member]->setVelocity(Vec2(valSign * 1000, 0));
}

void Obstacles::paddleStop(int member, bool isHit) {

    paddleBody[member]->setVelocity(Vec2::ZERO);
    paddleBody[member]->setAngularVelocity(0);

    int start, count;

    if (member == 0) {start = 0; count = holdersCount / 2;}
    else {start = holdersCount / 2; count = holdersCount;}
    
    for (int i = start; i < count; i++) holderBody[i]->setVelocity(Vec2::ZERO);

    if (isHit) {
        
        isFuncPaddleStopHit[member] = true;
        isPaddleMoved[member] = false;
    }
}

void Obstacles::setDefault_isPaddleStopped(int member) {

    isPaddleStopped[member] = true;
    isFuncPaddleStopHit[member] = false;
}

void Obstacles::holdersMove(int member, int valSign, bool isHit) {

    Action
        *moveToHolder[holdersCount],
        *moveToJoint[jointsCount];

    int start, count;
    float moveToHolder_dt, moveToJoint_dt, delayHolder_dt, delayJoint_dt;

    if (isHit) {

        moveToHolder_dt = 0.2;
        moveToJoint_dt = 0.2;
        delayHolder_dt = 0.2;
        delayJoint_dt = 0.2;
    }
    else {

        moveToHolder_dt = 0.0;
        moveToJoint_dt = 0.1;
        delayHolder_dt = 0.1;
        delayJoint_dt = 0.0;
    }

    if (member == 0) {
        
        start = 0;
        count = holdersCount / 2;
    }
    else {

        start = holdersCount / 2;
        count = holdersCount;
    }
    
    if (isPaddleMoved[member]) {

        for (int i = start; i < count; i++) {

            moveToHolder[i] = MoveTo::create(moveToHolder_dt, holderBuffPos[i]);
            moveToJoint[i] = MoveTo::create(moveToJoint_dt, jointBuffPos[i]);
        }
    }
    else {

        for (int i = start; i < count; i++) {

            moveToHolder[i] = MoveTo::create(moveToHolder_dt, holderBuffPos[i] + valSign * Vec2(tool->getOSz("pw") / 2, 0));
            moveToJoint[i] = MoveTo::create(moveToJoint_dt, jointBuffPos[i] + valSign * Vec2(tool->getOSz("pw") / 2, 0));
        }
    }

    for (int i = start; i < count; i++) {

        holder[i]->runAction(Sequence::create(DelayTime::create(delayHolder_dt), moveToHolder[i], nullptr));
        joint[i]->runAction(Sequence::create(DelayTime::create(delayJoint_dt), moveToJoint[i], nullptr));
        if (isHit == false) holderBody[i]->setVelocity(Vec2(valSign * 1000, 0));
    }
}

void Obstacles::set_isPaddleMoving_false() {for (int i = 0; i < 2; i++) isPaddleMoving[i] = false;}

bool Obstacles::get_isFuncPaddleStopHit(int member) {return isFuncPaddleStopHit[member];}
bool Obstacles::get_isPaddleStopped(int member)     {return isPaddleStopped[member];}
bool Obstacles::get_isPaddleMoved(int member)       {return isPaddleMoved[member];}
bool Obstacles::get_isPaddleMoving(int member)      {return isPaddleMoving[member];}