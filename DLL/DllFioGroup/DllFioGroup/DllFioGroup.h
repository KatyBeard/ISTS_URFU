// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the DLLFIOGROUP_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// DLLFIOGROUP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLLFIOGROUP_EXPORTS
#define DLLFIOGROUP_API __declspec(dllexport)
#else
#define DLLFIOGROUP_API __declspec(dllimport)
#endif

// This class is exported from the dll
class DLLFIOGROUP_API CDllFioGroup {
public:
	CDllFioGroup(void);
	// TODO: add your methods here.
};

extern DLLFIOGROUP_API int nDllFioGroup;

DLLFIOGROUP_API int fnDllFioGroup(void);
