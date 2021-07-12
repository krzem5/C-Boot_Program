#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <inspectable.h>
#include <objectarray.h>
#include <main.h>



const CLSID CLSID_ImmersiveShell={0xc2f03a33,0x21f5,0x47fa,0xb4,0xbb,0x15,0x63,0x62,0xa2,0xf2,0x39};
const CLSID CLSID_VirtualDesktopManagerInternal={0xc5e0cdca,0x7b6e,0x41b2,0x9f,0xc4,0xd9,0x39,0x75,0xcc,0x46,0x7b};



#define APPLICATION_VIEW_CLOAK_TYPE UINT
#define APPLICATION_VIEW_COMPATIBILITY_POLICY UINT
#define IApplicationViewChangeListener UINT
#define IApplicationViewOperation UINT
#define IApplicationViewPosition UINT
#define IAsyncCallback UINT
#define IImmersiveApplication UINT
#define IImmersiveMonitor UINT
#define IShellPositionerPriority UINT



enum AdjacentDesktop{
	LeftDirection=3,
	RightDirection=4
};



DECLARE_INTERFACE_IID_(IApplicationView,IInspectable,"372E1D3B-38D3-42E4-A15B-8AB2B178F513"){
	STDMETHOD(QueryInterface)(THIS_ REFIID riid,LPVOID FAR* ppvObject) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetIids)(__RPC__out ULONG*iidCount,__RPC__deref_out_ecount_full_opt(*iidCount) IID**iids) PURE;
	STDMETHOD(GetRuntimeClassName)(__RPC__deref_out_opt HSTRING*className) PURE;
	STDMETHOD(GetTrustLevel)(__RPC__out TrustLevel*trustLevel) PURE;
	STDMETHOD(SetFocus)(THIS) PURE;
	STDMETHOD(SwitchTo)(THIS) PURE;
	STDMETHOD(TryInvokeBack)(THIS_ IAsyncCallback*) PURE;
	STDMETHOD(GetThumbnailWindow)(THIS_ HWND*) PURE;
	STDMETHOD(GetMonitor)(THIS_ IImmersiveMonitor**) PURE;
	STDMETHOD(GetVisibility)(THIS_ int*) PURE;
	STDMETHOD(SetCloak)(THIS_ APPLICATION_VIEW_CLOAK_TYPE,int) PURE;
	STDMETHOD(GetPosition)(THIS_ REFIID,void**) PURE;
	STDMETHOD(SetPosition)(THIS_ IApplicationViewPosition*) PURE;
	STDMETHOD(InsertAfterWindow)(THIS_ HWND) PURE;
	STDMETHOD(GetExtendedFramePosition)(THIS_ RECT*) PURE;
	STDMETHOD(GetAppUserModelId)(THIS_ PWSTR*) PURE;
	STDMETHOD(SetAppUserModelId)(THIS_ PCWSTR) PURE;
	STDMETHOD(IsEqualByAppUserModelId)(THIS_ PCWSTR,int*) PURE;
	STDMETHOD(GetViewState)(THIS_ UINT*) PURE;
	STDMETHOD(SetViewState)(THIS_ UINT) PURE;
	STDMETHOD(GetNeediness)(THIS_ int*) PURE;
	STDMETHOD(GetLastActivationTimestamp)(THIS_ ULONGLONG*) PURE;
	STDMETHOD(SetLastActivationTimestamp)(THIS_ ULONGLONG) PURE;
	STDMETHOD(GetVirtualDesktopId)(THIS_ GUID*) PURE;
	STDMETHOD(SetVirtualDesktopId)(THIS_ REFGUID) PURE;
	STDMETHOD(GetShowInSwitchers)(THIS_ int*) PURE;
	STDMETHOD(SetShowInSwitchers)(THIS_ int) PURE;
	STDMETHOD(GetScaleFactor)(THIS_ int*) PURE;
	STDMETHOD(CanReceiveInput)(THIS_ BOOL*) PURE;
	STDMETHOD(GetCompatibilityPolicyType)(THIS_ APPLICATION_VIEW_COMPATIBILITY_POLICY*) PURE;
	STDMETHOD(SetCompatibilityPolicyType)(THIS_ APPLICATION_VIEW_COMPATIBILITY_POLICY) PURE;
	STDMETHOD(GetSizeConstraints)(THIS_ IImmersiveMonitor*,SIZE*,SIZE*) PURE;
	STDMETHOD(GetSizeConstraintsForDpi)(THIS_ UINT,SIZE*,SIZE*) PURE;
	STDMETHOD(SetSizeConstraintsForDpi)(THIS_ const UINT*,const SIZE*,const SIZE*) PURE;
	STDMETHOD(OnMinSizePreferencesUpdated)(THIS_ HWND) PURE;
	STDMETHOD(ApplyOperation)(THIS_ IApplicationViewOperation*) PURE;
	STDMETHOD(IsTray)(THIS_ BOOL*) PURE;
	STDMETHOD(IsInHighZOrderBand)(THIS_ BOOL*) PURE;
	STDMETHOD(IsSplashScreenPresented)(THIS_ BOOL*) PURE;
	STDMETHOD(Flash)(THIS) PURE;
	STDMETHOD(GetRootSwitchableOwner)(THIS_ IApplicationView**) PURE;
	STDMETHOD(EnumerateOwnershipTree)(THIS_ IObjectArray**) PURE;
	STDMETHOD(GetEnterpriseId)(THIS_ PWSTR*) PURE;
	STDMETHOD(IsMirrored)(THIS_ BOOL*) PURE;
	STDMETHOD(Unknown1)(THIS_ int*) PURE;
	STDMETHOD(Unknown2)(THIS_ int*) PURE;
	STDMETHOD(Unknown3)(THIS_ int*) PURE;
	STDMETHOD(Unknown4)(THIS_ int) PURE;
	STDMETHOD(Unknown5)(THIS_ int*) PURE;
	STDMETHOD(Unknown6)(THIS_ int) PURE;
	STDMETHOD(Unknown7)(THIS) PURE;
	STDMETHOD(Unknown8)(THIS_ int*) PURE;
	STDMETHOD(Unknown9)(THIS_ int) PURE;
	STDMETHOD(Unknown10)(THIS_ int,int) PURE;
	STDMETHOD(Unknown11)(THIS_ int) PURE;
	STDMETHOD(Unknown12)(THIS_ SIZE*) PURE;
};



