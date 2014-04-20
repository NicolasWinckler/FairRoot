//
//  SidsParameters.h
//  batTuto1
//
//  Created by Nicolas Winckler on 2/22/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#ifndef __batTuto1__SidsParameters__
#define __batTuto1__SidsParameters__

#include <iostream>
#include "FileReader.h"
#include <map>
#include <BAT/BCLog.h>

class SidsParameters : public FileReader
{
    
    public:
    
    SidsParameters();
    SidsParameters(vector<string> keyvalues, vector<string> keynames);
    ~SidsParameters( );
    
    int SetExperimentalParameter(string filename, bool print=false);
    double GetValue(string key);
    string GetName(string key);
    int PrintToBCLog();
    
    private :
    
    map<string, double> fParameterValues;
    map<string, string> fParameterNames;
    vector<string> fKeyValues;
    vector<string> fKeyNames;
//    ClassDef(SidsParameters,1);
};




#endif /* defined(__batTuto1__SidsParameters__) */
