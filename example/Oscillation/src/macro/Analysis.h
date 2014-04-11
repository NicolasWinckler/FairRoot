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

#include <BAT/BCLog.h>
#include <BAT/BCAux.h>
#include <BAT/BCDataSet.h>
#include <BAT/BCSummaryTool.h>

#include <BAT/BCDataPoint.h>
#include "CountingExp.h"
#include "OscModel.h"

#include "SidsParameters.h"
#include "SidsDataSet.h"

/**
 * class TXTDATA
 */

class Analysis
{
public:
    
    
	Analysis();
    ~Analysis();
    void Run();
    void RunTuto1();
	void GetBayesFactor(string filename);
    //ClassDef(Analysis,1);
};




#endif /* defined(__batTuto1__Analysis__) */
