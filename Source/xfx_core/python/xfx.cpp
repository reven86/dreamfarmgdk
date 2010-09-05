#include "xfx.h"




extern "C" {

ULONG_PTR _Py_ActivateActCtx()
{
    ULONG_PTR ret = 0;
	/*
    if (PyWin_DLLhActivationContext && pfnActivateActCtx)
        if (!(*pfnActivateActCtx)(PyWin_DLLhActivationContext, &ret)) {
            OutputDebugString("Python failed to activate the activation context before loading a DLL\n");
            ret = 0; // no promise the failing function didn't change it!
        }
		*/
    return ret;
}

void _Py_DeactivateActCtx(ULONG_PTR cookie)
{
	cookie;

	/*
    if (cookie && pfnDeactivateActCtx)
        if (!(*pfnDeactivateActCtx)(0, cookie))
            OutputDebugString("Python failed to de-activate the activation context\n");
			*/
}


}