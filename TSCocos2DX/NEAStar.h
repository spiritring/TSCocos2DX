#ifndef INCLUDE_NEASTAR_H
#define INCLUDE_NEASTAR_H

#include "TSPoint.h"
#include <math.h>
#include <list>
#include <iostream>
#include <stdlib.h> 
using namespace std;

class TSNode;
class TSMap;
class NEAStar;

const static int direction[8][2] = {
	{-1,0},
	{1,0},
	{0,-1},
	{0,1},
	{-1,1},
	{1,1},
	{1,-1},
	{-1,-1},
};

char tTSMap[] =
{
	0,0,0,0,0,0,0,0,0,
	0,0,1,1,1,1,0,0,0,
	0,0,1,0,0,1,0,0,0,
	1,1,1,0,0,1,0,0,0,
	0,0,0,0,0,1,0,0,0,
	0,0,0,1,0,1,1,1,0,
	0,0,1,1,0,0,0,0,0,
	0,0,1,0,0,0,0,0,1,
    0,0,1,0,0,0,0,0,1,
};


class TSMap {
public:
	TSMap() {
		m_width = 9;
		m_height = 9;
		m_TSMap = tTSMap;
		m_TSMapclose = new char[m_width*m_height];
		memset(m_TSMapclose,0,sizeof(char)*m_width*m_height);
	}
	~TSMap() {
        delete m_TSMapclose;
	}

	char* m_TSMap;
	char* m_TSMapclose;
	int m_width;
	int m_height;
};



class TSNode {
public:
	TSNode() {
		pFather = 0;
		nScore = 0;
	}

	TSNode(TSPoint& pos, TSPoint& start, TSPoint& end, TSNode* father) {
		pPos = pos;

		int x = pos.m_x - start.m_x;
		int y = pos.m_y - start.m_y;

		int x2 = pos.m_x - end.m_x;
		int y2 = pos.m_y - end.m_y;

		nScore = (x*x + y*y) + (x2*x2 + y2*y2);

		pFather = father;
	}

public:
	TSPoint pPos;
	unsigned long nScore;
	TSNode* pFather;
};



class NEAStar {
public:
	NEAStar()
    {
        
	}
    
    ~NEAStar()
    {
        for (std::list<TSNode*>::iterator iter = releasePool.begin(); iter != releasePool.end();
             iter++) {
            delete *iter;
        }
    }

	inline bool isCloseList(TSPoint& pos)
	{
		if (pTSMap->m_TSMapclose[pos.m_x*pTSMap->m_width+pos.m_y] == 1){
			return false;
		}
		return true; 
	}

	inline bool isTSMapBlock(TSPoint& pos)
	{
		if (pTSMap->m_TSMap[pos.m_x*pTSMap->m_width+pos.m_y] == 1){
			return false;
		}
		return true;
	}

	void run() {
		TSNode* note = new TSNode(pStart,pStart,pEnd,NULL);
        releasePool.push_back(note);
        
        memset(pTSMap->m_TSMapclose, 0, sizeof(char) * pTSMap->m_width * pTSMap->m_height);

		while (true)
		{
			if (pCurrentPos == pEnd) {
				break;
			}

			pCurrentPos = note->pPos;
			openList.remove(note);
			closeList.push_back(note);
            
            if (note->pPos.m_x*pTSMap->m_width+note->pPos.m_y > pTSMap->m_width * pTSMap->m_height)
            {
                printf("FFFF");
            }
            
			pTSMap->m_TSMapclose[note->pPos.m_x*pTSMap->m_width+note->pPos.m_y] = 1;

			for (int i = 0 ; i < 4 ; i++)
			{
				TSPoint _Pos;
				_Pos.m_x = pCurrentPos.m_x + direction[i][0];
				_Pos.m_y = pCurrentPos.m_y + direction[i][1];

				if (_Pos.m_x >= 0 && _Pos.m_x < pTSMap->m_height &&
					_Pos.m_y >= 0 && _Pos.m_y < pTSMap->m_width)
				{
					if (isTSMapBlock(_Pos) && isCloseList(_Pos))
					{
                        TSNode* _node = new TSNode(_Pos,pStart,pEnd,note);
                        releasePool.push_back(_node);
                        
						openList.push_back(_node);
						pTSMap->m_TSMapclose[_Pos.m_x*pTSMap->m_width+_Pos.m_y] = 1;
					}
				}
			}

			if (openList.empty())
			{
				return;
			}

			TSNode* minTSNode = openList.front();
			for (list<TSNode*>::iterator iter = openList.begin(); iter != openList.end() ; iter++)
			{
				TSNode* _note = (*iter);
				if (_note->nScore < minTSNode->nScore)
				{
					minTSNode = _note;
				}
			}

			note = minTSNode;
		}
	}

	void Init(TSPoint& start,TSPoint& end,TSMap* TSMap) {
		pStart = start;
		pCurrentPos = start;
		pEnd = end;
		pTSMap = TSMap;
        
        for (std::list<TSNode*>::iterator iter = releasePool.begin(); iter != releasePool.end();
             iter++) {
            delete *iter;
        }
        releasePool.clear();
        
        openList.clear();
        closeList.clear();
	}

	TSNode* getResult()
	{
		return closeList.back();
	}

public:
	TSPoint pCurrentPos;
	TSPoint pStart;
	TSPoint pEnd;
	TSMap* pTSMap;

	list<TSNode*> openList;
	list<TSNode*> closeList;
    
    list<TSNode*> releasePool;
};

#endif // INCLUDE_NEASTAR_H