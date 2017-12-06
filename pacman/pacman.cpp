// pacman.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "pacman.h"
#include  "GMap.h"
#include "GObject.h"
#include "Stage_1.h"
#include "Stage_2.h"
#include "Stage_3.h"

using namespace std;
#define WLENTH 700	//��
#define WHIGHT 740	//��
#define STAGE_COUNT 3	//һ��3��
static HWND g_hwnd;	//��Ϸ���ھ��

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: �ڴ˷��ô��롣

    // ��ʼ��ȫ���ַ���
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ִ��Ӧ�ó����ʼ��: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));
	// ��ǰ�Ĺؿ�
	int s_n = 0; // [0, 1, 2]
				 // ��ͼ
	GMap *MapArray[STAGE_COUNT] = { new Stage_1(), new Stage_2(), new Stage_3() };
	// ��Ҷ���
	// �Լ�
	auto g_me = std::make_shared<PacManPlayer>(P_ROW, P_ARRAY);
	// �趨�ĸ����˶���
	auto e1 = std::make_shared<RedOne>(E_ROW, E_ARRAY);			// ��ɫ�о�����
	auto e2 = std::make_shared<RedOne>(E_ROW, E_ARRAY);			// ��ɫ�о�����
	auto e3 = std::make_shared<BlueOne>(E_ROW, E_ARRAY);			// ��ɫ�о�����
	auto e4 = std::make_shared<YellowOne>(E_ROW, E_ARRAY);			// ��ɫ�о�����
																	// �ؿ�
	GObject::pStage = MapArray[s_n]; 								// ��ʼ��Ϊ��һ�ص�ͼ
																	// �趨���
	EnermyPlayer::player = g_me;										// ��һ��ָ��ָ����Ҷ���

	MSG msg;

	DWORD dwLastTime = 0;
	// ����Ϣѭ��:
	// ���û�б�ץ,���ҹؿ�<3
	while (!g_me->IsOver() && s_n < STAGE_COUNT) {
		// �ж��Ƿ�Ӯ�ñ���
		if (g_me->IsWin()) {
			s_n++;												// �ƶ�����һ��
			// �����Լ��͵���λ��
			g_me->SetPosition(P_ROW, P_ARRAY);
			e1->SetPosition(E_ROW, E_ARRAY);					// ���õо�һ��λ��
			e2->SetPosition(E_ROW, E_ARRAY); 					// ���õо�����λ��
			e3->SetPosition(E_ROW, E_ARRAY); 					// ���õо�����λ��
			e4->SetPosition(E_ROW, E_ARRAY); 					// ���õо��ĵ�λ��
			// �ж��Ƿ������3��,������,�˳���Ϸ,���������һ��
			if (s_n < 3) {
				MessageBox(g_hwnd, _T("��ϲ����"), _T("�Զ�����ʾ"), MB_OK);
				GObject::pStage = MapArray[s_n];				// 
				RECT screenRect;
				screenRect.top = 0;
				screenRect.left = 0;
				screenRect.right = WLENTH;
				screenRect.bottom = WHIGHT;

				HDC hdc = GetDC(g_hwnd);						// ��ȡ�豸
				std::shared_ptr<HDC__> dc(hdc, [](HDC hdc) {	// ����ָ�룬�Զ�������Դ
					::ReleaseDC(g_hwnd, hdc);
				});
				::FillRect(dc.get(), &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
				GObject::pStage->DrawMap(hdc);					// ����ͼ
				continue; 									// ��������ѭ��
			}
			else {
				// ����ѭ��
				break;
			}
		}
		// ��ȡ��Ϣ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);							// ������Ϣ
			DispatchMessage(&msg);							// �ַ���Ϣ
		}
		// �ж�ʱ��,���򻭵�̫��
		if (GetTickCount() - dwLastTime >= 40) {
			dwLastTime = GetTickCount();						// ��ס���ε�ʱ��
		}
		else {
			continue; 										// ʱ�䲻��,���β����л滭
		}
		{
			HDC hdc = ::GetDC(g_hwnd); 					// ����豸
			std::shared_ptr<HDC__> dc(hdc, [](auto hdc) {	// ��ʹ��ʱ�Զ��ͷ�
				::ReleaseDC(g_hwnd, hdc);					// �ͷ��豸
			});
			MapArray[s_n]->DrawPeas(hdc);					// ������
			MapArray[s_n]->DrawMap(hdc);					// ����ͼ

															// �����˼��Զ��˶�
			{
				e1->action();								// �о�һ����Ϊ����
				e1->DrawBlank(hdc);						// ���о�һ�Ŀհ�
				e1->Draw(hdc);							// ���о�һ�����岿��

				e2->action();								// �о�һ����Ϊ����
				e2->DrawBlank(hdc); 						// ���о�һ�Ŀհ�
				e2->Draw(hdc); 							// ���о�һ�����岿��

				e3->action();								// �о�һ����Ϊ����
				e3->DrawBlank(hdc); 						// ���о�һ�Ŀհ�
				e3->Draw(hdc); 							// ���о�һ�����岿��

				e4->action();								// �о�һ����Ϊ����
				e4->DrawBlank(hdc); 						// ���о�һ�Ŀհ�
				e4->Draw(hdc); 							// ���о�һ�����岿��
			}

			{
				// ���Լ�
				g_me->DrawBlank(hdc);
				g_me->Draw(hdc);
				// �Լ���ǰ�ƶ�
				g_me->action();

				// ��ȡ���� : �����Լ��ķ���
				if (GetAsyncKeyState(VK_DOWN) & 0x8000) {	// ��⵽�·��������
					g_me->SetTwCommand(DOWN);				// ������һ�����ƶ�����Ϊ����
				}
				if (GetAsyncKeyState(VK_LEFT) & 0x8000) {	// ��⵽���������
					g_me->SetTwCommand(LEFT); 				// ������һ�����ƶ�����Ϊ����
				}
				if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {	// ��⵽�ҷ��������
					g_me->SetTwCommand(RIGHT); 			// ������һ�����ƶ�����Ϊ����
				}
				if (GetAsyncKeyState(VK_UP) & 0x8000) {		// ��⵽�Ϸ��������
					g_me->SetTwCommand(UP); 				// ������һ�����ƶ�����Ϊ����
				}
			}
		}
	}
	// �����Ϸ����
	if (g_me->IsOver()) {
		MessageBoxA(NULL, "��ʦδ��", "�Զ�����ʾ", MB_OK);
	}
	// ����,��ʾӮ����Ϸ
	else {
		MessageBoxA(NULL, "��ϲ��Ӯ����ʤ��\r\nȷ������Ϸ�˳�", "�Զ�����ʾ", MB_OK);
	}

	return (int)msg.wParam;
	/*
    // ����Ϣѭ��: 
	bool bRunning = true;
	while (bRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		/*HDC hdc = ::GetDC(g_hwnd);	//��ȡ�豸���
		
		//�������
		{
			SetPixel(hdc, rand() % WLENTH, rand() % WHIGHT,
				RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		//���߶β���
		{
			HPEN pen = CreatePen(PS_SOLID, 2, RGB(rand() % 256, rand() % 256, rand() % 256));
			//ѡ�񻭱�
			HPEN oldPen = (HPEN)SelectObject(hdc, pen);
			//���߶�
			MoveToEx(hdc, rand() % WLENTH, rand() % WHIGHT, NULL);
			LineTo(hdc, rand() % WLENTH, rand() % WHIGHT);
			//�ָ�����
			SelectObject(hdc, oldPen);
			::ReleaseDC(g_hwnd, hdc);
			//��ͣ1��
			Sleep(1);
		}
		//���ľ��β���
		{

			//��������
			HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			HPEN oldPen = (HPEN)SelectObject(hdc, pen);
			//�����Σ�����
			Rectangle(hdc, 100, 200, 300, 500);
			//�ָ�����
			SelectObject(hdc, oldPen);
			DeleteObject(pen);

		}
		//��������
		{
			HBRUSH bBrush = CreateSolidBrush(RGB(0, 0, 255));
			//������
			RECT rect;
			rect.left = 50;
			rect.top = 270;
			rect.right = 150;
			rect.bottom = 370;
			FillRect(hdc, &rect, bBrush);
			DeleteObject(bBrush);
		}
		//��Բ����
		{
			//��Բ�������ĸ����֣�����һ��������
			Ellipse(hdc, 200, 150, 300, 250);
			//����Բ
			Ellipse(hdc, 200, 270, 340, 370);
			//����Բ
			Ellipse(hdc, 100, 100, 200, 150);
		}
		//�����߲���
		{
			Arc(hdc, 100, 100, 200, 300	//�������ϵ�����µ�
				, 150, 200	//���
				, 100, 200	//�յ�
				);
		}
		// �ۺ�Ӧ��, ��һ���������
		{
			static DWORD dwTime = GetTickCount();
			// �������ϻ�ͼ��ʱ�����50����ʱ,�Ž��б��λ���
			if (GetTickCount() - dwTime >= 50) {
				dwTime = GetTickCount();
			}
			else {
				continue;
			}
			//ģ�⵱ǰ��֡,������һ��5֡,ÿһ֡����ͬ��ͼ��
			static int iFrame = 0;
			++iFrame;
			if (iFrame >= 5) {
				iFrame = 0;
			}

			// ������������λ��
			int x = 300, y = 300;

			// ����İ뾶
			int r = 100;

			// dc ������
			HDC hdc = ::GetDC(g_hwnd);
			shared_ptr<HDC__> dc(::GetDC(g_hwnd), [](HDC hdc) {
				::ReleaseDC(g_hwnd, hdc);
			});
			// ��ȡ���ڿͻ�����С
			RECT rc;
			GetClientRect(g_hwnd, &rc);
			// ������ˢ
			std::shared_ptr<HBRUSH__> br(
				::CreateSolidBrush(RGB(255, 255, 255)),
				[](HBRUSH hbr) {
				::DeleteObject(hbr);
			});
			// ������(�����һ֡��������
			FillRect(dc.get(), &rc, br.get());
			switch (iFrame) {
			case 0: {
				Ellipse(dc.get(), x - r, y - r, x + r, y + r); 	// ��һ��Բ
				MoveToEx(dc.get(), x - r, y, NULL); 			// ��һ������
				LineTo(dc.get(), x, y);
				break;
			}
			case 1: {
				// ����(������������ƫ�� PI/4
				int x0, y0; 									// ���Ͻǵĵ�
				int x1, y1; 									// ���½ǵĵ�
				x0 = x - static_cast<int>(r * sin(PI * 0.75f));
				y0 = y + static_cast<int>(r * cos(PI * 0.75f));

				x1 = x + static_cast<int>(r * sin(PI * 1.25f));
				y1 = y - static_cast<int>(r * cos(PI * 1.25f));

				SetPixel(dc.get(), x0, y0, RGB(255, 0, 0));
				SetPixel(dc.get(), x1, y1, RGB(0, 255, 0));
				SetPixel(dc.get(), x, y, RGB(0, 0, 0));
				Arc(dc.get(), x - r, y - r, x + r, y + r 		// ��һ����Բ + һ������
					, x1, y1
					, x0, y0);

				MoveToEx(dc.get(), x0, y0, NULL); 				// ������
				LineTo(dc.get(), x, y);

				MoveToEx(dc.get(), x1, y1, NULL);
				LineTo(dc.get(), x, y);
				break;

			}
			case 2: {
				Arc(dc.get(), x - r, y - r, x + r, y + r 		// ��һ����Բ + һ������
					, x, y + r
					, x, y - r
				);
				// ������
				MoveToEx(dc.get(), x, y - r, NULL); 			// ��Բ������ĵ㿪ʼ
				LineTo(dc.get(), x, y + r); 					// ��Բ������ĵ����
				break;
			}
			case 3: {
				// ����(������������ƫ�� PI/4
				int x0, y0; 									// ���Ͻǵĵ�
				int x1, y1; 									// ���½ǵĵ�
				x0 = x - static_cast<int>(r * sin(PI * 0.75f));
				y0 = y + static_cast<int>(r * cos(PI * 0.75f));

				x1 = x + static_cast<int>(r * sin(PI * 1.25f));
				y1 = y - static_cast<int>(r * cos(PI * 1.25f));

				SetPixel(dc.get(), x0, y0, RGB(255, 0, 0));
				SetPixel(dc.get(), x1, y1, RGB(0, 255, 0));
				SetPixel(dc.get(), x, y, RGB(0, 0, 0));
				// ��һ����Բ + һ������
				Arc(dc.get(), x - r, y - r, x + r, y + r
					, x1, y1
					, x0, y0);

				// ������
				MoveToEx(dc.get(), x0, y0, NULL);
				LineTo(dc.get(), x, y);

				MoveToEx(dc.get(), x1, y1, NULL);
				LineTo(dc.get(), x, y);
				break;

			}
			case 4: {
				// ��һ��Բ
				Ellipse(dc.get(), x - r, y - r, x + r, y + r);
				// ��һ������
				MoveToEx(dc.get(), x - r, y, NULL);
				LineTo(dc.get(), x, y);
				break;
			}
			default:
				break;
			}
			Sleep(1);

		}
	}
    return (int) msg.wParam;*/
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PACMAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 
	  0,
	  WLENTH, 
	  WHIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   //������Ϸ����
   g_hwnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �����˵�ѡ��: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
