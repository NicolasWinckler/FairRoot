/* 
 * File:   OscillationAnalysis.h
 * Author: winckler
 *
 * Created on April 19, 2014, 11:38 PM
 */

#ifndef OSCILLATIONANALYSIS_H
#define	OSCILLATIONANALYSIS_H


#include "Analysis.h"

#include "CountingExp.h"
#include "OscModel.h"
#include "ExpModel.h"


class OscillationAnalysis : public Analysis
{
public:
    OscillationAnalysis();
    virtual ~OscillationAnalysis();
    void Run();
    void RunTuto1();
    void GetBayesFactor(string filename);
    void Test(string filename);
private:

};

#endif	/* OSCILLATIONANALYSIS_H */

