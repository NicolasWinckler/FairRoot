/* 
 * File:   OscillationAnalysis.h
 * Author: winckler
 *
 * Created on April 19, 2014, 11:38 PM
 */

#ifndef OSCILLATIONANALYSIS_H
#define	OSCILLATIONANALYSIS_H


#include "Analysis.h"

#include "OscModel.h"
#include "ExpModel.h"
#include "TF1.h"
#include "TH1D.h"
#include "BCHistogramFitter.h"

class OscillationAnalysis : public Analysis
{
public:
    OscillationAnalysis();
    OscillationAnalysis(string filename);
    virtual ~OscillationAnalysis();
    void RunAnalysis();
    void RunBinnedAnalysis();
    void SetM0Prior();
    void SetM1Prior();
    static double fitffunction0(double *t,double *par);
    static double fitffunction1(double *t,double *par);
    void SetTF1functions();
    void Run();
    void RunTest();
    
    double RunBayesTest(bool shiftMaxLikelihood);
    
    unsigned int GetSampleSize();
    
private:
    int InitField();
    ExpModel* fM0;
    OscModel* fM1;
    
    BCHistogramFitter* fhM0;
    BCHistogramFitter* fhM1;
    
    SidsParameters* fConfiguration;
    SidsDataSet* fDataSet;
    map<string, string> fOutPutNames;
    
    double fMaxLogL0;
    double fMaxLogL1;
    double fLogLRatio;// Log(L0/L1))
    double fxmin;
    double fxmax;
    
    TF1 *f0;
    TF1 *f1;
    
};



#endif	/* OSCILLATIONANALYSIS_H */