DECLARE_INTERFACE_IID_(IApplicationViewCollection,IUnknown,"1841C6D7-4F9D-42C0-AF41-8747538F10E5"){
	STDMETHOD(QueryInterface)(THIS_ REFIID riid,LPVOID FAR* ppvObject) PURE;
	STDMETHOD_(ULONG,AddRef)(THIS) PURE;
	STDMETHOD_(ULONG,Release)(THIS) PURE;
	STDMETHOD(GetViews)(THIS_ IObjectArray**) PURE;
	STDMETHOD(GetViewsByZOrder)(THIS_ IObjectArray**) PURE;
	STDMETHOD(GetViewsByAppUserModelId)(THIS_ PCWSTR,IObjectArray**) PURE;
	STDMETHOD(GetViewForHwnd)(THIS_ HWND,IApplicationView**) PURE;
	STDMETHOD(GetViewForApplication)(THIS_ IImmersiveApplication*,IApplicationView**) PURE;
	STDMETHOD(GetViewForAppUserModelId)(THIS_ PCWSTR,IApplicationView**) PURE;
	STDMETHOD(GetViewInFocus)(THIS_ IApplicationView**) PURE;
	STDMETHOD(Unknown1)(THIS_ IApplicationView**) PURE;
	STDMETHOD(RefreshCollection)(THIS) PURE;
	STDMETHOD(RegisterForApplicationViewChanges)(THIS_ IApplicationViewChangeListener*,DWORD*) PURE;
	STDMETHOD(UnregisterForApplicationViewChanges)(THIS_ DWORD) PURE;
};



MIDL_INTERFACE("FF72FFDD-BE7E-43FC-9C03-AD81681E88E4")
IVirtualDesktop:public IUnknown{
	public:
		virtual HRESULT STDMETHODCALLTYPE IsViewVisible(IApplicationView*,int*)=0;
		virtual HRESULT STDMETHODCALLTYPE GetID(GUID*)=0;
};



