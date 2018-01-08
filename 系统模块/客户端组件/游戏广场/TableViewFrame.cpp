#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "GlobalUnits.h"
#include "TableViewFrame.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//横行格式
#define	NT_LEFT						0x01								//左对齐
#define	NT_RIGHT					0x02								//右对齐

//纵向格式
#define	NT_TOP						0x10								//上对齐
#define	NT_BOTTOM					0x20								//下对齐

//滚动定义
#define WHELL_TIMES					2									//滚轮倍数
#define SCROLL_POS_LINE				10									//滚动像素

//定时器定义
#define IDI_TICK_TIMER				1									//常规定时
#define IDI_UPDATE_TIME				100									//更新时间
#define IDI_WAIT_ANIMATION			101									//等待动画

//时间定义
#define TIME_UPDATE_TIME			1000								//更新间隔
#define TIME_WAIT_ANIMATION			120									//动画时间

//位置模式
#define MATCH_PM_ABSOLUTE			0									//绝对位置
#define MATCH_PM_RELATIVE			1									//相对位置 

//////////////////////////////////////////////////////////////////////////////////
//静态声明
CTableResource * CTableResource::m_pTableResource=NULL;

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTableViewFrame, CWnd)

	//系统消息
	ON_WM_SIZE()	
	ON_WM_PAINT()	
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CMatchViewFrame, CFGuiWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
	
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableResource::CTableResource()
{
	//属性变量
	m_bShowUser=true;
	m_bShowChair=true;
	m_bShowTable=true;
	m_bShowFocus=true;
	m_bShowEnter=false;
	m_bCustomUser=false;
	m_bShowTableID=true;
	m_bRenderImage=true;
	m_bShowCellScore=false;

	//数目定义
	m_wTableItemCount=0;
	m_wChairItemCount=0;

	//类型数目
	m_wUserGenreCount=0;
	m_wTableGenreCount=0;
	m_wChairGenreCount=0;

	//动画变量
	m_nAnimationFrame=0;
	m_ptAnimation.SetPoint(0,0);

	//颜色属性
	m_crName=RGB(0,0,0);
	m_crMember=RGB(0,0,0);
	m_crMaster=RGB(0,0,0);

	//桌子属性
	m_SizeChair.SetSize(0,0);
	m_SizeTable.SetSize(0,0);
	m_SizeEnter.SetSize(0,0);

	//位置变量
	m_ptLock.SetPoint(0,0);
	m_ptEnter.SetPoint(0,0);
	m_ptTableID.SetPoint(0,0);

	//位置变量
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		m_ptReadyArray[i].SetPoint(0,0);
		m_rcNameArray[i].SetRect(0,0,0,0);
		m_rcChairArray[i].SetRect(0,0,0,0);
	}

	//辅助变量
	ZeroMemory(m_nDrawStyle,sizeof(m_nDrawStyle));
	ZeroMemory(m_szDirectory,sizeof(m_szDirectory));

	//加载资源
	m_ImageReady.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_READY"));
	m_ImageLocker.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_LOCKER"));
	m_ImageNumberBK.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_NUMBER_BK"));
	m_ImageNumberNB.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_NUMBER_NB"));

	//设置变量
	m_SizeNumberBK.SetSize(m_ImageNumberBK.GetWidth(),m_ImageNumberBK.GetHeight());
	m_SizeNumberNB.SetSize(m_ImageNumberNB.GetWidth()/10L,m_ImageNumberNB.GetHeight());

	if(m_pTableResource==NULL) m_pTableResource=this;

	return;
}

//析构函数
CTableResource::~CTableResource()
{
	//释放资源
	DestroyResource();

	//设置变量
	if(m_pTableResource==this) m_pTableResource=NULL;

	return;
}

//释放资源
VOID CTableResource::DestroyResource()
{
	//桌子属性
	m_SizeChair.SetSize(0,0);
	m_SizeTable.SetSize(0,0);

	//数目定义
	m_wTableItemCount=0;
	m_wChairItemCount=0;

	//动画帧数
	m_nAnimationFrame=0;
	m_ptAnimation.SetPoint(0,0);

	//输出控制
	ZeroMemory(m_nDrawStyle,sizeof(m_nDrawStyle));

	//位置属性
	ZeroMemory(&m_ptLock,sizeof(m_ptLock));
	ZeroMemory(m_rcNameArray,sizeof(m_rcNameArray));
	ZeroMemory(m_rcChairArray,sizeof(m_rcChairArray));
	ZeroMemory(m_ptReadyArray,sizeof(m_ptReadyArray));

	//颜色属性
	m_crName=RGB(0,0,0);
	m_crMember=RGB(0,0,0);
	m_crMaster=RGB(0,0,0);

	//释放位图
	m_ImageChair.Destroy();
	m_ImageTable.Destroy();
	m_ImageGround.Destroy();
	//m_ImageUserFace.Destroy();
	m_ImageWaitDistribute.Destroy();	

	//释放位图
	m_ImageEnter.DestroyImage();
	m_ImageCustomUser.DestroyImage();
	m_ImageCellNumber.DestroyImage();		
	m_ImageAnimation.DestroyImage();

	return;
}

//加载资源
bool CTableResource::LoadResource(LPCTSTR pszResDirectory, WORD wChairCount, bool bAvertCheatMode)
{
	//效验参数
	ASSERT((pszResDirectory!=NULL)&&(wChairCount<=MAX_CHAIR));
	if ((pszResDirectory==NULL)||(wChairCount>MAX_CHAIR)) return false;

	//资源路径
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
	if(bAvertCheatMode)
	{
		_sntprintf(m_szDirectory,CountArray(m_szDirectory),TEXT("%s\\%s\\AvertCheatMode"),szDirectory,pszResDirectory);

		//检测目录
		WIN32_FIND_DATA  wFindData;		
		HANDLE hFind = FindFirstFile(m_szDirectory, &wFindData);
		if ((hFind == INVALID_HANDLE_VALUE) || (wFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
		{
			//防作弊路径
			_sntprintf(m_szDirectory,CountArray(m_szDirectory),TEXT("%s\\AvertCheatMode"),szDirectory);
		}
		FindClose(hFind);
	}
	else
	{
		_sntprintf(m_szDirectory,CountArray(m_szDirectory),TEXT("%s\\%s"),szDirectory,pszResDirectory);
	}

	//参数路径
	TCHAR szParameterFile[MAX_PATH]=TEXT("");
	_sntprintf(szParameterFile,CountArray(szParameterFile),TEXT("%s\\TableResource.INI"),m_szDirectory);

	//资源判断
	if(bAvertCheatMode)
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Table.png"),m_szDirectory);

		//加载资源
		CPngImage ImageTable;
		if (ImageTable.LoadImage(szImagePath)==false)
		{
			//资源路径
			ZeroMemory(m_szDirectory, sizeof(m_szDirectory));
			_sntprintf(m_szDirectory,CountArray(m_szDirectory),TEXT("%s\\AvertCheatMode"),szDirectory);

			//参数路径
			ZeroMemory(szParameterFile, sizeof(szParameterFile));
			_sntprintf(szParameterFile,CountArray(szParameterFile),TEXT("%s\\TableResource.INI"),m_szDirectory);
		}

		//构造路径
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\WaitDistribute.bmp"),m_szDirectory);

		//加载资源
		if(FAILED(m_ImageWaitDistribute.Load(szImagePath)))
		{
			return false;
		}

		//构造路径
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\AnimalDistribute.png"),m_szDirectory);

		//加载资源
		m_ImageAnimation.LoadImage(szImagePath);
	}

	//颜色变量
	ReadColor(m_crName,TEXT("Color"),TEXT("Color_Name"),szParameterFile);
	ReadColor(m_crMember,TEXT("Color"),TEXT("Color_Member"),szParameterFile);
	ReadColor(m_crMaster,TEXT("Color"),TEXT("Color_Master"),szParameterFile);

	//位置坐标
	ReadPoint(m_ptLock,TEXT("Position"),TEXT("Point_Lock"),szParameterFile);
	ReadPoint(m_ptEnter,TEXT("Position"),TEXT("Point_Enter"),szParameterFile);
	ReadPoint(m_ptTableID,TEXT("Position"),TEXT("Point_TableID"),szParameterFile);

	//显示属性
	m_bShowUser=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowUser"),1L,szParameterFile)==TRUE);
	m_bShowChair=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowChair"),1L,szParameterFile)==TRUE);
	m_bShowTable=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowTable"),1L,szParameterFile)==TRUE);
	m_bShowFocus=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowFocus"),1L,szParameterFile)==TRUE);
	m_bShowEnter=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowEnter"),0L,szParameterFile)==TRUE);
	m_bCustomUser=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("CustomUser"),0L,szParameterFile)==TRUE);
	m_bShowTableID=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowTableID"),1L,szParameterFile)==TRUE);
	m_bRenderImage=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("RenderImage"),1L,szParameterFile)==TRUE);
	m_bShowCellScore=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowCellScore"),0L,szParameterFile)==TRUE);

	//数目属性
	m_wTableItemCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("TableItemCount"),2L,szParameterFile);
	m_wChairItemCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("ChairItemCount"),wChairCount,szParameterFile);

	//类型数目
	m_wUserGenreCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("UserGenreCount"),1L,szParameterFile);
	m_wTableGenreCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("TableGenreCount"),1L,szParameterFile);
	m_wChairGenreCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("ChairGenreCount"),1L,szParameterFile);

	//动画配置
	ReadPoint(m_ptAnimation,TEXT("Animation"),TEXT("Point_DistributeAnimal"),szParameterFile);
	m_nAnimationFrame=GetPrivateProfileInt(TEXT("Animation"),TEXT("DistributeAnimalFrame"),0L,szParameterFile);		

	//更新资源
	if(UpdateResource(CSkinRenderManager::GetInstance())==false)
	{
		return false;
	}

	//进入尺寸
	if (m_ImageEnter.IsNull()==false)
	{
		m_SizeEnter.SetSize(m_ImageEnter.GetWidth()/5,m_ImageEnter.GetHeight());
	}

	//椅子尺寸
	if (m_ImageChair.IsNull()==false)
	{
		m_SizeChair.SetSize(m_ImageChair.GetWidth()/m_wChairItemCount,m_ImageChair.GetHeight()/m_wChairGenreCount);
	}

	//桌子尺寸
	if (m_ImageTable.IsNull()==false)
	{
		m_SizeTable.SetSize(m_ImageTable.GetWidth()/m_wTableItemCount,m_ImageTable.GetHeight()/m_wTableGenreCount);
	}

	//用户尺寸
	if (m_ImageCustomUser.IsNull()==false)
	{
		m_SizeChair.SetSize(m_ImageCustomUser.GetWidth()/m_wChairItemCount,m_ImageCustomUser.GetHeight()/m_wUserGenreCount);
	}

	//位置坐标
	if (m_bShowTable==true)
	{
		for (WORD i=0;i<wChairCount;i++)
		{
			//变量定义
			TCHAR szSubItem[64]=TEXT("");

			//用户属性
			if (m_bShowUser==true)
			{
				//名字位置
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("Rect_Name%d"),i+1);
				ReadRect(m_rcNameArray[i],TEXT("Position"),szSubItem,szParameterFile);

				//准备位置
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("Point_Ready%d"),i+1);
				ReadPoint(m_ptReadyArray[i],TEXT("Position"),szSubItem,szParameterFile);

				//控制输出
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("DrawStyle%d"),i+1);
				m_nDrawStyle[i]=GetPrivateProfileInt(TEXT("Position"),szSubItem,0L,szParameterFile);

				//椅子位置
				POINT ChairPoint;
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("Point_Chair%d"),i+1);
				ReadPoint(ChairPoint,TEXT("Position"),szSubItem,szParameterFile);

				//椅子框架
				m_rcChairArray[i].top=ChairPoint.y;
				m_rcChairArray[i].left=ChairPoint.x;
				m_rcChairArray[i].right=m_rcChairArray[i].left+m_SizeChair.cx;
				m_rcChairArray[i].bottom=m_rcChairArray[i].top+m_SizeChair.cy;
			}
		}
	}

	return true;
}

//更新资源
bool CTableResource::UpdateResource(CSkinRenderManager * pSkinRenderManager)
{
	//构造路径
	TCHAR szImageGround[MAX_PATH]=TEXT("");
	_sntprintf(szImageGround,CountArray(szImageGround),TEXT("%s\\Ground.bmp"),m_szDirectory);

	//加载背景
	if (m_ImageGround.IsNull()==false)
	{
		m_ImageGround.Destroy();
	}

	//加载资源
	if (FAILED(m_ImageGround.Load(szImageGround)))
	{
		m_ImageGround.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_GROUND);
	}

	//渲染资源
	if (m_bRenderImage==true)
	{
		pSkinRenderManager->RenderImage(m_ImageGround);
	}

	//进入位图
	if ((m_bShowEnter==true)&&(m_ImageEnter.IsNull()==true))
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Enter.png"),m_szDirectory);

		//加载资源
		if (m_ImageEnter.LoadImage(szImagePath)==false) return false;
	}

	//用户位图
	if ((m_bCustomUser==true)&&(m_ImageCustomUser.IsNull()==true))
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\CustomUser.png"),m_szDirectory);

		//加载资源
		if (m_ImageCustomUser.LoadImage(szImagePath)==false) return false;
	}

	//底注位图
	if ((m_bShowCellScore==true)&&(m_ImageCellNumber.IsNull()==true))
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\CellNumber.png"),m_szDirectory);

		//加载资源
		if (m_ImageCellNumber.LoadImage(szImagePath)==false)
		{
			//加载内部资源
			if (m_ImageCellNumber.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_CELL_NUMBER"))==false)
				return false;
		}
	}

	//构造桌子
	if (m_bShowTable==true)
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Table.png"),m_szDirectory);

		//加载资源
		CPngImage ImageTable;
		if (ImageTable.LoadImage(szImagePath)==false) return false;

		//创建资源
		if (m_ImageTable.IsNull()==true)
		{
			m_ImageTable.Create(ImageTable.GetWidth(),ImageTable.GetHeight(),32);
		}

		//填充背景
		CImageDC TableDC(m_ImageTable);
		CDC * pTableDC=CDC::FromHandle(TableDC);
		m_ImageGround.DrawImageTile(pTableDC,0,0,ImageTable.GetWidth(),ImageTable.GetHeight());

		//构造桌子
		ImageTable.DrawImage(pTableDC,0,0,ImageTable.GetWidth(),ImageTable.GetHeight(),0,0);
	}

	//创建椅子
	if ((m_bShowChair==true)&&(m_bShowTable==true))
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Chair.png"),m_szDirectory);

		//加载资源
		CPngImage ImageChair;
		if (ImageChair.LoadImage(szImagePath)==false) return false;

		//创建资源
		if (m_ImageChair.IsNull()==true)
		{
			m_ImageChair.Create(ImageChair.GetWidth(),ImageChair.GetHeight(),32);
		}

		//填充背景
		CImageDC ChairDC(m_ImageChair);
		CDC * pChairDC=CDC::FromHandle(ChairDC);
		m_ImageGround.DrawImageTile(pChairDC,0,0,ImageChair.GetWidth(),ImageChair.GetHeight());

		//构造椅子
		ImageChair.DrawImage(pChairDC,0,0,ImageChair.GetWidth(),ImageChair.GetHeight(),0,0);
	}

	//头像
	{
		//工作目录
		TCHAR szDirectory[MAX_PATH]=TEXT("");
		CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
		TCHAR szTargetPath[MAX_PATH]=TEXT("");
		for (int i =0;i<200;i++)
		{
			_sntprintf(szTargetPath,CountArray(szTargetPath),TEXT("%s\\Resource\\AVATAR_48_48\\Avatar%d.png"),szDirectory,i);
			m_ImgGroupUserFace[i].LoadImage(szTargetPath);
		}

		////加载资源
		//CPngImage ImageUserFace;
		//if (ImageUserFace.LoadImage(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("USER_FACE_IMAGE"))==false) return false;

		////创建资源
		//if (m_ImageUserFace.IsNull()==true)
		//{
		//	m_ImageUserFace.Create(ImageUserFace.GetWidth(),ImageUserFace.GetHeight(),32);
		//}

		////填充背景
		//CImageDC UserFaceDC(m_ImageUserFace);
		//CDC * pUserFaceDC=CDC::FromHandle(UserFaceDC);
		//m_ImageGround.DrawImageTile(pUserFaceDC,0,0,ImageUserFace.GetWidth(),ImageUserFace.GetHeight());

		////构造椅子
		//ImageUserFace.DrawImage(pUserFaceDC,0,0,ImageUserFace.GetWidth(),ImageUserFace.GetHeight(),0,0);
	}

	return true;
}

