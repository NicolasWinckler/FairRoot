//
//  OscModel.h
//  batTuto1
//
//  Created by Nicolas Winckler on 2/20/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#ifndef __batTuto1__OscModel__
#define __batTuto1__OscModel__

#include <iostream>

#include <BAT/BCDataPoint.h>
#include <BAT/BCParameter.h>
#include <BAT/BCMath.h>
#include <BAT/BCModel.h>

#include "TMath.h"
#include <cmath>
#include "SidsParameters.h"
#include "BCDataSet.h"

class OscModel : public BCModel
{
    
    public:
        
    bool use_maxLogL;
    bool fsimulation_on;
    OscModel();
    OscModel(SidsParameters* Sidspar);
    ~OscModel();
    
    // Methods to overload
    void DefineParameters();
    void DefineParameters(SidsParameters* Sidspar);
    double LogLikelihood(const std::vector<double> &parameters);
    // void MCMCIterationInterface();
    
    void SetMyDataSet(BCDataSet* dataset, double unit=1);
    double GetMaximumLogLikelihood(){return fMaxLogL;}
    
    private :
    
    double fxmin;
    double fxmax;
    double fSampleMean;
    double fMaxLogL;
    
    double fMLE_lambda;
    double fMLE_amp;
    double fMLE_omega;
    double fMLE_phi;
    
//    ClassDef(OscModel,1);
};


#endif /* defined(__batTuto1__OscModel__) */
