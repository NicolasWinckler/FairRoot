/* 
 * File:   IsomerAnalysis.h
 * Author: winckler
 *
 * Created on April 19, 2014, 11:29 PM
 */

#ifndef ISOMERANALYSIS_H
#define	ISOMERANALYSIS_H

#include "Analysis.h"
#include "OscModel.h"
#include "ExpModel.h"

class IsomerAnalysis : public Analysis
{
public:
    
    IsomerAnalysis();
    virtual ~IsomerAnalysis();
    
    void MarginalizeAllVariables( string filename);

    
private:

};

#endif	/* ISOMERANALYSIS_H */

