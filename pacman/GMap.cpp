#include "stdafx.h"
#include "GMap.h"

int GMap::LD = 36;	//ǽ���
int GMap::PD = 3;	//���Ӱ뾶


void GMap::InitOP()
{
	peaMapData[E_ROW][E_ARRAY] = false;	//�з�λ��û�ж��ӳ���
	peaMapData[P_ROW][P_ARRAY] = false;		//���λ��û�ж��ӳ���
}

GMap::~GMap()
{
}
/*����mapdata�洢��ǽ�����ݣ��������飬������ǽ��ʱ����һ������ģ��ǽ��*/
void GMap::DrawMap(HDC &memDC)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	for (int i = 0; i < MAPLENTH; i++) {
		for (int j = 0; j < MAPLENTH; j++) {
			//����ǽ��
			if (!mapData[i][j]) {
				RECT rect;
				rect.left = j * LD;
				rect.top = i * LD;
				rect.right = (j + 1) * LD;
				rect.bottom = (i + 1) * LD;
				FillRect(memDC, &rect, hBrush);	// ����������ģ��ǽ��
			}
		}
	}
	DeleteObject(hBrush);							// ɾ����ˢ����
}

void GMap::DrawPeas(HDC & hdc)	// �����Ӻ���
{
	for (int i = 0; i < MAPLENTH; i++) {				// ������������
		for (int j = 0; j < MAPLENTH; j++) {
			if (peaMapData[i][j]) {					// ����ô��ж���

				Ellipse(hdc, (LD / 2 - PD) + j * LD,	// ��Բ��ģ�ⶹ��
					(LD / 2 - PD) + i * LD,
					(LD / 2 + PD) + j * LD,
					(LD / 2 + PD) + i * LD);
			}
		}
	}
	// �������B��ֱ�ӹ���
	if (GetAsyncKeyState('B') & 0x8000) {
		MessageBoxA(NULL, "������������������", "", MB_OK);
		for (int i = 0; i < MAPLENTH; i++) {
			for (int j = 0; j < MAPLENTH; j++) {
				peaMapData[i][j] = false;
			}
		}
	}
}
