/*

Copyright (C) 2011 huchifeng@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

CONTACT: huchifeng@gmail.com
HOMEPAGE: https://github.com/huchifeng/huchifeng

*/
/*
NtQueryVolumeInformationFile
  volume id, changed after every formatting


*/

#include <time.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <windows.h>

using namespace std;

typedef struct _FILE_FS_VOLUME_INFORMATION {

	LARGE_INTEGER           VolumeCreationTime;
	ULONG                   VolumeSerialNumber;
	ULONG                   VolumeLabelLength;
	BOOLEAN                 SupportsObjects;
	WCHAR                   VolumeLabel[1];
} FILE_FS_VOLUME_INFORMATION, *PFILE_FS_VOLUME_INFORMATION;

typedef struct _IO_STATUS_BLOCK{
	NTSTATUS Status;
	ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef enum _FS_INFORMATION_CLASS {
    FileFsVolumeInformation=1,
    FileFsLabelInformation,
    FileFsSizeInformation,
    FileFsDeviceInformation,
    FileFsAttributeInformation,
    FileFsControlInformation,
    FileFsFullSizeInformation,
    FileFsObjectIdInformation,
    FileFsMaximumInformation
} FS_INFORMATION_CLASS, *PFS_INFORMATION_CLASS;

void main(){
	HMODULE h = LoadLibraryA("ntdll.dll");
	cerr << h << endl;
	void (*NtQueryVolumeInformationFile)(
		IN HANDLE               FileHandle,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		OUT PVOID               FileSystemInformation,
		IN ULONG                Length,
		IN FS_INFORMATION_CLASS FileSystemInformationClass );
	*(void**)&NtQueryVolumeInformationFile = GetProcAddress(h, "NtQueryVolumeInformationFile");
	cerr << NtQueryVolumeInformationFile << endl;
	HANDLE hDisk = CreateFileA("\\\\.\\C:", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	// fail to open, win7, need admin
        // even admin, CreateFile will frozen several seconds and return -1
	cerr << hDisk << endl;
	//NtQueryVolumeInformationFile(  );
	cerr << "ok" << endl;
}



