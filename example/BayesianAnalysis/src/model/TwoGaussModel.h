/* 
 * File:   TwoGaussModel.h
 * Author: winckler
 *
 * Created on April 20, 2014, 1:09 AM
 */

#ifndef TWOGAUSSMODEL_H
#define	TWOGAUSSMODEL_H

#include <iostream>

#include <BAT/BCDataPoint.h>
#include <BAT/BCParameter.h>
#include <BAT/BCMath.h>
#include <BAT/BCModel.h>

#include "TMath.h"
#include <cmath>
#include "SidsParameters.h"
#include "BCDataSet.h"


class TwoGaussModel : public BCModel
{
public:
    
    TwoGaussModel();
    TwoGaussModel(SidsParameters* Sidspar);
    virtual ~TwoGaussModel ( );
    
    // Methods to overload
    void DefineParameters();
    void DefineParameters(SidsParameters* Sidspar);
    double LogLikelihood(const std::vector<double> &parameters);
    // void MCMCIterationInterface();
    
    void SetMyDataSet(BCDataSet* dataset, double unit=1);
    
    private :
    
    double fxmin;
    double fxmax;
    double fSampleMean;
    
};


#endif	/* TWOGAUSSMODEL_H */

