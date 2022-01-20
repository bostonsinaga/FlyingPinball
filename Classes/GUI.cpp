#include "GUI.h"

USING_NS_CC;

void GUI::init(Layer *layer_in, Tool *tool_in, bool isAtMainMenu) {

    layer = layer_in;
    tool = tool_in;

    if (isAtMainMenu) {
        
        balDat[0].isPurchased = 1;

        for (int i = 1; i < ballsCount; i++) {

            char str[32];
            std::sprintf(str, "IS BALL %d PURCHASED", i + 1);
            balDat[i].isPurchased = tool->getUDef()->getIntegerForKey(str, 0);
        }

        for (int i = 0; i < 3; i++) {
            
            balDat[i].isUnlocked = true;
            balDat[i].reachScore = 0;
        }

        for (int i = 3; i < ballsCount; i++) {

            balDat[i].reachScore = i == 3 ? 5000 : 7500;

            if (tool->getUDef()->getIntegerForKey("TOTAL SCORE", 0) >= balDat[i].reachScore)
                balDat[i].isUnlocked = true;
            else balDat[i].isUnlocked = false;
        }

        balDat[0].price = 0;
        balDat[1].price = 1000;
        balDat[2].price = 1000;
        balDat[3].price = 2500;
        balDat[4].price = 2500;
    }
}

void GUI::generate(std::string sceneName) {

    sceneDesignation = sceneName;

    if (sceneName == "main menu") {

        // play button board //

        generateBoard(

            boardContentString, 0, 3,
            tool->pos(1.0, 0.0, "bw", -2.145, "bh", 2.145),
            Size(tool->getOSz("bw") * 3.29, tool->getOSz("bh") * 3.29),
            false, 0.0, 191
        );

        // selection buttons board //

        generateBoard(
            
            boardContentString, 1, 3,
            tool->pos(0.0, 0.0, "bw", 3.39, "bh", 1.98),
            Size(tool->getOSz("bw") * 5.78, tool->getOSz("bh") * 2.959),
            false, 0.0, 191
        );

        // total collected coins bar //

        int totColCoins = tool->getUDef()->getIntegerForKey("TOTAL COINS", 0);
        std::string labelCoins = std::to_string(totColCoins);

        setBoardContentString(labelCoins, "ITEMS/coin_1.png");

        generateBoard(

            boardContentString, 3, 4,
            tool->pos(1.0, 1.0) - getBoardSize(labelCoins) / 2 - tool->getOSz('b') / 2,
            getBoardSize(labelCoins),
            false, getBoardSize(labelCoins).height,
            127, Color3B(0, 127, 255), Color3B(255, 218, 24)
        );

        // total reached score bar //

        setBoardContentString("HIGH SCORE:", "");

        generateBoard(

            boardContentString, 11, 4,
            tool->pos(0.0, 1.0) -
                Vec2(-getBoardSize(boardContentString[0]).width / 4, getBoardSize(boardContentString[0]).height / 2) -
                Vec2(-tool->getOSz('b').width / 2, tool->getOSz('b').height / 2),
            getBoardSize(boardContentString[0]) / 2,
            false, getBoardSize(boardContentString[0]).height / 2,
            127, Color3B(0, 127, 255)
        );

        int totAchScore = tool->getUDef()->getIntegerForKey("TOTAL SCORE", 0);
        std::string labelScore = std::to_string(totAchScore);

        setBoardContentString(labelScore, "");

        generateBoard(

            boardContentString, 12, 4,
            tool->pos(0.0, 1.0) -
                Vec2(-getBoardSize(boardContentString[0]).width / 2, getBoardSize(boardContentString[0]).height * 7/4) -
                Vec2(-tool->getOSz('b').width / 2, tool->getOSz('b').height / 2),
            getBoardSize(boardContentString[0]),
            false, getBoardSize(boardContentString[0]).height,
            127, Color3B(0, 127, 255)
        );

        // notice board //

        generateDarkenNoticeBG(5);
        setBoardContentString("NOTICE", "ITEMS/coin_1.png");

        generateBoard(

            boardContentString, 6, 5,
            Vec2::ZERO, Size::ZERO, true, 0.0, 255
        );

        hideBoard(true, 6);

        // notice button //

        setBoardContentString("NOTICE", "");
        generateButton(boardContentString, 1, 7, 5);
        generateButton(boardContentString, 2, 9, 5);
        hideButton(true, 1, 7);
        hideButton(true, 2, 9);

        // price board //

        generatePriceBoard();

        // buy button
        setBoardContentString("BUY", "");
        generateButton(
    
            boardContentString, 0, 4, 4,
            Point(
            
                board[1].pos.x + board[1].size.width / 2 - getBoardSize("BUY").width / 2,
                board[1].pos.y + board[1].size.height / 2 + getBoardSize("BUY").height
            ),
            Color3B::GREEN
        );

        // settings button
        setBoardContentString("", "GUI/settings_sign.png");
        generateButton(
    
            boardContentString, 3, 14, 4,
            tool->pos(1.0, 1.0) -
                Vec2(getBoardSize(boardContentString[0]).width / 2,
                    getBoardSize(boardContentString[0]).height * 9/4) -
                tool->getOSz('b') / 2,
            Color3B(0, 127, 255)
        );

        // pre-build buttons //

        generatePrebuildButton("play", 4);
        generatePrebuildButton("arrow right", 4);
        generatePrebuildButton("arrow left", 4);

        // for hiding brought or default ball
        syncBallPrice();

        // should be specify if new button generated //

        setBtnsTag(0, 3);
        setPreBtnsTag(0, 2);
    }
    else if (sceneName == "game") {

        // current collected coins bar //

        setBoardContentString("0", "ITEMS/coin_1.png");

        generateBoard(

            boardContentString, 0, 5,
            tool->pos(1.0, 1.0) - getBoardSize(boardContentString[0]) / 2 - tool->getOSz('b') / 2,
            getBoardSize(boardContentString[0]),
            false, getBoardSize(boardContentString[0]).height,
            127, Color3B(0, 127, 255), Color3B(255, 218, 24)
        );

        // current reached score bar //

        setBoardContentString("0", "");

        generateBoard(

            boardContentString, 1, 5,
            tool->pos(1.0, 1.0) -
                Vec2(getBoardSize(boardContentString[0]).width / 2,
                    getBoardSize(boardContentString[0]).height * 7/4) - tool->getOSz('b') / 2,
            getBoardSize(boardContentString[0]),
            false, getBoardSize(boardContentString[0]).height,
            127, Color3B(0, 127, 255)
        );

        setBoardContentString("", "GUI/pause_sign.png");

        generateButton(
    
            boardContentString, 0, 2, 5,
            tool->pos(0.0, 1.0) -
                Vec2(-getBoardSize(boardContentString[0]).width / 2, getBoardSize(boardContentString[0]).height / 2) -
                Vec2(-tool->getOSz('b').width / 2, tool->getOSz('b').height / 2),
            Color3B(0, 127, 255)
        );

        durationBar_init();

        // should be specify if new button generated //

        setBtnsTag(0, 0);
        setPreBtnsTag(-1, -2);
    }
    else if (sceneName == "paused") {

        // notice board //

        generateDarkenNoticeBG(1);
        setBoardContentString("NOTICE", "");

        generateBoard(

            boardContentString, 0, 2,
            Vec2::ZERO, Size::ZERO, true, 0.0, 255
        );

        hideBoard(true, 0);

        // notice button //

        setBoardContentString("NOTICE", "");
        generateButton(boardContentString, 0, 1, 3);
        generateButton(boardContentString, 1, 3, 3);
        generateButton(boardContentString, 2, 5, 3);
        generateButton(boardContentString, 3, 7, 3);
        hideButton(true, 0, 1);
        hideButton(true, 1, 3);
        hideButton(true, 2, 5);
        hideButton(true, 3, 7);

        // should be specify if new button generated //

        setBtnsTag(0, 3);
        setPreBtnsTag(-1, -2);
    }
    else if (sceneName == "game over") {

        setBoardContentString("", "");

        // notice board //

        generateDarkenNoticeBG(7);

        generateBoard(

            boardContentString, 0, 7,
            Vec2::ZERO, Size::ZERO, true, 0.0, 255
        );

        hideBoard(true, 0);

        // home button board //

        generateBoard(

            boardContentString, 1, 7,
            tool->pos(1.0, 0.0, "bw", -2.145, "bh", 2.145),
            Size(tool->getOSz("bw") * 3.29, tool->getOSz("bh") * 3.29),
            false, 0.0, 191
        );

        // home button //

        generatePrebuildButton("home", 7);
        hidePrebuildButton(true, 3);
        hideBoard(true, 1);

        // should be specify if new button generated //

        setBtnsTag(-1, -2);
        setPreBtnsTag(3, 3);
    }
}

