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


class OscillationAnalysis : public Analysis
{
public:
    OscillationAnalysis();
    OscillationAnalysis(string filename);
    virtual ~OscillationAnalysis();
    void RunAnalysis();
    void SetM0Prior();
    void SetM1Prior();
    
    
    void Run();
    
private:
    int InitField();
    ExpModel* fM0;
    OscModel* fM1;
    SidsParameters* fConfiguration;
    SidsDataSet* fDataSet;
    map<string, string> fOutPutNames;
    
    double fMaxLogL0;
    double fMaxLogL1;
    double fLogLRatio;// Log(L0/L1))
    
};



#endif	/* OSCILLATIONANALYSIS_H */