//读取数值
LONG CTableResource::ReadStringValue(LPCTSTR & pszSring)
{
	//效验参数
	ASSERT((pszSring!=NULL)&&(pszSring[0]!=0));
	if ((pszSring==NULL)||(pszSring[0]==0)) return 0L;

	//变量定义
	LONG lSymbol=1;
	
	//设置符号
	if(pszSring[0]=='-') lSymbol=-1;
	if(pszSring[0]==',' && pszSring[1]=='-') lSymbol=-1;

	//寻找开始
	while (((pszSring[0]>0)&&(pszSring[0]<TEXT('0')))||(pszSring[0]>TEXT('9'))) pszSring++;

	//读取数值
	LONG lValue=0L;
	while ((pszSring[0]>=TEXT('0'))&&(pszSring[0]<=TEXT('9')))
	{
		lValue=lValue*10L+pszSring[0]-TEXT('0');
		++pszSring;
	}

	return lSymbol*lValue;
}

//读取矩形
bool CTableResource::ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValueRect,sizeof(ValueRect));

	//读取字符
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),pszIniFile);
	if (szReadData[0]!=0)
	{
		//读取变量
		LPCTSTR pszString=szReadData;
		ValueRect.left=ReadStringValue(pszString);
		ValueRect.top=ReadStringValue(pszString);
		ValueRect.right=ReadStringValue(pszString);
		ValueRect.bottom=ReadStringValue(pszString);

		return true;
	}

	return false;
}

//读取坐标
bool CTableResource::ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValuePoint,sizeof(ValuePoint));

	//读取字符
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),pszIniFile);
	if (szReadData[0]!=0)
	{
		//读取变量
		LPCTSTR pszString=szReadData;
		ValuePoint.x=ReadStringValue(pszString);
		ValuePoint.y=ReadStringValue(pszString);

		return true;
	}

	return false;
}

//读取颜色
bool CTableResource::ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValueColor,sizeof(ValueColor));

	//读取字符
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),pszIniFile);
	if (szReadData[0]!=0)
	{
		//读取变量
		LPCTSTR pszString=szReadData;
		ValueColor=RGB(ReadStringValue(pszString),ReadStringValue(pszString),ReadStringValue(pszString));

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableView::CTableView()
{
	//组件接口
	m_pITableViewFrame=NULL;

	//状态变量
	m_bMouseDown=false;
	m_wHoverChairID=INVALID_CHAIR;
	ZeroMemory(&m_TableAttribute,sizeof(m_TableAttribute));
	m_lCellScore=0;

	return;
}

//析构函数
CTableView::~CTableView()
{
}

//接口查询
VOID * CTableView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableView,Guid,dwQueryVer);
	return NULL;
}


//空椅子数
WORD CTableView::GetNullChairCount(WORD & wNullChairID)
{
	//设置变量
	wNullChairID=INVALID_CHAIR;

	//寻找位置
	WORD wNullCount=0;
	for (WORD i=0;i<m_TableAttribute.wChairCount;i++)
	{
		if (m_TableAttribute.pIClientUserItem[i]==NULL)
		{
			//设置数目
			wNullCount++;

			//设置结果
			if (wNullChairID==INVALID_CHAIR) wNullChairID=i;
		}
	}

	return wNullCount;
}

//配置函数
VOID CTableView::InitTableView(WORD wTableID, WORD wChairCount,ITableViewFrame * pITableViewFrame)
{
	//设置属性
	m_TableAttribute.wTableID=wTableID;
	m_TableAttribute.wChairCount=wChairCount;

	//设置接口
	m_pITableViewFrame=QUERY_OBJECT_PTR_INTERFACE(pITableViewFrame,ITableViewFrame);

	return;
}

//获取用户
IClientUserItem * CTableView::GetClientUserItem(WORD wChairID)
{
	//效验参数
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return NULL;

	//获取用户
	return m_TableAttribute.pIClientUserItem[wChairID];
}

//设置信息
bool CTableView::SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return false;

	//设置用户
	m_TableAttribute.pIClientUserItem[wChairID]=pIClientUserItem;

	//更新界面
	m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);

	return true;
}

//鼠标按下
VOID CTableView::SetMouseDown(bool bMouseDown)
{
	//设置标志
	if (m_bMouseDown!=bMouseDown)
	{
		//设置变量
		m_bMouseDown=bMouseDown; 

		//更新界面
		m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);
	}

	return;
}

//焦点框架
VOID CTableView::SetFocusFrame(bool bFocusFrame)
{
	//设置标志
	if (m_TableAttribute.bFocusFrame!=bFocusFrame)
	{
		//设置变量
		m_TableAttribute.bFocusFrame=bFocusFrame;

		//更新界面
		m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);
	}

	return;
}

//盘旋位置
VOID CTableView::SetHoverStatus(WORD wHoverChairID)
{
	//设置标志
	if (m_wHoverChairID!=wHoverChairID)
	{
		//设置变量
		m_wHoverChairID=wHoverChairID;

		//更新界面
		m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);
	}

	return;
}

//桌子状态 
VOID CTableView::SetTableStatus(bool bPlaying, bool bLocker,LONG lCellScore)
{
	//设置标志
	if ((m_TableAttribute.bLocker!=bLocker)||(m_TableAttribute.bPlaying!=bPlaying))
	{
		//设置变量
		m_TableAttribute.bLocker=bLocker; 
		m_TableAttribute.bPlaying=bPlaying;		
	}
	m_lCellScore=lCellScore;
	//更新界面
	m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);

	return;
}

//绘画函数
VOID CTableView::DrawTableView(CDC * pDC, DWORD dwServerRule, CTableResource * pTableResource)
{
	//椅子大小
	CSize SizeChair=pTableResource->m_SizeChair;
	CSize SizeTable=pTableResource->m_SizeTable;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bMySelfTable=false;
	bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(dwServerRule);

	TCHAR szScore[32]=TEXT("");
	_sntprintf(szScore,CountArray(szScore), TEXT("%ld"),m_lCellScore);

	//绘画桌子
	if ((bAvertCheatMode==false)||(m_TableAttribute.wTableID==0))
	{
		INT nXPosTable=(m_TableAttribute.bPlaying==true && pTableResource->m_wTableItemCount>1)?SizeTable.cx:0;
		pTableResource->m_ImageTable.BitBlt(pDC->m_hDC,0,0,SizeTable.cx,SizeTable.cy,nXPosTable,0);
	}
	else
	{
		INT nXPosTable=SizeTable.cx;
		pTableResource->m_ImageTable.BitBlt(pDC->m_hDC,0,0,SizeTable.cx,SizeTable.cy,nXPosTable,0);
	}

	//绘画进入
	if ((pTableResource->m_bShowEnter==true)&&(pTableResource->m_ImageEnter.IsNull()==false))
	{
		//计算位置
		INT nImageIndex=0;
		if (m_TableAttribute.bPlaying==true) nImageIndex=1;
		if ((m_wHoverChairID==INDEX_ENTER_CHAIR)&&(m_TableAttribute.bPlaying==false)&&(m_bMouseDown==true)) nImageIndex=1;
		if ((m_wHoverChairID==INDEX_ENTER_CHAIR)&&(m_TableAttribute.bPlaying==false)&&(m_bMouseDown==false)) nImageIndex=2;

		//绘画标志
		CSize SizeEnter=pTableResource->m_SizeEnter;
		pTableResource->m_ImageEnter.DrawImage(pDC,SizeTable.cx/2+pTableResource->m_ptEnter.x,SizeTable.cy/2+pTableResource->m_ptEnter.y,SizeEnter.cx,SizeEnter.cy,SizeEnter.cx*nImageIndex,0);
	}

	//桌面信息
	if(m_TableAttribute.wChairCount < MAX_CHAIR)
	{
		for (WORD i=0;i<m_TableAttribute.wChairCount;i++)
		{
			//变量定义
			CRect rcChair=pTableResource->m_rcChairArray[i];

			//绘画椅子
			if ((bAvertCheatMode==true)&&(pTableResource->m_bShowChair==true)&&(m_TableAttribute.wTableID==0))
			{
				ASSERT(pTableResource->m_ImageChair.IsNull()==false);
				pTableResource->m_ImageChair.BitBlt(pDC->m_hDC,rcChair.left,rcChair.top,SizeChair.cx,SizeChair.cy,SizeChair.cx*i,0);
			}

			//绘画椅子
			if ((bAvertCheatMode==false)&&(pTableResource->m_bShowChair==true)&&(m_TableAttribute.pIClientUserItem[i]==NULL))
			{
				ASSERT(pTableResource->m_ImageChair.IsNull()==false);
				pTableResource->m_ImageChair.BitBlt(pDC->m_hDC,rcChair.left,rcChair.top,SizeChair.cx,SizeChair.cy,SizeChair.cx*i,0);
			}

			//绘画用户
			if (pTableResource->m_bShowUser==true)
			{
				//绘画用户
				if ((bAvertCheatMode==false)&&(m_TableAttribute.pIClientUserItem[i]!=NULL))
				{
					//获取用户
					IClientUserItem * pIClientUserItem=m_TableAttribute.pIClientUserItem[i];
					tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

					//自己判断
					if (m_TableAttribute.pIClientUserItem[i]->GetUserID()==pGlobalUserData->dwUserID)
					{
						bMySelfTable=true;
					}

					//绘画头像
					if (pTableResource->m_bCustomUser==false)
					{
						//计算位置
						INT nXExcursion=(rcChair.Width()-FACE_CX)/2;
						INT nYExcursion=(rcChair.Height()-FACE_CY)/2;

						//绘画头像
						DrawUserAvatar(pDC,rcChair.left+nXExcursion,rcChair.top+nYExcursion,pIClientUserItem,pTableResource);
					}
					else
					{
						//自定头像
						INT nXImagePos=i*rcChair.Width();
						INT nYImagePos=(m_TableAttribute.pIClientUserItem[i]->GetGender()==GENDER_FEMALE)?rcChair.Height():0;
						pTableResource->m_ImageCustomUser.DrawImage(pDC,rcChair.left,rcChair.top,rcChair.Width(),rcChair.Height(),nXImagePos,nYImagePos);
					}

					//同位判断
					WORD wTableID=pIClientUserItem->GetTableID();
					WORD wChairID=pIClientUserItem->GetChairID();
					bool bSameStation=(wTableID==m_TableAttribute.wTableID)&&(wChairID==i);

					//同意标志
					if ((bSameStation==true)&&(m_TableAttribute.bPlaying==false)&&(pIClientUserItem->GetUserStatus()==US_READY))
					{
						INT nXPos=pTableResource->m_ptReadyArray[i].x;
						INT nYPos=pTableResource->m_ptReadyArray[i].y;
						pTableResource->m_ImageReady.DrawImage(pDC,nXPos,nYPos);
					}

					//用户名字
					DrawTableName(pDC,i,pIClientUserItem->GetNickName(),pTableResource);
				}

				//隐藏模式
				if ((bAvertCheatMode==true)&&(pTableResource->m_bShowUser==true))
				{
					//用户头像
					if (m_TableAttribute.wTableID>0)
					{
						//计算位置
						INT nXExcursion=(rcChair.Width()-FACE_CX)/2;
						INT nYExcursion=(rcChair.Height()-FACE_CY)/2;

						//绘画头像
						if (pTableResource->m_bCustomUser==false)
						{
							//系统头像
							IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
							pIFaceItemControl->DrawFaceNormal(pDC,rcChair.left+nXExcursion,rcChair.top+nYExcursion,((m_TableAttribute.wTableID*m_TableAttribute.wChairCount)+i)%pIFaceItemControl->GetFaceCount());
						}
						else
						{
							//自定头像
							INT nXImagePos=i*rcChair.Width();
							INT nYImagePos=(((m_TableAttribute.wTableID*m_TableAttribute.wChairCount)+i)%2==GENDER_FEMALE)?rcChair.Height():0;
							pTableResource->m_ImageCustomUser.DrawImage(pDC,rcChair.left,rcChair.top,rcChair.Width(),rcChair.Height(),nXImagePos,nYImagePos);
						}
					}

					//用户名字
					WORD wTableID=m_TableAttribute.wTableID;
					DrawTableName(pDC,i,(wTableID==0)?TEXT("点击加入"):TEXT("分配位置"),pTableResource);
				}

				//绘画边框
				if ((bAvertCheatMode==false)||(m_TableAttribute.wTableID==0))
				{
					//自定边框
					if ((pTableResource->m_bCustomUser==true)&&(m_wHoverChairID==i))
					{
						pDC->Draw3dRect(rcChair.left,rcChair.top,rcChair.Width(),rcChair.Height(),RGB(125,125,125),RGB(125,125,125));
					}

					//系统边框
					if ((pTableResource->m_bCustomUser==false)&&((m_wHoverChairID==i)||(m_TableAttribute.pIClientUserItem[i]!=NULL)))
					{
						//计算位置
						INT nXExcursion=(rcChair.Width()-FACE_CX)/2;
						INT nYExcursion=(rcChair.Height()-FACE_CY)/2;

						//计算类型
						BYTE cbFrameKind=ITEM_FRAME_NORMAL;
						if ((m_wHoverChairID==i)&&(m_bMouseDown==false)) cbFrameKind=ITEM_FRAME_HOVER;
						if ((m_wHoverChairID==i)&&(m_bMouseDown==true)) cbFrameKind=ITEM_FRAME_HITDOWN;

						//绘画框架
						IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
						pIFaceItemControl->DrawFaceItemFrame(pDC,rcChair.left+nXExcursion,rcChair.top+nYExcursion,cbFrameKind);
					}
				}
			}
		}

	}

	//绘画锁图
	if (m_TableAttribute.bLocker==true)
	{
		INT nXPos=pTableResource->m_ptLock.x;
		INT nYPos=pTableResource->m_ptLock.y;
		pTableResource->m_ImageLocker.DrawImage(pDC,nXPos,nYPos);
	}

	//桌子号码
	if (pTableResource->m_bShowTableID==true)
	{
		//获取大小
		CSize SizeNumberBK=pTableResource->m_SizeNumberBK;
		CSize SizeNumberNB=pTableResource->m_SizeNumberNB;

		//变量定义
		WORD wItemCount=0;
		WORD wNumberTemp=m_TableAttribute.wTableID+1;
		WORD wTableNumber=m_TableAttribute.wTableID+1;

		//计算数目
		do
		{
			wItemCount++;
			wNumberTemp/=10;
		} while (wNumberTemp>0);

		//绘画背景
		INT nXNumberBK=pTableResource->m_ptTableID.x-SizeNumberBK.cx/2;
		INT nYNumberBK=pTableResource->m_ptTableID.y-SizeNumberBK.cy/2;
		pTableResource->m_ImageNumberBK.DrawImage(pDC,nXNumberBK,nYNumberBK);

		//位置定义
		INT nYNumberNB=pTableResource->m_ptTableID.y-SizeNumberNB.cy/2;
		INT nXNumberNB=pTableResource->m_ptTableID.x+(wItemCount*SizeNumberNB.cx)/2-SizeNumberNB.cx;

		//绘画桌号
		for (INT i=0;i<wItemCount;i++)
		{
			//绘画号码
			WORD wNumber=wTableNumber%10;
			pTableResource->m_ImageNumberNB.DrawImage(pDC,nXNumberNB,nYNumberNB,SizeNumberNB.cx,SizeNumberNB.cy,wNumber*SizeNumberNB.cx,0);

			//设置变量
			wTableNumber/=10;
			nXNumberNB-=SizeNumberNB.cx;
		};
	}

	//绘画焦点
	if ((pTableResource->m_bShowFocus==true)&&(pParameterGlobal->m_bSalienceTable==true)&&(bMySelfTable==true))
	{
		pDC->Draw3dRect(0,0,SizeTable.cx,SizeTable.cy,RGB(125,125,125),RGB(125,125,125));
	}

	//绘画底注
	if ((bAvertCheatMode==false)||(m_TableAttribute.wTableID==0))
	{
		if (m_lCellScore>0 && pTableResource->m_bShowCellScore==true)
		{
			DrawNumberString(pDC,pTableResource->m_ImageCellNumber,m_lCellScore,SizeTable.cx/2-5,SizeTable.cy/2,TA_CENTER);
		}
	}

	return;
}

