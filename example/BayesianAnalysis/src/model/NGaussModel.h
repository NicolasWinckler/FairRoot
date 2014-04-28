/* 
 * File:   NGaussModel.h
 * Author: winckler
 *
 * Created on April 26, 2014, 9:41 PM
 */

#ifndef NGAUSSMODEL_H
#define	NGAUSSMODEL_H

#include <iostream>

#include <BAT/BCDataPoint.h>
#include <BAT/BCParameter.h>
#include <BAT/BCMath.h>
#include <BAT/BCModel.h>

#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooAbsArg.h"


#include "TMath.h"
#include <vector>
#include <cmath>
#include "SidsParameters.h"
#include "BCDataSet.h"

class NGaussModel : public BCModel
{
public:
    NGaussModel();
    NGaussModel(SidsParameters* Sidspar, int N=2);
    virtual ~NGaussModel();
    
    // Methods to overload
    void DefineParameters();
    void DefineParameters(SidsParameters* Sidspar, int N=2);
    double LogLikelihood(const std::vector<double> &parameters);
    // void MCMCIterationInterface();
    
    void SetMyDataSet(BCDataSet* dataset, double unit=1);
    
    private :
    
    double fxmin;
    double fxmax;
    double fSampleMean;
    double fmumin;
    double fmumax;
    double fsigmamin;
    double fsigmamax;
    double fcoefmin;
    double fcoefmax;

    
    vector<RooRealVar> fMeanSet;
    vector<RooRealVar> fWidthSet;
    vector<RooRealVar> fWeightSet;
    vector<RooGaussian*> fGaussianSet;
    RooRealVar* fXvar;
    
    RooArgList fMeanList;
    RooArgList fWidthList;
    
    RooArgList fGaussiansList;
    RooArgList fCoefList;
    RooAddPdf* fNGaussModel;
};

#endif	/* ONEGAUSSMODEL_H */