void GUI::remove(std::string sceneName) {

    if (sceneName == "main menu") {

        for (int i = preBtnsTag.start; i <= preBtnsTag.end; i++) {

            if (prebuildButton[i][0] != nullptr) {

                prebuildButton[i][0]->runAction(RemoveSelf::create());
                prebuildButton[i][1]->runAction(RemoveSelf::create());
            }
        }

        for (int i = 0; i < boardsCurrentCount; i++) {

            if (board[i].part[0] != nullptr)
                for (int j = 0; j < boardPartsCount; j++) board[i].part[j]->runAction(RemoveSelf::create());
            if (board[i].label != nullptr) board[i].label->runAction(RemoveSelf::create());
            if (board[i].sign != nullptr) board[i].sign->runAction(RemoveSelf::create());
            if (board[i].titleBar != nullptr) board[i].titleBar->runAction(RemoveSelf::create());
            if (board[i].titleLabel != nullptr) board[i].titleLabel->runAction(RemoveSelf::create());
        }

        if (darkenNoticeBG != nullptr) darkenNoticeBG->runAction(RemoveSelf::create());
    }
    else if (sceneName == "game") {

        for (int i = 0; i < boardsCurrentCount; i++) {

            if (board[i].part[0] != nullptr)
                for (int j = 0; j < boardPartsCount; j++) board[i].part[j]->runAction(RemoveSelf::create());
            if (board[i].label != nullptr) board[i].label->runAction(RemoveSelf::create());
            if (board[i].sign != nullptr) board[i].sign->runAction(RemoveSelf::create());
            if (board[i].titleBar != nullptr) board[i].titleBar->runAction(RemoveSelf::create());
            if (board[i].titleLabel != nullptr) board[i].titleLabel->runAction(RemoveSelf::create());
        }
    }
    else if (sceneName == "paused") {

        for (int i = 0; i < boardsCurrentCount; i++) {

            if (board[i].part[0] != nullptr)
                for (int j = 0; j < boardPartsCount; j++) board[i].part[j]->runAction(RemoveSelf::create());
            if (board[i].label != nullptr) board[i].label->runAction(RemoveSelf::create());
            if (board[i].sign != nullptr) board[i].sign->runAction(RemoveSelf::create());
            if (board[i].titleBar != nullptr) board[i].titleBar->runAction(RemoveSelf::create());
            if (board[i].titleLabel != nullptr) board[i].titleLabel->runAction(RemoveSelf::create());
        }

        if (darkenNoticeBG != nullptr) darkenNoticeBG->runAction(RemoveSelf::create());
    }

    boardsCurrentCount = 0;
}

void GUI::press(std::string buttonName, char touchCondition) {

    // main menu //

    if (buttonName == "play") {
        
        if (touchCondition == 'b') press_prebtn(0, false);
        else if (touchCondition == 'e') {

            press_prebtn(0, true);
            tool->playSFX("select on");
        }
    }
    else if (buttonName == "arrow left") {
        
        if (touchCondition == 'b') press_prebtn(1, false);
        else if (touchCondition == 'e') {
            
            press_prebtn(1, true);
            tool->playSFX("select ball");
        }
    }
    else if (buttonName == "arrow right") {
        
        if (touchCondition == 'b') press_prebtn(2, false);
        else if (touchCondition == 'e') {
            
            press_prebtn(2, true);
            tool->playSFX("select ball");
        }
    }
    else if (buttonName == "home") {
        
        if (touchCondition == 'b') press_prebtn(3, false);
        else if (touchCondition == 'e') {
            
            press_prebtn(3, true);
            tool->playSFX("select off");
        }
    }
    else if (buttonName == "buy") {
        
        if (touchCondition == 'b') press_btn(0, false);
        else if (touchCondition == 'e') {
            
            press_btn(0, true);
            tool->playSFX("select on");
        }
    }
    else if (buttonName == "payment buy") {
        
        if (touchCondition == 'b') press_btn(1, false);
        else if (touchCondition == 'e') {
            
            press_btn(1, true);
            tool->playSFX("select on");
        }
    }
    else if (buttonName == "payment cancel") {
        
        if (touchCondition == 'b') press_btn(2, false);
        else if (touchCondition == 'e') {
            
            press_btn(2, true);
            tool->playSFX("select off");
        }
    }
    else if (buttonName == "exit yes") {
        
        if (touchCondition == 'b') press_btn(1, false);
        else if (touchCondition == 'e') {
            
            press_btn(1, true);
            tool->playSFX("select off");
        }
    }
    else if (buttonName == "exit no") {
        
        if (touchCondition == 'b') press_btn(2, false);
        else if (touchCondition == 'e') {
            
            press_btn(2, true);
            tool->playSFX("select on");
        }
    }
    else if (buttonName == "not enough ok") {
        
        if (touchCondition == 'b') press_btn(1, false);
        else if (touchCondition == 'e') {
            
            press_btn(1, true);
            tool->playSFX("select off");
        }
    }
    else if (buttonName == "ball purchased ok") {
        
        if (touchCondition == 'b') press_btn(1, false);
        else if (touchCondition == 'e') {
            
            press_btn(1, true);
            tool->playSFX("select on");
        }
    }
    else if (buttonName == "settings") {
    
        if (touchCondition == 'b') press_btn(3, false);
        else if (touchCondition == 'e') {
            
            press_btn(3, true);
            tool->playSFX("select on");
        }
    }
    else if (buttonName == "settings music") {
        
        if (touchCondition == 'b') press_btn(1, false);
        else if (touchCondition == 'e') press_btn(1, true);
    }
    else if (buttonName == "settings sound") {
        
        if (touchCondition == 'b') press_btn(2, false);
        else if (touchCondition == 'e') press_btn(2, true);
    }

    // game //

    else if (buttonName == "pause") {
    
        if (touchCondition == 'b') press_btn(0, false);
        else if (touchCondition == 'e') {
            
            press_btn(0, true);
            tool->playSFX("select on");
        }
    }

    // paused //

    else if (buttonName == "paused resume") {
    
        if (touchCondition == 'b') press_btn(0, false);
        else if (touchCondition == 'e') {
            
            press_btn(0, true);
            tool->playSFX("select on");
        }
    }
    else if (buttonName == "paused quit") {
    
        if (touchCondition == 'b') press_btn(1, false);
        else if (touchCondition == 'e') {
            
            press_btn(1, true);
            tool->playSFX("select off");
        }
    }
    else if (buttonName == "paused music") {
    
        if (touchCondition == 'b') press_btn(2, false);
        else if (touchCondition == 'e') press_btn(2, true);
    }
    else if (buttonName == "paused sound") {
    
        if (touchCondition == 'b') press_btn(3, false);
        else if (touchCondition == 'e') press_btn(3, true);
    }
    else if (buttonName == "quit yes") {
        
        if (touchCondition == 'b') press_btn(0, false);
        else if (touchCondition == 'e') {
            
            press_btn(0, true);
            tool->playSFX("select off");
        }
    }
    else if (buttonName == "quit no") {
        
        if (touchCondition == 'b') press_btn(1, false);
        else if (touchCondition == 'e') {
        
            press_btn(1, true);
            tool->playSFX("select on");
        }
    }

    // undetected //

    else {
        
        if (preBtnsTag.end != -2 && btnsTag.end != -2) {

            for (int i = preBtnsTag.start; i <= preBtnsTag.end; i++) press_prebtn(i, true);
            for (int i = btnsTag.start; i <= btnsTag.end; i++) press_btn(i, true);
        }
        else if (preBtnsTag.end != -2) {

            for (int i = preBtnsTag.start; i <= preBtnsTag.end; i++) press_prebtn(i, true);
        }
        else if (btnsTag.end != -2) {

            for (int i = btnsTag.start; i <= btnsTag.end; i++) press_btn(i, true);
        }
    }
}

void GUI::press_prebtn(int tag, bool isRelease) {

    if (isHide_prebuildButton[tag] == false) {
        
        prebuildButton[tag][0]->setVisible(isRelease);
        prebuildButton[tag][1]->setVisible(!isRelease);
    }
}

