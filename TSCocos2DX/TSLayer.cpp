//
//  TSLayer.cpp
//  TSCocos2DX
//
//  Created by TSEnel on 13-2-17.
//
//

#include "TSLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* TSLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TSLayer *layer = TSLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TSLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    

    CCSprite* pSprite = CCSprite::create("background.png");
    {
        pSprite->setPosition( ccp(size.width/2, size.height/2) );
        pSprite->setScale(2);
        this->addChild(pSprite, 0, 1000);
    }

    CCSprite* pMesh = CCSprite::create("board_test.png");
    {
        pMesh->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(pMesh, 0, 1001);
    }
    
    CCRect pR = pMesh->getTextureRect();
    CCPoint pOO = ccp(size.width/2 - pR.size.width/2, size.height/2 - pR.size.height/2);
    
    CCSprite* pBall = CCSprite::create("chess0.png");
    pBall->setPosition(pOO);
    this->addChild(pBall, 2, 1);
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            CCSprite* pT = CCSprite::create("chess1.png");
            CCPoint pX = ccp(pOO.x + i * 33 + 32/2, pOO.y + j * 33 + 32/2);
            pT->setPosition(pX);
            pT->setScale(2);
            this->addChild(pT, 2, 1);
        }
    }
    
    return true;
}


