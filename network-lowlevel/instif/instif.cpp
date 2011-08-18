#define  UNICODE     
#define  _UNICODE         

#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <ws2spi.h>
#include <sporder.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "ole32.lib")
                           
GUID  filterguid={0x4d1e91fd,0x116a,0x44aa,{0x8f,0xd4,0x1d,0x2c,0xf2,0x7b,0xd9,0xa9}};

GUID  filterchainguid={0xd3c21121,0x85e1,0x48f3,{0x9a,0xb6,0x23,0xd9,0x0c,0x73,0x07,0xef}};

BOOL  getfilter();
void  freefilter();
void  installfilter();
void  removefilter();
void  start();
void  usage();

int                   totalprotos=0;
DWORD                 protoinfosize=0;
LPWSAPROTOCOL_INFOW   protoinfo=NULL;

int main(int argc,char *argv[])           
{
	start();

	if(argc==2)
	{
		if(strcmp(argv[1],"-install")==0)   
		{
			installfilter();
			return 0;
		}
		else if(strcmp(argv[1],"-remove")==0)  
		{
			removefilter();
			return 0;
		}
	}
	usage();
	return 0;
}

BOOL getfilter()
{
	int  errorcode;

	protoinfo=NULL;
	totalprotos=0;
	protoinfosize=0;

	if(WSCEnumProtocols(NULL,protoinfo,&protoinfosize,&errorcode)==SOCKET_ERROR)
	{
		if(errorcode!=WSAENOBUFS)
		{
			_tprintf(_T("First WSCEnumProtocols Error: %d\n"),errorcode);
			return FALSE;
		}
	}

	if((protoinfo=(LPWSAPROTOCOL_INFOW)GlobalAlloc(GPTR,protoinfosize))==NULL)
	{
		_tprintf(_T("GlobalAlloc in getfilter Error: %d\n"),GetLastError());
		return FALSE;
	}

	if((totalprotos=WSCEnumProtocols(NULL,protoinfo,&protoinfosize,&errorcode))==SOCKET_ERROR)
	{
		_tprintf(_T("Second WSCEnumProtocols Error: %d\n"),GetLastError());
		return FALSE;
	}

	_tprintf(_T("Found %d protocols!\n"),totalprotos); 
	return TRUE;
}

void freefilter()
{
	GlobalFree(protoinfo);
}

