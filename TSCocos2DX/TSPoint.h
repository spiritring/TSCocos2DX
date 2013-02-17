#ifndef INCLUDE_TSPoint_H
#define INCLUDE_TSPoint_H

class TSPoint
{
public:
	TSPoint();
	TSPoint(int x, int y);
	~TSPoint(){}

	bool operator==(TSPoint& p){
		if (m_x == p.m_x && m_y == p.m_y){
			return true;
		}
		return false;
	}

	void operator=(TSPoint& p){
		m_x = p.m_x;
		m_y = p.m_y;
	}

	int m_x;
	int m_y;
};

TSPoint::TSPoint() : m_x(0),m_y(0){}

TSPoint::TSPoint(int x, int y){
	m_x = x;
	m_y = y;
}

#endif // INCLUDE_TSPoint_H
