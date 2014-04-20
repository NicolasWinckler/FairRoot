/* 
 * File:   OneGaussModel.h
 * Author: winckler
 *
 * Created on April 20, 2014, 2:55 AM
 */

#ifndef ONEGAUSSMODEL_H
#define	ONEGAUSSMODEL_H


#include <iostream>

#include <BAT/BCDataPoint.h>
#include <BAT/BCParameter.h>
#include <BAT/BCMath.h>
#include <BAT/BCModel.h>

#include "TMath.h"
#include <cmath>
#include "SidsParameters.h"
#include "BCDataSet.h"

class OneGaussModel : public BCModel
{
public:
    OneGaussModel();
    OneGaussModel(SidsParameters Sidspar);
    virtual ~OneGaussModel();
    
    // Methods to overload
    void DefineParameters();
    void DefineParameters(SidsParameters Sidspar);
    double LogLikelihood(const std::vector<double> &parameters);
    // void MCMCIterationInterface();
    
    void SetMyDataSet(BCDataSet* dataset, double unit=1);
    
    private :
    
    double fxmin;
    double fxmax;
    double fSampleMean;

};

#endif	/* ONEGAUSSMODEL_H */

