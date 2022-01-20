#ifndef __GUI_H__
#define __GUI_H__

#include "cocos2d.h"
#include "Tool.h"

class GUI {

    public:

        void init(cocos2d::Layer *layer_in, Tool *tool_in, bool isAtMainMenu);
        void generate(std::string sceneName);
        void remove(std::string sceneName);
        void press(std::string buttonName, char touchCondition);
        void press_prebtn(int tag, bool isRelease);
        void press_btn(int boardTag, bool isRelease);

        void setBoardString(
            
            int tag,
            std::string str,
            cocos2d::Size sizePerAlphabet,
            cocos2d::Vec2 pos = cocos2d::Vec2::ZERO
        );

        void showNoticeBoard(bool isShow, std::string designation);

        void setBtnsTag(int tag_st, int tag_en);
        void setPreBtnsTag(int tag_st, int tag_en);

        void hideBoard(bool isHide, int tag);
        void hideButton(bool isHide, int buttonTag, int boardTag);
        void hidePrebuildButton(bool isHide, int tag);

        void syncBallPrice();
        void durationBar_on(std::string name);
        void hideDurationBars();

        std::string buttonDetection(
            
            char touchCondition,
            cocos2d::Vec2 began,
            cocos2d::Vec2 ended = cocos2d::Vec2::ZERO
        );

        cocos2d::Size getBoardSize(std::string str);

        void set_ball_isPurchased();
        bool get_ball_isPurchased();
        int getBallPrice();

        float gameOverSequence(int curCo, int curSc);

        std::string getNoticeDesignation();

    private:

        // METHODS //

        void generateDarkenNoticeBG(int zOrder);

        void generateBoard(
            
            std::string *boardContentStr,
            int tag,
            int zOrder,
            cocos2d::Vec2 pos,
            cocos2d::Size size,
            bool isWithTitleBar,
            float labelHeight,
            int opacity,
            cocos2d::Color3B boardColor = cocos2d::Color3B::WHITE,
            cocos2d::Color3B labelColor = cocos2d::Color3B::WHITE
        );

        void generateButton(
            
            std::string *buttonLabel,
            int buttonTag,
            int boardTag,
            int zOrder,
            cocos2d::Vec2 pos = cocos2d::Vec2::ZERO,
            cocos2d::Color3B color = cocos2d::Color3B::WHITE,
            cocos2d::Vec2 ratioBoard = cocos2d::Vec2(1, 1),
            float ratioLabel = 1.0
        );

        void generatePrebuildButton(std::string buttonName, int zOrder);

        void getBoardSizePos(
    
            cocos2d::Size size,
            cocos2d::Vec2 pos,
            cocos2d::Size *partSize,
            cocos2d::Vec2 *partPos
        );

        void setBoardSize(int tag, cocos2d::Size size, cocos2d::Vec2 pos);

        void generatePriceBoard();
        cocos2d::Vec2 priceBoardPos(std::string strForLength);

        void durationBar_init();

        void setBoardContentString();
        void setBoardContentString(std::string str_1, std::string str_2);

        cocos2d::Color3B getButtonColorBG(cocos2d::Color3B color);
        
        void editButton(

            std::string str,
            int buttonTag,
            int boardTag,
            cocos2d::Vec2 pos,
            cocos2d::Color3B color
        );

        void editNoticeBoard(
            
            std::string title_str,
            std::string board_str,
            int tag,
            cocos2d::Color3B labelColor
        );

        void notice_hideSign_relocateLabel(bool isHide, int tag);

        void showNoticeButton(
            
            int count,
            std::string *str,
            int *buttonTag,
            int *boardTag,
            int noticeBoardTag,
            bool isHorizontal,
            cocos2d::Color3B *color
        );

        void unshowNoticeButton(int count, int *buttonTag, int *boardTag, int noticeBoardTag);

        void musicCondition(std::string *buttonString, cocos2d::Color3B *buttonColor);
        void soundCondition(std::string *buttonString, cocos2d::Color3B *buttonColor);

        void switchBtns(

            int *count,
            bool *isUnbreaked,
            std::string *detectedMessage,
            char *touchCondition,
            Vec2 *began,
            Vec2 *ended
        );

        int getCurrentSelected();

        // OBJECTS //

        static const int
            boardsCount = 16,
            boardPartsCount = 7,
            buttonsCount = 10,
            preBtnsCount = 4,
            ballsCount = 5;

        cocos2d::Layer *layer;
        cocos2d::Sprite *prebuildButton[preBtnsCount][2];
        cocos2d::DrawNode *darkenNoticeBG;

        // collected items duration bar
        cocos2d::Sprite *magnetSign[2], *shieldSign[2];
        cocos2d::DrawNode *magnetBar[2], *magnetBar_bg[2], *shieldBar[2], *shieldBar_bg[2];

        struct Board {

            cocos2d::Sprite *part[boardPartsCount];
            cocos2d::Sprite *sign;
            cocos2d::Label *label;

            cocos2d::Sprite *titleBar;
            cocos2d::Label *titleLabel;

            cocos2d::Size size;
            cocos2d::Vec2 pos;
        } board[boardsCount];

        struct Button {
            
            struct Board *buttonBoard[2];
        } button[buttonsCount];

        struct BallData {
            
            int isPurchased, price, reachScore;
            bool isUnlocked;
        } balDat[ballsCount];

        Tool *tool;

        // VARIABLES //

        int boardsCurrentCount = 0;

        std::string
            boardContentString[2] = {"", ""},
            noticeDesignation = "",
            sceneDesignation;

        bool
            isHide_button[buttonsCount],
            isPressed[buttonsCount],
            isHide_prebuildButton[preBtnsCount];
        
        struct BTNSTAG {int start, end;} btnsTag;
        struct PREBTNSTAG {int start, end;} preBtnsTag;
};

#endif // __GUI_H__