void GUI::press_btn(int tag, bool isRelease) {

    if (isHide_button[tag] == false) {
        
        Action *act[boardPartsCount];
        float distance = button[tag].buttonBoard[1]->size.height * 0.1;
        Vec2 position[boardPartsCount], posDist = Vec2(distance, distance);

        for (int i = 0; i < boardPartsCount; i++) {

            position[i] = button[tag].buttonBoard[1]->part[i]->getPosition();

            if (isRelease) {
                
                if (isPressed[tag]) {
                    
                    act[i] = MoveTo::create(0, position[i] + posDist);
                    if (i == boardPartsCount - 1) isPressed[tag] = false;
                }
                else act[i] = MoveTo::create(0, position[i]);
            }
            else {
                
                act[i] = MoveTo::create(0, position[i] - posDist);
                if (i == 0) isPressed[tag] = true;
            }
        }

        for (int i = 0; i < boardPartsCount; i++)
            button[tag].buttonBoard[1]->part[i]->runAction(act[i]->clone());

        if (button[tag].buttonBoard[1]->label != nullptr) button[tag].buttonBoard[1]->label->runAction(act[0]->clone());
        if (button[tag].buttonBoard[1]->sign != nullptr) button[tag].buttonBoard[1]->sign->runAction(act[0]->clone());
    }
}

void GUI::generateDarkenNoticeBG(int zOrder) {

    darkenNoticeBG = DrawNode::create();
    darkenNoticeBG->setPosition(tool->ctrPos());
    darkenNoticeBG->drawSolidRect(

        Vec2(-tool->getVSz().width / 2, -tool->getVSz().height / 2),
        Vec2(tool->getVSz().width / 2, tool->getVSz().height / 2),
        Color4F(0, 0, 0, 127)
    );
    darkenNoticeBG->setOpacity(0);
    layer->addChild(darkenNoticeBG, zOrder);
}

void GUI::generateBoard(

    std::string *boardContentStr, // board label and sign file name
    int tag,
    int zOrder,
    cocos2d::Vec2 pos,
    cocos2d::Size size,
    bool isWithTitleBar,
    float labelHeight,
    int opacity,
    cocos2d::Color3B boardColor,
    cocos2d::Color3B labelColor
) {

    std::string fileName[7] = {

        "board_body.png",
        "board_body.png",
        "board_body.png",
        "board_corner.png",
        "board_corner.png",
        "board_corner.png",
        "board_corner.png"
    };

    Size partSize[boardPartsCount];
    Vec2 partPos[boardPartsCount];

    if (isWithTitleBar) {
        
        board[tag].titleBar = tool->mySprite("GUI/title_bar.png");

        auto titleSize = board[tag].titleBar->getContentSize();
        size = Size(titleSize.width, titleSize.width * 3/4);
        pos = tool->ctrPos();

        board[tag].titleBar->setPosition(pos.x, pos.y + size.height / 2 - titleSize.height / 2);
        layer->addChild(board[tag].titleBar, zOrder + 1);

        board[tag].titleLabel = tool->myLabel("", titleSize.height);
        board[tag].titleLabel->setPosition(board[tag].titleBar->getContentSize() / 2);
        board[tag].titleLabel->enableBold();
        board[tag].titleBar->addChild(board[tag].titleLabel);

        labelHeight = getBoardSize("").height;
    }
    else {

        board[tag].titleBar = nullptr;
        board[tag].titleLabel = nullptr;
    }

    getBoardSizePos(size, pos, partSize, partPos);

    for (int i = 0; i < boardPartsCount; i++) {

        board[tag].part[i] = Sprite::create(fileName[i]);
        board[tag].part[i]->setContentSize(partSize[i]);
        board[tag].part[i]->setPosition(partPos[i]);
        if(boardColor != Color3B::WHITE) board[tag].part[i]->setColor(boardColor);
        board[tag].part[i]->setOpacity(opacity);
        board[tag].part[i]->setFlippedX(i == 3 ? true : (i == 4 ? true : false));
        board[tag].part[i]->setFlippedY(i == 4 ? true : (i == 6 ? true : false));
        layer->addChild(board[tag].part[i], zOrder);
    }

    board[tag].size = size;
    board[tag].pos = pos;

    float signWidth;

    if (boardContentStr[1] != "") {
        
        board[tag].sign = tool->mySprite(boardContentStr[1]);
        signWidth = board[tag].sign->getContentSize().width *
            (labelHeight * 3/4 / board[tag].sign->getContentSize().height);
    }
    
    if (boardContentStr[0] != "") {

        board[tag].label = tool->myLabel(boardContentStr[0], labelHeight);
        board[tag].label->setPosition(Point(pos.x - (boardContentStr[1] != "" ? signWidth * 3/4 : 0), pos.y));
        if (labelColor != Color3B::WHITE) board[tag].label->setColor(labelColor);
        board[tag].label->enableBold();
        layer->addChild(board[tag].label, zOrder);
    }
    else board[tag].label = nullptr;

    if (boardContentStr[1] != "") {

        board[tag].sign->setContentSize(Size(signWidth, labelHeight * 3/4));
        if (boardContentStr[0] != "") board[tag].sign->setPosition(Point(pos.x + size.width / 2 - signWidth * 3/4, pos.y));
        else board[tag].sign->setPosition(pos);
        layer->addChild(board[tag].sign, zOrder);
    }
    else board[tag].sign = nullptr;
    
    boardsCurrentCount++;
}

void GUI::getBoardSizePos(
    
    cocos2d::Size size,
    cocos2d::Vec2 pos,
    cocos2d::Size *partSize,
    cocos2d::Vec2 *partPos
) {

    Size cornerSize = size.width > size.height ?
        Size(size.height / 8, size.height / 8):
        Size(size.width / 8, size.width / 8);

    partSize[0] = Size(size.width - cornerSize.width * 2, size.height);
    partSize[1] = Size(cornerSize.width, size.height - cornerSize.height * 2);
    partSize[2] = Size(cornerSize.width, size.height - cornerSize.height * 2);
    for (int i = boardPartsCount - 4; i < boardPartsCount; i++) partSize[i] = Size(cornerSize);

    partPos[0] = pos;
    partPos[1] = Point(pos.x - size.width / 2 + cornerSize.width / 2, pos.y);
    partPos[2] = Point(pos.x + size.width / 2 - cornerSize.width / 2, pos.y);
    partPos[3] = Point(pos.x - size.width / 2 + cornerSize.width / 2, pos.y + size.height / 2 - cornerSize.height / 2);
    partPos[4] = Point(pos.x - size.width / 2 + cornerSize.width / 2, pos.y - size.height / 2 + cornerSize.height / 2);
    partPos[5] = Point(pos.x + size.width / 2 - cornerSize.width / 2, pos.y + size.height / 2 - cornerSize.height / 2);
    partPos[6] = Point(pos.x + size.width / 2 - cornerSize.width / 2, pos.y - size.height / 2 + cornerSize.height / 2);
}

void GUI::setBoardSize(int tag, cocos2d::Size size, cocos2d::Vec2 pos) {

    Size partSize[boardPartsCount];
    Vec2 partPos[boardPartsCount];

    getBoardSizePos(size, pos, partSize, partPos);

    for (int i = 0; i < boardPartsCount; i++) {

        board[tag].part[i]->setContentSize(partSize[i]);
        board[tag].part[i]->setPosition(partPos[i]);
    }

    board[tag].size = size;
    board[tag].pos = pos;
}

void GUI::setBoardString(int tag, std::string str, cocos2d::Size sizePerAlphabet, cocos2d::Vec2 pos) {

    Size size = Size(sizePerAlphabet.width * str.length() + tool->getOSz("bw"), sizePerAlphabet.height);

    if (str != board[tag].label->getString()) {
        
        board[tag].label->setString(str);
        if (pos == Vec2::ZERO) pos = board[tag].part[0]->getPosition();

        Size partSize[boardPartsCount];
        Vec2 partPos[boardPartsCount];

        getBoardSizePos(size, pos, partSize, partPos);

        for (int i = 0; i < boardPartsCount; i++) {

            board[tag].part[i]->setContentSize(partSize[i]);
            board[tag].part[i]->setPosition(partPos[i]);
        }

        board[tag].size = size;
        board[tag].pos = pos;
    }

    if (board[tag].sign != nullptr) {

        if (board[tag].sign->isVisible()) {

            float
                signWidth = board[tag].sign->getContentSize().width *
                    (sizePerAlphabet.height * 3/4 / board[tag].sign->getContentSize().height);

            board[tag].label->setPosition(Point(pos.x - signWidth * 3/4, pos.y));
            board[tag].sign->setPosition(Point(pos.x + size.width / 2 - signWidth * 3/4, pos.y));
        }
        else board[tag].label->setPosition(pos);
    }
    else board[tag].label->setPosition(pos);
}