//绘画名字
VOID CTableView::DrawTableName(CDC * pDC, WORD wChairID, LPCTSTR pszNickName, CTableResource * pTableResource)
{
	//用户名字
	CRect rcName=pTableResource->m_rcNameArray[wChairID];

	//绘图计算
	CRect rcDrawRect=rcName;
	INT nNameLength=lstrlen(pszNickName);
	INT nDrawStyte=pTableResource->m_nDrawStyle[wChairID];
	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS;
	INT nNameHeight=pDC->DrawText(pszNickName,nNameLength,&rcDrawRect,nDrawFormat|DT_CALCRECT);

	//横向位置
	if (nNameHeight<=12)
	{
		if (nDrawStyte&NT_LEFT) nDrawFormat|=DT_LEFT;
		else if (nDrawStyte&NT_RIGHT) nDrawFormat|=DT_RIGHT;
		else nDrawFormat|=(DT_CENTER|DT_SINGLELINE);
	}
	else nDrawFormat|=DT_LEFT;

	//竖向位置
	INT nYPos=0;
	if (nDrawStyte&NT_BOTTOM) nYPos=rcName.Height()-nNameHeight;
	else if (nDrawStyte&NT_TOP) nYPos=0;
	else nYPos=(rcName.Height()-nNameHeight)/2;

	//构造位置
	rcDrawRect=rcName;
	rcDrawRect.left=rcName.left;
	rcDrawRect.right=rcName.right;
	rcDrawRect.top=__max(rcName.top,rcName.top+nYPos);
	rcDrawRect.bottom=__min(rcName.bottom,rcDrawRect.top+nNameHeight);

	//用户名字
	pDC->SetTextColor(pTableResource->m_crName);
	pDC->DrawText(pszNickName,nNameLength,&rcDrawRect,nDrawFormat);

	return;
}

//绘画头像
VOID CTableView::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem, CTableResource * pTableResource)
{
	DrawUserAvatar(pDC, nXPos, nYPos, FACE_CX, FACE_CY, pIClientUserItem, pTableResource);

	return;
}

//绘画头像
VOID CTableView::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, IClientUserItem * pIClientUserItem, CTableResource * pTableResource)
{
	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//绘画头像
	if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
	{
		IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
		pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,pCustomFaceInfo->dwCustomFace);
	}
	else
	{
		//WIN 7 执行
		if(CD2DEngine::GetD2DEngine())
		{
			WORD wFaceID=pIClientUserItem->GetFaceID();
			pTableResource->m_ImgGroupUserFace[wFaceID].DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,0,0,FACE_CX,FACE_CY);
			////设置变量
			//WORD wLineCount=pTableResource->m_ImageUserFace.GetWidth()/FACE_CX;
			//WORD wFullCount=wLineCount*pTableResource->m_ImageUserFace.GetHeight()/(FACE_CY*2);

			//变量定义
			//WORD wFaceID=pIClientUserItem->GetFaceID();
			
			//bool bOffLine=(pIClientUserItem->GetUserStatus()==US_OFFLINE);

			////绘画头像
			//if (bOffLine==false)
			//{
			//	//调整参数
			//	if (wFaceID>=wFullCount) wFaceID=0;

			//	//绘画头像
			//	INT nXImagePos=wFaceID%wLineCount;
			//	INT nYImagePos=(wFaceID/wLineCount)*2L;
			//	pTableResource->m_ImageUserFace.BitBlt(pDC->m_hDC,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY);
			//}
			//else
			//{
			//	//调整参数
			//	if (wFaceID>=wFullCount) wFaceID=0;

			//	//绘画头像
			//	INT nXImagePos=wFaceID%wLineCount;
			//	INT nYImagePos=(wFaceID/wLineCount)*2L;
			//	pTableResource->m_ImageUserFace.BitBlt(pDC->m_hDC,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY+FACE_CY);
			//}
		}
		else
		{
			IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,pIClientUserItem->GetFaceID());
		}
	}

	return;
}

//绘画数字
VOID CTableView::DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign)
{
	//加载资源
	INT nNumberHeight=PngNumber.GetHeight();
	INT nNumberWidth=PngNumber.GetWidth()/10;

	//计算数目
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos;
	if(cbAlign==TA_RIGHT)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth;
	else if(cbAlign==TA_CENTER)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth/2;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10L);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

		//设置变量
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableViewFrame::CTableViewFrame()
{
	//状态变量
	m_bHovering=false;
	m_bLMouseDown=false;
	m_bRMouseDown=false;
	m_bWaitDistribute=false;
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_nAnimalFrameIndex=0;

	//属性变量
	m_wTableCount=0;
	m_wChairCount=0;
	m_dwServerRule=0;
	m_wServerType=0;

	//滚动信息
	m_nXExcursion=0;
	m_nYExcursion=0;
	m_nXTableCount=0;
	m_nYTableCount=0;

	//滚动信息
	m_nScrollPos=0;
	m_nScrollPosMax=0;	
	m_nVisibleLineCount=0;

	//索引变量
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;

	//设置指针
	m_pITableViewFrameSink=NULL;

	return;
}

//析构函数
CTableViewFrame::~CTableViewFrame()
{
	//删除桌子
	for (INT_PTR i=0;i<m_TableViewArray.GetCount();i++)
	{
		CTableView * pTableView=m_TableViewArray[i];
		if (pTableView!=NULL) SafeDelete(pTableView);
	}

	//删除数组
	m_TableViewArray.RemoveAll();

	return;
}

//接口查询
VOID * CTableViewFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableViewFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableViewFrame,Guid,dwQueryVer);
	return NULL;
}

//创建函数
bool CTableViewFrame::CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx)
{
	//设置变量
	m_bHovering=false;
	m_bLMouseDown=false;
	m_bRMouseDown=false;	
	m_cbButtonDown=0;
	m_cbButtonHover=0;
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;

	//设置接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink)!=NULL);
	m_pITableViewFrameSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink);

	//创建窗口
	CRect rcCreate(0,0,0,0);
	Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,pParentWnd,uWndID);

	return true;
}

//配置函数
bool CTableViewFrame::ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, WORD wServerType, WORD wServerID, LPCTSTR pszResDirectory)
{
	//效验参数
	ASSERT(wChairCount<=MAX_CHAIR);
	ASSERT((pszResDirectory!=NULL)&&(pszResDirectory[0]!=0));

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	//定义变量
	bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(dwServerRule);

	//加载资源
	//加载资源
	try
	{
		if (m_TableResource.LoadResource(pszResDirectory,wChairCount, bAvertCheatMode)==false)
		{
			return false;
		}
	}
	catch(...)
	{
		return false;
	}

	//设置变量
	m_wTableCount=wTableCount;
	m_wChairCount=wChairCount;
	m_dwServerRule=dwServerRule;
	m_wServerType=wServerType;
	m_TableViewArray.SetSize(m_wTableCount);
	ZeroMemory(m_TableViewArray.GetData(),m_wTableCount*sizeof(CTableView *));

	//创建桌子
	for (WORD i=0;i<m_wTableCount;i++)
	{
		m_TableViewArray[i]=new CTableView;
		m_TableViewArray[i]->InitTableView(i,wChairCount,this);
	}

	//调整位置
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	return true;
}

//获取用户
IClientUserItem * CTableViewFrame::GetClientUserItem(WORD wTableID, WORD wChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取用户
	if (pITableView!=NULL)
	{
		return pITableView->GetClientUserItem(wChairID);
	}

	return NULL;
}

//设置信息
bool CTableViewFrame::SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem)
{
	ITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView!=NULL) pITableView->SetClientUserItem(wChairID,pIClientUserItem);
	return true;
}

//游戏标志
bool CTableViewFrame::GetPlayFlag(WORD wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=NULL)
	{
		return pITableView->GetPlayFlag();
	}

	return false;
}

//密码标志
bool CTableViewFrame::GetLockerFlag(WORD wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=NULL)
	{
		return pITableView->GetLockerFlag();
	}

	return false;
}

//焦点框架
VOID CTableViewFrame::SetFocusFrame(WORD wTableID, bool bFocusFrame)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=NULL) pITableView->SetFocusFrame(bFocusFrame);

	return;
}

//桌子状态 
VOID CTableViewFrame::SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker,LONG lCellScore)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=NULL) pITableView->SetTableStatus(bPlaying,bLocker,lCellScore);

	return;
}

//桌子状态 
VOID CTableViewFrame::SetTableStatus(bool bWaitDistribute)
{
	m_bWaitDistribute=bWaitDistribute;	
	RedrawWindow();

	//设置定时器
	if(m_bWaitDistribute == true && m_TableResource.m_nAnimationFrame > 0)
	{
		m_nAnimalFrameIndex=0;
		SetTimer(IDI_WAIT_ANIMATION,TIME_WAIT_ANIMATION,NULL);
	}
	else
	{
		KillTimer(IDI_WAIT_ANIMATION);
	}

	return;
}

//桌子可视
bool CTableViewFrame::VisibleTable(WORD wTableID)
{
	//效验参数
	ASSERT(wTableID<m_wTableCount);
	if (wTableID>=m_wTableCount) return false;

	//计算位置
	INT nLastPos=m_nScrollPos;
	INT nXPos=(wTableID%m_nXTableCount)*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	INT nYPos=(wTableID/m_nXTableCount)*m_TableResource.m_SizeTable.cy-m_nScrollPos;

	//构造位置
	CRect rcTable,rcClient;
	GetClientRect(&rcClient);
	rcTable.SetRect(nXPos,nYPos,nXPos+m_TableResource.m_SizeTable.cx,nYPos+m_TableResource.m_SizeTable.cy);

	//位置调整
	if (rcTable.top<0L) m_nScrollPos+=rcTable.top;
	if ((rcTable.top>0)&&(rcTable.bottom>rcClient.bottom)) m_nScrollPos+=rcTable.bottom-rcClient.bottom;
	m_nScrollPos=__max(0,__min(m_nScrollPos,m_nScrollPosMax-m_nVisibleLineCount*m_TableResource.m_SizeTable.cy));

	//滚动视图
	if (nLastPos!=m_nScrollPos)
	{
		SetScrollPos(SB_VERT,m_nScrollPos);
		ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	}

	return true;
}

//闪动桌子
bool CTableViewFrame::FlashGameTable(WORD wTableID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);

	//错误判断
	if (pITableView==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	INT nXItem=wTableID%m_nXTableCount;
	INT nYItem=wTableID/m_nXTableCount;

	//计算位置
	CRect rcTable;
	rcTable.left=nXItem*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.top=nYItem*m_TableResource.m_SizeTable.cy-m_nScrollPos;
	rcTable.right=(nXItem+1)*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.bottom=(nYItem+1)*m_TableResource.m_SizeTable.cy+m_nYExcursion-m_nScrollPos;
	
	//定义参数
	INT nFlashTimes=20,nStepWidth=4;
	INT nExpandPos=nFlashTimes*nStepWidth;
	rcTable.InflateRect(nExpandPos,nExpandPos);

	//更新窗口
	UpdateWindow();

	//循环绘画
	CWindowDC WindowDC(this);
	CPen LinePen(PS_SOLID,0,GetSysColor(COLOR_BTNHILIGHT));

	//设置环境
	WindowDC.SetROP2(R2_XORPEN);
	WindowDC.SelectObject(&LinePen);

	//循环绘画
	for (INT nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		//绘画线条
		WindowDC.MoveTo(rcTable.left,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.top);

		//绘画暂停
		Sleep((nFlashTimes-nTimes)*2);

		//绘画线条
		WindowDC.MoveTo(rcTable.left,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.top);

		//设置矩形
		rcTable.DeflateRect(nStepWidth,nStepWidth);
	}

	//释放资源
	WindowDC.DeleteDC();
	LinePen.DeleteObject();

	return true;
}

//闪动椅子
bool CTableViewFrame::FlashGameChair(WORD wTableID, WORD wChairID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);

	//错误判断
	if (pITableView==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	INT nXItem=wTableID%m_nXTableCount;
	INT nYItem=wTableID/m_nXTableCount;
	CRect rcChair=m_TableResource.m_rcChairArray[wChairID];

	//获取位置
	INT nXTablePos=nXItem*m_TableResource.m_SizeTable.cx;
	INT nYTablePos=nYItem*m_TableResource.m_SizeTable.cy;
	rcChair.OffsetRect(nXTablePos+m_nXExcursion,nYTablePos+m_nYExcursion-m_nScrollPos);

	//定义参数
	INT nFlashTimes=20,nStepWidth=4;
	INT nExpandPos=nFlashTimes*nStepWidth;
	rcChair.InflateRect(nExpandPos,nExpandPos);

	//更新窗口
	UpdateWindow();

	//循环绘画
	CClientDC ClientDC(this);
	CPen LinePen(PS_SOLID,0,RGB(255,255,255));

	//设置环境
	ClientDC.SetROP2(R2_XORPEN);
	CPen * pOldPen=ClientDC.SelectObject(&LinePen);

	//循环绘画
	for (INT nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		//绘画线条
		ClientDC.MoveTo(rcChair.left,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.top);

		//绘画暂停
		Sleep((nFlashTimes-nTimes)*2);

		//绘画线条
		ClientDC.MoveTo(rcChair.left,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.top);

		//设置矩形
		rcChair.DeflateRect(nStepWidth,nStepWidth);
	}

	//释放资源
	ClientDC.SelectObject(pOldPen);

	return true;
}

