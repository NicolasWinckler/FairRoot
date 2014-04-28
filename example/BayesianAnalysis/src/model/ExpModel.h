//
//  ExpModel.h
//  BC2014Oscv0.0
//
//  Created by Nicolas Winckler on 3/9/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#ifndef __BC2014Oscv0_0__ExpModel__
#define __BC2014Oscv0_0__ExpModel__

#include <iostream>

#include <BAT/BCDataPoint.h>
#include <BAT/BCParameter.h>
#include <BAT/BCMath.h>
#include <BAT/BCModel.h>

#include "TMath.h"
#include <cmath>
#include "SidsParameters.h"


class ExpModel : public BCModel
{
    
    double MaxLogL;
public:
    bool use_maxLogL;
    ExpModel();
    ExpModel(SidsParameters* Sidspar);
    ~ExpModel ( );
    
    // Methods to overload
    void DefineParameters();
    double LogLikelihood(const std::vector<double> &parameters);
    // void MCMCIterationInterface();
    
    void SetMyDataSet(BCDataSet* dataset, double unit=1);
    
    private :
    
    double fxmin;
    double fxmax;
    double fSampleMean;
    
};




#endif /* defined(__BC2014Oscv0_0__ExpModel__) */
