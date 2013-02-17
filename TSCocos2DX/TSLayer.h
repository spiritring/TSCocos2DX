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
#include <vector>
#include "cocos2d.h"
#include "TSSprite.h"

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
    
    // touch call back
    bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* event);

    // Game ////
    
    // Random 3 ball
    bool random3Ball();
    
    bool randomBall();
    
    TSSprite* GetMeshSprite(TSPoint& pos);
    
    // RemoveBall
    bool removeBall();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(TSLayer);
    
    void draw();
    
private:
    cocos2d::CCPoint m_pMeshPos[9][9];
    cocos2d::CCPoint m_pOO;
    TSMap* m_Map;
    NEAStar* m_Star;
    
    //Game
    TSSprite* m_Choose;
    std::vector<TSSprite*> m_SpiritPool;
    
    std::list<cocos2d::CCSprite*> m_pPathSpriteList;
    
    std::vector<TSPoint> m_pPath;
    int m_iIndexPath;
    
    int m_iStat;
};

#endif /* defined(__TSCocos2DX__TSLayer__) */
