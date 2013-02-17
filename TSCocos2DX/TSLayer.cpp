//
//  TSLayer.cpp
//  TSCocos2DX
//
//  Created by TSEnel on 13-2-17.
//
//

#include "TSLayer.h"
#include "SimpleAudioEngine.h"
#include "NEAStar.h"
#include "TSPoint.h"

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
        
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            m_pMeshPos[i][j] = ccp(pOO.x + i * 33 + 32/2, pOO.y + j * 33 + 32/2);
        }
    }
    
    NEAStar star;
	TSMap map;
    
    for (int i = 0; i < map.m_width*map.m_height; i++) {
        int l = i / map.m_width;
        int h = i % map.m_height;
        
        if (map.m_TSMap[i] == 0) {
            continue;
        }
        
        CCSprite* pT = CCSprite::create("chess1.png");
        CCPoint pP = m_pMeshPos[l][h];
        pT->setPosition(pP);
        pT->setScale(2);
        this->addChild(pT, 2, 1);
    }
    
    TSPoint pO = TSPoint(0,0);
    TSPoint pT = TSPoint(2,4);
    
	star.Init(pO, pT, &map);
    star.run();
    
    TSNode* TSNode = star.getResult();
    while (TSNode->pFather != NULL)
    {
        CCSprite* pT = CCSprite::create("chess2.png");
        CCPoint pP = m_pMeshPos[TSNode->pPos.m_x][TSNode->pPos.m_y];
        pT->setPosition(pP);
        //pT->setScale(2);
        this->addChild(pT, 2, 1);
        
        TSNode = TSNode->pFather;
    }
    
    CCSprite* pBall = CCSprite::create("chess0.png");
    pBall->setPosition(m_pMeshPos[pO.m_x][pO.m_y]);
    this->addChild(pBall, 2, 1);

    
    return true;
}