void GUI::generateButton(
    
    std::string *buttonLabel,
    int buttonTag,
    int boardTag,
    int zOrder,
    cocos2d::Vec2 pos,
    cocos2d::Color3B color,
    cocos2d::Vec2 ratioBoard,
    float ratioLabel
) {

    std::string str[2] = {"", ""};

    generateBoard(

        str, boardTag, zOrder, pos,
        Size(
            
            getBoardSize(buttonLabel[0]).width * ratioBoard.x,
            getBoardSize(buttonLabel[0]).height * ratioBoard.y
        ),
        false, getBoardSize(buttonLabel[0]).height * ratioLabel,
        255, getButtonColorBG(color)
    );

    generateBoard(

        buttonLabel, boardTag + 1, zOrder,
        Point(
            
            board[boardTag].pos.x + board[boardTag].size.height * 0.1,
            board[boardTag].pos.y + board[boardTag].size.height * 0.1
        ),
        Size(
            
            getBoardSize(buttonLabel[0]).width * ratioBoard.x,
            getBoardSize(buttonLabel[0]).height * ratioBoard.y
        ),
        false, getBoardSize(buttonLabel[0]).height * ratioLabel,
        255, color
    );

    button[buttonTag].buttonBoard[0] = &board[boardTag];
    button[buttonTag].buttonBoard[1] = &board[boardTag + 1];

    isHide_button[buttonTag] = false;
    isPressed[buttonTag] = false;
}

cocos2d::Color3B GUI::getButtonColorBG(cocos2d::Color3B color) {

    Color3B color_bg;

    if      (color == Color3B::WHITE)   color_bg = Color3B(255 - 64, 255 - 64, 255 - 64);
    else if (color == Color3B::YELLOW)  color_bg = Color3B(255 - 64, 255 - 64,        0);
    else if (color == Color3B::GREEN)   color_bg = Color3B(       0, 255 - 64,        0);
    else if (color == Color3B::BLUE)    color_bg = Color3B(       0,        0, 255 - 64);
    else if (color == Color3B::RED)     color_bg = Color3B(255 - 64,        0,        0);
    else if (color == Color3B::MAGENTA) color_bg = Color3B(255 - 64,        0, 255 - 64);
    else if (color == Color3B::BLACK)   color_bg = Color3B(      64,       64,       64);
    else if (color == Color3B::ORANGE)  color_bg = Color3B(255 - 64, 127 - 64,        0);
    else if (color == Color3B::GRAY)    color_bg = Color3B(166 - 64, 166 - 64, 166 - 64);

    else if (color == Color3B(0, 127, 255)) color_bg = Color3B(0, 127 - 64, 255 - 64);

    return color_bg;
}

void GUI::editButton(
    
    std::string str,
    int buttonTag,
    int boardTag,
    cocos2d::Vec2 pos,
    cocos2d::Color3B color
) {

    Vec2 partPos, majorPos;
    Size size = button[buttonTag].buttonBoard[0]->size;

    auto setPos = [=](Vec2 *targetPos, int memPart)
        {*targetPos = button[buttonTag].buttonBoard[0]->part[memPart]->getPosition();};

    setPos(&majorPos, 0);

    for (int i = 0; i < boardPartsCount; i++) {
        
        setPos(&partPos, i);
        button[buttonTag].buttonBoard[0]->part[i]->setPosition(partPos - majorPos + pos);
        button[buttonTag].buttonBoard[1]->part[i]->setPosition(partPos - majorPos + pos +
            Vec2(size.height * 0.1, size.height * 0.1));

        button[buttonTag].buttonBoard[0]->part[i]->setColor(getButtonColorBG(color));
        button[buttonTag].buttonBoard[1]->part[i]->setColor(color);
    }

    setBoardSize(
        
        boardTag, 
        getBoardSize(str),
        button[buttonTag].buttonBoard[0]->part[0]->getPosition()
    );

    setBoardString(
        
        boardTag + 1, str,
        Size(tool->getOSz("bw") * 2/5, tool->getOSz("bh") * 2/3),
        button[buttonTag].buttonBoard[1]->part[0]->getPosition()
    );

    button[buttonTag].buttonBoard[1]->label->setPosition(button[buttonTag].buttonBoard[1]->pos);
}

void GUI::generatePrebuildButton(std::string buttonName, int zOrder) {

    int tag;
    std::string fileName[2];
    Vec2 pos;

    if (buttonName == "play") {

        tag = 0;
        fileName[0] = "GUI/play_button_unpressed.png";
        fileName[1] = "GUI/play_button_pressed.png";

        pos = board[0].pos;
    }
    else if (buttonName == "arrow left") {

        tag = 1;
        fileName[0] = "GUI/arrow_button_unpressed_1.png";
        fileName[1] = "GUI/arrow_button_pressed_1.png";
        pos = board[1].pos - Vec2(board[1].size.width / 2 - tool->getOSz("bw") * 0.82, 0);
    }
    else if (buttonName == "arrow right") {

        tag = 2;
        fileName[0] = "GUI/arrow_button_unpressed_2.png";
        fileName[1] = "GUI/arrow_button_pressed_2.png";
        pos = board[1].pos + Vec2(board[1].size.width / 2 - tool->getOSz("bw") * 0.82, 0);
    }
    else if (buttonName == "home") {

        tag = 3;
        fileName[0] = "GUI/home_button_unpressed.png";
        fileName[1] = "GUI/home_button_pressed.png";
        pos = board[1].pos;
    }

    prebuildButton[tag][0] = tool->mySprite(fileName[0]);
    prebuildButton[tag][0]->setPosition(pos);
    layer->addChild(prebuildButton[tag][0], zOrder);

    prebuildButton[tag][1] = tool->mySprite(fileName[1]);
    prebuildButton[tag][1]->setPosition(pos - Vec2(tool->getOSz("bw") * 0.1, tool->getOSz("bh") * 0.1));
    prebuildButton[tag][1]->setVisible(false);
    layer->addChild(prebuildButton[tag][1], zOrder);

    isHide_prebuildButton[tag] = false;
}

void GUI::musicCondition(std::string *buttonString, cocos2d::Color3B *buttonColor) {

    if (tool->getUDef()->getBoolForKey("MUSIC ON")) {
        
        *buttonString = "MUSIC: ON";
        *buttonColor  = Color3B::MAGENTA;
    }
    else {
        
        *buttonString = "MUSIC: OFF";
        *buttonColor  = Color3B::GRAY;
    }
}

void GUI::soundCondition(std::string *buttonString, cocos2d::Color3B *buttonColor) {

    if (tool->getUDef()->getBoolForKey("SOUND ON")) {
                    
        *buttonString = "SOUND: ON";
        *buttonColor  = Color3B::ORANGE;
    }
    else {
        
        *buttonString = "SOUND: OFF";
        *buttonColor  = Color3B::GRAY;
    }
}

// notice board configuration (include it's buttons) //

