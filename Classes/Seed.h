#ifndef __SEED_H__
#define __SEED_H__

#include "cocos2d.h"

USING_NS_CC;

class Seed {

    public:

        int getBricksCountPattern() const {return bricksCountPattern;}

        int getBrickTag(int member) {

            updateSelectPattern();

            int whatPat = selPat.tag, randInt;

            std::srand(std::time(0));
            randInt = std::rand() % patternWrap[whatPat];
            
            return brickTag[randInt][member];
        }

        int getCatMask(std::string name) const {

            if      (name == "ball")        return ball_catMask;
            else if (name == "reflection")  return reflection_catMask;
            else if (name == "obstacle")    return obstacle_catMask;
            else if (name == "blank")       return blank_catMask;
            else if (name == "left thorn")  return leftThorn_catMask;
            else if (name == "right thorn") return rightThorn_catMask;
            else if (name == "full thorn")  return fullThorn_catMask;
            else if (name == "laser")       return laser_catMask;
            else if (name == "coin")        return coin_catMask;
            else if (name == "magnet")      return magnet_catMask;
            else if (name == "shield")      return shield_catMask;
            else if (name == "particle")    return particle_catMask;
            else return 0;
        }

        int getColMask(std::string name) const {

            if      (name == "ball")        return ball_colMask;
            else if (name == "reflection")  return reflection_colMask;
            else if (name == "obstacle")    return obstacle_colMask;
            else if (name == "blank")       return blank_colMask;
            else if (name == "left thorn")  return leftThorn_colMask;
            else if (name == "right thorn") return rightThorn_colMask;
            else if (name == "full thorn")  return fullThorn_colMask;
            else if (name == "laser")       return laser_colMask;
            else if (name == "coin")        return coin_colMask;
            else if (name == "magnet")      return magnet_colMask;
            else if (name == "shield")      return shield_colMask;
            else if (name == "particle")    return particle_colMask;
            else return 0;
        }

        float getVelY() {return selPat.velY;}

        bool get_isVelocityChanged() {return selPat.isVelocityChanged;}

    private:

        struct SelectPattern {

            int tag;                // what pattern
            float velY;             // y velocity for brick
            bool isVelocityChanged; // is velocity has changed
        } selPat;

        void updateSelectPattern() {

            int startTime = UserDefault::getInstance()->getIntegerForKey("START TIME");

            if      (std::time(0) - startTime <= 25)  {selPat.tag = 0; selPat.velY = -200.0;}
            else if (std::time(0) - startTime <= 50)  {selPat.tag = 0; selPat.velY = -210.0;}
            else if (std::time(0) - startTime <= 75)  {selPat.tag = 1; selPat.velY = -210.0;}
            else if (std::time(0) - startTime <= 100) {selPat.tag = 1; selPat.velY = -220.0;}
            else if (std::time(0) - startTime <= 125) {selPat.tag = 2; selPat.velY = -220.0;}
            else if (std::time(0) - startTime <= 150) {selPat.tag = 2; selPat.velY = -230.0;}
            else if (std::time(0) - startTime <= 175) {selPat.tag = 3; selPat.velY = -230.0;}
            else                                      {selPat.tag = 3; selPat.velY = -240.0;}

            if (velYBuffer != selPat.velY) {
            
                velYBuffer = selPat.velY;
                selPat.isVelocityChanged = true;
            }
            else selPat.isVelocityChanged = false;
        }

        static const int
            patternsCount = 152,
            bricksCountPattern = 10;

        const int patternWrap[4] = {86, 106, 135, 152};

        const int // category bitmasks
            ball_catMask = 1,
            reflection_catMask = 2,
            obstacle_catMask = 4,
            blank_catMask = 8,
            leftThorn_catMask = 16,
            rightThorn_catMask = 32,
            fullThorn_catMask = 64,
            laser_catMask = 128,
            coin_catMask = 256,
            magnet_catMask = 512,
            shield_catMask = 1024,
            particle_catMask = 2048;

        const int // collision bitmasks
            ball_colMask = obstacle_catMask + laser_catMask + leftThorn_catMask + rightThorn_catMask + fullThorn_catMask,
            reflection_colMask = ball_colMask,
            obstacle_colMask = ball_catMask + reflection_catMask + particle_catMask,
            blank_colMask = blank_catMask,
            laser_colMask = ball_catMask + particle_catMask,
            leftThorn_colMask = ball_catMask + particle_catMask,
            rightThorn_colMask = ball_catMask + particle_catMask,
            fullThorn_colMask = ball_catMask + particle_catMask,
            coin_colMask = coin_catMask,
            magnet_colMask = magnet_catMask,
            shield_colMask = shield_catMask,
            particle_colMask = ball_colMask + particle_catMask;

        float velYBuffer = 0.0;

