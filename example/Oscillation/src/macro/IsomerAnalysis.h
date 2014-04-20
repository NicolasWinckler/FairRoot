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
#include "TH1D.h"

class IsomerAnalysis : public Analysis
{
public:
    
    IsomerAnalysis();
    virtual ~IsomerAnalysis();
    
    void MarginalizeAllVariables( string filename);
    void SaveDataHistogram( string filename, BCDataSet* data);

    
private:

};

#endif	/* ISOMERANALYSIS_H */

