#ifndef __EXPORT_MAIN__
#define __EXPORT_MAIN__

#include "Options.h"
#include "SceneSaver.h"





//-----------------------------------------------------------------------------
//                           Classes
//-----------------------------------------------------------------------------

//
// Export model base class
//

class ExportModel : public SceneExport
{
	Options							mOptions;
	SceneSaver						mSaver;

public:
	ExportModel														();
	virtual ~ExportModel											();

	virtual int						ExtCount						() {return 1;};
	virtual const TCHAR *			Ext								(int i) {return i ? _T ("") : _T ("mesh");};
	virtual const TCHAR *			LongDesc						() {return _T ("Dream Farm Games - Mesh Exporter for Dream Farm GDK");};
	virtual const TCHAR *			ShortDesc						() {return _T ("DFG MeshExporter");};
	virtual const TCHAR *			AuthorName						() {return _T ("Andrew Karpushin");};
	virtual const TCHAR *			CopyrightMessage				() {return _T ("Copyright (C) 2005-2008 Andrew Karpushin.");};
	virtual const TCHAR *			OtherMessage1					() {return _T ("");};
	virtual const TCHAR *			OtherMessage2					() {return _T ("");};
	virtual unsigned int			Version							() {return 100;};
	virtual void					ShowAbout						(HWND hWnd) {};

	virtual void					DeleteThis						() {delete this;};

	virtual int						DoExport						(const TCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts = FALSE, DWORD options = 0);
};



//
// Export model class desc
//

class ExportClassDesc : public ClassDesc
{
public:
	int								IsPublic						() {return TRUE;};
	void *							Create							(BOOL loading = FALSE) {return new ExportModel ();};
	const TCHAR *					ClassName						() {return _T ("MeshExporter");};
	SClass_ID						SuperClassID					() {return SCENE_EXPORT_CLASS_ID;};
	Class_ID						ClassID							() {return Class_ID (0x56724073, 0x6438a7);};
	const TCHAR *					Category						() {return _T ("");};
};

extern ExportClassDesc gExportClassDesc;



#endif