//更新桌子
bool CTableViewFrame::UpdateTableView(WORD wTableID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);
	if ((pITableView==NULL)||(m_nXTableCount==0)) return false;

	//变量定义
	INT nXItem=wTableID%m_nXTableCount;
	INT nYItem=wTableID/m_nXTableCount;

	//更新桌子
	CRect rcTable;
	rcTable.left=nXItem*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.right=(nXItem+1)*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.top=nYItem*m_TableResource.m_SizeTable.cy+m_nYExcursion-m_nScrollPos;
	rcTable.bottom=(nYItem+1)*m_TableResource.m_SizeTable.cy+m_nYExcursion-m_nScrollPos;

	//更新桌子
	RedrawWindow(&rcTable,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_UPDATENOW);

	return true;
}

//获取桌子
ITableView * CTableViewFrame::GetTableViewItem(WORD wTableID)
{
	//获取桌子
	if (wTableID!=INVALID_TABLE)
	{
		//效验参数
		ASSERT(wTableID<m_TableViewArray.GetCount());
		if (wTableID>=m_TableViewArray.GetCount()) return NULL;

		//获取桌子
		ITableView * pITableView=m_TableViewArray[wTableID];

		return pITableView;
	}

	return NULL;
}

//空椅子数
WORD CTableViewFrame::GetNullChairCount(WORD wTableID, WORD & wNullChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取状态
	if (pITableView!=NULL)
	{
		return pITableView->GetNullChairCount(wNullChairID);
	}

	return 0;
}

//重画消息
VOID CTableViewFrame::OnPaint()
{
	//创建 DC
	CPaintDC dc(this);

	//获取区域
	CRect rcClip;
	CRect rcClient;
	dc.GetClipBox(&rcClip);
	GetClientRect(&rcClient);
	//GetClientRect(&rcClip);

	//绘画桌子
	if ((m_TableViewArray.GetCount()>0L)&&(m_TableResource.m_bShowTable==true))
	{
		//定义变量
		CSize SizeTable=m_TableResource.m_SizeTable;

		//变量定义
		WORD wTableCount=m_wTableCount;
		WORD nXTableCount=m_nXTableCount;
		INT  nXExcursion=m_nXExcursion;
		INT  nYExcursion=m_nYExcursion;

		//定义变量
		bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);		
		if(bAvertCheatMode)
		{
			wTableCount=1;
			nXTableCount=1;

			if(bAvertCheatMode && m_bWaitDistribute)
			{
				SizeTable.cx=m_TableResource.m_ImageWaitDistribute.GetWidth();
				SizeTable.cy=m_TableResource.m_ImageWaitDistribute.GetHeight();
				nXExcursion=(__max(SizeTable.cx,rcClient.Width()) - SizeTable.cx)/2;
				nYExcursion=(__max(SizeTable.cy,rcClient.Height()) - SizeTable.cy)/2;
			}
		}

		//建立缓冲
		CImage ImageBuffer;
		ImageBuffer.Create(SizeTable.cx,SizeTable.cy,32);

		//创建 DC
		CImageDC BufferDC(ImageBuffer);
		CDC * pDCBuffer=CDC::FromHandle(BufferDC);

		//设置环境
		pDCBuffer->SetBkMode(TRANSPARENT);
		pDCBuffer->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//绘画准备
		INT nXStartItem=(rcClip.left-nXExcursion)/SizeTable.cx;
		INT nYStartItem=(m_nScrollPos-nYExcursion+rcClip.top )/SizeTable.cy;
		INT nXConcludeItem=(rcClip.right-nXExcursion+SizeTable.cx-1)/SizeTable.cx;
		INT nYConcludeItem=(m_nScrollPos-nYExcursion+rcClip.bottom+SizeTable.cy-1)/SizeTable.cy;

		//绘画桌子
		for (INT nYIndex=nYStartItem;nYIndex<nYConcludeItem;nYIndex++)
		{
			for (INT nXIndex=nXStartItem;nXIndex<nXConcludeItem;nXIndex++)
			{
				//位置定义
				INT nIndex=nYIndex*m_nXTableCount+nXIndex;
				INT nXTablePos=nXIndex*SizeTable.cx+nXExcursion;
				INT nYTablePos=nYIndex*SizeTable.cy+nYExcursion-m_nScrollPos;

				if(SizeTable.cx > rcClient.Width())
				{
					nXTablePos = (rcClient.Width() - SizeTable.cx)/2;
				}

				//绘画桌子
				if ((nIndex>=0)&&(nXIndex<nXTableCount)&&(nIndex<wTableCount))
				{
					//绘画桌子
					if(bAvertCheatMode && m_bWaitDistribute)
					{
						m_TableResource.m_ImageWaitDistribute.BitBlt(pDCBuffer->GetSafeHdc(),0,0);

						//变量定义
						if(m_TableResource.m_ImageAnimation.IsNull() == false)
						{
							//变量定义
							CSize SizeAnimation;
							SizeAnimation.cy=m_TableResource.m_ImageAnimation.GetHeight();
							SizeAnimation.cx=m_TableResource.m_ImageAnimation.GetWidth()/m_TableResource.m_nAnimationFrame;

							//绘画动画
							
							//pDCBuffer->FillSolidRect(SizeTable.cx/2+m_TableResource.m_ptAnimation.x,SizeTable.cy/2+m_TableResource.m_ptAnimation.y,SizeAnimation.cx,SizeAnimation.cy,RGB(255,0,0));
							
							m_TableResource.m_ImageAnimation.DrawImage(pDCBuffer,SizeTable.cx/2+m_TableResource.m_ptAnimation.x,SizeTable.cy/2+m_TableResource.m_ptAnimation.y,SizeAnimation.cx,SizeAnimation.cy,m_nAnimalFrameIndex*SizeAnimation.cx,0,SizeAnimation.cx,SizeAnimation.cy);
						}
					}
					else
					{
						m_TableViewArray[nIndex]->DrawTableView(pDCBuffer,m_dwServerRule,&m_TableResource);
					}

					//绘画界面
					dc.BitBlt(nXTablePos,nYTablePos,SizeTable.cx,SizeTable.cy,pDCBuffer,0,0,SRCCOPY);
				}
				else
				{
					//绘画空隙
					m_TableResource.m_ImageGround.DrawImageTile(&dc,nXTablePos,nYTablePos,SizeTable.cx,SizeTable.cy);
				}
			}
		}

		//绘画空隙
		if (nXExcursion>rcClip.left)
		{
			INT nTileWidth=nXExcursion-rcClip.left;
			m_TableResource.m_ImageGround.DrawImageTile(&dc,rcClip.left,rcClip.top,nTileWidth,rcClip.Height());
		}

		//绘画空隙
		if ((nYExcursion-m_nScrollPos)>rcClip.top)
		{
			INT nTileHeight=(nYExcursion-m_nScrollPos)-rcClip.top;
			m_TableResource.m_ImageGround.DrawImageTile(&dc,rcClip.left,rcClip.top,rcClip.Width(),nTileHeight);
		}
	}
	else 
	{
		//绘画空隙
		if (m_TableResource.m_ImageGround.IsNull()==false)
		{
			m_TableResource.m_ImageGround.DrawImageTile(&dc,rcClip);
		}
		else
		{
			dc.FillSolidRect(&rcClip,RGB(0,0,0));
		}
	}

	return;
}

//销毁消息
VOID CTableViewFrame::OnNcDestroy()
{
	__super::OnNcDestroy();

	//删除桌子
	for (INT_PTR i=0;i<m_TableViewArray.GetCount();i++)
	{
		CTableView * pTableView=m_TableViewArray[i];
		if (pTableView!=NULL) SafeDelete(pTableView);
	}

	//删除数组
	m_TableViewArray.RemoveAll();

	//销毁资源
	m_TableResource.DestroyResource();
}

//时钟消息
VOID CTableViewFrame::OnTimer(UINT nIDEvent)
{
	if(IDI_WAIT_ANIMATION == nIDEvent)
	{
		//更新索引
		m_nAnimalFrameIndex=(m_nAnimalFrameIndex+1)%m_TableResource.m_nAnimationFrame;

		//动画帧尺寸
		INT nImageHeight=m_TableResource.m_ImageAnimation.GetHeight();
		INT nImageWidth=m_TableResource.m_ImageAnimation.GetWidth()/m_TableResource.m_nAnimationFrame;

		//变量定义
		CRect rcPaint; 
		rcPaint.left=m_TableResource.m_ptAnimation.x;
		rcPaint.top=m_TableResource.m_ptAnimation.y;
		rcPaint.right=m_TableResource.m_ptAnimation.x+nImageWidth;
		rcPaint.bottom=m_TableResource.m_ptAnimation.y+nImageHeight;

		//获取区域
		CRect rcClient;
		GetClientRect(&rcClient);
		//定义变量
		CSize SizeTable=m_TableResource.m_SizeTable;
		//求取位置
		INT  nXExcursion=m_nXExcursion;
		INT  nYExcursion=m_nYExcursion;
		nXExcursion=(__max(SizeTable.cx,rcClient.Width()) - SizeTable.cx)/2;
		nYExcursion=(__max(SizeTable.cy,rcClient.Height()) - SizeTable.cy)/2;


		rcPaint.OffsetRect(nXExcursion+m_TableResource.m_SizeTable.cx/2,nYExcursion + m_TableResource.m_SizeTable.cy/2);

		//更新界面
		InvalidateRect(&rcPaint);

	}
}

