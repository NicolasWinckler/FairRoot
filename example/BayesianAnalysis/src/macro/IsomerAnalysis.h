/* 
 * File:   IsomerAnalysis.h
 * Author: winckler
 *
 * Created on April 19, 2014, 11:29 PM
 */

#ifndef ISOMERANALYSIS_H
#define	ISOMERANALYSIS_H

#include "Analysis.h"
#include "OneGaussModel.h"
#include "TwoGaussModel.h"
#include "NGaussModel.h"
#include "TH1D.h"

class IsomerAnalysis : public Analysis
{
public:
    
    IsomerAnalysis();
    IsomerAnalysis(string filename);
    virtual ~IsomerAnalysis();
    
    void RunAnalysis();
    void SaveDataHistogram( string filename, BCDataSet* data);
    void SetM0Prior();
    void SetM1Prior();
    
    void FitOtherPeaks();

    
private:
    int InitField();
    OneGaussModel* fM0;
    TwoGaussModel* fM1;
    SidsParameters* fConfiguration;
    SidsDataSet* fDataSet;
    
    map<string, string> fOutPutNames;
};

#endif	/* ISOMERANALYSIS_H */

