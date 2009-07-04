// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXPORTMODEL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXPORTMODEL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef EXPORTMODEL_EXPORTS
#define EXPORTMODEL_API __declspec(dllexport)
#else
#define EXPORTMODEL_API __declspec(dllimport)
#endif

// This class is exported from the ExportModel.dll
class EXPORTMODEL_API CExportModel {
public:
	CExportModel(void);
	// TODO: add your methods here.
};

extern EXPORTMODEL_API int nExportModel;

EXPORTMODEL_API int fnExportModel(void);