//位置消息
VOID CTableViewFrame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//消息解释
BOOL CTableViewFrame::PreTranslateMessage(MSG * pMsg)
{
	//消息过滤
	if (pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_DOWN:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_PRIOR:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_NEXT:
		case VK_SPACE:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_HOME:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_TOP,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_END:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,m_nScrollPos),NULL);
				return TRUE;
			}
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//滚动消息
VOID CTableViewFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	//设置焦点
	SetFocus();

	//获取参数
	CRect rcClient;
	GetClientRect(&rcClient);
	INT nLastPos=m_nScrollPos;

	//移动坐标
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_nScrollPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_nScrollPos=m_nScrollPosMax-m_nVisibleLineCount*m_TableResource.m_SizeTable.cy;
			break;
		}
	case SB_LINEUP:
		{
			m_nScrollPos-=SCROLL_POS_LINE;
			break;
		}
	case SB_PAGEUP:
		{
			m_nScrollPos-=rcClient.bottom;
			break;
		}
	case SB_LINEDOWN:
		{
			m_nScrollPos+=SCROLL_POS_LINE;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_nScrollPos+=rcClient.bottom;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_nScrollPos=nPos;
			break;
		}
	}
	
	//调整位置
	m_nScrollPos=__min(m_nScrollPos,m_nScrollPosMax-rcClient.bottom);
	if (m_nScrollPos<0) m_nScrollPos=0;

	//滚动窗口
	if (nLastPos!=m_nScrollPos)
	{
		SetScrollPos(SB_VERT,m_nScrollPos);
		ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonDblClk(UINT nFlags, CPoint MousePoint)
{
	//设置焦点
	SetFocus();

	//事件通知
	m_pITableViewFrameSink->OnDButtonHitTable(m_wDownTableID);

	return;
}

//鼠标消息
LRESULT CTableViewFrame::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_bHovering=false;

	//设置界面
	if (m_wDownTableID!=INVALID_TABLE) 
	{
		//设置变量
		WORD wTableID=m_wDownTableID;
		m_wDownTableID=INVALID_TABLE;

		//设置界面
		ITableView * pITableView=GetTableViewItem(wTableID);
		if (pITableView!=NULL) pITableView->SetHoverStatus(INVALID_CHAIR);
	}

	return 0;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	//设置焦点
	SetFocus();

	//按下处理
	if (m_cbButtonHover!=0)
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_cbButtonDown=m_cbButtonHover;

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE);
	}

	//动作处理
	if ((m_bRMouseDown==false)&&(m_bLMouseDown==false))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_bLMouseDown=true;
		m_wDownTableID=SwitchTableID(MousePoint);

		//获取桌子
		ITableView * pITableView=NULL;
		if (m_wDownTableID!=INVALID_TABLE) pITableView=GetTableViewItem(m_wDownTableID);

		//动作处理
		if (pITableView!=NULL)
		{
			//位置定义
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//按键测试
			m_wDownChairID=SwitchChairID(TablePoint);

			//结果处理
			if (m_wDownChairID!=INVALID_CHAIR) 
			{
				pITableView->SetMouseDown(m_bLMouseDown);
				pITableView->SetHoverStatus(m_wDownChairID);
			}
		}
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	//结果动作
	BYTE cbButtonDown=0;
	WORD wViewItemDown=INVALID_WORD;

	//点击判断
	if ((m_bLMouseDown==true)&&(m_bRMouseDown==false))
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_bLMouseDown=false;

		//变量定义
		WORD wChairID=INVALID_CHAIR;
		WORD wTableID=SwitchTableID(MousePoint);

		//清理桌子
		if (m_wDownTableID!=INVALID_TABLE)
		{
			//获取桌子
			ASSERT(GetTableViewItem(m_wDownTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(m_wDownTableID);

			//设置状态
			pITableView->SetMouseDown(false);
			pITableView->SetHoverStatus(INVALID_CHAIR);
		}

		//获取位置
		if ((wTableID!=INVALID_TABLE)&&(wTableID==m_wDownTableID))
		{
			//获取桌子
			ASSERT(GetTableViewItem(wTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(wTableID);

			//计算位置
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//椅子测试
			wChairID=SwitchChairID(TablePoint);
		}

		//结果处理
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;

			//事件通知
			m_pITableViewFrameSink->OnLButtonHitTable(wTableID,wChairID);
		}
		else
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnRButtonDown(UINT nFlags, CPoint MousePoint)
{
	//设置焦点
	SetFocus();

	//动作处理
	if ((m_bRMouseDown==false)&&(m_bLMouseDown==false))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_bRMouseDown=true;

		//桌子测试
		m_wDownTableID=SwitchTableID(MousePoint);

		//设置状态
		if (m_wDownTableID!=INVALID_TABLE)
		{
			//获取桌子
			ASSERT(GetTableViewItem(m_wDownTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(m_wDownTableID);

			//计算位置
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//椅子测试
			m_wDownChairID=SwitchChairID(TablePoint);

			//结果处理
			if (m_wDownChairID!=INVALID_CHAIR)
			{
				pITableView->SetMouseDown(m_bRMouseDown);
				pITableView->SetHoverStatus(m_wDownChairID);
			}
		}
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnRButtonUp(UINT nFlags, CPoint MousePoint)
{
	if (m_bRMouseDown==true)
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_bRMouseDown=false;

		//变量定义
		WORD wChairID=INVALID_CHAIR;
		WORD wTableID=SwitchTableID(MousePoint);

		//清理状态
		if (m_wDownTableID!=INVALID_TABLE)
		{
			//获取桌子
			ASSERT(GetTableViewItem(m_wDownTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(m_wDownTableID);

			//设置状态
			pITableView->SetMouseDown(false);
			pITableView->SetHoverStatus(INVALID_CHAIR);
		}

		//获取位置
		if ((wTableID!=INVALID_TABLE)&&(wTableID==m_wDownTableID))
		{
			//获取桌子
			ASSERT(GetTableViewItem(wTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(wTableID);

			//计算位置
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//椅子测试
			wChairID=SwitchChairID(TablePoint);
		}

		//结果处理
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;

			//事件通知
			m_pITableViewFrameSink->OnRButtonHitTable(wTableID,wChairID);
		}
		else
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//鼠标消息
BOOL CTableViewFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置滚动
	INT nLastPos=m_nScrollPos;
	INT nMaxPos=m_nScrollPosMax-rcClient.bottom;
	m_nScrollPos=__max(__min(m_nScrollPos-zDelta/WHELL_TIMES,nMaxPos),0L);

	//滚动窗口
	SetScrollPos(SB_VERT,m_nScrollPos);
	ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);

	return TRUE;
}

//光标消息
BOOL CTableViewFrame::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//窗口位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//获取光标
	CPoint Point;
	GetCursorPos(&Point);
	ScreenToClient(&Point);

	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	//状态判断
	if ((m_bRMouseDown==false)&&(m_bLMouseDown==false))
	{
		//获取光标
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//变量定义
		WORD wNewChairID=INVALID_CHAIR;
		WORD wNewTableID=SwitchTableID(MousePoint);

		//获取椅子
		if (wNewTableID!=INVALID_TABLE)
		{
			//变量定义
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//获取椅子
			wNewChairID=SwitchChairID(TablePoint);
		}
		
		//设置盘旋
		if (((wNewTableID!=m_wDownTableID)||(wNewChairID!=m_wDownChairID))&&((wNewTableID!=INVALID_CHAIR)||(m_wDownChairID!=INVALID_CHAIR)))
		{
			//清理盘旋
			if (m_wDownTableID!=INVALID_TABLE)
			{
				GetTableViewItem(m_wDownTableID)->SetHoverStatus(INVALID_CHAIR);
			}

			//设置盘旋
			if ((wNewTableID!=INVALID_TABLE)&&(wNewChairID!=INVALID_CHAIR))
			{
				GetTableViewItem(wNewTableID)->SetHoverStatus(wNewChairID);
			}
		}
		
		//设置变量
		m_wDownTableID=wNewTableID;
		m_wDownChairID=wNewChairID;

		//设置光标
		if ((m_wDownTableID!=INVALID_TABLE)&&(m_wDownChairID!=INVALID_CHAIR))
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//桌子测试
WORD CTableViewFrame::SwitchTableID(POINT MousePoint)
{
	//状态效验
	if (m_TableResource.m_SizeTable.cx==0) return INVALID_TABLE;
	if (m_TableResource.m_SizeTable.cy==0) return INVALID_TABLE;

	//位置计算
	INT nXItem=(MousePoint.x-m_nXExcursion)/m_TableResource.m_SizeTable.cx;
	INT nYItem=(MousePoint.y-m_nYExcursion+m_nScrollPos)/m_TableResource.m_SizeTable.cy;

	//结果判断
	if ((nYItem>=0)&&(nXItem>=0)&&(nYItem<m_nYTableCount)&&(nXItem<m_nXTableCount))
	{
		//设置结果
		WORD wResultItem=nYItem*m_nXTableCount+nXItem;

		//结果判断
		if (wResultItem<m_wTableCount)
		{
			bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
			if ((wResultItem==0)||(bAvertCheatMode==false)) return wResultItem;
		}
	}

	return INVALID_TABLE;
}

//椅子测试
WORD CTableViewFrame::SwitchChairID(POINT MousePoint)
{
	//位置搜索
	if ((m_TableResource.m_bShowUser==true)||(m_TableResource.m_bShowChair==true))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//横向判断
			if (MousePoint.x<m_TableResource.m_rcChairArray[i].left) continue;
			if (MousePoint.x>m_TableResource.m_rcChairArray[i].right) continue;

			//竖向判断
			if (MousePoint.y<m_TableResource.m_rcChairArray[i].top) continue;
			if (MousePoint.y>m_TableResource.m_rcChairArray[i].bottom) continue;

			return i;
		}
	}

	//进入区域
	if (m_TableResource.m_bShowEnter==true)
	{
		//变量定义
		bool bOutSide=false;

		CRect rcClient;
		GetClientRect(&rcClient);

		//变量定义
		CSize SizeTable = m_TableResource.m_SizeTable;
		if(SizeTable.cx > rcClient.Width()) SizeTable.cx=rcClient.Width();

		//横向判断
		if (MousePoint.x<SizeTable.cx/2+m_TableResource.m_ptEnter.x) bOutSide=true;
		if (MousePoint.x>SizeTable.cx/2+m_TableResource.m_ptEnter.x+m_TableResource.m_SizeEnter.cx) bOutSide=true;

		//竖向判断
		if (MousePoint.y<SizeTable.cy/2+m_TableResource.m_ptEnter.y) bOutSide=true;
		if (MousePoint.y>SizeTable.cy/2+m_TableResource.m_ptEnter.y+m_TableResource.m_SizeEnter.cy) bOutSide=true;

		//结果判断
		if (bOutSide==false)
		{
			return INDEX_ENTER_CHAIR;
		}
	}

	return INVALID_CHAIR;
}

//调整控件
VOID CTableViewFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//调整位置
	if ((m_wTableCount!=0)&&(m_TableResource.m_SizeTable.cx>0L)&&(m_TableResource.m_SizeTable.cy>0L))
	{
		//变量定义
		INT nViewWidth=__max(m_TableResource.m_SizeTable.cx,nWidth);
		INT nViewHeight=__max(m_TableResource.m_SizeTable.cy,nHeight);

		//视图参数
		m_nXTableCount=nViewWidth/m_TableResource.m_SizeTable.cx;
		m_nYTableCount=(m_wTableCount+m_nXTableCount-1)/m_nXTableCount;
		m_nVisibleLineCount=nViewHeight/m_TableResource.m_SizeTable.cy;

		//定义变量
		bool bAvertCheatMode=CServerRule::IsAllowAvertCheatMode(m_dwServerRule);
		if(bAvertCheatMode)
		{
			m_nYTableCount=1;
			m_nVisibleLineCount=1;
		}

		//偏移位置
		if (m_wTableCount==1 || bAvertCheatMode)
		{
			m_nXExcursion=(nViewWidth-m_TableResource.m_SizeTable.cx)/2;
			m_nYExcursion=__max((m_nVisibleLineCount>=m_nYTableCount)?(nHeight-m_nYTableCount*m_TableResource.m_SizeTable.cy)/2:0,0);
		}
		else
		{
			m_nYExcursion=0L;
			m_nXExcursion=(nViewWidth-m_nXTableCount*m_TableResource.m_SizeTable.cx)/2;
		}
	}
	
	//滚动数据
	m_nScrollPosMax=m_nYTableCount*m_TableResource.m_SizeTable.cy;
	m_nScrollPos=__max(0,__min(m_nScrollPos,m_nScrollPosMax-m_nVisibleLineCount*m_TableResource.m_SizeTable.cy));

	//设置滚动
	SCROLLINFO ScrollInfoVert;
	ScrollInfoVert.cbSize=sizeof(ScrollInfoVert);
	ScrollInfoVert.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoVert.nMin=0;
	ScrollInfoVert.nMax=m_nScrollPosMax;
	ScrollInfoVert.nPage=nHeight;
	ScrollInfoVert.nPos=m_nScrollPos;
	SetScrollInfo(SB_VERT,&ScrollInfoVert,FALSE);

	return;
}

//事件消息
LRESULT CTableViewFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//界面更新
	if (wParam==EVENT_SKIN_CONFIG_UPDATE)
	{
		//渲染判断
		if (m_TableResource.m_bRenderImage==true)
		{
			//更新资源
			CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
			if (pSkinRenderManager!=NULL) m_TableResource.UpdateResource(pSkinRenderManager);

			//更新界面
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
		}

		return 0;
	}

	return 0L;
}

//绘画数字
VOID CTableViewFrame::DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,BYTE cbAlign)
{
	//加载资源
	INT nNumberHeight=PngNumber.GetHeight();
	INT nNumberWidth=PngNumber.GetWidth()/10;

	//计算数目
	LONG lNumberCount=0;
	LONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0);

	//位置定义
	INT nYDrawPos=nYPos-nNumberHeight/2;
	INT nXDrawPos=nXPos;
	if(cbAlign==TA_RIGHT)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth;
	else if(cbAlign==TA_CENTER)
		nXDrawPos+=(lNumberCount-1)*nNumberWidth/2;

	//绘画桌号
	for (LONG i=0;i<lNumberCount;i++)
	{
		//绘画号码
		LONG lCellNumber=(LONG)(lNumber%10L);
		PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,lCellNumber*nNumberWidth,0);

		//设置变量
		lNumber/=10;
		nXDrawPos-=nNumberWidth;
	};

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CMatchViewFrame::CMatchViewFrame()
{
	//状态变量
	m_bHovering=false;
	m_cbButtonDown=0;
	m_cbButtonHover=0;

	//属性变量	
	m_wTableCount=0;
	m_wChairCount=0;
	m_wServerID=INVALID_WORD;

	//比赛变量
	m_dwTotalUser=0;
	m_dwWaitting=0;
	m_wLackUserCount=0;	
	m_cbMatchStatus=MS_FREE;	
	m_cbUserMatchStatus=MUS_NULL;

	//属性变量
	ZeroMemory(&m_MatchPosition,sizeof(m_MatchPosition));

	//设置指针
	m_pITableViewFrameSink=NULL;	

	//控件变量
	m_pBtMatchJoin=NULL;
	m_pBtMatchQuit=NULL;
	m_pBtMatchDetails=NULL;

	return;
}

//析构函数
CMatchViewFrame::~CMatchViewFrame()
{
	//删除桌子
	for (INT_PTR i=0;i<m_TableViewArray.GetCount();i++)
	{
		CTableView * pTableView=m_TableViewArray[i];
		if (pTableView!=NULL) SafeDelete(pTableView);
	}

	//删除数组
	m_TableViewArray.RemoveAll();

	//释放资源	
	if(m_ImageGround.IsNull()==false) m_ImageGround.Destroy();
	if(m_ImageStatus.IsNull()==false) m_ImageStatus.DestroyImage();	
	if(m_ImageNumber1.IsNull()==false) m_ImageNumber1.DestroyImage();
	if(m_ImageNumber2.IsNull()==false) m_ImageNumber2.DestroyImage();

	//释放资源
	if(m_ImageMatchBack.IsNull()==false) m_ImageMatchBack.DestroyImage();
	if(m_ImageMatchText.IsNull()==false) m_ImageMatchText.DestroyImage();
	if(m_ImageMatchShow.IsNull()==false) m_ImageMatchShow.DestroyImage();

	return;
}

//接口查询
VOID * CMatchViewFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableViewFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableViewFrame,Guid,dwQueryVer);
	return NULL;
}

//消息提醒
void CMatchViewFrame::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//参赛按钮
		if(pControlUI==m_pBtMatchJoin)
		{
			return m_pITableViewFrameSink->OnHitTableButton(BT_JOIN_MATCH);
		}

		//退赛按钮
		if(pControlUI==m_pBtMatchQuit)
		{
			return m_pITableViewFrameSink->OnHitTableButton(BT_QUIT_MATCH);
		}

		//分组按钮
		if(pControlUI==m_pBtMatchGroup)
		{
			return m_pITableViewFrameSink->OnHitTableButton(BT_GROUP_MATCH);
		}

		//复活按钮
		if(pControlUI==m_pBtMatchRevive)
		{
			return m_pITableViewFrameSink->OnHitTableButton(BT_REVIVE_MATH);
		}

		//游戏详情
		if(pControlUI==m_pBtMatchDetails)
		{
			//获取对象
			ASSERT(CGlobalWebLink::GetInstance()!=NULL);
			CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();			

			//构造地址
			TCHAR szNavigation[256]=TEXT("");
			_sntprintf(szNavigation,CountArray(szNavigation),TEXT("%s/Match/MatchView.aspx?param=%d&type=%d"),pGlobalWebLink->GetPlatformLink(),m_GameMatch.dwMatchID,m_GameMatch.cbMatchType);

			//打开页面
			ShellExecute(NULL,TEXT("OPEN"),szNavigation,NULL,NULL,SW_NORMAL);
		}
	}
}