        // brick patterns (seeds)
        const short int brickTag[patternsCount][bricksCountPattern] = {

            /*
                EXPLANATION:
                0 = blank
                1 = brick
                2 = left thorn
                3 = right thorn
                4 = full thorn
                5 = laser
                6 = coin
                7 = magnet
                8 = shield
                9 = container
            */

            // second 25 //

            {9, 1, 1, 1, 1, 0, 0, 0, 0, 0}, // blank
            {0, 0, 0, 0, 0, 1, 1, 1, 1, 9},
            {9, 1, 1, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 1, 1, 9},
            {9, 1, 1, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 1, 1, 9},
            {9, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 1, 9},
            {9, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 9},
            {9, 0, 0, 0, 0, 0, 1, 1, 1, 9},
            {9, 1, 1, 1, 0, 0, 0, 0, 0, 9},
            {9, 1, 0, 0, 0, 0, 0, 1, 1, 9},
            {9, 1, 1, 0, 0, 0, 0, 0, 1, 9},
            {0, 0, 0, 9, 1, 1, 9, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {9, 0, 0, 0, 1, 1, 0, 0, 0, 9},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
            {9, 1, 1, 0, 0, 0, 0, 1, 1, 9},
            {9, 1, 0, 0, 0, 0, 0, 0, 1, 9},
            {9, 0, 0, 0, 0, 0, 0, 0, 0, 9},
            {0, 0, 0, 9, 0, 0, 9, 0, 0, 0},
            {0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
            {0, 0, 9, 9, 0, 0, 9, 9, 0, 0},

            {9, 1, 1, 1, 1, 6, 6, 6, 6, 6}, // coins
            {6, 6, 6, 6, 6, 1, 1, 1, 1, 9},
            {9, 1, 1, 1, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 1, 1, 1, 9},
            {9, 1, 1, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 1, 1, 9},
            {9, 1, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 1, 9},
            {9, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 9},
            {9, 6, 6, 6, 6, 6, 1, 1, 1, 9},
            {9, 1, 1, 1, 6, 6, 6, 6, 6, 9},
            {9, 1, 6, 6, 6, 6, 6, 1, 1, 9},
            {9, 1, 1, 6, 6, 6, 6, 6, 1, 9},
            {6, 6, 6, 9, 1, 1, 9, 6, 6, 6},
            {6, 6, 6, 6, 1, 1, 6, 6, 6, 6},
            {9, 6, 6, 6, 1, 1, 6, 6, 6, 9},
            {6, 6, 6, 6, 1, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 1, 6, 6, 6, 6},
            {6, 6, 6, 1, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 1, 6, 6, 6},
            {9, 1, 1, 6, 6, 6, 6, 1, 1, 9},
            {9, 1, 6, 6, 6, 6, 6, 6, 1, 9},
            {9, 6, 6, 6, 6, 6, 6, 6, 6, 9},
            {6, 6, 6, 9, 6, 6, 9, 6, 6, 6},
            {6, 6, 1, 6, 6, 6, 6, 1, 6, 6},
            {6, 6, 9, 9, 6, 6, 9, 9, 6, 6},

            {9, 1, 1, 1, 1, 6, 6, 7, 6, 6}, // items
            {6, 6, 8, 6, 6, 1, 1, 1, 1, 9},
            {9, 1, 1, 1, 6, 6, 6, 7, 6, 6},
            {6, 6, 8, 6, 6, 6, 1, 1, 1, 9},
            {9, 1, 1, 6, 6, 6, 7, 6, 6, 6},
            {6, 6, 6, 8, 6, 6, 6, 1, 1, 9},
            {9, 1, 6, 6, 7, 6, 6, 8, 6, 6},
            {6, 6, 7, 6, 6, 8, 6, 6, 1, 9},
            {9, 6, 6, 7, 6, 6, 6, 8, 6, 6},
            {6, 6, 7, 6, 6, 6, 8, 6, 6, 9},
            {9, 6, 6, 7, 6, 6, 1, 1, 1, 9},
            {9, 1, 1, 1, 6, 6, 8, 6, 6, 9},
            {9, 1, 6, 6, 7, 6, 6, 1, 1, 9},
            {9, 1, 1, 6, 6, 8, 6, 6, 1, 9},
            {6, 6, 7, 9, 1, 1, 9, 8, 6, 6},
            {6, 6, 6, 7, 1, 1, 8, 6, 6, 6},
            {9, 6, 7, 6, 1, 1, 6, 8, 6, 9},
            {6, 6, 6, 6, 1, 6, 6, 7, 6, 6},
            {6, 6, 8, 6, 6, 1, 6, 6, 6, 6},
            {6, 6, 6, 1, 6, 6, 6, 7, 6, 6},
            {6, 6, 8, 6, 6, 6, 1, 6, 6, 6},
            {9, 1, 1, 6, 7, 8, 6, 1, 1, 9},
            {9, 1, 6, 7, 6, 6, 8, 6, 1, 9},
            {9, 6, 6, 7, 6, 6, 8, 6, 6, 9},
            {6, 6, 7, 9, 6, 6, 9, 8, 6, 6},
            {6, 6, 1, 6, 7, 8, 6, 1, 6, 6},
            {6, 6, 9, 9, 7, 8, 9, 9, 6, 6},

            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, // bonus
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},

            // second 75 //

            {1, 5, 5, 5, 1, 1, 0, 0, 0, 9}, // level
            {9, 0, 0, 0, 1, 1, 5, 5, 5, 1},
            {0, 0, 0, 1, 5, 5, 1, 0, 0, 0},
            {0, 0, 1, 5, 5, 5, 5, 1, 0, 0},
            {0, 0, 1, 1, 5, 5, 1, 1, 0, 0},

            {1, 5, 5, 5, 1, 1, 6, 6, 6, 9}, // level coins
            {9, 6, 6, 6, 1, 1, 5, 5, 5, 1},
            {6, 6, 6, 1, 5, 5, 1, 6, 6, 6},
            {6, 6, 1, 5, 5, 5, 5, 1, 6, 6},
            {6, 6, 1, 1, 5, 5, 1, 1, 6, 6},

            {1, 5, 5, 5, 1, 1, 7, 6, 6, 9}, // level items
            {9, 6, 6, 8, 1, 1, 5, 5, 5, 1},
            {6, 6, 7, 1, 5, 5, 1, 8, 6, 6},
            {6, 7, 1, 5, 5, 5, 5, 1, 8, 6},
            {6, 7, 1, 1, 5, 5, 1, 1, 8, 6},

            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, // bonus
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},

            // second 125 //
            
            {9, 1, 1, 1, 2, 0, 0, 0, 0, 0}, // level
            {9, 1, 1, 1, 3, 0, 0, 0, 0, 0},
            {9, 1, 1, 2, 0, 0, 0, 0, 0, 0},
            {9, 1, 1, 3, 0, 0, 0, 0, 0, 0},
            {9, 1, 2, 0, 0, 0, 0, 1, 1, 9},
            {9, 1, 3, 0, 0, 0, 0, 1, 1, 9},
            {0, 0, 2, 0, 0, 0, 0, 1, 0, 0},
            {0, 0, 3, 0, 0, 0, 0, 1, 0, 0},

            {9, 1, 1, 1, 2, 6, 6, 6, 6, 6}, // level coins
            {9, 1, 1, 1, 3, 6, 6, 6, 6, 6},
            {9, 1, 1, 2, 6, 6, 6, 6, 6, 6},
            {9, 1, 1, 3, 6, 6, 6, 6, 6, 6},
            {9, 1, 2, 6, 6, 6, 6, 1, 1, 9},
            {9, 1, 3, 6, 6, 6, 6, 1, 1, 9},
            {6, 6, 2, 6, 6, 6, 6, 1, 6, 6},
            {6, 6, 3, 6, 6, 6, 6, 1, 6, 6},

            {9, 1, 1, 1, 2, 6, 6, 7, 6, 6}, // level items
            {9, 1, 1, 1, 3, 6, 6, 8, 6, 6},
            {9, 1, 1, 2, 6, 6, 6, 7, 6, 6},
            {9, 1, 1, 3, 6, 6, 6, 8, 6, 6},
            {9, 1, 2, 6, 8, 7, 6, 1, 1, 9},
            {9, 1, 3, 6, 8, 7, 6, 1, 1, 9},
            {6, 6, 2, 6, 8, 7, 6, 1, 6, 6},
            {6, 6, 3, 6, 8, 7, 6, 1, 6, 6},

            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, // bonus
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},

            // second 175 //
            
            {9, 1, 1, 1, 4, 0, 0, 0, 0, 0}, // level
            {9, 1, 1, 4, 0, 0, 0, 0, 0, 0},
            {9, 1, 4, 0, 0, 0, 0, 1, 1, 9},
            {0, 0, 4, 0, 0, 0, 0, 1, 0, 0},

            {9, 1, 1, 1, 4, 6, 6, 6, 6, 6}, // level coins
            {9, 1, 1, 4, 6, 6, 6, 6, 6, 6},
            {9, 1, 4, 6, 6, 6, 6, 1, 1, 9},
            {6, 6, 4, 6, 6, 6, 6, 1, 6, 6},

            {9, 1, 1, 1, 4, 6, 6, 7, 6, 6}, // level items
            {9, 1, 1, 4, 6, 6, 6, 8, 6, 6},
            {9, 1, 4, 6, 8, 7, 6, 1, 1, 9},
            {6, 6, 4, 6, 8, 7, 6, 1, 6, 6},

            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}, // bonus
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
            {6, 6, 6, 6, 6, 6, 6, 6, 6, 6}
        };
};

#endif // __SEED_H__