void GUI::showNoticeBoard(bool isShow, std::string designation) {

    std::string buttonString[4];
    int buttonTag[4], boardTag[4];
    Color3B buttonColor[4];

    if (isShow) {
        
        noticeDesignation = designation;
        darkenNoticeBG->runAction(FadeIn::create(0.1));
        if (sceneDesignation == "main menu") isHide_prebuildButton[2] = true;
    }
    else {
        
        noticeDesignation = "";
        darkenNoticeBG->runAction(FadeOut::create(0.1));
        if (sceneDesignation == "main menu") isHide_prebuildButton[2] = false;
    }

    if (sceneDesignation == "main menu") {

        if (designation == "payment") {

            if (isShow) {

                std::string str = "COST  " + std::to_string(balDat[getCurrentSelected()].price);
                editNoticeBoard("BUY BALL?", str, 6, Color3B(255, 218, 24));

                buttonString[0] = "BUY";            buttonString[1] = "CANCEL";
                buttonTag[0]    = 1;                buttonTag[1]    = 2;
                boardTag[0]     = 7;                boardTag[1]     = 9;
                buttonColor[0]  = Color3B::GREEN;   buttonColor[1]  = Color3B::RED;

                showNoticeButton(2, buttonString, buttonTag, boardTag, 6, true, buttonColor);
                notice_hideSign_relocateLabel(false, 6);
            }
            else {
                
                buttonTag[0] = 1;   buttonTag[1] = 2;
                boardTag[0]  = 7;   boardTag[1]  = 9;

                unshowNoticeButton(2, buttonTag, boardTag, 6);
            }
        }
        else if (designation == "exit") {

            if (isShow) {

                editNoticeBoard("EXIT?", "ARE YOU SURE?", 6, Color3B::RED);

                buttonString[0] = "YES";           buttonString[1] = "NO";
                buttonTag[0]    = 1;               buttonTag[1]    = 2;
                boardTag[0]     = 7;               boardTag[1]     = 9;
                buttonColor[0]  = Color3B::RED;    buttonColor[1]  = Color3B::GREEN;

                showNoticeButton(2, buttonString, buttonTag, boardTag, 6, true, buttonColor);
                notice_hideSign_relocateLabel(true, 6);

                tool->playSFX("notice");
            }
            else {

                buttonTag[0] = 1;   buttonTag[1] = 2;
                boardTag[0]  = 7;   boardTag[1]  = 9;
                
                unshowNoticeButton(2, buttonTag, boardTag, 6);
            }
        }
        else if (designation == "not enough") {

            if (isShow) {

                editNoticeBoard("NOTICE!", "NOT ENOUGH COINS!", 6, Color3B(0, 127, 255));

                buttonString[0] = "OK";             buttonString[1] = "";
                buttonTag[0]    = 1;                buttonTag[1]    = 0;
                boardTag[0]     = 7;                boardTag[1]     = 0;
                buttonColor[0]  = Color3B::BLUE;    buttonColor[1]  = Color3B::WHITE;

                showNoticeButton(1, buttonString, buttonTag, boardTag, 6, true, buttonColor);
                notice_hideSign_relocateLabel(true, 6);

                tool->playSFX("notice");
            }
            else {

                buttonTag[0] = 1;   buttonTag[1] = 0;
                boardTag[0]  = 7;   boardTag[1]  = 0;

                unshowNoticeButton(1, buttonTag, boardTag, 6);
            }
        }
        else if (designation == "ball purchased") {

            if (isShow) {

                editNoticeBoard("NOTICE!", "BALL HAS PURCHASED!", 6, Color3B(63, 255, 63));

                buttonString[0] = "OK";             buttonString[1] = "";
                buttonTag[0]    = 1;                buttonTag[1]    = 0;
                boardTag[0]     = 7;                boardTag[1]     = 0;
                buttonColor[0]  = Color3B::GREEN;    buttonColor[1]  = Color3B::WHITE;

                showNoticeButton(1, buttonString, buttonTag, boardTag, 6, true, buttonColor);
                notice_hideSign_relocateLabel(true, 6);

                tool->playSFX("purchased");
            }
            else {

                buttonTag[0] = 1;   buttonTag[1] = 0;
                boardTag[0]  = 7;   boardTag[1]  = 0;

                unshowNoticeButton(1, buttonTag, boardTag, 6);
            }
        }
        else if (designation == "settings") {

            if (isShow) {

                editNoticeBoard("SETTINGS", "", 6, Color3B(0, 127, 255));

                musicCondition(&buttonString[0], &buttonColor[0]);
                soundCondition(&buttonString[1], &buttonColor[1]);

                buttonTag[0] = 1;    buttonTag[1] = 2;
                boardTag[0]  = 7;    boardTag[1]  = 9;

                showNoticeButton(2, buttonString, buttonTag, boardTag, 6, false, buttonColor);

                board[6].label->setVisible(false);
                board[6].sign->setVisible(false);
            }
            else {

                buttonTag[0] = 1;    buttonTag[1] = 2;
                boardTag[0]  = 7;    boardTag[1]  = 9;

                unshowNoticeButton(2, buttonTag, boardTag, 6);
            }
        }
    }
    else if (sceneDesignation == "paused") {

        if (designation == "paused") {

            if (isShow) {

                editNoticeBoard("PAUSED", "", 0, Color3B::WHITE);

                buttonString[0] = "RESUME";            
                buttonTag[0]    = 0;                   
                boardTag[0]     = 1;                   
                buttonColor[0]  = Color3B(0, 127, 255);

                buttonString[1] = "QUIT";
                buttonTag[1]    = 1;
                boardTag[1]     = 3;
                buttonColor[1]  = Color3B(0, 127, 255);

                musicCondition(&buttonString[2], &buttonColor[2]);
                buttonTag[2] = 2;
                boardTag[2]  = 5;

                soundCondition(&buttonString[3], &buttonColor[3]);
                buttonTag[3] = 3;
                boardTag[3]  = 7;

                showNoticeButton(4, buttonString, buttonTag, boardTag, 0, false, buttonColor);

                board[0].label->setVisible(false);
            }
            else {
                
                buttonTag[0] = 0;
                boardTag[0]  = 1;

                buttonTag[1] = 1;
                boardTag[1]  = 3;

                buttonTag[2] = 2;
                boardTag[2]  = 5;

                buttonTag[3] = 3;
                boardTag[3]  = 7;

                unshowNoticeButton(4, buttonTag, boardTag, 0);
            }
        }
        else if (designation == "quit") {

            if (isShow) {

                editNoticeBoard("QUIT?", "YOUR PROGRESS WILL BE LOST!", 0, Color3B::RED);

                buttonString[0] = "YES";           buttonString[1] = "NO";
                buttonTag[0]    = 0;               buttonTag[1]    = 1;
                boardTag[0]     = 1;               boardTag[1]     = 3;
                buttonColor[0]  = Color3B::RED;    buttonColor[1]  = Color3B::GREEN;

                showNoticeButton(2, buttonString, buttonTag, boardTag, 0, true, buttonColor);

                tool->playSFX("notice");
            }
            else {

                buttonTag[0] = 0;   buttonTag[1] = 1;
                boardTag[0]  = 1;   boardTag[1]  = 3;
                
                unshowNoticeButton(2, buttonTag, boardTag, 0);
            }
        }
    }
    else if (sceneDesignation == "game over") {

        if (designation == "game over") {

            if (isShow) {

                editNoticeBoard("GAME OVER", "", 0, Color3B::WHITE);
                hideBoard(false, 0);
            }
        }
    }
}

void GUI::editNoticeBoard(std::string title_str, std::string board_str, int tag, Color3B labelColor) {

    float titleWidth = board[tag].titleBar->getContentSize().width;
    board[tag].titleLabel->setString(title_str);
    
    if (board_str != "") {

        board[tag].label->setColor(labelColor);

        setBoardString(
        
            tag,
            board_str,
            Size(tool->getOSz("bw") * 2/5, tool->getOSz("bh") * 2/3),
            board[tag].pos
        );

        board[tag].label->setDimensions(titleWidth * 3/4, titleWidth / 4);
        board[tag].label->setHorizontalAlignment(TextHAlignment::CENTER);
        board[tag].label->setVerticalAlignment(TextVAlignment::CENTER);
    }

    setBoardSize(tag, Size(titleWidth, titleWidth * 3/4), tool->ctrPos());
}

void GUI::notice_hideSign_relocateLabel(bool isHide, int tag) {

    // hide sign
    board[tag].sign->setVisible(!isHide);

    // relocate label
    setBoardString(
        
        tag,
        board[tag].label->getString(),
        Size(tool->getOSz("bw") * 2/5, tool->getOSz("bh") * 2/3),
        board[tag].pos
    );
}

void GUI::showNoticeButton(
    
    int count,
    std::string *str,
    int *buttonTag,
    int *boardTag,
    int noticeBoardTag,
    bool isHorizontal,
    cocos2d::Color3B *color
) {

    Vec2 pos;
    
    for (int i = 0; i < count; i++) {
    
        if (isHorizontal) {

            if (i == 0) {

                if (count == 1) {

                    pos = Vec2(board[noticeBoardTag].pos.x, board[noticeBoardTag].pos.y -
                        board[noticeBoardTag].size.height / 2 + getBoardSize(str[0]).height * 5/4);
                }
                else {

                    pos = board[noticeBoardTag].pos - board[noticeBoardTag].size / 2 + getBoardSize(str[0]) / 2 +
                        Vec2(getBoardSize(str[0]).height, getBoardSize(str[0]).height) * 3/4;
                }
            }
            else {

                pos = board[noticeBoardTag].pos - Size(-board[noticeBoardTag].size.width, board[noticeBoardTag].size.height) / 2 +
                    Size(-getBoardSize(str[1]).width, getBoardSize(str[1]).height) / 2 +
                    Vec2(-getBoardSize(str[1]).height, getBoardSize(str[1]).height) * 3/4;
            }
        }
        else {

            pos = Vec2(board[noticeBoardTag].pos.x, board[noticeBoardTag].pos.y +
                    board[noticeBoardTag].size.height / 2 -
                    getBoardSize(str[0]).height * 3/2 * (
                        
                (count == 2 ? 3 : 2) +
                (count == 2 ? 1.5 : 1.1) * i
            ));
        }

        editButton(str[i], buttonTag[i], boardTag[i], pos, color[i]);
        hideBoard(false, noticeBoardTag);
        hideButton(false, buttonTag[i], boardTag[i]);
    }
}

void GUI::unshowNoticeButton(int count, int *buttonTag, int *boardTag, int noticeBoardTag) {

    for (int i = 0; i < count; i++) {

        hideBoard(true, noticeBoardTag);
        hideButton(true, buttonTag[i], boardTag[i]);
    }
}

