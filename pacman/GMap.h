#pragma once
#include <list>
#define MAPLENTH 19	//�߼���ͼ��С
#define P_ROW 10	//�ҷ���λ������
#define P_ARRAY 9	//�ҷ���λ������
#define E_ROW 8		//�з���λ������
#define E_ARRAY	9	//�з���λ������

class GMap
{
protected:
	static int LD;	//�ϰ���ߴ�
	static int PD;	//���Ӱ뾶
	void InitOP();	//����˫������λ��û�ж��ӳ���
	bool mapData[MAPLENTH][MAPLENTH];	//�ϰ����߼���ͼ����
	bool peaMapData[MAPLENTH][MAPLENTH];	//�����߼���ͼ����
	COLORREF color;	//��ͼ��ǽ����ɫ
public:
	GMap()
	{

	}
	virtual ~GMap();
	void DrawMap(HDC &memDC);		//���Ƶ�ͼ
	void DrawPeas(HDC &hdc);	//���ƶ���
	friend class GObject;	//����������ʹ��ֱ�ߵ������յ���Ϣ����ײ���
	friend class PacMan;	//���������ʶ��ӵ�ͼ
};

