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
    void SetM0Prior(string priorset="");
    void SetM1Prior(string priorset="");
    static double fitffunction0(double *t,double *par);
    static double fitffunction1(double *t,double *par);
    void SetTF1functions();
    void Run();
    void RunTest();
    
    double RunBayesTest(bool shiftMaxLikelihood);
    
    double GetPriorM0(){return fPriorM0;}
    double GetPriorM1(){return fPriorM1;}
    
    double GetProbM0(){return fPM0D;}
    double GetProbM1(){return fPM1D;}
    
    double GetEvidenceM0(){return fPDM0;}
    double GetEvidenceM1(){return fPDM1;}
    
    double GetB01(){return fB01;}
    double GetB10(){return fB10;}
    
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
    double fPriorM0;    // P(M0)
    double fPriorM1;    // P(M1)
    double fPDM0;       // P(D|M0)
    double fPDM1;       // P(D|M1)
    double fPM0D;       // P(M0|D)
    double fPM1D;       // P(M1|D)
    double fB01;
    double fB10;
    
    TF1 *f0;
    TF1 *f1;
    
};



#endif	/* OSCILLATIONANALYSIS_H */