// buttons tag (limit of member used) //

void GUI::setPreBtnsTag(int tag_st, int tag_en) {

    preBtnsTag.start = tag_st;
    preBtnsTag.end = tag_en;
}

void GUI::setBtnsTag(int tag_st, int tag_en) {

    btnsTag.start = tag_st;
    btnsTag.end = tag_en;
}

// hide gui //

void GUI::hideBoard(bool isHide, int tag) {

    for (int i = 0; i < boardPartsCount; i++) board[tag].part[i]->setVisible(!isHide);
    
    if (board[tag].label != nullptr) board[tag].label->setVisible(!isHide);
    if (board[tag].sign != nullptr) board[tag].sign->setVisible(!isHide);
    if (board[tag].titleBar != nullptr) board[tag].titleBar->setVisible(!isHide);
    if (board[tag].titleLabel != nullptr) board[tag].titleLabel->setVisible(!isHide);
}

void GUI::hideButton(bool isHide, int buttonTag, int boardTag) {

    for (int i = 0; i < 2; i++) hideBoard(isHide, boardTag + i);
    isHide_button[buttonTag] = isHide;
}

void GUI::hidePrebuildButton(bool isHide, int tag) {

    if (isHide) for (int i = 0; i < 2; i++) prebuildButton[tag][i]->setVisible(false);
    else {
        
        prebuildButton[tag][0]->setVisible(true);
        prebuildButton[tag][1]->setVisible(false);
    }

    isHide_prebuildButton[tag] = isHide;
}

cocos2d::Size GUI::getBoardSize(std::string str) {

    return Size(
        
        tool->getOSz("bw") * 2/5 * str.length() + tool->getOSz("bw"),
        tool->getOSz("bh") * 2/3
    );
}

void GUI::setBoardContentString(std::string str_1, std::string str_2) {

    boardContentString[0] = str_1;
    boardContentString[1] = str_2;
}

// ball data //

void GUI::syncBallPrice() {

    bool isHide = true;
    std::string priceString = std::to_string(balDat[getCurrentSelected()].price);

    if (balDat[getCurrentSelected()].isPurchased == false) isHide = false;

    hideBoard(isHide, 2);
    hideButton(isHide, 0, 4);
    isHide_button[0] = isHide;

    if (balDat[getCurrentSelected()].isPurchased == false) {

        if (balDat[getCurrentSelected()].isUnlocked == false) {

            hideBoard(true, 2);
            hideButton(true, 0, 4);

            char str[32];
            std::sprintf(str, "REACH %d HIGH SCORE", balDat[getCurrentSelected()].reachScore);

            setBoardString(
            
                13, str,
                Size(tool->getOSz("bw") * 2/5, tool->getOSz("bh") * 2/3) * 3/4,
                priceBoardPos(str)
            );

            setBoardSize(

                13,
                (getBoardSize(str) - Size(tool->getOSz("bw"), 0)) * 3/4, 
                priceBoardPos(str)
            );

            setBoardSize(

                13, board[13].size, 
                Point(
                    
                    board[1].pos.x - board[1].size.width / 2 + board[13].size.width / 2,
                    board[13].pos.y - board[13].size.height / 4
                )
            );

            board[13].label->setPosition(board[13].pos);

            hideBoard(false, 13);
        }
        else {
            
            setBoardString(
            
                2, priceString,
                Size(tool->getOSz("bw") * 2/5, tool->getOSz("bh") * 2/3),
                priceBoardPos(priceString)
            );

            hideBoard(true, 13);
        }
    }
    else hideBoard(true, 13);
}

void GUI::generatePriceBoard() {

    setBoardContentString(std::to_string(balDat[getCurrentSelected()].price), "ITEMS/coin_1.png");

    generateBoard(
        
        boardContentString, 2, 3,
        priceBoardPos(boardContentString[0]),
        getBoardSize(boardContentString[0]),
        false, getBoardSize(boardContentString[0]).height,
        255, Color3B::WHITE, Color3B(255, 218, 24)
    );

    setBoardContentString("REACH XXXX HIGH SCORE", "");

    generateBoard(
        
        boardContentString, 13, 3,
        priceBoardPos(boardContentString[0]),
        (getBoardSize(boardContentString[0]) - Size(tool->getOSz("bw"), 0)) * 3/4,
        false, getBoardSize(boardContentString[0]).height * 3/4,
        255, Color3B::WHITE, Color3B::RED
    );

    hideBoard(true, 13);
}

Vec2 GUI::priceBoardPos(std::string strForLength) {

    return Vec2(
        
        board[1].pos.x - board[1].size.width / 2 + getBoardSize(strForLength).width / 2,
        board[1].pos.y + board[1].size.height / 2 + getBoardSize(strForLength).height
    );
}

void GUI::set_ball_isPurchased() {
    
    char str[32];
    std::sprintf(str, "IS BALL %d PURCHASED", getCurrentSelected() + 1);

    tool->setUserValue(str, true);
    balDat[getCurrentSelected()].isPurchased = true;

    int totColCoins = tool->getUDef()->getIntegerForKey("TOTAL COINS", 0);
    std::string labelCoins = std::to_string(totColCoins);

    setBoardString(
            
        3, labelCoins,
        Size(tool->getOSz("bw") * 2/5, tool->getOSz("bh") * 2/3),
        tool->pos(1.0, 1.0) - getBoardSize(labelCoins) / 2 - tool->getOSz('b') / 2
    );

    syncBallPrice();
    hidePrebuildButton(false, 0); // play button
    hideBoard(false, 0); // play board
}

bool GUI::get_ball_isPurchased() {return balDat[getCurrentSelected()].isPurchased;}

int GUI::getCurrentSelected()  {return tool->getUDef()->getIntegerForKey("CURRENT SELECTED", 0);}
int GUI::getBallPrice()        {return balDat[getCurrentSelected()].price;}

std::string GUI::getNoticeDesignation() {return noticeDesignation;}

// collected items duration bar //

void GUI::durationBar_init() {

    float signWidth[2], signHeight = board[0].sign->getContentSize().height * 3/4;

    for (int i = 0; i < 2; i++) {

        magnetSign[i] = tool->mySprite("ITEMS/magnet_1.png");
        shieldSign[i] = tool->mySprite("ITEMS/shield_1.png");

        if (i == 0) {
            
            signWidth[0] = magnetSign[i]->getContentSize().width * (signHeight / magnetSign[i]->getContentSize().height);
            signWidth[1] = shieldSign[i]->getContentSize().width * (signHeight / shieldSign[i]->getContentSize().height);
        }

        magnetBar[i] = DrawNode::create();
        magnetBar_bg[i] = DrawNode::create();

        shieldBar[i] = DrawNode::create();
        shieldBar_bg[i] = DrawNode::create();

        magnetSign[i]->setContentSize(Size(signWidth[0], signHeight));
        shieldSign[i]->setContentSize(Size(signWidth[1], signHeight));

        magnetSign[i]->setOpacity(0);
        shieldSign[i]->setOpacity(0);

        magnetBar[i]->setOpacity(0);
        magnetBar_bg[i]->setOpacity(0);

        shieldBar[i]->setOpacity(0);
        shieldBar_bg[i]->setOpacity(0);

        if (i == 0) {

            magnetSign[i]->setPosition(
            
                board[1].pos.x,
                board[1].pos.y - board[1].size.height / 2 - magnetSign[i]->getContentSize().height
            );
        }
        else {

            magnetSign[i]->setPosition(
            
                magnetSign[0]->getPosition().x,
                magnetSign[0]->getPosition().y - magnetSign[0]->getContentSize().height / 2 -
                    signHeight * 7/24 - magnetSign[i]->getContentSize().height * 5/4
            );
        }

        shieldSign[i]->setPosition(magnetSign[i]->getPosition());
        magnetBar[i]->setPosition(Vec2(signWidth[0] / 2, -signHeight / 6));
        shieldBar[i]->setPosition(Vec2(signWidth[1] / 2, -signHeight / 6));
        magnetBar_bg[i]->setPosition(magnetBar[i]->getPosition());
        shieldBar_bg[i]->setPosition(shieldBar[i]->getPosition());

        magnetBar[i]->drawSolidRect(Vec2(-signWidth[0] / 2, -signHeight / 2), Vec2(signWidth[0] / 2, -signHeight / 4), Color4F::WHITE);
        magnetBar_bg[i]->drawSolidRect(Vec2(-signWidth[0] / 2, -signHeight / 2), Vec2(signWidth[0] / 2, -signHeight / 4), Color4F(0, 0, 0, 127));

        shieldBar[i]->drawSolidRect(Vec2(-signWidth[1] / 2, -signHeight / 2), Vec2(signWidth[1] / 2, -signHeight / 4), Color4F::WHITE);
        shieldBar_bg[i]->drawSolidRect(Vec2(-signWidth[1] / 2, -signHeight / 2), Vec2(signWidth[1] / 2, -signHeight / 4), Color4F(0, 0, 0, 127));

        layer->addChild(magnetSign[i], 5);
        layer->addChild(shieldSign[i], 5);

        magnetSign[i]->addChild(magnetBar[i], 1);
        magnetSign[i]->addChild(magnetBar_bg[i]);
        shieldSign[i]->addChild(shieldBar[i], 1);
        shieldSign[i]->addChild(shieldBar_bg[i]);
    }
}

