#ifndef MYDEFFFFF_2303
#define MYDEFFFFF_2303
class CBitmapButton;
class CNoFalshButton;

//�������Ҫ�����ݽṹ
typedef struct tagMainData
{
	int iNum;//���� == 10��i + j
	BOOL bCheck;//�Ƿ�ѡ��
	int iBMPID1;//�����ͼƬ�� ����״̬��������CNoFalshButton LoadBitmaps��ʾ
	int iBMPID2;//�����ͼƬ��ѡ��״̬��������CNoFalshButton LoadBitmaps��ʾ
	int i;//�� ��0-9��
	int j;//�У�0-9��
	int iPos[2];//��ť����
	BOOL bDispel;//�Ƿ��Ѿ���������
	CNoFalshButton/*CBitmapButton*/ *pBMPButton;//��Ӧ�İ�ť
}MainData, *pMainData;

typedef struct tagSave//������Ϸ���ݣ�ûʵ��
{
}Save, *pSave;
#endif
