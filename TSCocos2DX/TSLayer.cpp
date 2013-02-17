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
    
    CCSprite* pBall = CCSprite::create("chess0.png");
    pBall->setPosition(pOO);
    this->addChild(pBall, 2, 1);
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            //CCSprite* pT = CCSprite::create("chess1.png");
            m_pMeshPos[i][j] = ccp(pOO.x + i * 33 + 32/2, pOO.y + j * 33 + 32/2);
            //pT->setPosition(m_pMeshPos[i][j]);
            //pT->setScale(2);
            //this->addChild(pT, 2, 1);
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
    
    TSPoint pO = TSPoint(2,0);
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
    
    //_TSMap[star.pStart.m_x*pTSMap.m_width+star.pStart.m_y] = 3;
    //_TSMap[star.pEnd.m_x*pTSMap.m_width+star.pEnd.m_y] = 2;
    
//    for (int i = 0; i < pTSMap.m_height ; i++)
//    {
//        for (int j = 0 ; j < pTSMap.m_width ; j++)
//        {
//            char c = _TSMap[i*pTSMap.m_width+j];
//            if (c == 0)
//            {
//                cout << " ";
//            }
//            else if (c == 1)
//            {
//                cout << "0";
//            }
//            else if (c == 2)
//            {
//                cout << "+";
//            }
//            else
//            {
//                cout << c;
//            }
//        }
//        cout << endl;
//    }
    
    
    return true;
}