MIDL_INTERFACE("a5cd92ff-29be-454c-8d04-d82879fb3f1b")
IVirtualDesktopManager:public IUnknown{
	public:
		virtual HRESULT STDMETHODCALLTYPE IsWindowOnCurrentVirtualDesktop(__RPC__in HWND,__RPC__out BOOL*)=0;
		virtual HRESULT STDMETHODCALLTYPE GetWindowDesktopId(__RPC__in HWND,__RPC__out GUID*)=0;
		virtual HRESULT STDMETHODCALLTYPE MoveWindowToDesktop(__RPC__in HWND,__RPC__in REFGUID)=0;
};



MIDL_INTERFACE("f31574d6-b682-4cdc-bd56-1827860abec6")
IVirtualDesktopManagerInternal:public IUnknown{
	public:
		virtual HRESULT STDMETHODCALLTYPE GetCount(UINT*)=0;
		virtual HRESULT STDMETHODCALLTYPE MoveViewToDesktop(IApplicationView*,IVirtualDesktop*)=0;
		virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(IApplicationView*,int*)=0;
		virtual HRESULT STDMETHODCALLTYPE GetCurrentDesktop(IVirtualDesktop** )=0;
		virtual HRESULT STDMETHODCALLTYPE GetDesktops(IObjectArray**)=0;
		virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(IVirtualDesktop*,AdjacentDesktop,IVirtualDesktop**)=0;
		virtual HRESULT STDMETHODCALLTYPE SwitchDesktop(IVirtualDesktop*)=0;
		virtual HRESULT STDMETHODCALLTYPE CreateDesktopW(IVirtualDesktop**)=0;
		virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(IVirtualDesktop*,IVirtualDesktop*)=0;
		virtual HRESULT STDMETHODCALLTYPE FindDesktop(GUID*,IVirtualDesktop**)=0;
};



extern "C" BOOL _move_to_desktop(HWND hwnd,UINT n){
	CoInitialize(NULL);
	IServiceProvider* sp=nullptr;
	CoCreateInstance(CLSID_ImmersiveShell,NULL,CLSCTX_LOCAL_SERVER,__uuidof(IServiceProvider),(PVOID*)&sp);
	IApplicationViewCollection* avc=nullptr;
	sp->QueryService(__uuidof(IApplicationViewCollection),&avc);
	IVirtualDesktopManager* dm=nullptr;
	sp->QueryService(__uuidof(IVirtualDesktopManager),&dm);
	IVirtualDesktopManagerInternal* dmi=nullptr;
	sp->QueryService(CLSID_VirtualDesktopManagerInternal,__uuidof(IVirtualDesktopManagerInternal),(PVOID*)&dmi);
	IObjectArray* dl=nullptr;
	HRESULT hr=dmi->GetDesktops(&dl);
	IVirtualDesktop* td=nullptr;
	dl->GetAt(n,__uuidof(IVirtualDesktop),(void**)&td);
	dl->Release();
	IApplicationView* av=nullptr;
	avc->GetViewForHwnd(hwnd,&av);
	if (!hwnd||!td||!av){
		dmi->Release();
		dm->Release();
		avc->Release();
		sp->Release();
		return 1;
	}
	dmi->MoveViewToDesktop(av,td);
	dmi->Release();
	dm->Release();
	avc->Release();
	sp->Release();
	return 0;
}



extern "C" BOOL _switch_to_desktop(UINT n){
	CoInitialize(NULL);
	IServiceProvider* sp=nullptr;
	CoCreateInstance(CLSID_ImmersiveShell,NULL,CLSCTX_LOCAL_SERVER,__uuidof(IServiceProvider),(PVOID*)&sp);
	IVirtualDesktopManager* dm=nullptr;
	sp->QueryService(__uuidof(IVirtualDesktopManager),&dm);
	IVirtualDesktopManagerInternal* dmi=nullptr;
	sp->QueryService(CLSID_VirtualDesktopManagerInternal,__uuidof(IVirtualDesktopManagerInternal),(PVOID*)&dmi);
	IObjectArray* dl=nullptr;
	HRESULT hr=dmi->GetDesktops(&dl);
	IVirtualDesktop* td=nullptr;
	dl->GetAt(n,__uuidof(IVirtualDesktop),(void**)&td);
	dl->Release();
	if (!td){
		dmi->Release();
		dm->Release();
		sp->Release();
		return 1;
	}
	dmi->SwitchDesktop(td);
	dmi->Release();
	dm->Release();
	sp->Release();
	return 0;
}
