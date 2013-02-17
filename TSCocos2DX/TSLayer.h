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
#include "cocos2d.h"

class TSLayer : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(TSLayer);
    
private:
    cocos2d::CCPoint m_pMeshPos[9][9];
};

#endif /* defined(__TSCocos2DX__TSLayer__) */
