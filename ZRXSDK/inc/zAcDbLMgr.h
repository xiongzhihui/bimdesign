

#ifndef   _ZCDBLMGR_H
#define   _ZCDBLMGR_H

#include "zadesk.h"
#include "zdbid.h"
#include "zdblayout.h"
#include "zdblaymgrrctr.h"
#pragma pack (push, 8)

#define  MAX_PSPACE_LAYOUTS     256

class ZcDbObjectId;

class ZcDbLayoutManager: public ZcRxObject  {
  public:

    virtual Zcad::ErrorStatus
        setCurrentLayout        (const ZTCHAR * newname) = 0;
    virtual Zcad::ErrorStatus
        setCurrentLayoutId      (ZcDbObjectId layoutId) = 0;
    virtual const ZTCHAR *
        findActiveLayout        (bool allowModel) = 0;

    virtual ZcDbObjectId
        getActiveLayoutBTRId    () = 0;

    virtual ZcDbLayout *
        findLayoutNamed         (const ZTCHAR * name,
                                 bool leaveRecordOpen = false) = 0;
    virtual Zcad::ErrorStatus
        copyLayout              (const ZTCHAR * copyname,
                                 const ZTCHAR * newname) = 0;

    virtual Zcad::ErrorStatus
        deleteLayout            (const ZTCHAR * delname) = 0;

    virtual Zcad::ErrorStatus
        createLayout            (const ZTCHAR *newname,
                                 ZcDbObjectId& layoutId,
                                 ZcDbObjectId& blockTableRecId,
                                 ZcDbDatabase* pDatabase = NULL) = 0;
    virtual Zcad::ErrorStatus
        renameLayout            (const ZTCHAR * oldname,
                                 const ZTCHAR * newname) = 0;

    virtual Zcad::ErrorStatus
        cloneLayout             (ZcDbLayout* pLBTR,
                                 const ZTCHAR * newname,
                                 int newTabOrder = 0) = 0;

    virtual ZcDbObjectId
        getNonRectVPIdFromClipId (ZcDbObjectId clipId) = 0;

    virtual bool
        isVpnumClipped          (int index) = 0;

    virtual int
        countLayouts            (ZcDbDatabase *useDb = NULL) = 0;

    virtual void
        addReactor              (ZcDbLayoutManagerReactor * newObj) = 0;

    virtual void
        removeReactor           (ZcDbLayoutManagerReactor * delObj) = 0;
};

#if !defined(_WIN64) && !defined(_ZC64)
Zcad::ErrorStatus zcdbDoSetupForLayouts(ZcDbDatabase* pDatabase,
                                        unsigned int& contextHandle);

Zcad::ErrorStatus zcdbClearSetupForLayouts(unsigned int contextHandle);
#else
Zcad::ErrorStatus zcdbDoSetupForLayouts(ZcDbDatabase* pDatabase,
                                        ZSoft::ULongPtr& contextHandle);

Zcad::ErrorStatus zcdbClearSetupForLayouts(ZSoft::ULongPtr contextHandle);
#endif

Zcad::ErrorStatus zcdbDeleteLayout (const ZTCHAR* delname, ZcDbDatabase* pDb = NULL);

Zcad::ErrorStatus zcdbRenameLayout (const ZTCHAR* oldname,
                                    const ZTCHAR* newname,
                                    ZcDbDatabase* pDb = NULL);

Zcad::ErrorStatus zcdbCopyLayout (const ZTCHAR* copyname,
                                  const ZTCHAR* newname,
                                  ZcDbDatabase* pDb = NULL);

#pragma pack (pop)
#endif 
