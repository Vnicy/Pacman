#pragma once
#include "GMap.h"
/*�ڶ��ص�ͼ���*/
class Stage_2 :
	public GMap
{
public:
	Stage_2();
	~Stage_2();
private:
	bool static initData[MAPLENTH][MAPLENTH];	//��ͼ����
};

