//
//  TSSprite.h
//  TSCocos2DX
//
//  Created by TSEnel on 13-2-17.
//
//

#ifndef __TSCocos2DX__TSSprite__
#define __TSCocos2DX__TSSprite__

#include <iostream>
#include <string>
#include "cocos2d.h"
#include "TSPoint.h"
class TSSprite : public cocos2d::CCSprite
{
public:
    TSSprite()
    {
        iColor = 0;
    }
    
    static TSSprite* CreateSprite(TSPoint sPos, char* cColor)
    {
        std::string filepath = "chess" + std::string(cColor) + ".png";

        TSSprite *pobSprite = new TSSprite();
        pobSprite->pos = sPos;
        pobSprite->iColor = atoi(cColor);
        
        if (pobSprite && pobSprite->initWithFile(filepath.c_str()))
        {
            pobSprite->autorelease();
            return pobSprite;
        }
        CC_SAFE_DELETE(pobSprite);
        return NULL;
    }
    
public:
    TSPoint pos;
    int iColor;
};

#endif /* defined(__TSCocos2DX__TSSprite__) */
