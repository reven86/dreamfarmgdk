#include "pch.h"
#include "Options.h"
#include "resource.h"
#include <d3d8.h>

extern HINSTANCE ghInstance;





//-----------------------------------------------------------------------------
//                           Methods
//-----------------------------------------------------------------------------
INT_PTR CALLBACK sOptionsDlgProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParams);

Options::Options () :
	mExportFlags (EX_NORMAL | EX_SHORT_INDICES),
	mpInterface (NULL)
{
	mTexCoordSize[0] = 2;
	for (int i = 1; i < 8; i++)
		mTexCoordSize[i] = 0;
}

bool Options::ShowDialog (Interface * ip)
{
	mpInterface = ip;

	return (DialogBoxParam (ghInstance, MAKEINTRESOURCE (IDD_OPTIONSDIALOG), mpInterface->GetMAXHWnd (), (DLGPROC) sOptionsDlgProc, (LPARAM) this) == IDOK);
}

DWORD Options::MakeFVF () const
{
	DWORD res = D3DFVF_XYZ;

	if (mExportFlags & EX_DIFFUSE)
		res |= D3DFVF_DIFFUSE;

	if (mExportFlags & EX_NORMAL)
		res |= D3DFVF_NORMAL;

	static DWORD fvf_tex[] = {D3DFVF_TEX1, D3DFVF_TEX2, D3DFVF_TEX3, D3DFVF_TEX4, D3DFVF_TEX5, D3DFVF_TEX5, D3DFVF_TEX7, D3DFVF_TEX8};

	int ind = 0;
	for (int i = 0; i < 8; i++)
		if (mTexCoordSize[i])
		{
			res |= fvf_tex[ind++];
			
			switch (mTexCoordSize[i])
			{
			case 1:
				res |= D3DFVF_TEXCOORDSIZE1 (i);
				break;

			case 3:
				res |= D3DFVF_TEXCOORDSIZE3 (i);
				break;

			default:		// 2
				res |= D3DFVF_TEXCOORDSIZE2 (i);
				break;
			}
		}

	return res;
}

DWORD Options::GetVertexSize () const
{
	DWORD res = sizeof (xfx::Vec3);

	if (mExportFlags & EX_DIFFUSE)
		res += sizeof (DWORD);

	if (mExportFlags & EX_NORMAL)
		res += sizeof (xfx::Vec3);

	for (int i = 0; i < 8; i++)
		res += mTexCoordSize[i] * sizeof (float);

	return res;
}

#define INIT_COMBO(ind) \
	SendDlgItemMessage (hWnd, IDC_MAP##ind##COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM> ("0")); \
	SendDlgItemMessage (hWnd, IDC_MAP##ind##COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM> ("1")); \
	SendDlgItemMessage (hWnd, IDC_MAP##ind##COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM> ("2")); \
	SendDlgItemMessage (hWnd, IDC_MAP##ind##COMBO, CB_ADDSTRING, 0, reinterpret_cast<LPARAM> ("3")); \
	SendDlgItemMessage (hWnd, IDC_MAP##ind##COMBO, CB_SETCURSEL, TexCoordSize (ind - 1), 0);

INT_PTR CALLBACK Options::OptionsDlgProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		CenterWindow (hWnd, GetParent (hWnd));

		CheckDlgButton (hWnd, IDC_DIFFUSECOLORCHECK, ExportFlags () & EX_DIFFUSE);
		CheckDlgButton (hWnd, IDC_NORMALCHECK, ExportFlags () & EX_NORMAL);
		CheckDlgButton (hWnd, IDC_SHORTINDICESCHECK, ExportFlags () & EX_SHORT_INDICES);

		INIT_COMBO (1);
		INIT_COMBO (2);
		INIT_COMBO (3);
		INIT_COMBO (4);
		INIT_COMBO (5);
		INIT_COMBO (6);
		INIT_COMBO (7);
		INIT_COMBO (8);
		break;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			if (IsDlgButtonChecked (hWnd, IDC_DIFFUSECOLORCHECK))
				mExportFlags |= EX_DIFFUSE;
			else
				mExportFlags &= ~EX_DIFFUSE;
			
			if (IsDlgButtonChecked (hWnd, IDC_NORMALCHECK))
				mExportFlags |= EX_NORMAL;
			else
				mExportFlags &= ~EX_NORMAL;

			if (IsDlgButtonChecked (hWnd, IDC_SHORTINDICESCHECK))
				mExportFlags |= EX_SHORT_INDICES;
			else
				mExportFlags &= ~EX_SHORT_INDICES;

			mTexCoordSize[0] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP1COMBO, CB_GETCURSEL, 0, 0));
			mTexCoordSize[1] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP2COMBO, CB_GETCURSEL, 0, 0));
			mTexCoordSize[2] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP3COMBO, CB_GETCURSEL, 0, 0));
			mTexCoordSize[3] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP4COMBO, CB_GETCURSEL, 0, 0));
			mTexCoordSize[4] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP5COMBO, CB_GETCURSEL, 0, 0));
			mTexCoordSize[5] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP6COMBO, CB_GETCURSEL, 0, 0));
			mTexCoordSize[6] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP7COMBO, CB_GETCURSEL, 0, 0));
			mTexCoordSize[7] = static_cast<DWORD> (SendDlgItemMessage (hWnd, IDC_MAP8COMBO, CB_GETCURSEL, 0, 0));

			EndDialog (hWnd, IDOK);
			break;

		case IDCANCEL:
			EndDialog (hWnd, IDCANCEL);
			break;
		}
		break;

	default:
		return FALSE;
	}

	return TRUE;
};



//
// DlgProc
//

static INT_PTR CALLBACK sOptionsDlgProc (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		SetWindowLongPtr (hWnd, GWLP_USERDATA, static_cast<LONG> (lParam));
		break;
	};

	Options * op = reinterpret_cast<Options *> (static_cast<LPARAM> (GetWindowLongPtr (hWnd, GWLP_USERDATA)));

	return op ? op->OptionsDlgProc (hWnd, msg, wParam, lParam) : FALSE;
};