void installfilter()
{
	int                i;
	int                provcnt;
	int                cataindex;
	int                errorcode;
	BOOL               rawip=FALSE;
	BOOL               udpip=FALSE;
	DWORD              iplayercataid=0,udporigcataid=0;
	TCHAR              filter_path[MAX_PATH];            
	TCHAR              filter_name[MAX_PATH];
	TCHAR              chainname[WSAPROTOCOL_LEN+1];      
	LPDWORD            cataentries;
	WSAPROTOCOL_INFOW  iplayerinfo,udpchaininfo,chainarray[1];

	getfilter();
    
	for(i=0;i<totalprotos;i++)
	{
		if(!rawip
		&& protoinfo[i].iAddressFamily==AF_INET
		&& protoinfo[i].iProtocol==IPPROTO_IP)
		{
			rawip=TRUE;
			memcpy(&iplayerinfo,&protoinfo[i],sizeof(WSAPROTOCOL_INFOW));
			iplayerinfo.dwServiceFlags1=protoinfo[i].dwServiceFlags1 & (~XP1_IFS_HANDLES);
		}

		if(!udpip
		&& protoinfo[i].iAddressFamily==AF_INET
		&& protoinfo[i].iProtocol==IPPROTO_UDP)
		{
			udpip=TRUE;
			udporigcataid=protoinfo[i].dwCatalogEntryId;
			memcpy(&udpchaininfo,&protoinfo[i],sizeof(WSAPROTOCOL_INFOW));
			udpchaininfo.dwServiceFlags1=protoinfo[i].dwServiceFlags1 & (~XP1_IFS_HANDLES);
		}
	}

	_tcscpy(iplayerinfo.szProtocol,_T("T-IpFilter"));
	iplayerinfo.ProtocolChain.ChainLen=LAYERED_PROTOCOL;

	if(GetCurrentDirectory(MAX_PATH,filter_path)==0)
	{
		_tprintf(_T("GetCurrentDirectory Error: %d\n"),GetLastError());
		return ;
	}
	_tcscpy(filter_name,_T("\\ipfilter.dll"));  
	_tcscat(filter_path,filter_name);

	if(WSCInstallProvider(&filterguid,filter_path,&iplayerinfo,1,&errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCInstallProvider Error: %d\n"),errorcode);
		return ;
	}

	freefilter();

	getfilter();

	for(i=0;i<totalprotos;i++)
	{
		if(memcmp(&protoinfo[i].ProviderId,&filterguid,sizeof(GUID))==0)
		{
			iplayercataid=protoinfo[i].dwCatalogEntryId;
			break;
		}
	}

	provcnt=0;
	if(udpip)
	{
		_tcscpy(chainname,_T("T-UdpFilter"));
		_tcscpy(udpchaininfo.szProtocol,chainname);

		if(udpchaininfo.ProtocolChain.ChainLen==BASE_PROTOCOL)
		{
			udpchaininfo.ProtocolChain.ChainEntries[1]=udporigcataid;
		}
		else
		{
			for(i=udpchaininfo.ProtocolChain.ChainLen;i>0;i--)
			{
				udpchaininfo.ProtocolChain.ChainEntries[i+1]=udpchaininfo.ProtocolChain.ChainEntries[i];
			}
		}

		udpchaininfo.ProtocolChain.ChainLen++;
		udpchaininfo.ProtocolChain.ChainEntries[0]=iplayercataid;

		memcpy(&chainarray[provcnt++],&udpchaininfo,sizeof(WSAPROTOCOL_INFOW));
	}

	if(WSCInstallProvider(&filterchainguid,filter_path,chainarray,provcnt,&errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCInstallProvider for chain Error: %d\n"),errorcode);
		return ;
	}

	freefilter();

	getfilter();

	if((cataentries=(LPDWORD)GlobalAlloc(GPTR,totalprotos*sizeof(WSAPROTOCOL_INFOW)))==NULL)
	{
		_tprintf(_T("GlobalAlloc int installfilter Error: %d\n"),errorcode);
		return ;
	}

	cataindex=0;
	for(i=0;i<totalprotos;i++)
	{
		if(memcmp(&protoinfo[i].ProviderId,&filterguid,sizeof(GUID))==0 
		|| memcmp(&protoinfo[i].ProviderId,&filterchainguid,sizeof(GUID))==0)
		{
		cataentries[cataindex++]=protoinfo[i].dwCatalogEntryId;
		}
	}

	for(i=0;i<totalprotos;i++)
	{
		if(memcmp(&protoinfo[i].ProviderId,&filterguid,sizeof(GUID))!=0 
		&& memcmp(&protoinfo[i].ProviderId,&filterchainguid,sizeof(GUID))!=0)
		{
			cataentries[cataindex++]=protoinfo[i].dwCatalogEntryId;
		}
	}

	if((errorcode==WSCWriteProviderOrder(cataentries,totalprotos))!=ERROR_SUCCESS)
	{
		_tprintf(_T("WSCWriteProviderOrder Error: %d\n"),GetLastError());
		return ;
	}

	freefilter();

	_tprintf(_T("\nInstall IP Filter Successfully"));
	return ;
}

void removefilter()
{
	int  errorcode;
	BOOL signal=TRUE;

	if(WSCDeinstallProvider(&filterguid,&errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCDeinstall filterguid Error: %d\n"),errorcode);
		signal=FALSE;
	}

	if(WSCDeinstallProvider(&filterchainguid,&errorcode)==SOCKET_ERROR)
	{
		_tprintf(_T("WSCDeinstall filterchainguid Error: %d\n"),errorcode);
		signal=FALSE;
	}

	if(signal)
	{
		_tprintf(_T("Deinstall IP Filter Successfully"));
	}
	return ;
}

void  start()
{
	_tprintf(_T("Install IP Filter, by TOo2y\n"));
	_tprintf(_T("E-Mail: TOo2y@safechina.net\n"));
	_tprintf(_T("HomePage: www.safechina.net\n"));
	_tprintf(_T("Date: 10-29-2002\n\n"));
	return ;
}

void  usage()
{
	_tprintf(_T("Usage:  instif  [ -install | -remove ]\n"));
	return ;
}

