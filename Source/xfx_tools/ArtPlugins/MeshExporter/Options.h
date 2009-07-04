#ifndef __OPTIONS__
#define __OPTIONS__





//-----------------------------------------------------------------------------
//                           Classes
//-----------------------------------------------------------------------------

class Options
{
public:
	enum ExportFlags
	{
		EX_DIFFUSE				= (1 << 0),
		EX_NORMAL				= (1 << 1),
		EX_SHORT_INDICES		= (1 << 2),
	};

private:
	DWORD						mExportFlags;
	DWORD						mTexCoordSize[8];

	Interface					* mpInterface;

public:
	Options																();
	~Options															() {};

	const DWORD&						ExportFlags						() const {return mExportFlags;};
	const DWORD&						TexCoordSize					(int level) const {return mTexCoordSize[level];};

	DWORD								MakeFVF							() const;
	DWORD								GetVertexSize					() const;

	bool								ShowDialog						(Interface * ip);

	INT_PTR CALLBACK					OptionsDlgProc					(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};



#endif