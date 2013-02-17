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
#include "CCArray.h"
#include <cmath>
#include <stdlib.h>

using namespace cocos2d;
using namespace CocosDenshion;


float effectXXX = 1.0f;
float effectXXXIndex = 1.0f;

TSLayer::TSLayer()
: m_Map(NULL),m_Star(NULL),m_Choose(NULL),m_iIndexPath(0),m_iStat(0)
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
    //this->schedule(schedule_selector(TSLayer::GameUpdata),0.1);
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
    
    random3Ball();
    
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

TSSprite* TSLayer::GetMeshSprite(TSPoint& pos)
{
    TSSprite* spr = NULL;
    std::set<TSSprite*>::iterator iter = m_SpiritPool.begin();
    for (; iter != m_SpiritPool.end(); iter++) {
        TSSprite* spr = *iter;
        if (spr->pos.m_x == pos.m_x && spr->pos.m_y == pos.m_y) {
            return *iter;
        }
    }
    return spr;
}

bool TSLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* event)
{
    CCPoint touchLocation = convertTouchToNodeSpace(pTouch);
    CCPoint pGY = CCPoint(touchLocation.x - m_pOO.x , touchLocation.y - m_pOO.y);
    
    TSPoint xy;
    
    xy.m_x = pGY.x / 33;
    xy.m_y = pGY.y / 33;
    
    if (xy.m_x > 8 || xy.m_y > 8) {
        return false;
    }
    
    printf("我被点中了! x = %d y = %d \n", xy.m_x, xy.m_y);
    
    if (m_iStat == 0) {
        if (m_Choose != NULL) {
            m_Choose->setScale(2);
            effectXXX = 1;
            effectXXXIndex = 1;
        }
        
        m_Choose = GetMeshSprite(xy);
        if (m_Choose != NULL) {
            m_iStat = 1;
            m_Choose->setScale(2);
            effectXXX = 1;
            effectXXXIndex = 1;
        }
    }
    else if(m_iStat == 1)
    {
        m_pPath.clear();
        m_iIndexPath = 0;
        
        //new to find for best path
        TSPoint pO = TSPoint(m_Choose->pos.m_x, m_Choose->pos.m_y);
        TSPoint pT = xy;
        
        m_Star->Init(pO, pT, m_Map);
        m_Star->run();
        
        TSNode* TSNode = m_Star->getResult();
        if (TSNode->pPos.m_x != pT.m_x || TSNode->pPos.m_y != pT.m_y) {
            printf("错误的寻路!");
            m_iStat = 0;
            m_Choose->setScale(2);
            effectXXX = 1;
            effectXXXIndex = 1;
            m_Choose = NULL;
            return false;
        }
        
        std::list<TSPoint*> pR;
        while (TSNode->pFather != NULL)
        {
//            CCSprite* pT = CCSprite::create("chess2.png");
//            CCPoint pP = m_pMeshPos[TSNode->pPos.m_x][TSNode->pPos.m_y];
//            pT->setPosition(pP);
//            //pT->setScale(2);
//            this->addChild(pT, 2, 1);
//            m_pPathSpriteList.push_back(pT);
            
            pR.push_front(&TSNode->pPos);
            
            TSNode = TSNode->pFather;
        }
        
        std::list<TSPoint*>::iterator iter = pR.begin();
        for (; iter != pR.end(); iter++) {
            m_pPath.push_back(**iter);
        }
        
        m_iStat = 2;
        m_Map->m_TSMap[m_Choose->pos.m_x * m_Map->m_width + m_Choose->pos.m_y] = 0;
        m_MapSpr[m_Choose->pos.m_x][m_Choose->pos.m_y] = NULL;
    }
    
    return true;
}