//开始绘画
void CMatchViewFrame::OnBeginPaintWindow(HDC hDC)
{
	//创建 DC
	CDC * pDC=CDC::FromHandle(hDC);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->SelectObject(m_PaintManager.GetFont(8));

	//获取区域
	CRect rcClient;
	GetClientRect(&rcClient);

	//绘画空隙
	if (m_ImageGround.IsNull()==false)
	{
		m_ImageGround.DrawImageTile(pDC,rcClient);
	}
	else
	{
		pDC->FillSolidRect(&rcClient,RGB(0,0,0));
	}

	//背景图片
	if(m_ImageMatchBack.IsNull()==false)
	{
		m_ImageMatchBack.DrawImage(pDC,(rcClient.Width()-m_ImageMatchBack.GetWidth())/2,(rcClient.Height()-m_ImageMatchBack.GetHeight())/2);
	}

	//变量定义
	INT nXStartPos=rcClient.Width()/2+m_MatchPosition.ptMatchShow.x;
	INT nYStartPos=rcClient.Height()/2+m_MatchPosition.ptMatchShow.y;

	//比赛展示
	if(m_ImageMatchShow.IsNull()==false)
	{
		m_ImageMatchShow.DrawImage(pDC,nXStartPos,nYStartPos);
	}

	//比赛文字
	/*if(m_ImageMatchText.IsNull()==false)
	{
		m_ImageMatchText.DrawImage(pDC,nXStartPos+m_MatchPosition.ptMatchText.x,nYStartPos+m_MatchPosition.ptMatchText.y);
	}*/

	//比赛信息
	if (m_GameMatch.dwMatchID!=0)
	{
		//变量定义
		CRect rcMatchInfo;
		DWORD dwFormat=DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS;

		//比赛奖励
		if (m_ImageMatchRank.IsNull()==false)
		{
			//变量定义
			CPoint ptMatchRank(nXStartPos+m_MatchPosition.ptMatchRank.x,nYStartPos+m_MatchPosition.ptMatchRank.y);
			CPoint ptMatchReward(nXStartPos+m_MatchPosition.ptMatchReward.x,nYStartPos+m_MatchPosition.ptMatchReward.y);
			CSize SizeMatchRank(m_ImageMatchRank.GetWidth()/CountArray(m_GameMatch.MatchRewardInfo),m_ImageMatchRank.GetHeight());

			//比赛奖励
			for (int i=0;i<m_GameMatch.wRewardCount;i++)
			{
				//绘画名次
				m_ImageMatchRank.DrawImage(pDC,ptMatchRank.x,ptMatchRank.y+SizeMatchRank.cy*i,SizeMatchRank.cx,SizeMatchRank.cy,i*SizeMatchRank.cx,0,SizeMatchRank.cx,SizeMatchRank.cy);

				//构造奖励
				CRect rcRewardContent;
				TCHAR szRewardContent[128];			
				_sntprintf(szRewardContent,CountArray(szRewardContent),TEXT("游戏币%I64d 元宝%I64d 经验%d"),m_GameMatch.MatchRewardInfo[i].lRewardGold,m_GameMatch.MatchRewardInfo[i].lRewardIngot,m_GameMatch.MatchRewardInfo[i].dwRewardExperience);

				//构造区域
				rcRewardContent.left=ptMatchReward.x+m_MatchPosition.ptRewardOffset.x;
				rcRewardContent.top=ptMatchReward.y+i*m_MatchPosition.ptRewardOffset.y;
				rcRewardContent.right=rcRewardContent.left+m_MatchPosition.ptRewardSize.x;
				rcRewardContent.bottom=rcRewardContent.top+m_MatchPosition.ptRewardSize.y;

				//绘画奖励
				DrawText(pDC->GetSafeHdc(),szRewardContent,lstrlen(szRewardContent),rcRewardContent,dwFormat);					
			}
		}

		//定时赛制
		if(m_GameMatch.cbMatchType==MATCH_TYPE_LOCKTIME)
		{
			//获取定时赛
			tagLockTimeMatch * pLockTimeMatch=(tagLockTimeMatch *)m_GameMatch.cbMatchRule;

            //比赛模式
			TCHAR szMatchMode[16]=TEXT("定时赛");
			rcMatchInfo=m_MatchPosition.rcMatchMode;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szMatchMode,lstrlen(szMatchMode),rcMatchInfo,dwFormat);
			
			//开赛时间
			TCHAR szStartTime[32]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcMatchStartTime;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//常规开赛
			if (pLockTimeMatch->cbMatchMode==MATCH_MODE_NORMAL)
			{
				_sntprintf(szStartTime,CountArray(szStartTime),TEXT("%04d-%02d-%0d %02d:%02d:%02d"),pLockTimeMatch->MatchStartTime.wYear,pLockTimeMatch->MatchStartTime.wMonth,pLockTimeMatch->MatchStartTime.wDay,pLockTimeMatch->MatchStartTime.wHour,pLockTimeMatch->MatchStartTime.wMinute,pLockTimeMatch->MatchStartTime.wSecond);
			}

			//循环开赛
			if (pLockTimeMatch->cbMatchMode==MATCH_MODE_ROUND)
			{
				_sntprintf(szStartTime,CountArray(szStartTime),TEXT("每天 %02d:%02d:%02d"),pLockTimeMatch->MatchStartTime.wHour,pLockTimeMatch->MatchStartTime.wMinute,pLockTimeMatch->MatchStartTime.wSecond);
			}

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szStartTime,lstrlen(szStartTime),rcMatchInfo,dwFormat);

			//结束时间
			TCHAR szEndTime[32]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcMatchEndTime;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//常规开赛
			if (pLockTimeMatch->cbMatchMode==MATCH_MODE_NORMAL)
			{
				_sntprintf(szEndTime,CountArray(szEndTime),TEXT("%04d-%02d-%0d %02d:%02d:%02d"),pLockTimeMatch->MatchEndTime.wYear,pLockTimeMatch->MatchEndTime.wMonth,pLockTimeMatch->MatchEndTime.wDay,pLockTimeMatch->MatchEndTime.wHour,pLockTimeMatch->MatchEndTime.wMinute,pLockTimeMatch->MatchEndTime.wSecond);
			}

			//循环开赛
			if (pLockTimeMatch->cbMatchMode==MATCH_MODE_ROUND)
			{
				_sntprintf(szEndTime,CountArray(szEndTime),TEXT("每天 %02d:%02d:%02d"),pLockTimeMatch->MatchEndTime.wHour,pLockTimeMatch->MatchEndTime.wMinute,pLockTimeMatch->MatchEndTime.wSecond);
			}

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szEndTime,lstrlen(szEndTime),rcMatchInfo,dwFormat);	

			//截止时间
			TCHAR szSignupEndTime[32]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcMatchSingupEnd;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//常规开赛
			if (pLockTimeMatch->cbMatchMode==MATCH_MODE_NORMAL)
			{
				_sntprintf(szSignupEndTime,CountArray(szSignupEndTime),TEXT("%04d-%02d-%0d %02d:%02d:%02d"),pLockTimeMatch->SignupEndTime.wYear,pLockTimeMatch->SignupEndTime.wMonth,pLockTimeMatch->SignupEndTime.wDay,pLockTimeMatch->SignupEndTime.wHour,pLockTimeMatch->SignupEndTime.wMinute,pLockTimeMatch->SignupEndTime.wSecond);	
			}

			//循环开赛
			if (pLockTimeMatch->cbMatchMode==MATCH_MODE_ROUND)
			{
				_sntprintf(szSignupEndTime,CountArray(szSignupEndTime),TEXT("每天 %02d:%02d:%02d"),pLockTimeMatch->SignupEndTime.wHour,pLockTimeMatch->SignupEndTime.wMinute,pLockTimeMatch->SignupEndTime.wSecond);
			}

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szSignupEndTime,lstrlen(szSignupEndTime),rcMatchInfo,dwFormat);				

			//报名费用
			TCHAR szMatchFee[32]=TEXT("");			
			rcMatchInfo=m_MatchPosition.rcMatchFee;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//格式化费用
			if(m_GameMatch.lSignupFee > 0)
			{
				LPCTSTR pszFeeType[]={TEXT("游戏币"),TEXT("元宝")};
				_sntprintf(szMatchFee,CountArray(szMatchFee),TEXT("%I64d %s"),m_GameMatch.lSignupFee,pszFeeType[m_GameMatch.cbFeeType-FEE_TYPE_GOLD]);
			}
			else
			{
				_sntprintf(szMatchFee,CountArray(szMatchFee),TEXT("免费"));
			}


			//绘画文字
			DrawText(pDC->GetSafeHdc(),szMatchFee,lstrlen(szMatchFee),rcMatchInfo,dwFormat);				

			//参数条件
			rcMatchInfo=m_MatchPosition.rcMatchCondition;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);			

			//变量定义
			CString strJoinCondition;

			//会员等级
			if (m_GameMatch.cbJoinCondition&MATCH_JOINCD_MEMBER_ORDER)
			{
				//变量定义
				LPCTSTR pszMemberName[]={TEXT("普通玩家"),TEXT("会员VIP1"),TEXT("会员VIP2"),TEXT("会员VIP3"),TEXT("会员VIP4"),TEXT("会员VIP5"),TEXT("会员VIP6"),TEXT("会员VIP7"),};

				strJoinCondition.Append(pszMemberName[m_GameMatch.cbMemberOrder]);
				strJoinCondition.Append(TEXT(" "));
			}

			//经验等级
			if (m_GameMatch.cbJoinCondition&MATCH_JOINCD_EXPERIENCE)
			{
				TCHAR szExperience[32];
				_sntprintf(szExperience,CountArray(szExperience),TEXT("经验值 %d"),m_GameMatch.lExperience);
				strJoinCondition.Append(szExperience);
			}

			//绘画文字
			DrawText(pDC->GetSafeHdc(),strJoinCondition,strJoinCondition.GetLength(),rcMatchInfo,dwFormat);			

			//比赛局数
			TCHAR szMatchPlayCount[16]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcMatchPlayCount;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//总成绩排名
			if (m_GameMatch.cbRankingMode==RANKING_MODE_TOTAL_GRADES)
			{
				_sntprintf(szMatchPlayCount,CountArray(szMatchPlayCount),TEXT("玩满 %d 局成绩有效"),pLockTimeMatch->wMinPlayCount);
			}
		
			//特定成绩排名
			if (m_GameMatch.cbRankingMode==RANKING_MODE_SPECIAL_GRADES)
			{
				_sntprintf(szMatchPlayCount,CountArray(szMatchPlayCount),TEXT("每 %d 局统计一次成绩"),m_GameMatch.wCountInnings);
			}
			
			//绘画文字
			DrawText(pDC->GetSafeHdc(),szMatchPlayCount,lstrlen(szMatchPlayCount),rcMatchInfo,dwFormat);

			//排名方式
			TCHAR szRankingMode[32]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcRankingMode;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//总成绩排名
			if (m_GameMatch.cbRankingMode==RANKING_MODE_TOTAL_GRADES)
			{
				_sntprintf(szRankingMode,CountArray(szRankingMode),TEXT("按比赛总成绩排名"));	
			}

			//特定成绩排名
			if (m_GameMatch.cbRankingMode==RANKING_MODE_SPECIAL_GRADES)
			{
				LPCTSTR szFilterMode[]={TEXT("最佳成绩"),TEXT("平均成绩"),TEXT("去高去低取平均成绩")}; 
				_sntprintf(szRankingMode,CountArray(szRankingMode),TEXT("按%s排名"),szFilterMode[m_GameMatch.cbFilterGradesMode]);
			}

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szRankingMode,lstrlen(szRankingMode),rcMatchInfo,dwFormat);	
			
			//比赛倒计时
			if (m_ImageNumber1.IsNull()==false)
			{
				//变量定义
				SYSTEMTIME StartTime;	
				CTime CurrentTime = CTime::GetCurrentTime();		
				StartTime=pLockTimeMatch->MatchStartTime;

				//计算时间戳
				DWORD dwCurrentStamp = CurrentTime.GetHour()*3600+CurrentTime.GetMinute()*60+CurrentTime.GetSecond();
				DWORD dwStartStamp = StartTime.wHour*3600+StartTime.wMinute*60+StartTime.wSecond;
				DWORD dwDiffStemp = dwStartStamp>dwCurrentStamp?dwStartStamp-dwCurrentStamp:0;

				//计算时间
				DWORD wDiffHour = dwDiffStemp/3600;
				DWORD wDiffMinute = (dwDiffStemp-wDiffHour*3600)/60;
				DWORD wDiffSecond = dwDiffStemp-wDiffHour*3600-wDiffMinute*60;

				if (m_ImageMatchText.IsNull()==false && m_cbMatchStatus!=MS_WAITPLAY)
				{
					m_ImageMatchText.DrawImage(pDC,nXStartPos+m_MatchPosition.ptMatchText.x,nYStartPos+m_MatchPosition.ptMatchText.y);
				}

				//状态判断
				if (m_cbMatchStatus==MS_FREE)			//比赛未开始
				{
					//变量定义
					TCHAR szFormatNum[16]=TEXT("");
					_sntprintf(szFormatNum,CountArray(szFormatNum),TEXT("%02d:%02d:%02d"),wDiffHour,wDiffMinute,wDiffSecond);

					//绘制数字
					DrawNumber(pDC,nXStartPos+m_MatchPosition.ptNumber1.x,nYStartPos+m_MatchPosition.ptNumber1.y,&m_ImageNumber1,TEXT(":0123456789"),0,szFormatNum);
				}
				else if(m_cbMatchStatus==MS_WAITPLAY)	//等待开始
				{
					if(m_ImageMatchWaitPlay.IsNull()==false)
					{
						m_ImageMatchWaitPlay.DrawImage(pDC,nXStartPos+m_MatchPosition.ptMatchWaitPlay.x,nYStartPos+m_MatchPosition.ptMatchWaitPlay.y);
					}

					//绘制数字
					DrawNumber(pDC,nXStartPos+m_MatchPosition.ptNumber2.x,nYStartPos+m_MatchPosition.ptNumber2.y,&m_ImageNumber2,TEXT("0123456789"),m_dwWaitting,TEXT("%d"));

					//绘制数字
					DrawNumber(pDC,nXStartPos+m_MatchPosition.ptNumber3.x,nYStartPos+m_MatchPosition.ptNumber3.y,&m_ImageNumber2,TEXT("0123456789"),m_dwTotalUser-m_dwWaitting,TEXT("%d"));

				}
				else if(m_cbMatchStatus==MS_MATCHING)	//比赛进行中	
				{
					//绘制文字
					DrawWords(pDC,nXStartPos+m_MatchPosition.ptNumber1.x,nYStartPos+m_MatchPosition.ptNumber1.y,&m_ImageStatus,TEXT("比赛进行中已结束"),TEXT("比赛进行中"));
				} 
				else if(m_cbMatchStatus==MS_MATCHEND)	//比赛已结束
				{
					//绘制文字
					DrawWords(pDC,nXStartPos+m_MatchPosition.ptNumber1.x,nYStartPos+m_MatchPosition.ptNumber1.y,&m_ImageStatus,TEXT("比赛进行中已结束"),TEXT("比赛已结束"));
				}
			}
		}

		//即时赛制
		if(m_GameMatch.cbMatchType==MATCH_TYPE_IMMEDIATE)
		{
			//获取即时赛
			tagImmediateMatch * pImmediateMatch=(tagImmediateMatch *)m_GameMatch.cbMatchRule;

			//比赛模式
			TCHAR szMatchMode[16]=TEXT("即时赛");
			rcMatchInfo=m_MatchPosition.rcMatchMode;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szMatchMode,lstrlen(szMatchMode),rcMatchInfo,dwFormat);

			//开赛条件
			TCHAR szStartUserCount[32]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcStartCount;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);
			_sntprintf(szStartUserCount,CountArray(szStartUserCount),TEXT("满%d人开赛"),pImmediateMatch->wStartUserCount);

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szStartUserCount,lstrlen(szStartUserCount),rcMatchInfo,dwFormat);
			

			//报名费用
			TCHAR szMatchFee[32]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcMatchFee;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//格式化费用
			if(m_GameMatch.lSignupFee > 0)
			{
				LPCTSTR pszFeeType[]={TEXT("游戏币"),TEXT("元宝")};
				_sntprintf(szMatchFee,CountArray(szMatchFee),TEXT("%I64d %s"),m_GameMatch.lSignupFee,pszFeeType[m_GameMatch.cbFeeType-FEE_TYPE_GOLD]);
			}
			else
			{
				_sntprintf(szMatchFee,CountArray(szMatchFee),TEXT("免费"));
			}

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szMatchFee,lstrlen(szMatchFee),rcMatchInfo,dwFormat);	

			//参赛条件
			rcMatchInfo=m_MatchPosition.rcMatchCondition;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);

			//变量定义
			CString strJoinCondition;

			//会员等级
			if (m_GameMatch.cbJoinCondition&MATCH_JOINCD_MEMBER_ORDER)
			{
				//变量定义
				LPCTSTR pszMemberName[]={TEXT("普通玩家"),TEXT("会员VIP1"),TEXT("会员VIP2"),TEXT("会员VIP3"),TEXT("会员VIP4"),TEXT("会员VIP5"),TEXT("会员VIP6"),TEXT("会员VIP7"),};

				//格式化等级
				strJoinCondition.Append(pszMemberName[m_GameMatch.cbMemberOrder]);
				strJoinCondition.Append(TEXT(" "));
			}

			//经验等级
			if (m_GameMatch.cbJoinCondition&MATCH_JOINCD_EXPERIENCE)
			{
				TCHAR szExperience[32];
				_sntprintf(szExperience,CountArray(szExperience),TEXT("经验值 %d"),m_GameMatch.lExperience);
				strJoinCondition.Append(szExperience);
			}

			//绘画文字
			DrawText(pDC->GetSafeHdc(),strJoinCondition,strJoinCondition.GetLength(),rcMatchInfo,dwFormat);

			//游戏局数
			TCHAR szPlayCount[16]=TEXT("");
			rcMatchInfo=m_MatchPosition.rcPalyCount;
			rcMatchInfo.OffsetRect(nXStartPos,nYStartPos);
			_sntprintf(szPlayCount,CountArray(szPlayCount),TEXT("%d 局"),pImmediateMatch->wPlayCount);

			//绘画文字
			DrawText(pDC->GetSafeHdc(),szPlayCount,lstrlen(szPlayCount),rcMatchInfo,dwFormat);

			//比赛文字
			if(m_ImageMatchText.IsNull()==false)
			{
				m_ImageMatchText.DrawImage(pDC,nXStartPos+m_MatchPosition.ptMatchText.x,nYStartPos+m_MatchPosition.ptMatchText.y);
			}

			//报名人数
			if(m_ImageNumber1.IsNull()==false)
			{
				DrawNumber(pDC,nXStartPos+m_MatchPosition.ptNumber1.x,nYStartPos+m_MatchPosition.ptNumber1.y,&m_ImageNumber1,TEXT("0123456789"),m_dwWaitting,TEXT("%d"),DT_CENTER);
			}

			//等待人数
			if(m_ImageNumber1.IsNull()==false)
			{
				DrawNumber(pDC,nXStartPos+m_MatchPosition.ptNumber2.x,nYStartPos+m_MatchPosition.ptNumber2.y,&m_ImageNumber1,TEXT("0123456789"),m_dwTotalUser-m_dwWaitting,TEXT("%d"),DT_CENTER);
			}
		}
	}
}

