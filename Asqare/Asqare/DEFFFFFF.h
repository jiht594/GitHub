#ifndef MYDEFFFFF_2303
#define MYDEFFFFF_2303
class CBitmapButton;
class CNoFalshButton;

//这个是主要的数据结构
typedef struct tagMainData
{
	int iNum;//号码 == 10×i + j
	BOOL bCheck;//是否被选中
	int iBMPID1;//保存的图片， 正常状态。可以由CNoFalshButton LoadBitmaps显示
	int iBMPID2;//保存的图片，选中状态。可以由CNoFalshButton LoadBitmaps显示
	int i;//行 （0-9）
	int j;//列（0-9）
	int iPos[2];//按钮坐标
	BOOL bDispel;//是否已经被消除了
	CNoFalshButton/*CBitmapButton*/ *pBMPButton;//对应的按钮
}MainData, *pMainData;

typedef struct tagSave//保存游戏数据，没实现
{
}Save, *pSave;
#endif
