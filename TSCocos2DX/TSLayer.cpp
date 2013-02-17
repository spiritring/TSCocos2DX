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

TSLayer::TSLayer()
: m_Map(NULL),m_Star(NULL),m_Choose(NULL)
{
    m_Map = new TSMap();
    m_Star = new NEAStar();
}

TSLayer::~TSLayer()
{
    delete m_Map;
    delete m_Star;
}

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
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
    //this->setTouchEnabled(true);
    
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
    m_pOO = ccp(size.width/2 - pR.size.width/2, size.height/2 - pR.size.height/2);
        
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            m_pMeshPos[i][j] = ccp(m_pOO.x + i * 33 + 32/2, m_pOO.y + j * 33 + 32/2);
        }
    }
    
    for (int i = 0; i < m_Map->m_width*m_Map->m_height; i++) {
        int l = i / m_Map->m_width;
        int h = i % m_Map->m_height;
        
        if (m_Map->m_TSMap[i] == 0) {
            continue;
        }
        
        CCSprite* pT = CCSprite::create("chess1.png");
        CCPoint pP = m_pMeshPos[l][h];
        pT->setPosition(pP);
        pT->setScale(2);
        this->addChild(pT, 2, 1);
    }
    
    return true;
}

static CCRect getRect(CCNode* pNode)
{
    CCRect rc;
    rc.origin = pNode->getPosition();
    rc.size = pNode->getContentSize();
    rc.origin.x -= rc.size.width*0.5;
    rc.origin.y -= rc.size.height*0.5;
    return rc;
}

bool TSLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    CCPoint touchLocation = convertTouchToNodeSpace(pTouch);
    CCPoint pGY = CCPoint(touchLocation.x - m_pOO.x , touchLocation.y - m_pOO.y);
    
    int x = 0;
    int y = 0;
    
    x = pGY.x / 33;
    y = pGY.y / 33;
    
    if (x > 8 || y > 8) {
        return false;
    }
    
    printf("我被点中了! x = %d y = %d \n", x, y);
    
    //clear path sprite
    for (std::list<CCSprite*>::iterator iter = m_pPathSpriteList.begin(); iter != m_pPathSpriteList.end(); iter++) {
        this->removeChild(*iter, true);
    }
    
    //new to find for best path
    TSPoint pO = TSPoint(5,4);
    TSPoint pT = TSPoint(x,y);
    
    //m_Star = new NEAStar();
    
	m_Star->Init(pO, pT, m_Map);
    m_Star->run();
    
    TSNode* TSNode = m_Star->getResult();
    if (TSNode->pPos.m_x != x && TSNode->pPos.m_y != y) {
        printf("错误的寻路!");
        return false;
    }
    
    while (TSNode->pFather != NULL)
    {
        CCSprite* pT = CCSprite::create("chess2.png");
        CCPoint pP = m_pMeshPos[TSNode->pPos.m_x][TSNode->pPos.m_y];
        pT->setPosition(pP);
        //pT->setScale(2);
        this->addChild(pT, 2, 1);
        m_pPathSpriteList.push_back(pT);
        
        TSNode = TSNode->pFather;
    }
    
    
    return true;
}


