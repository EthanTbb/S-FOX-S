#pragma comment(lib, "comsuppw.lib")

//////////////////////////////////////////////////////////////////////////
// Interfaces imported from flash.ocx

#include <comdef.h>

#pragma pack(push, 8)

namespace ShockwaveFlashObjects2 
{

//
// Forward references and typedefs
//

struct __declspec(uuid("d27cdb6b-ae6d-11cf-96b8-444553540000"))
/* LIBID */ __ShockwaveFlashObjects1;
struct __declspec(uuid("d27cdb6c-ae6d-11cf-96b8-444553540000"))
/* dual interface */ IShockwaveFlash2;
struct __declspec(uuid("d27cdb6d-ae6d-11cf-96b8-444553540000"))
/* dispinterface */ _IShockwaveFlashEvents2;
struct /* coclass */ ShockwaveFlash1;
struct /* coclass */ FlashProp1;
struct __declspec(uuid("d27cdb70-ae6d-11cf-96b8-444553540000"))
/* interface */ IFlashFactory2;
struct __declspec(uuid("d27cdb72-ae6d-11cf-96b8-444553540000"))
/* interface */ IFlashObjectInterface2;
//struct __declspec(uuid("a6ef9860-c720-11d0-9337-00a0c90dcaa9"))
///* interface */ IDispatchEx;
struct /* coclass */ FlashObjectInterface2;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(IShockwaveFlash2, __uuidof(IShockwaveFlash2));
_COM_SMARTPTR_TYPEDEF(_IShockwaveFlashEvents2, __uuidof(_IShockwaveFlashEvents2));
_COM_SMARTPTR_TYPEDEF(IFlashFactory2, __uuidof(IFlashFactory2));
//_COM_SMARTPTR_TYPEDEF(IDispatchEx, __uuidof(IDispatchEx));
_COM_SMARTPTR_TYPEDEF(IFlashObjectInterface2, __uuidof(IFlashObjectInterface2));

//
// Type library items
//

struct __declspec(uuid("d27cdb6c-ae6d-11cf-96b8-444553540000"))
IShockwaveFlash2 : IDispatch
{
    //
    // Property data
    //

    __declspec(property(get=GetStacking,put=PutStacking))
    _bstr_t Stacking;
    __declspec(property(get=GetWMode,put=PutWMode))
    _bstr_t WMode;
    __declspec(property(get=GetMovie,put=PutMovie))
    _bstr_t Movie;
    __declspec(property(get=GetSAlign,put=PutSAlign))
    _bstr_t SAlign;
    __declspec(property(get=GetMenu,put=PutMenu))
    VARIANT_BOOL Menu;
    __declspec(property(get=GetQuality,put=PutQuality))
    int Quality;
    __declspec(property(get=GetLoop,put=PutLoop))
    VARIANT_BOOL Loop;
    __declspec(property(get=GetFrameNum,put=PutFrameNum))
    long FrameNum;
    __declspec(property(get=GetBase,put=PutBase))
    _bstr_t Base;
    __declspec(property(get=Getscale,put=Putscale))
    _bstr_t scale;
    __declspec(property(get=GetDeviceFont,put=PutDeviceFont))
    VARIANT_BOOL DeviceFont;
    __declspec(property(get=GetEmbedMovie,put=PutEmbedMovie))
    VARIANT_BOOL EmbedMovie;
    __declspec(property(get=GetBGColor,put=PutBGColor))
    _bstr_t BGColor;
    __declspec(property(get=GetQuality2,put=PutQuality2))
    _bstr_t Quality2;
    __declspec(property(get=GetReadyState))
    long ReadyState;
    __declspec(property(get=GetScaleMode,put=PutScaleMode))
    int ScaleMode;
    __declspec(property(get=GetAlignMode,put=PutAlignMode))
    int AlignMode;
    __declspec(property(get=GetBackgroundColor,put=PutBackgroundColor))
    long BackgroundColor;
    __declspec(property(get=GetTotalFrames))
    long TotalFrames;
    __declspec(property(get=GetPlaying,put=PutPlaying))
    VARIANT_BOOL Playing;
    __declspec(property(get=GetSWRemote,put=PutSWRemote))
    _bstr_t SWRemote;

    //
    // Raw methods provided by interface
    //

      virtual HRESULT __stdcall get_ReadyState (				//
        /*[out,retval]*/ long * thestate ) = 0;
      virtual HRESULT __stdcall get_TotalFrames (				// 获取动画总帧数
        /*[out,retval]*/ long * numframes ) = 0;
      virtual HRESULT __stdcall get_Playing (					// 获取是否播放中
        /*[out,retval]*/ VARIANT_BOOL * Playing ) = 0;
      virtual HRESULT __stdcall put_Playing (					// 
        /*[in]*/ VARIANT_BOOL Playing ) = 0;
      virtual HRESULT __stdcall get_Quality (
        /*[out,retval]*/ int * Quality ) = 0;
      virtual HRESULT __stdcall put_Quality (
        /*[in]*/ int Quality ) = 0;
      virtual HRESULT __stdcall get_ScaleMode (
        /*[out,retval]*/ int * scale ) = 0;
      virtual HRESULT __stdcall put_ScaleMode (
        /*[in]*/ int scale ) = 0;
      virtual HRESULT __stdcall get_AlignMode (
        /*[out,retval]*/ int * align ) = 0;
      virtual HRESULT __stdcall put_AlignMode (
        /*[in]*/ int align ) = 0;
      virtual HRESULT __stdcall get_BackgroundColor (
        /*[out,retval]*/ long * color ) = 0;
      virtual HRESULT __stdcall put_BackgroundColor (
        /*[in]*/ long color ) = 0;
      virtual HRESULT __stdcall get_Loop (
        /*[out,retval]*/ VARIANT_BOOL * Loop ) = 0;
      virtual HRESULT __stdcall put_Loop (
        /*[in]*/ VARIANT_BOOL Loop ) = 0;
      virtual HRESULT __stdcall get_Movie (
        /*[out,retval]*/ BSTR * path ) = 0;
      virtual HRESULT __stdcall put_Movie (
        /*[in]*/ BSTR path ) = 0;
      virtual HRESULT __stdcall get_FrameNum (
        /*[out,retval]*/ long * FrameNum ) = 0;
      virtual HRESULT __stdcall put_FrameNum (
        /*[in]*/ long FrameNum ) = 0;
      virtual HRESULT __stdcall SetZoomRect (
        /*[in]*/ long left,
        /*[in]*/ long top,
        /*[in]*/ long right,
        /*[in]*/ long bottom ) = 0;
      virtual HRESULT __stdcall Zoom (
        /*[in]*/ int factor ) = 0;
      virtual HRESULT __stdcall Pan (
        /*[in]*/ long x,
        /*[in]*/ long y,
        /*[in]*/ int mode ) = 0;
      virtual HRESULT __stdcall Play ( ) = 0;					// 播放
      virtual HRESULT __stdcall Stop ( ) = 0;					// 停止
      virtual HRESULT __stdcall Back ( ) = 0;					// 上一帧
      virtual HRESULT __stdcall Forward ( ) = 0;				// 快进
      virtual HRESULT __stdcall Rewind ( ) = 0;					// 返回动画第一帧
      virtual HRESULT __stdcall StopPlay ( ) = 0;				// 停止播放
      virtual HRESULT __stdcall GotoFrame (						// 跳转到指定帧
		/*[in]*/ long FrameNum ) = 0;
      virtual HRESULT __stdcall CurrentFrame (					// 返回当前帧
		/*[out,retval]*/ long * FrameNum ) = 0;
      virtual HRESULT __stdcall IsPlaying (						// 动画是否正在播放
		/*[out,retval]*/ VARIANT_BOOL * Playing ) = 0;
      virtual HRESULT __stdcall PercentLoaded (					// 动画加载的进度百分比
		/*[out,retval]*/ long * __MIDL_0011 ) = 0;
      virtual HRESULT __stdcall FrameLoaded (					// 帧
		/*[in]*/ long FrameNum,
		/*[out,retval]*/ VARIANT_BOOL * loaded ) = 0;
      virtual HRESULT __stdcall FlashVersion (					// Flash版本
        /*[out,retval]*/ long * version ) = 0;
      virtual HRESULT __stdcall get_WMode (						//
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_WMode (						//
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_SAlign (					//
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_SAlign (					//
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Menu (						//
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_Menu (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_Base (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Base (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_scale (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_scale (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_DeviceFont (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_DeviceFont (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_EmbedMovie (
        /*[out,retval]*/ VARIANT_BOOL * pVal ) = 0;
      virtual HRESULT __stdcall put_EmbedMovie (
        /*[in]*/ VARIANT_BOOL pVal ) = 0;
      virtual HRESULT __stdcall get_BGColor (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_BGColor (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Quality2 (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Quality2 (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall LoadMovie (
        /*[in]*/ int layer,
        /*[in]*/ BSTR url ) = 0;
      virtual HRESULT __stdcall TGotoFrame (
        /*[in]*/ BSTR target,
        /*[in]*/ long FrameNum ) = 0;
      virtual HRESULT __stdcall TGotoLabel (
        /*[in]*/ BSTR target,
        /*[in]*/ BSTR label ) = 0;
      virtual HRESULT __stdcall TCurrentFrame (
        /*[in]*/ BSTR target,
        /*[out,retval]*/ long * FrameNum ) = 0;
      virtual HRESULT __stdcall TCurrentLabel (
        /*[in]*/ BSTR target,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall TPlay (
        /*[in]*/ BSTR target ) = 0;
      virtual HRESULT __stdcall TStopPlay (
        /*[in]*/ BSTR target ) = 0;
      virtual HRESULT __stdcall SetVariable (
        /*[in]*/ BSTR name,
        /*[in]*/ BSTR value ) = 0;
      virtual HRESULT __stdcall GetVariable (
        /*[in]*/ BSTR name,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall TSetProperty (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[in]*/ BSTR value ) = 0;
      virtual HRESULT __stdcall TGetProperty (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall TCallFrame (
        /*[in]*/ BSTR target,
        /*[in]*/ int FrameNum ) = 0;
      virtual HRESULT __stdcall TCallLabel (
        /*[in]*/ BSTR target,
        /*[in]*/ BSTR label ) = 0;
      virtual HRESULT __stdcall TSetPropertyNum (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[in]*/ double value ) = 0;
      virtual HRESULT __stdcall TGetPropertyNum (
        /*[in]*/ BSTR target,
        /*[in]*/ int property,
        /*[out,retval]*/ double * pVal ) = 0;
      virtual HRESULT __stdcall get_SWRemote (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_SWRemote (
        /*[in]*/ BSTR pVal ) = 0;
      virtual HRESULT __stdcall get_Stacking (
        /*[out,retval]*/ BSTR * pVal ) = 0;
      virtual HRESULT __stdcall put_Stacking (
        /*[in]*/ BSTR pVal ) = 0;
};

struct __declspec(uuid("d27cdb6d-ae6d-11cf-96b8-444553540000"))
_IShockwaveFlashEvents2 : IDispatch
{
    //
    // Wrapper methods for error-handling
    //

    // Methods:
    HRESULT OnReadyStateChange (
        long newState );
    HRESULT OnProgress (
        long percentDone );
    HRESULT FSCommand (
        _bstr_t command,
        _bstr_t args );
};

struct __declspec(uuid("d27cdb6e-ae6d-11cf-96b8-444553540000"))
ShockwaveFlash1;
    // [ default ] interface IShockwaveFlash2
    // [ default, source ] dispinterface _IShockwaveFlashEvents2

struct __declspec(uuid("1171a62f-05d2-11d1-83fc-00a0c9089c5a"))
FlashProp1;
    // [ default ] interface IUnknown

struct __declspec(uuid("d27cdb70-ae6d-11cf-96b8-444553540000"))
IFlashFactory2 : IUnknown
{};
//
//struct __declspec(uuid("a6ef9860-c720-11d0-9337-00a0c90dcaa9"))
//IDispatchEx : IDispatch
//{
//    //
//    // Wrapper methods for error-handling
//    //
//
//    HRESULT GetDispID (
//        _bstr_t bstrName,
//        unsigned long grfdex,
//        long * pid );
//    HRESULT RemoteInvokeEx (
//        long id,
//        unsigned long lcid,
//        unsigned long dwFlags,
//        struct DISPPARAMS * pdp,
//        VARIANT * pvarRes,
//        struct EXCEPINFO * pei,
//        struct IServiceProvider * pspCaller,
//        unsigned int cvarRefArg,
//        unsigned int * rgiRefArg,
//        VARIANT * rgvarRefArg );
//    HRESULT DeleteMemberByName (
//        _bstr_t bstrName,
//        unsigned long grfdex );
//    HRESULT DeleteMemberByDispID (
//        long id );
//    HRESULT GetMemberProperties (
//        long id,
//        unsigned long grfdexFetch,
//        unsigned long * pgrfdex );
//    HRESULT GetMemberName (
//        long id,
//        BSTR * pbstrName );
//    HRESULT GetNextDispID (
//        unsigned long grfdex,
//        long id,
//        long * pid );
//    HRESULT GetNameSpaceParent (
//        IUnknown * * ppunk );
//
//    //
//    // Raw methods provided by interface
//    //
//
//      virtual HRESULT __stdcall raw_GetDispID (
//        /*[in]*/ BSTR bstrName,
//        /*[in]*/ unsigned long grfdex,
//        /*[out]*/ long * pid ) = 0;
//      virtual HRESULT __stdcall raw_RemoteInvokeEx (
//        /*[in]*/ long id,
//        /*[in]*/ unsigned long lcid,
//        /*[in]*/ unsigned long dwFlags,
//        /*[in]*/ struct DISPPARAMS * pdp,
//        /*[out]*/ VARIANT * pvarRes,
//        /*[out]*/ struct EXCEPINFO * pei,
//        /*[in]*/ struct IServiceProvider * pspCaller,
//        /*[in]*/ unsigned int cvarRefArg,
//        /*[in]*/ unsigned int * rgiRefArg,
//        /*[in,out]*/ VARIANT * rgvarRefArg ) = 0;
//      virtual HRESULT __stdcall raw_DeleteMemberByName (
//        /*[in]*/ BSTR bstrName,
//        /*[in]*/ unsigned long grfdex ) = 0;
//      virtual HRESULT __stdcall raw_DeleteMemberByDispID (
//        /*[in]*/ long id ) = 0;
//      virtual HRESULT __stdcall raw_GetMemberProperties (
//        /*[in]*/ long id,
//        /*[in]*/ unsigned long grfdexFetch,
//        /*[out]*/ unsigned long * pgrfdex ) = 0;
//      virtual HRESULT __stdcall raw_GetMemberName (
//        /*[in]*/ long id,
//        /*[out]*/ BSTR * pbstrName ) = 0;
//      virtual HRESULT __stdcall raw_GetNextDispID (
//        /*[in]*/ unsigned long grfdex,
//        /*[in]*/ long id,
//        /*[out]*/ long * pid ) = 0;
//      virtual HRESULT __stdcall raw_GetNameSpaceParent (
//        /*[out]*/ IUnknown * * ppunk ) = 0;
//};

//struct __declspec(uuid("d27cdb72-ae6d-11cf-96b8-444553540000"))
//IFlashObjectInterface2 : IDispatchEx
//{};

struct __declspec(uuid("d27cdb71-ae6d-11cf-96b8-444553540000"))
FlashObjectInterface2;
    // [ default ] interface IFlashObjectInterface2

} // namespace ShockwaveFlashObjects2

#pragma pack(pop)

using namespace ShockwaveFlashObjects2;

const CLSID CLSID_ShockwaveFlash =
  { 0xD27CDB6E, 0xAE6D, 0x11cf, {0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00} };

typedef HRESULT (__stdcall *DllGetClassObjectFunc)(REFCLSID rclsid, REFIID riid, LPVOID * ppv); 

