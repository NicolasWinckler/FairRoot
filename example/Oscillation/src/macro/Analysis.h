//
//  Analysis.h
//  batTuto1
//
//  Created by Nicolas Winckler on 2/20/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#ifndef __batTuto1__Analysis__
#define __batTuto1__Analysis__

#include <iostream>
#include <string>
#include <vector>
#include <time.h>

#include <BAT/BCLog.h>
#include <BAT/BCAux.h>
#include <BAT/BCDataSet.h>
#include <BAT/BCSummaryTool.h>
#include <BAT/BCModelManager.h>
#include <BAT/BCDataPoint.h>


#include "SidsParameters.h"
#include "SidsDataSet.h"

#include "CountingExp.h"


/**
 * class TXTDATA
 */

class Analysis
{
public:
    
    
    Analysis();
    virtual ~Analysis();
    string get_date();
    //string get_prefix(SidsParameters Sidspar);
    string get_prefix(SidsParameters* Sidspar);
    //void SetModelOption(BCModel* model, SidsParameters Sidspar);
    void SetModelOption(BCModel* model, SidsParameters* Sidspar);
    void RunTuto1();
    
protected:
    
    vector <string> fcharfield;
    vector <string> fvalfield;
    
        
    //ClassDef(Analysis,1);
};




#endif /* defined(__batTuto1__Analysis__) */
