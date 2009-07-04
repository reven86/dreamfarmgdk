// ExportModel.cpp : Defines the entry point for the DLL application.
//

#include "pch.h"
#include "ExportModel.h"
#include "ExportMain.h"

bool gControlsInit = false;
HINSTANCE ghInstance;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (!gControlsInit)
	{
		gControlsInit = true;

		InitCustomControls (ghInstance = static_cast<HINSTANCE> (hModule));
		InitCommonControls ();
	}

    return TRUE;
}

extern "C"
{

EXPORTMODEL_API int LibNumberClasses () {return 1;};

EXPORTMODEL_API ClassDesc * LibClassDesc (int i)
{
	if (i == 0)
		return &gExportClassDesc;

	return NULL;
}

EXPORTMODEL_API const TCHAR * LibDescription ()
{
	return _T ("Mesh Exporter plugin. Copyright (C) 2005-2008 Andrew Karpushin.");
}

EXPORTMODEL_API ULONG LibVersion ()
{
	return VERSION_3DSMAX;
}

}