void GUI::durationBar_on(std::string name) {

    int floor = 0;
    float signWidth[2], signHeight = board[0].sign->getContentSize().height * 3/4;

    if (name == "magnet") {

        if (shieldSign[0]->getOpacity() != 0 || magnetSign[1]->getOpacity() != 0) floor = 1;

        signWidth[0] = magnetSign[floor]->getContentSize().width * (signHeight / magnetSign[floor]->getContentSize().height);

        magnetSign[floor]->setOpacity(255);
        magnetBar[floor]->setOpacity(255);
        magnetBar_bg[floor]->setOpacity(255);
        
        magnetBar[floor]->stopAllActions();
        magnetBar[floor]->runAction(Sequence::create(

            ScaleTo::create(0, 1),
            MoveTo::create(0, Vec2(signWidth[0] / 2, -signHeight / 6)), 
            Spawn::create(

                ScaleTo::create(15, 0, 1),
                MoveBy::create(15, Vec2(-signWidth[0] / 2, 0)), 
                nullptr
            ),
            FadeOut::create(0),
            nullptr
        ));

        magnetSign[floor]->stopAllActions();
        magnetBar_bg[floor]->stopAllActions();
        magnetSign[floor]->runAction(Sequence::create(DelayTime::create(15), FadeOut::create(0), nullptr));
        magnetBar_bg[floor]->runAction(Sequence::create(DelayTime::create(15), FadeOut::create(0), nullptr));
    }
    else if (name == "shield") {

        if (magnetSign[0]->getOpacity() != 0 || shieldSign[1]->getOpacity() != 0) floor = 1;

        signWidth[1] = shieldSign[floor]->getContentSize().width * (signHeight / shieldSign[floor]->getContentSize().height);

        shieldSign[floor]->setOpacity(255);
        shieldBar[floor]->setOpacity(255);
        shieldBar_bg[floor]->setOpacity(255);

        shieldBar[floor]->stopAllActions();
        shieldBar[floor]->runAction(Sequence::create(

            ScaleTo::create(0, 1),
            MoveTo::create(0, Vec2(signWidth[1] / 2, -signHeight / 6)), 
            Spawn::create(

                ScaleTo::create(15, 0, 1),
                MoveBy::create(15, Vec2(-signWidth[1] / 2, 0)), 
                nullptr
            ),
            FadeOut::create(0),
            nullptr
        ));

        shieldSign[floor]->stopAllActions();
        shieldBar_bg[floor]->stopAllActions();
        shieldSign[floor]->runAction(Sequence::create(DelayTime::create(15), FadeOut::create(0), nullptr));
        shieldBar_bg[floor]->runAction(Sequence::create(DelayTime::create(15), FadeOut::create(0), nullptr));
    }
}

void GUI::hideDurationBars() {

    for (int i = 0; i < 2; i++) {
        
        magnetSign[i]->setVisible(false);
        magnetBar[i]->setVisible(false);
        magnetBar_bg[i]->setVisible(false);

        shieldSign[i]->setVisible(false);
        shieldBar[i]->setVisible(false);
        shieldBar_bg[i]->setVisible(false);
    }
}

// game over special
float GUI::gameOverSequence(int curCo, int curSc) {

    Label 
        *coCoTl = tool->myLabel("Coins: ", getBoardSize("").height),
        *coCoLb = tool->myLabel("0", getBoardSize("").height),
        *hiScTl = tool->myLabel("High Score: ", getBoardSize("").height),
        *hiScLb = tool->myLabel("0", getBoardSize("").height),
        *coScTl = tool->myLabel("Score: ", getBoardSize("").height),
        *coScLb = tool->myLabel("0", getBoardSize("").height);

    float
        topY = tool->ctrPos().y + board[0].size.height / 2 - tool->getOSz("bh") * 3/2,
        midY = topY - tool->getOSz("bh") * 1.75,
        botY = midY - tool->getOSz("bh") * 1.75;

    // coins //

    coCoTl->setPosition(Point(tool->ctrPos().x, topY));
    coCoTl->setColor(Color3B(255, 218, 24));
    coCoTl->enableBold();
    layer->addChild(coCoTl, 8);

    coCoLb->setPosition(Point(
        
        tool->ctrPos().x,
        topY - coCoTl->getContentSize().height / 2 - coCoLb->getContentSize().height / 2
    ));
    coCoLb->setColor(Color3B(255, 218, 24));
    coCoLb->enableBold();
    layer->addChild(coCoLb, 8);

    auto coCoSg = tool->mySprite("ITEMS/coin_1.png");

    float signWidth = coCoSg->getContentSize().width *
        (getBoardSize("").height * 3/4 / coCoSg->getContentSize().height);

    coCoSg->setContentSize(Size(signWidth, getBoardSize("").height * 3/4));

    auto coCoSg_pos = [=]() {

        coCoSg->setPosition(
        
            coCoLb->getPosition().x + coCoLb->getContentSize().width / 2 + coCoSg->getContentSize().width,
            coCoLb->getPosition().y
        );
    };

    coCoSg_pos();
    layer->addChild(coCoSg, 8);

    // high score //

    hiScTl->setPosition(Point(tool->ctrPos().x, midY));
    hiScTl->setColor(Color3B::BLUE);
    hiScTl->enableBold();
    layer->addChild(hiScTl, 8);

    hiScLb->setPosition(Point(
        
        tool->ctrPos().x,
        midY - hiScTl->getContentSize().height / 2 - hiScLb->getContentSize().height / 2
    ));
    hiScLb->setColor(Color3B::BLUE);
    hiScLb->enableBold();
    layer->addChild(hiScLb, 8);

    // current score //

    coScTl->setPosition(Point(tool->ctrPos().x, botY));
    coScTl->setColor(Color3B(0, 127, 255));
    coScTl->enableBold();
    layer->addChild(coScTl, 8);

    coScLb->setPosition(Point(
        
        tool->ctrPos().x,
        botY - coScTl->getContentSize().height / 2 - coScLb->getContentSize().height / 2
    ));
    coScLb->setColor(Color3B(0, 127, 255));
    coScLb->enableBold();
    layer->addChild(coScLb, 8);

    // actions //

    int hiSc_uint = tool->getUDef()->getIntegerForKey("TOTAL SCORE", 0);

    std::string
        coCo_str[10],
        hiSc_str[10],
        coSc_str[10];
    
    for (int i = 0; i < 10; i++) {

        coCo_str[i] = std::to_string(curCo     / (10 / (i+1)));
        hiSc_str[i] = std::to_string(hiSc_uint / (10 / (i+1)));
        coSc_str[i] = std::to_string(curSc     / (10 / (i+1)));
    }

    CallFunc
        *callFunc_coCo[10],
        *callFunc_hiSc[10],
        *callFunc_coSc[10];
    
    for (int i = 0; i < 10; i++) {

        callFunc_coCo[i] = CallFunc::create([=]() {
            
            coCoLb->setString(coCo_str[i]);
            coCoSg_pos();
        });

        callFunc_hiSc[i]  = CallFunc::create([=]() {
            
            hiScLb->setString(hiSc_str[i]);
        });

        callFunc_coSc[i]  = CallFunc::create([=]() {
            
            coScLb->setString(coSc_str[i]);
        });
    }

    coCoLb->runAction(Sequence::create(

        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[0],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[1],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[2],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[3],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[4],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[5],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[6],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[7],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[8],
        DelayTime::create(curCo != 0 ? 0.1 : 0.0), callFunc_coCo[9],
        nullptr
    ));

    hiScLb->runAction(Sequence::create(

        DelayTime::create(curCo     != 0 ? 1.0 : 0.0),
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[0],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[1],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[2],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[3],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[4],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[5],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[6],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[7],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[8],
        DelayTime::create(hiSc_uint != 0 ? 0.1 : 0.0), callFunc_hiSc[9],
        nullptr
    ));

    coScLb->runAction(Sequence::create(

        DelayTime::create(hiSc_uint != 0 ?

            (curCo != 0 ? 2.0 : 1.0) :
            (curCo != 0 ? 1.0 : 0.0)
        ),
        DelayTime::create(0.1), callFunc_coSc[0],
        DelayTime::create(0.1), callFunc_coSc[1],
        DelayTime::create(0.1), callFunc_coSc[2],
        DelayTime::create(0.1), callFunc_coSc[3],
        DelayTime::create(0.1), callFunc_coSc[4],
        DelayTime::create(0.1), callFunc_coSc[5],
        DelayTime::create(0.1), callFunc_coSc[6],
        DelayTime::create(0.1), callFunc_coSc[7],
        DelayTime::create(0.1), callFunc_coSc[8],
        DelayTime::create(0.1), callFunc_coSc[9],
        CallFunc::create([=]() {

            auto countDown = [=]() {

                auto ctDownLb = tool->myLabel("Main Menu in 3", getBoardSize("").height / 2);
                ctDownLb->setPosition(board[1].pos.x, board[1].pos.y + board[1].size.height / 2 + ctDownLb->getContentSize().height);
                ctDownLb->setColor(Color3B::YELLOW);
                ctDownLb->enableBold();
                ctDownLb->setOpacity(0);
                layer->addChild(ctDownLb, 7);

                CallFunc *callFunc_ctDown_str[3] = {
                        
                    CallFunc::create([=]() {ctDownLb->setString("Main Menu in 2");}),
                    CallFunc::create([=]() {ctDownLb->setString("Main Menu in 1");}),
                    CallFunc::create([=]() {ctDownLb->setString("Main Menu in 0");})
                };

                ctDownLb->runAction(Sequence::create(

                    FadeIn::create(0),
                    DelayTime::create(1), callFunc_ctDown_str[0],
                    DelayTime::create(1), callFunc_ctDown_str[1],
                    DelayTime::create(1), callFunc_ctDown_str[2],
                    nullptr
                ));
            };

            auto unhideHomeButton = [=]() {

                hidePrebuildButton(false, 3);
                hideBoard(false, 1);
                countDown();
            };

            if (curSc > hiSc_uint) {
                
                hiScLb->setString(coScLb->getString());
                hiScLb->runAction(Sequence::create(

                    TintTo::create(0.25, Color3B(255, 218, 24)),
                    TintTo::create(0.25, Color3B::BLUE),
                    TintTo::create(0.25, Color3B(255, 218, 24)),
                    TintTo::create(0.25, Color3B::BLUE),
                    TintTo::create(0.25, Color3B(255, 218, 24)),
                    TintTo::create(0.25, Color3B::BLUE),
                    nullptr
                ));

                tool->playSFX("new high score");

                auto newHiScTl = tool->myLabel("NEW HIGH SCORE!!!", getBoardSize("").height);
                newHiScTl->setPosition(tool->ctrPos());
                newHiScTl->setColor(Color3B(127, 255, 127));
                newHiScTl->enableOutline(Color4B(0, 191, 0, 127), 3);
                layer->addChild(newHiScTl, 9);
                newHiScTl->runAction(Sequence::create(

                    Spawn::create(

                        MoveBy::create(1.5, Vec2(0, tool->getVSz().height / 4)),
                        Sequence::create(

                            DelayTime::create(0.75),
                            FadeOut::create(0.75),
                            nullptr
                        ),
                        nullptr
                    ),
                    CallFunc::create([=]() {unhideHomeButton();}),
                    nullptr
                ));
            }
            else unhideHomeButton();
        }),
        nullptr
    ));

    if (curSc > hiSc_uint) return 6.0 + 1.5 + 0.25;
    else return 6.0 + 0.25;
}