bool TSLayer::removeBall(TSSprite* pChoose)
{
    if (pChoose == NULL) {
        return false;
    }
    TSPoint pO = pChoose->pos;
    std::list<TSSprite*> pRList;
    
    std::list<TSSprite*> pUDList;
    int count = 0;
    int index = 0;
    //up down
    for (; ; index++) {
        pO.m_y--;
        if (pO.m_y < 0) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pUDList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    pO = pChoose->pos;
    index = 0;
    //up down
    for (; ; index++) {
        pO.m_y++;
        if (pO.m_y > 8) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pUDList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    if (count >= 4) {
        pRList.merge(pUDList);
    }
    
    
    /////////////////////////////
    pO = pChoose->pos;
    count = 0;
    index = 0;
    std::list<TSSprite*> pLRList;
    //left right
    for (; ; index++) {
        pO.m_x--;
        if (pO.m_x < 0) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pLRList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    pO = pChoose->pos;
    index = 0;
    //up down
    for (; ; index++) {
        pO.m_x++;
        if (pO.m_x > 8) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pLRList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    if (count >= 4) {
        pRList.merge(pLRList);
    }
    
    
    //  \//
    pO = pChoose->pos;
    count = 0;
    index = 0;
    std::list<TSSprite*> pXList;
    //left right
    for (; ; index++) {
        pO.m_x--;
        pO.m_y--;
        if (pO.m_x < 0 || pO.m_y < 0) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pXList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    pO = pChoose->pos;
    index = 0;
    //up down
    for (; ; index++) {
        pO.m_x++;
        pO.m_y++;
        if (pO.m_x > 8 || pO.m_y > 8) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pXList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    if (count >= 4) {
        pRList.merge(pXList);
    }
    
    ///////////////////
    pO = pChoose->pos;
    count = 0;
    index = 0;
    std::list<TSSprite*> pXXList;
    for (; ; index++) {
        pO.m_x--;
        pO.m_y++;
        if (pO.m_x < 0 || pO.m_y > 8) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pXXList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    pO = pChoose->pos;
    index = 0;
    for (; ; index++) {
        pO.m_x++;
        pO.m_y--;
        if (pO.m_x > 8 || pO.m_y < 0) {
            break;
        }
        
        TSSprite* spr = m_MapSpr[pO.m_x][pO.m_y];
        if (spr != NULL) {
            if (spr->iColor == pChoose->iColor) {
                count ++;
                pXXList.push_back(spr);
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
    
    if (count >= 4) {
        pRList.merge(pXXList);
    }
    
    std::list<TSSprite*>::iterator iter = pRList.begin();
    for (; iter != pRList.end(); iter++) {
        TSSprite* spr = *iter;
        
        m_Map->m_TSMap[spr->pos.m_x * m_Map->m_width + spr->pos.m_y] = 0;
        m_MapSpr[spr->pos.m_x][spr->pos.m_y] = 0;
        
        this->removeChild(spr, true);
        
        m_SpiritPool.erase(spr);
    }
    
    if (pRList.size() > 0) {
        TSSprite* spr = pChoose;
        
        m_Map->m_TSMap[spr->pos.m_x * m_Map->m_width + spr->pos.m_y] = 0;
        m_MapSpr[spr->pos.m_x][spr->pos.m_y] = 0;
        this->removeChild(spr, true);
        m_SpiritPool.erase(spr);
 
        return true;
    }
    
    
    
    return false;
}



void TSLayer::draw()
{
    if (m_iStat != 2) {
        if (m_Choose != NULL) {
            
            if (effectXXX < 1) {
                effectXXXIndex = 1;
                effectXXX = 1;
            }
            else if (effectXXX >= 2) {
                effectXXXIndex = -1;
                effectXXX = 2;
            }
            
            effectXXX += 0.1 * effectXXXIndex;
            
            m_Choose->setScale(effectXXX);
        }
        
        return;
    }
    
    if (m_pPath.size() <= 0) {
        return;
    }
    
    if (m_pPath.size() <= m_iIndexPath) {
        
        TSPoint iter = m_pPath.back();
        int l = iter.m_x;
        int h = iter.m_y;
        m_Map->m_TSMap[l * m_Map->m_width + h] = 1;
        m_MapSpr[l][h] = m_Choose;
        m_Choose->setScale(2);
        m_iStat = 0;
        
        if (removeBall(m_Choose)) {
            //add score
            
            m_Choose = NULL;
        }
        else {
            //add 3 ball
            if (!random3Ball()) {

            }
            
            if (m_SpiritPool.size() >= 81) {
                memset(m_Map->m_TSMap, 0, m_Map->m_width*m_Map->m_height);
                m_MapSpr.clear();
                std::set<TSSprite*>::iterator iter = m_SpiritPool.begin();
                for (; iter != m_SpiritPool.end(); iter++) {
                    this->removeChild(*iter, true);
                }
                m_SpiritPool.clear();
                m_Choose = NULL;
                
                random3Ball();
            }            
        }
        
        for (std::list<CCSprite*>::iterator iter = m_pPathSpriteList.begin(); iter != m_pPathSpriteList.end(); iter++) {
            this->removeChild(*iter, true);
        }
        m_pPathSpriteList.clear();

        for (int i = 0; i < m_Map->m_width*m_Map->m_height; i++) {
            int l = i / m_Map->m_width;
            int h = i % m_Map->m_height;
        
            if (m_Map->m_TSMap[i] == 0) {
                continue;
            }
            CCSprite* pT = CCSprite::create("chess1.png");
            m_pPathSpriteList.push_back(pT);
            CCPoint pP = m_pMeshPos[l][h];
            pT->setPosition(pP);
            //pT->setScale(2);
            this->addChild(pT, 2, 1);
        }
        return;
    }
    
    TSPoint pPos = m_pPath[m_iIndexPath];
    
    CCPoint pEnd = m_pMeshPos[pPos.m_x][pPos.m_y];
    const CCPoint& pT = m_Choose->getPosition();
    //{
        CCPoint pMove = pT;
        
        if (abs(pEnd.x - pT.x) < 8 && abs(pEnd.y - pT.y) < 8 )
        {
            m_iIndexPath ++;
            m_Choose->pos = pPos;
            pMove = pEnd;
        }
        else
        {
            if (pEnd.x < pT.x)
            {
                pMove.x -= 8;
                
            }
            else if (pEnd.x > pT.x)
            {
                pMove.x += 8;
            }
            
            if (pEnd.y < pT.y)
            {
                pMove.y -= 8;
            }
            else if (pEnd.y > pT.y)
            {
                pMove.y += 8;
            }
        }
        
        m_Choose->setPosition(pMove);
    //}
}

bool TSLayer::randomBall()
{
    std::vector<std::pair<int, int> > EmptyMap;
    for (int i = 0; i < m_Map->m_width * m_Map->m_height; i++) {
        if (m_Map->m_TSMap[i] == 0) {
            int l = i / m_Map->m_width;
            int h = i % m_Map->m_height;
            EmptyMap.push_back(std::make_pair(l, h));
        }
    }
    
    if (EmptyMap.size() <= 0) {
        return false;
    }
    
    int choose = rand() % EmptyMap.size();
    
    TSPoint Pos;
    Pos.m_x = EmptyMap[choose].first;
    Pos.m_y = EmptyMap[choose].second;
    
    char buffer[32] = {0};
    sprintf(buffer, "%d", (int)rand()%5);
    TSSprite* spr = TSSprite::CreateSprite(Pos, buffer);
    CCPoint loc = m_pMeshPos[Pos.m_x][Pos.m_y];
    spr->setPosition(loc);
    spr->setScale(2);
    this->addChild(spr);
    
    m_Map->m_TSMap[Pos.m_x * m_Map->m_width + Pos.m_y] = 1;
    m_MapSpr[Pos.m_x][Pos.m_y] = spr;
    
    m_SpiritPool.insert(spr);
    
    removeBall(spr);
    
    return true;
}

bool TSLayer::random3Ball()
{
    for (int i = 0; i < 3; i++) {
        if(!randomBall())
        {
            return false;
        }
    }
    return true;
}










