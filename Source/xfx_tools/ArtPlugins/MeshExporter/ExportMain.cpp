#include "pch.h"
#include "ExportMain.h"

ExportClassDesc gExportClassDesc;





//-----------------------------------------------------------------------------
//                           Methods
//-----------------------------------------------------------------------------

//
// ExportModel
//
ExportModel::ExportModel ()
{
}

ExportModel::~ExportModel ()
{
}

int ExportModel::DoExport (const TCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts, DWORD options)
{
	if (!mOptions.ShowDialog (i))
		return 1;

	mSaver.Reset (i, &mOptions);

	ei->theScene->EnumTree (&mSaver);

	mSaver.Save (name);

	return 1;
}