//创建函数
bool CMatchViewFrame::CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx)
{
	//设置变量
	m_bHovering=false;	
	m_cbButtonDown=0;
	m_cbButtonHover=0;

	//设置接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink)!=NULL);
	m_pITableViewFrameSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink);

	//创建窗口
	CRect rcCreate(0,0,0,0);
	Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,pParentWnd,uWndID);

	return true;
}

//配置函数
bool CMatchViewFrame::ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, WORD wServerType, WORD wServerID,LPCTSTR pszResDirectory)
{
	//效验参数
	ASSERT(wChairCount<=MAX_CHAIR);
	ASSERT((pszResDirectory!=NULL)&&(pszResDirectory[0]!=0));

	//设置变量
	m_wServerID=wServerID;
	m_wTableCount=wTableCount;
	m_wChairCount=wChairCount;
	m_TableViewArray.SetSize(m_wTableCount);
	ZeroMemory(m_TableViewArray.GetData(),m_wTableCount*sizeof(CTableView *));

	//创建桌子
	for (WORD i=0;i<m_wTableCount;i++)
	{
		m_TableViewArray[i]=new CTableView;
		m_TableViewArray[i]->InitTableView(i,wChairCount,this);
	}

	//加载资源
	if (LoadResource(pszResDirectory)==false)
	{
		return false;
	}

	//设置定时器
	if(m_GameMatch.cbMatchType==MATCH_TYPE_LOCKTIME && m_ImageNumber1.IsNull()==false)
	{
		SetTimer(IDI_UPDATE_TIME,TIME_UPDATE_TIME,NULL);
	}

	return true;
}

//视图数据
bool CMatchViewFrame::OnEventViewData(WORD wCmdID, VOID * pData, WORD wDataSize)
{
	switch(wCmdID)
	{
	case SUB_GR_MATCH_RULE:			//比赛规则
		{
			//参数效验
			ASSERT(wDataSize<=sizeof(CMD_GR_Match_Rule));
			if(wDataSize>sizeof(CMD_GR_Match_Rule)) return false;

			//提取数据
			CMD_GR_Match_Rule * pMatchRule=(CMD_GR_Match_Rule *)pData;

			//基本信息
			m_GameMatch.dwMatchID=pMatchRule->dwMatchID;
			m_GameMatch.cbMatchType=pMatchRule->cbMatchType;
			lstrcpyn(m_GameMatch.szMatchName,CW2CT(pMatchRule->szMatchName),CountArray(m_GameMatch.szMatchName));	

			//报名信息
			m_GameMatch.cbFeeType=pMatchRule->cbFeeType;
			m_GameMatch.lSignupFee=pMatchRule->lSignupFee;	
			m_GameMatch.lExperience=pMatchRule->dwExperience;	
			m_GameMatch.cbDeductArea=pMatchRule->cbDeductArea;
			m_GameMatch.cbSignupMode=pMatchRule->cbSignupMode;
			m_GameMatch.cbMemberOrder=pMatchRule->cbMemberOrder;
			m_GameMatch.cbJoinCondition=pMatchRule->cbJoinCondition;

			//报名信息
			m_GameMatch.cbRankingMode=pMatchRule->cbRankingMode;
			m_GameMatch.wCountInnings=pMatchRule->wCountInnings;	
			m_GameMatch.cbFilterGradesMode=pMatchRule->cbFilterGradesMode;

			//比赛规则
			CopyMemory(m_GameMatch.cbMatchRule,pMatchRule->cbMatchRule,sizeof(m_GameMatch.cbMatchRule));

			//比赛奖励
			m_GameMatch.wRewardCount=pMatchRule->wRewardCount;
			CopyMemory(m_GameMatch.MatchRewardInfo,pMatchRule->MatchRewardInfo,sizeof(m_GameMatch.MatchRewardInfo));

			return true;
		}
	case SUB_GR_MATCH_NUM:			//参赛人数
		{
			//参数效验
			ASSERT(wDataSize==sizeof(CMD_GR_Match_Num));
			if(wDataSize!=sizeof(CMD_GR_Match_Num)) return false;

			//提取数据
			CMD_GR_Match_Num *pMatchNum=(CMD_GR_Match_Num*)pData;

			//设置变量
			m_dwTotalUser=pMatchNum->dwTotal;
			m_dwWaitting=pMatchNum->dwWaitting;
			
			//刷新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

			return true;
		}
	case SUB_GR_MATCH_STATUS:		//比赛状态
		{
			//效验参数
			ASSERT(wDataSize==sizeof(BYTE));
			if(wDataSize!=sizeof(BYTE)) return false;

			//设置状态
			m_cbMatchStatus=*(BYTE*)pData;

			//调整控件
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			//刷新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

			return true;
		}
	case SUB_GR_MATCH_USTATUS:		//报名状态
		{
			//效验参数
			ASSERT(wDataSize==sizeof(BYTE));
			if(wDataSize!=sizeof(BYTE)) return false;

			//设置状态
			m_cbUserMatchStatus=*(BYTE*)pData;

			//调整控件
			CRect rcClient;
			GetClientRect(&rcClient);
			RectifyControl(rcClient.Width(),rcClient.Height());

			//刷新界面
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

			return true;
		}
	case SUB_GR_MATCH_DESC:			//比赛描述
		{
			//参数效验
			ASSERT(wDataSize==sizeof(CMD_GR_MatchDesc));
			if(wDataSize!=sizeof(CMD_GR_MatchDesc)) return false;

			//变量定义
			CMD_GR_MatchDesc *pMatchDesc=(CMD_GR_MatchDesc*)pData;

			return true;
		}
	}


	return false;
}

//获取用户
IClientUserItem * CMatchViewFrame::GetClientUserItem(WORD wTableID, WORD wChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取用户
	if (pITableView!=NULL)
	{
		return pITableView->GetClientUserItem(wChairID);
	}

	return NULL;
}

//设置信息
bool CMatchViewFrame::SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem)
{
	ITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView!=NULL) pITableView->SetClientUserItem(wChairID,pIClientUserItem);
	return true;
}

//游戏标志
bool CMatchViewFrame::GetPlayFlag(WORD wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=NULL)
	{
		return pITableView->GetPlayFlag();
	}

	return false;
}

//密码标志
bool CMatchViewFrame::GetLockerFlag(WORD wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=NULL)
	{
		return pITableView->GetLockerFlag();
	}

	return false;
}

//焦点框架
VOID CMatchViewFrame::SetFocusFrame(WORD wTableID, bool bFocusFrame)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=NULL) pITableView->SetFocusFrame(bFocusFrame);

	return;
}

//桌子状态 
VOID CMatchViewFrame::SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker,LONG lCellScore)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=NULL) pITableView->SetTableStatus(bPlaying,bLocker,lCellScore);

	return;
}

//桌子状态 
VOID CMatchViewFrame::SetTableStatus(bool bWaitDistribute)
{
	return;
}

//桌子可视
bool CMatchViewFrame::VisibleTable(WORD wTableID)
{
	return true;
}

//闪动桌子
bool CMatchViewFrame::FlashGameTable(WORD wTableID)
{
	return true;
}

//闪动椅子
bool CMatchViewFrame::FlashGameChair(WORD wTableID, WORD wChairID)
{
	return true;
}

//更新桌子
bool CMatchViewFrame::UpdateTableView(WORD wTableID)
{
	return true;
}

//获取桌子
ITableView * CMatchViewFrame::GetTableViewItem(WORD wTableID)
{
	//获取桌子
	if (wTableID!=INVALID_TABLE)
	{
		//效验参数
		ASSERT(wTableID<m_TableViewArray.GetCount());
		if (wTableID>=m_TableViewArray.GetCount()) return NULL;

		//获取桌子
		ITableView * pITableView=m_TableViewArray[wTableID];

		return pITableView;
	}

	return NULL;
}

//空椅子数
WORD CMatchViewFrame::GetNullChairCount(WORD wTableID, WORD & wNullChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取状态
	if (pITableView!=NULL)
	{
		return pITableView->GetNullChairCount(wNullChairID);
	}

	return 0;
}

//销毁消息
VOID CMatchViewFrame::OnNcDestroy()
{
	//关闭定时器
	KillTimer(IDI_UPDATE_TIME);

	//删除桌子
	for (INT_PTR i=0;i<m_TableViewArray.GetCount();i++)
	{
		CTableView * pTableView=m_TableViewArray[i];
		if (pTableView!=NULL) SafeDelete(pTableView);
	}

	//删除数组
	m_TableViewArray.RemoveAll();

	//释放资源	
	if(m_ImageGround.IsNull()==false) m_ImageGround.Destroy();
	if(m_ImageStatus.IsNull()==false) m_ImageStatus.DestroyImage();	
	if(m_ImageNumber1.IsNull()==false) m_ImageNumber1.DestroyImage();
	if(m_ImageNumber2.IsNull()==false) m_ImageNumber2.DestroyImage();

	//释放资源
	if(m_ImageMatchBack.IsNull()==false) m_ImageMatchBack.DestroyImage();
	if(m_ImageMatchText.IsNull()==false) m_ImageMatchText.DestroyImage();
	if(m_ImageMatchShow.IsNull()==false) m_ImageMatchShow.DestroyImage();

    return __super::OnNcDestroy();
}

