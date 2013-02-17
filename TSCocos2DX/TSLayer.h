//
//  TSLayer.h
//  TSCocos2DX
//
//  Created by TSEnel on 13-2-17.
//
//

#ifndef __TSCocos2DX__TSLayer__
#define __TSCocos2DX__TSLayer__

#include <iostream>
#include <list>
#include "cocos2d.h"

class NEAStar;
class TSMap;

class TSLayer : public cocos2d::CCLayer
{
public:
    TSLayer();
    virtual ~TSLayer();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(TSLayer);
    
private:
    cocos2d::CCPoint m_pMeshPos[9][9];
    cocos2d::CCPoint m_pOO;
    TSMap* m_Map;
    NEAStar* m_Star;
    
    //Game
    cocos2d::CCSprite* m_Choose;
    
    std::list<cocos2d::CCSprite*> m_pPathSpriteList;
};

#endif /* defined(__TSCocos2DX__TSLayer__) */
