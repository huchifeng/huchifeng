#define  UNICODE
#define  _UNICODE

#include <ws2spi.h>
#include <tchar.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "ole32.lib")

GUID  filterguid={0x4d1e91fd,0x116a,0x44aa,{0x8f,0xd4,0x1d,0x2c,0xf2,0x7b,0xd9,0xa9}};

LPWSAPROTOCOL_INFOW  protoinfo=NULL;
WSPPROC_TABLE        nextproctable;
DWORD                protoinfosize=0;
int                  totalprotos=0;

BOOL getfilter()
{
	int    errorcode;

	protoinfo=NULL;
	protoinfosize=0;
	totalprotos=0;

	if(WSCEnumProtocols(NULL,protoinfo,&protoinfosize,&errorcode)==SOCKET_ERROR)
	{
		if(errorcode!=WSAENOBUFS)
		{
			OutputDebugString(_T("First WSCEnumProtocols Error!")); 
			return FALSE;
		}
	}

	if((protoinfo=(LPWSAPROTOCOL_INFOW)GlobalAlloc(GPTR,protoinfosize))==NULL)
	{
		OutputDebugString(_T("GlobalAlloc Error!"));                
		return FALSE;
	}

	if((totalprotos=WSCEnumProtocols(NULL,protoinfo,&protoinfosize,&errorcode))==SOCKET_ERROR)
	{
		OutputDebugString(_T("Second WSCEnumProtocols Error!"));   
		return FALSE;
	}

	return TRUE;
}

void freefilter()
{
	GlobalFree(protoinfo);
}


BOOL WINAPI DllMain(HINSTANCE hmodule,
DWORD     reason,
LPVOID    lpreserved)
{
	TCHAR   processname[MAX_PATH];
	TCHAR   showmessage[MAX_PATH+25];


	if(reason==DLL_PROCESS_ATTACH)
	{
		GetModuleFileName(NULL,processname,MAX_PATH);
		_tcscpy(showmessage,processname);
		_tcscat(showmessage,_T(" Loading IPFilter ..."));
		OutputDebugString(showmessage);  
	}
	return TRUE;
}

int WSPAPI WSPSendTo(SOCKET	s,
  LPWSABUF         lpbuffer,
  DWORD            dwbuffercount,
  LPDWORD          lpnumberofbytessent,
  DWORD            dwflags,
  const struct     sockaddr FAR *lpto,
  int              itolen,
  LPWSAOVERLAPPED  lpoverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE  lpcompletionroutine,
  LPWSATHREADID    lpthreadid,
  LPINT            lperrno)
{

	struct sockaddr_in sin;

	sin=*(const struct sockaddr_in *)lpto;
	if(sin.sin_port==htons(8000))        
	{
		OutputDebugString(_T("WSPSendTo Tencent Filtered"));
		return 0;
	}
	else
	{
		return nextproctable.lpWSPSendTo(s,lpbuffer,dwbuffercount,
			lpnumberofbytessent,dwflags,lpto,itolen,
			lpoverlapped,lpcompletionroutine,lpthreadid,lperrno);
	}
}

int WSPAPI WSPStartup(
WORD wversionrequested,
LPWSPDATA         lpwspdata,
LPWSAPROTOCOL_INFOW lpprotoinfo,
WSPUPCALLTABLE upcalltable,
LPWSPPROC_TABLE lpproctable
)
{
	OutputDebugString(_T("IPFilter WSPStartup ..."));

	int           i;
	int           errorcode; 
	int           filterpathlen;
	DWORD         layerid=0; 
	DWORD         nextlayerid=0;
	TCHAR         *filterpath;
	HINSTANCE     hfilter;
	LPWSPSTARTUP  wspstartupfunc=NULL;

	if(lpprotoinfo->ProtocolChain.ChainLen<=1)
	{
		OutputDebugString(_T("ChainLen<=1"));  
		return FALSE;
	}

	getfilter();

	for(i=0;i<totalprotos;i++)
	{
		if(memcmp(&protoinfo[i].ProviderId,&filterguid,sizeof(GUID))==0)
		{
			layerid=protoinfo[i].dwCatalogEntryId;
			break;
		}
	}

	for(i=0;i<lpprotoinfo->ProtocolChain.ChainLen;i++)
	{
		if(lpprotoinfo->ProtocolChain.ChainEntries[i]==layerid)
		{
			nextlayerid=lpprotoinfo->ProtocolChain.ChainEntries[i+1];
			break;
		}
	}

	filterpathlen=MAX_PATH;
	filterpath=(TCHAR*)GlobalAlloc(GPTR,filterpathlen);  
	for(i=0;i<totalprotos;i++)
	{
		if(nextlayerid==protoinfo[i].dwCatalogEntryId)
		{
			if(WSCGetProviderPath(&protoinfo[i].ProviderId,filterpath,&filterpathlen,&errorcode)==SOCKET_ERROR)
			{
				OutputDebugString(_T("WSCGetProviderPath Error!"));  
				return WSAEPROVIDERFAILEDINIT;
			}
			break;
		}
	}

	if(!ExpandEnvironmentStrings(filterpath,filterpath,MAX_PATH))
	{
		OutputDebugString(_T("ExpandEnvironmentStrings Error!"));    
		return WSAEPROVIDERFAILEDINIT;
	}

	if((hfilter=LoadLibrary(filterpath))==NULL)
	{
		OutputDebugString(_T("LoadLibrary Error!")); 
		return WSAEPROVIDERFAILEDINIT;
	}

	if((wspstartupfunc=(LPWSPSTARTUP)GetProcAddress(hfilter,"WSPStartup"))==NULL)
	{
		OutputDebugString(_T("GetProcessAddress Error!"));  
		return WSAEPROVIDERFAILEDINIT;
	}

	if((errorcode=wspstartupfunc(wversionrequested,lpwspdata,lpprotoinfo,upcalltable,lpproctable))!=ERROR_SUCCESS)
	{
		OutputDebugString(_T("wspstartupfunc Error!"));  
		return errorcode;
	}

	nextproctable=*lpproctable;
	lpproctable->lpWSPSendTo=WSPSendTo;

	freefilter();
	return 0;
}