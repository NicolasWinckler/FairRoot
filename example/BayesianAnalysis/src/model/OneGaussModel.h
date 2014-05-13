/* 
 * File:   OneGaussModel.h
 * Author: winckler
 *
 * Created on April 20, 2014, 2:55 AM
 */

#ifndef ONEGAUSSMODEL_H
#define	ONEGAUSSMODEL_H


#include <iostream>

#include "RooGaussian.h"
#include "RooDataSet.h"
#include "RooRealVar.h" 
#include "RooPlot.h"
#include <BAT/BCDataPoint.h>
#include <BAT/BCParameter.h>
#include <BAT/BCMath.h>
#include <BAT/BCModel.h>

#include "TMath.h"
#include "TCanvas.h"
#include <cmath>
#include "SidsParameters.h"
#include "SidsDataSet.h"
#include "BCDataSet.h"

class OneGaussModel : public BCModel
{
public:
    
    bool use_maxLogL;
    OneGaussModel();
    OneGaussModel(SidsParameters* Sidspar);
    virtual ~OneGaussModel();
    
    // Methods to overload
    void DefineParameters();
    void DefineParameters(SidsParameters* Sidspar);
    double LogLikelihood(const std::vector<double> &parameters);
    // void MCMCIterationInterface();
    
    void SetMyDataSet(BCDataSet* dataset, double unit=1);
    void SetMyDataSet(SidsDataSet* dataset, double unit=1);
    double GetMaximumLogLikelihood(){return fMaxLogL;}
    void RooFitToData(bool print=true);

    
    protected :
    
    RooRealVar* fx;
    RooRealVar* fmean;//("mean2","mean of gaussian 2",0) ;
    RooRealVar* fsigma;//("sigma1","width of 1gaussians",1) ;
    RooGaussian* fRooModel;//("gauss1","gaussian PDF",*fXvar,mean1,sigma1) ;
    RooDataSet* frooData;
    TTree* ftree;

    double fMaxLogL;
    double fxmin;
    double fxmax;
    double fSampleMean;

};

#endif	/* ONEGAUSSMODEL_H */

