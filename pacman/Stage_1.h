#pragma once
#include "GMap.h"
/*��һ�ص�ͼ���*/
class Stage_1 :
	public GMap
{
public:
	Stage_1();
	~Stage_1();
private:
	bool static initData[MAPLENTH][MAPLENTH];	//��ͼ����
};