// part of 'buttonDetection'
void GUI::switchBtns(

    int *count,
    bool *isUnbreak,
    std::string *detectedMessage,
    char *touchCondition,
    Vec2 *began,
    Vec2 *ended
) {

    int i = *count;

    for (i = btnsTag.start; i <= btnsTag.end; i++) {

        int k = 0;
        for (int j = 0; j < boardPartsCount; j++) {

            if (tool->isAtTouchPos(button[i].buttonBoard[1]->part[j], *touchCondition, *began, *ended)
                && isHide_button[i] == false) break;
            k++;
        }

        if (k < boardPartsCount) break;
        if (i == btnsTag.end) *isUnbreak = true;
    }

    if (*isUnbreak || i == -1) {
        
        i = -1;
        *isUnbreak = false;
    }

    if (sceneDesignation == "main menu") {

        switch (i) {

            case 0: *detectedMessage = "buy"; break;
            case 1: *detectedMessage = "+"; break;
            case 2: *detectedMessage = "-"; break;
            case 3: *detectedMessage = "settings"; break;
            default: *detectedMessage = "";
        }
    }
    else if (sceneDesignation == "game") {

        switch (i) {

            case 0: *detectedMessage = "pause"; break;
            default: *detectedMessage = "";
        }
    }
    else if (sceneDesignation == "paused") {

        switch (i) {

            case 0: *detectedMessage = "1"; break;
            case 1: *detectedMessage = "2"; break;
            case 2: *detectedMessage = "3"; break;
            case 3: *detectedMessage = "4"; break;
            default: *detectedMessage = "";
        }
    }

    *count = i;
}

// should be invoked in scene touch listener
std::string GUI::buttonDetection(char touchCondition, Vec2 began, Vec2 ended) {

    std::string detectedMessage = "";
    int i = 0;
    bool isUnbreak = false;

    if (preBtnsTag.end != -2) {

        for (i = preBtnsTag.start; i <= preBtnsTag.end; i++) {

            if (tool->isAtTouchPos(prebuildButton[i][0], touchCondition, began, ended)
                && isHide_prebuildButton[i] == false) break;
            if (i == preBtnsTag.end) isUnbreak = true;
        }

        if (isUnbreak) {
            
            i = -1;
            isUnbreak = false;
        }

        if (sceneDesignation == "main menu") {
            
            switch (i) {

                case 0: detectedMessage = "play"; break;
                case 1: detectedMessage = "arrow left"; break;
                case 2: detectedMessage = "arrow right"; break;
                default: switchBtns(&i, &isUnbreak, &detectedMessage, &touchCondition, &began, &ended);
            }
        }
        else if (sceneDesignation == "game") {
            
            switch (i) {

                case 0: break;
                default: switchBtns(&i, &isUnbreak, &detectedMessage, &touchCondition, &began, &ended);
            }
        }
        else if (sceneDesignation == "paused") {
            
            switch (i) {

                case 0: break;
                default: switchBtns(&i, &isUnbreak, &detectedMessage, &touchCondition, &began, &ended);
            }
        }
        else if (sceneDesignation == "game over") {

            switch (i) {

                case 3: detectedMessage = "home"; break;
            }
        }
    }
    else if (btnsTag.end != -2) {

        switchBtns(&i, &isUnbreak, &detectedMessage, &touchCondition, &began, &ended);
    }

    if (noticeDesignation != "" && detectedMessage != "home") {

        if (detectedMessage == "+" || detectedMessage == "-") {

            if (sceneDesignation == "main menu") {

                if (noticeDesignation == "payment")
                    detectedMessage = detectedMessage == "+" ?
                        "payment buy" : "payment cancel";

                else if (noticeDesignation == "exit")
                    detectedMessage = detectedMessage == "+" ?
                        "exit yes" : "exit no";

                else if (noticeDesignation == "not enough")
                    detectedMessage = detectedMessage == "+" ?
                        "not enough ok" : "";

                else if (noticeDesignation == "ball purchased")
                    detectedMessage = detectedMessage == "+" ?
                        "ball purchased ok" : "";

                else if (noticeDesignation == "settings")
                    detectedMessage = detectedMessage == "+" ?
                        "settings music" : "settings sound";
            }
        }
        else if (detectedMessage == "1" || detectedMessage == "2"
                || detectedMessage == "3" || detectedMessage == "4") {

            if (sceneDesignation == "paused") {

                if (noticeDesignation == "paused") {

                    if      (detectedMessage == "1") detectedMessage = "paused resume";
                    else if (detectedMessage == "2") detectedMessage = "paused quit";
                    else if (detectedMessage == "3") detectedMessage = "paused music";
                    else if (detectedMessage == "4") detectedMessage = "paused sound";
                }
                else if (noticeDesignation == "quit") {

                    if      (detectedMessage == "1") detectedMessage = "quit yes";
                    else if (detectedMessage == "2") detectedMessage = "quit no";
                }
            }
        }
        else detectedMessage = "";
    }

    return detectedMessage;
}