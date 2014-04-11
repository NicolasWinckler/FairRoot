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



class OscModel : public BCModel
{
    public:
    
	//OscModel ( );
    OscModel();
    ~OscModel ( );
    
    // Methods to overload
    void DefineParameters();
    double LogAPrioriProbability(const std::vector<double> &parameters);
    double LogLikelihood(const std::vector<double> &parameters);
    // void MCMCIterationInterface();
    
    void SetDataSet(BCDataSet* dataset, double unit=1);
    
    private :
    
    double ftmin;
    double ftmax;
    double fSampleMean;
    ClassDef(OscModel,1);
};


#endif /* defined(__batTuto1__OscModel__) */
