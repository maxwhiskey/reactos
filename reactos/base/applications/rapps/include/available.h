#pragma once

#include <windef.h>
#include <atlstr.h> 
#include <atlsimpcoll.h>
#include <atlcoll.h>
#include "misc.h"

/* EnumType flags for EnumAvailableApplications */
enum AvailableCategories
{
    ENUM_ALL_AVAILABLE,
    ENUM_CAT_AUDIO,
    ENUM_CAT_VIDEO,
    ENUM_CAT_GRAPHICS,
    ENUM_CAT_GAMES,
    ENUM_CAT_INTERNET,
    ENUM_CAT_OFFICE,
    ENUM_CAT_DEVEL,
    ENUM_CAT_EDU,
    ENUM_CAT_ENGINEER,
    ENUM_CAT_FINANCE,
    ENUM_CAT_SCIENCE,
    ENUM_CAT_TOOLS,
    ENUM_CAT_DRIVERS,
    ENUM_CAT_LIBS,
    ENUM_CAT_OTHER,
    ENUM_AVAILABLE_MIN = ENUM_ALL_AVAILABLE,
    ENUM_AVAILABLE_MAX = ENUM_CAT_OTHER
};

inline BOOL IsAvailableEnum(INT x)
{
    return (x >= ENUM_AVAILABLE_MIN && x <= ENUM_AVAILABLE_MAX);
}

typedef enum LICENSE_TYPE
{
    None,
    OpenSource,
    Freeware,
    Trial,
    Max = Trial,
    Min = None
} *PLICENSE_TYPE;

struct CAvailableApplicationInfo
{
    INT Category;
    LICENSE_TYPE LicenseType;
    ATL::CStringW szName;
    ATL::CStringW szRegName;
    ATL::CStringW szVersion;
    ATL::CStringW szLicense;
    ATL::CStringW szDesc;
    ATL::CStringW szSize;
    ATL::CStringW szUrlSite;
    ATL::CStringW szUrlDownload;
    ATL::CStringW szCDPath;
    ATL::CSimpleArray<LCID> Languages;

    // Caching mechanism related entries
    ATL::CStringW sFileName;
    FILETIME ftCacheStamp;

    // Optional integrity checks (SHA-1 digests are 160 bit = 40 characters in hex string form)
    ATL::CStringW szSHA1;
    ATL::CStringW szInstalledVersion;

    CAvailableApplicationInfo(const ATL::CStringW& sFileNameParam);

    // Load all info from the file
    VOID RefreshAppInfo();
    BOOL HasLanguageInfo() const;
    BOOL HasNativeLanguage() const;
    BOOL HasEnglishLanguage() const;
    BOOL IsInstalled() const;
    BOOL HasInstalledVersion() const;
    BOOL HasUpdate() const;

    // Set a timestamp
    VOID SetLastWriteTime(FILETIME* ftTime);

private:
    BOOL m_IsInstalled = FALSE;
    BOOL m_HasLanguageInfo = FALSE;
    BOOL m_HasInstalledVersion = FALSE;
    CConfigParser m_Parser;

    inline BOOL GetString(LPCWSTR lpKeyName, ATL::CStringW& ReturnedString);

    // Lazily load general info from the file
    VOID RetrieveGeneralInfo();
    VOID RetrieveInstalledStatus();
    VOID RetrieveInstalledVersion();
    VOID RetrieveLanguages();
    VOID RetrieveLicenseType();
    inline BOOL FindInLanguages(LCID what) const;
};

typedef BOOL(CALLBACK *AVAILENUMPROC)(CAvailableApplicationInfo *Info, LPCWSTR szFolderPath);

class CAvailableApps
{
    ATL::CAtlList<CAvailableApplicationInfo*> m_InfoList;
    static ATL::CStringW m_szPath;
    static ATL::CStringW m_szCabPath;
    static ATL::CStringW m_szAppsPath;
    static ATL::CStringW m_szSearchPath;

    static BOOL InitializeStaticStrings();

public:
    CAvailableApps();

    static BOOL UpdateAppsDB();
    static BOOL ForceUpdateAppsDB();

    VOID FreeCachedEntries();
    static VOID DeleteCurrentAppsDB();
    BOOL EnumAvailableApplications(INT EnumType, AVAILENUMPROC lpEnumProc);
    CAvailableApplicationInfo* FindInfo(const ATL::CStringW& szAppName);
    ATL::CSimpleArray<CAvailableApplicationInfo*> FindInfoList(const ATL::CSimpleArray<ATL::CStringW> &arrAppsNames);
    const ATL::CStringW& GetFolderPath();
    const ATL::CStringW& GetAppPath();
    const ATL::CStringW& GetCabPath();
    const LPCWSTR GetFolderPathString();
    const LPCWSTR GetAppPathString();
    const LPCWSTR GetCabPathString();
};