//位置消息
VOID CMatchViewFrame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//判断目录
bool CMatchViewFrame::IsDirectoryExist(LPCTSTR pszDirectofy)
{
	//变量定义
    WIN32_FIND_DATA wFindData;
    bool bValue=false;

	//查找目录
    HANDLE hFile = FindFirstFile(pszDirectofy, &wFindData);
    if ((hFile != INVALID_HANDLE_VALUE) && (wFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
       bValue=true;   
    }

	//关闭句柄
    FindClose(hFile);

    return bValue;
}

//加载资源
bool CMatchViewFrame::LoadResource(LPCTSTR pszResDirectory)
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//比赛目录
	TCHAR szMatchDirectory[MAX_PATH]=TEXT("");
	_sntprintf(szMatchDirectory,CountArray(szMatchDirectory),TEXT("%s\\%s\\MatchMode%d"),szDirectory,pszResDirectory,m_GameMatch.cbMatchType);

	//判断目录
	if(IsDirectoryExist(szMatchDirectory)==false) return false;

	//参数路径
	TCHAR szParameterFile[MAX_PATH]=TEXT("");
	_sntprintf(szParameterFile,CountArray(szParameterFile),TEXT("%s\\MatchResource.INI"),szMatchDirectory);

	//变量定义
	CTableResource TableResource;

	//位置模式
	m_MatchPosition.cbPostionMode=GetPrivateProfileInt(TEXT("Attribute"),TEXT("PositionMode"),1L,szParameterFile);

	//绝对模式
	if(m_MatchPosition.cbPostionMode==MATCH_PM_ABSOLUTE)
	{
		//位置坐标
		TableResource.ReadPoint(m_MatchPosition.ptMatchShow,TEXT("Position"),TEXT("Point_MatchShow"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptMatchText,TEXT("Position"),TEXT("Point_MatchText"),szParameterFile);

		//奖励位置		
		TableResource.ReadPoint(m_MatchPosition.ptMatchRank,TEXT("Position"),TEXT("Point_MatchRank"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptRewardSize,TEXT("Position"),TEXT("Point_RewardSize"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptMatchReward,TEXT("Position"),TEXT("Point_MatchReward"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptRewardOffset,TEXT("Position"),TEXT("Point_RewardOffset"),szParameterFile);		

		//位置坐标
		TableResource.ReadPoint(m_MatchPosition.ptMatchJoin,TEXT("Position"),TEXT("Point_MatchJoin"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptMatchDetails,TEXT("Position"),TEXT("Point_MatchDetails"),szParameterFile);

		//位置坐标
		TableResource.ReadPoint(m_MatchPosition.ptNumber1,TEXT("Position"),TEXT("Point_Number1"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptNumber2,TEXT("Position"),TEXT("Point_Number2"),szParameterFile);	
		TableResource.ReadPoint(m_MatchPosition.ptNumber3,TEXT("Position"),TEXT("Point_Number3"),szParameterFile);	

		//位置坐标
		TableResource.ReadRect(m_MatchPosition.rcMatchMode,TEXT("Position"),TEXT("Rect_MatchMode"),szParameterFile);
		TableResource.ReadRect(m_MatchPosition.rcMatchFee,TEXT("Position"),TEXT("Rect_MatchFee"),szParameterFile);		
		TableResource.ReadRect(m_MatchPosition.rcStartCount,TEXT("Position"),TEXT("Rect_StartCount"),szParameterFile);
		TableResource.ReadRect(m_MatchPosition.rcPalyCount,TEXT("Position"),TEXT("Rect_PlayCount"),szParameterFile);
		TableResource.ReadRect(m_MatchPosition.rcRankingMode,TEXT("Position"),TEXT("Rect_RankingMode"),szParameterFile);
		TableResource.ReadRect(m_MatchPosition.rcMatchEndTime,TEXT("Position"),TEXT("Rect_MatchEndTime"),szParameterFile);
		TableResource.ReadRect(m_MatchPosition.rcMatchPlayCount,TEXT("Position"),TEXT("Rect_MatchPlayCount"),szParameterFile);					
		TableResource.ReadRect(m_MatchPosition.rcMatchStartTime,TEXT("Position"),TEXT("Rect_MatchStartTime"),szParameterFile);
		TableResource.ReadRect(m_MatchPosition.rcMatchCondition,TEXT("Position"),TEXT("Rect_MatchCondition"),szParameterFile);
		TableResource.ReadRect(m_MatchPosition.rcMatchSingupEnd,TEXT("Position"),TEXT("Rect_MatchSingupEnd"),szParameterFile);	
	}

	//相对模式.
	if(m_MatchPosition.cbPostionMode==MATCH_PM_RELATIVE)
	{
		//变量定义
		CPoint ptInfoOffset;

		//偏移位置
		TableResource.ReadPoint(ptInfoOffset,TEXT("Position"),TEXT("Point_InfoOffset"),szParameterFile);

		//位置坐标
		TableResource.ReadPoint(m_MatchPosition.ptMatchShow,TEXT("Position"),TEXT("Point_MatchShow"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptMatchText,TEXT("Position"),TEXT("Point_MatchText"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptMatchWaitPlay,TEXT("Position"),TEXT("Point_MatchWaitPlay"),szParameterFile);		

		//奖励位置
		TableResource.ReadPoint(m_MatchPosition.ptMatchRank,TEXT("Position"),TEXT("Point_MatchRank"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptRewardSize,TEXT("Position"),TEXT("Point_RewardSize"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptMatchReward,TEXT("Position"),TEXT("Point_MatchReward"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptRewardOffset,TEXT("Position"),TEXT("Point_RewardOffset"),szParameterFile);

		//位置坐标
		TableResource.ReadPoint(m_MatchPosition.ptMatchJoin,TEXT("Position"),TEXT("Point_MatchJoin"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptMatchDetails,TEXT("Position"),TEXT("Point_MatchDetails"),szParameterFile);

		//位置坐标
		TableResource.ReadPoint(m_MatchPosition.ptNumber1,TEXT("Position"),TEXT("Point_Number1"),szParameterFile);
		TableResource.ReadPoint(m_MatchPosition.ptNumber2,TEXT("Position"),TEXT("Point_Number2"),szParameterFile);	
		TableResource.ReadPoint(m_MatchPosition.ptNumber3,TEXT("Position"),TEXT("Point_Number3"),szParameterFile);	

		//位置坐标
		TableResource.ReadRect(m_MatchPosition.rcMatchMode,TEXT("Position"),TEXT("Rect_MatchMode"),szParameterFile);
		
		//变量定义
		CRect rcMatchInfo=m_MatchPosition.rcMatchMode;

		//定时赛制
		if(m_GameMatch.cbMatchType==MATCH_TYPE_LOCKTIME)
		{
			//开始时间
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchStartTime=rcMatchInfo;

			//结束时间
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchEndTime=rcMatchInfo;

			//结束时间
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchSingupEnd=rcMatchInfo;

			//报名费用
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchFee=rcMatchInfo;

			//参数条件
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchCondition=rcMatchInfo;

			//游戏局数
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchPlayCount=rcMatchInfo;

			//排名方式
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcRankingMode=rcMatchInfo;
		}

		//即时赛制
		if(m_GameMatch.cbMatchType==MATCH_TYPE_IMMEDIATE)
		{
			//开赛条件
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcStartCount=rcMatchInfo;

			//报名费用
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchFee=rcMatchInfo;

			//参赛条件
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcMatchCondition=rcMatchInfo;

			//比赛局数
			rcMatchInfo.OffsetRect(ptInfoOffset.x,ptInfoOffset.y);
			m_MatchPosition.rcPalyCount=rcMatchInfo;
		}
	}

	//变量定义
	TCHAR szImagePath[MAX_PATH]=TEXT("");

	//资源句柄
	HINSTANCE hInstance=::AfxGetResourceHandle();

	//环绕图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Ground.bmp"),szMatchDirectory);
	if (FAILED(m_ImageGround.Load(szImagePath)))
	{
		m_ImageGround.LoadFromResource(hInstance,IDB_TABLE_GROUND);
	}	

	//背景图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_MATCH_BACK.png"),szMatchDirectory);
	m_ImageMatchBack.LoadImage(szImagePath);

    //展示图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_MATCH_SHOW.png"),szMatchDirectory);
	m_ImageMatchShow.LoadImage(szImagePath);

	//文字图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_MATCH_TEXT.png"),szMatchDirectory);
	m_ImageMatchText.LoadImage(szImagePath);

	//等待提示
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_MATCH_WAITPLAY.png"),szMatchDirectory);
	m_ImageMatchWaitPlay.LoadImage(szImagePath);	

	//比赛名称
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_MATCH_RANK.png"),szMatchDirectory);
	m_ImageMatchRank.LoadImage(szImagePath);	

	//数字1图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_NUMBER1.png"),szMatchDirectory);
	m_ImageNumber1.LoadImage(szImagePath);

	//数字2图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_NUMBER2.png"),szMatchDirectory);
	m_ImageNumber2.LoadImage(szImagePath);	

	//状态文字
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\IMAGE_WORDS.png"),szMatchDirectory);
	m_ImageStatus.LoadImage(szImagePath);

	return CreateControlUI(pszResDirectory);
}

//创建控件
bool CMatchViewFrame::CreateControlUI(LPCTSTR pszResDirectory)
{
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//游戏目录
	TCHAR szGameDirectory[MAX_PATH]=TEXT("");
	_sntprintf(szGameDirectory,CountArray(szGameDirectory),TEXT("%s\\%s"),szDirectory,pszResDirectory);

	//比赛目录
	TCHAR szMatchDirectory[MAX_PATH]=TEXT("");
	_sntprintf(szMatchDirectory,CountArray(szMatchDirectory),TEXT("MatchMode%d"),m_GameMatch.cbMatchType);

	//设置路径
	CStdString strResourcePath = m_PaintManager.GetResourcePath();
	m_PaintManager.SetResourcePath(szGameDirectory);

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//创建按钮
	m_pBtMatchJoin=CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pBtMatchQuit=CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pBtMatchGroup=CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pBtMatchRevive=CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pBtMatchDetails=CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));

	//变量定义
	TCHAR szImagePath[MAX_PATH]=TEXT("");

	//设置图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("file='%s\\BT_MATCH_JOIN.png' restype='FILE'"),szMatchDirectory);
	m_pBtMatchJoin->SetStatusImage(szImagePath);

	//设置图片
	_sntprintf(szImagePath,CountArray(szImagePath),TEXT("file='%s\\BT_MATCH_DETAILS.png' restype='FILE'"),szMatchDirectory);
	m_pBtMatchDetails->SetStatusImage(szImagePath);

	//取消报名
	if (m_GameMatch.cbMatchType==MATCH_TYPE_IMMEDIATE)
	{		
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("file='%s\\BT_MATCH_QUIT.png' restype='FILE'"),szMatchDirectory);	
		m_pBtMatchQuit->SetStatusImage(szImagePath);
	}

	//分组比赛
	if (m_GameMatch.cbMatchType==MATCH_TYPE_LOCKTIME)
	{
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("file='%s\\BT_MATCH_GROUP.png' restype='FILE'"),szMatchDirectory);	
		m_pBtMatchGroup->SetStatusImage(szImagePath);	
	}	

	//分组比赛
	if (m_GameMatch.cbMatchType==MATCH_TYPE_LOCKTIME)
	{
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("file='%s\\BT_MATCH_REVIVE.png' restype='FILE'"),szMatchDirectory);	
		m_pBtMatchRevive->SetStatusImage(szImagePath);	
	}

	//还原路径
	m_PaintManager.SetResourcePath(strResourcePath);

	//调整控件
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return true;
}

//比赛状态
VOID CMatchViewFrame::SetMatchStatus(BYTE cbMatchStatus)
{
	//设置状态
	m_cbMatchStatus=cbMatchStatus;

	if (m_cbMatchStatus==MS_MATCHEND)
	{
		CServerListData *pServerListData = CServerListData::GetInstance();
		if (pServerListData!=NULL)
		{
			CGameServerItem *pGameServerItem = pServerListData->SearchGameServer(m_wServerID);
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_bSignuped=false;
			}
		}
	}

	//更新界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//玩家参赛状态
VOID CMatchViewFrame::SetUserMatchStatus(BYTE cbUserMatchStatus)
{
	//设置状态
	m_cbUserMatchStatus=cbUserMatchStatus;

	if (m_cbUserMatchStatus==MUS_NULL)
	{
		CServerListData *pServerListData = CServerListData::GetInstance();
		if (pServerListData!=NULL)
		{
			CGameServerItem *pGameServerItem = pServerListData->SearchGameServer(m_wServerID);
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_bSignuped=false;
			}
		}
	}

	//更新界面
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	return;
}

//调整控件
VOID CMatchViewFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//判断指针
	if (m_GameMatch.dwMatchID==0 || nWidth==0 || nHeight==0) return;

	//定时赛制
	if(m_GameMatch.cbMatchType==MATCH_TYPE_LOCKTIME)
	{
		//获取定时赛
		tagLockTimeMatch * pLockTimeMatch=(tagLockTimeMatch *)m_GameMatch.cbMatchRule;

		//可视设置		
		m_pBtMatchQuit->SetVisible(false);		
		m_pBtMatchDetails->SetVisible(true);				
		m_pBtMatchJoin->SetVisible(m_cbUserMatchStatus==MUS_NULL);
		m_pBtMatchRevive->SetVisible(m_cbUserMatchStatus==MUS_OUT);
		m_pBtMatchGroup->SetVisible((m_cbMatchStatus>=MS_FREE) && (m_cbUserMatchStatus>=MUS_SIGNUPED));	


		//使能设置
		m_pBtMatchQuit->SetEnabled(false);		
		m_pBtMatchDetails->SetEnabled(true);				
		m_pBtMatchJoin->SetEnabled(m_cbUserMatchStatus==MUS_NULL);		
		m_pBtMatchGroup->SetEnabled(m_cbMatchStatus>=MS_FREE && m_cbMatchStatus<MS_MATCHEND);	
		m_pBtMatchRevive->SetEnabled(m_cbMatchStatus==MS_MATCHING && pLockTimeMatch->cbReviveEnabled);
	}

	//即时赛制
	if(m_GameMatch.cbMatchType==MATCH_TYPE_IMMEDIATE)
	{
		m_pBtMatchRevive->SetVisible(false);
		m_pBtMatchGroup->SetVisible(false);

		//可视设置
		m_pBtMatchDetails->SetVisible(true);
		m_pBtMatchJoin->SetVisible(m_cbUserMatchStatus==MUS_NULL);		
		m_pBtMatchQuit->SetVisible(m_cbUserMatchStatus>=MUS_SIGNUPED);	

		//使能设置		
		m_pBtMatchDetails->SetEnabled(true);
		m_pBtMatchJoin->SetEnabled(m_cbUserMatchStatus==MUS_NULL);		
		m_pBtMatchQuit->SetEnabled(m_cbUserMatchStatus>=MUS_SIGNUPED);	
	}

	//变量定义
	INT nXStartPos=nWidth/2+m_MatchPosition.ptMatchShow.x;
	INT nYStartPos=nHeight/2+m_MatchPosition.ptMatchShow.y;

	//加入比赛
	CRect rcMatchJoin;
	rcMatchJoin.left=nXStartPos+m_MatchPosition.ptMatchJoin.x;
	rcMatchJoin.top=nYStartPos+m_MatchPosition.ptMatchJoin.y;
	rcMatchJoin.right=rcMatchJoin.left+m_pBtMatchJoin->GetFixedWidth();
	rcMatchJoin.bottom=rcMatchJoin.top+m_pBtMatchJoin->GetFixedHeight();

	//比赛详情
	CRect rcMatchDetails;
	rcMatchDetails.left=nXStartPos+m_MatchPosition.ptMatchDetails.x;
	rcMatchDetails.top=nYStartPos+m_MatchPosition.ptMatchDetails.y;
	rcMatchDetails.right=rcMatchDetails.left+m_pBtMatchDetails->GetFixedWidth();
	rcMatchDetails.bottom=rcMatchDetails.top+m_pBtMatchDetails->GetFixedHeight();

	//设置位置
	m_pBtMatchJoin->SetPos(rcMatchJoin);
	m_pBtMatchQuit->SetPos(rcMatchJoin);
	m_pBtMatchGroup->SetPos(rcMatchJoin);
	m_pBtMatchRevive->SetPos(rcMatchJoin);
	m_pBtMatchDetails->SetPos(rcMatchDetails);

	return;
}

//绘制文字
VOID CMatchViewFrame::DrawWords(CDC * pDC,INT nXPos,INT nYPos,CPngImage * pWordsImage,LPCTSTR pszWords,LPCTSTR pszText,UINT nFormat)
{
	//参数校验
	if(pWordsImage==NULL) return;

	//变量定义
	CString strWords(pszWords);
	CSize SizeWord(pWordsImage->GetWidth()/strWords.GetLength(),pWordsImage->GetHeight());

	//变量定义	
	TCHAR szText[32]=TEXT("");
	lstrcpyn(szText,pszText,CountArray(szText));

	//调整横坐标
	if((nFormat&DT_LEFT)!=0) nXPos -= 0;
	if((nFormat&DT_CENTER)!=0) nXPos -= (lstrlen(szText)*SizeWord.cx)/2;
	if((nFormat&DT_RIGHT)!=0) nXPos -= lstrlen(szText)*SizeWord.cx;
	
	//调整纵坐标
	if((nFormat&DT_TOP)!=0) nYPos -= 0;
	if((nFormat&DT_VCENTER)!=0) nYPos -= SizeWord.cy/2;
	if((nFormat&DT_BOTTOM)!=0) nYPos -= SizeWord.cy;

	//绘画数字
	INT nIndex=0;
	for (INT i=0; i<lstrlen(szText);i++)
	{
		nIndex = strWords.Find(szText[i]);
		if(nIndex!=-1)
		{
			pWordsImage->DrawImage(pDC,nXPos,nYPos,SizeWord.cx,SizeWord.cy,SizeWord.cx*nIndex,0,SizeWord.cx,SizeWord.cy);
			nXPos += SizeWord.cx;
		}
	}

	return;
}

//绘制数字
VOID CMatchViewFrame::DrawNumber(CDC * pDC,INT nXPos,INT nYPos,CPngImage * pNumberImage,LPCTSTR pszNumber,INT nNumber,LPCTSTR pszFormat,UINT nFormat)
{
	//参数校验
	if(pNumberImage==NULL) return;

	//变量定义
	CString strNumber(pszNumber);
	CSize SizeNumber(pNumberImage->GetWidth()/strNumber.GetLength(),pNumberImage->GetHeight());

	//变量定义	
	TCHAR szValue[32]=TEXT("");
	_sntprintf(szValue,CountArray(szValue),pszFormat,nNumber);

	//调整横坐标
	if((nFormat&DT_LEFT)!=0) nXPos -= 0;
	if((nFormat&DT_CENTER)!=0) nXPos -= (lstrlen(szValue)*SizeNumber.cx)/2;
	if((nFormat&DT_RIGHT)!=0) nXPos -= lstrlen(szValue)*SizeNumber.cx;
	
	//调整纵坐标
	if((nFormat&DT_TOP)!=0) nYPos -= 0;
	if((nFormat&DT_VCENTER)!=0) nYPos -= SizeNumber.cy/2;
	if((nFormat&DT_BOTTOM)!=0) nYPos -= SizeNumber.cy;

	//绘画数字
	INT nIndex=0;
	for(INT i=0; i<lstrlen(szValue);i++)
	{
		nIndex = strNumber.Find(szValue[i]);
		if(nIndex!=-1)
		{
			pNumberImage->DrawImage(pDC,nXPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*nIndex,0,SizeNumber.cx,SizeNumber.cy);
			nXPos += SizeNumber.cx;
		}
	}

	return;
}

//定时器消息
void CMatchViewFrame::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);

	if (IDI_UPDATE_TIME==nIDEvent)
	{
		//获取区域
		CRect rcClient;
		GetClientRect(&rcClient);

		//起始位置
		INT nXStartPos=rcClient.Width()/2+m_MatchPosition.ptMatchShow.x;
		INT nYStartPos=rcClient.Height()/2+m_MatchPosition.ptMatchShow.y;

		//设置区域
		CRect rcRepaint;		
		rcRepaint.left=nXStartPos+m_MatchPosition.ptNumber1.x;
		rcRepaint.top=nYStartPos+m_MatchPosition.ptNumber1.y;
		rcRepaint.right=rcRepaint.left+200;
		rcRepaint.bottom=rcRepaint.top+30;

		//刷新界面
		RedrawWindow(&rcRepaint,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////