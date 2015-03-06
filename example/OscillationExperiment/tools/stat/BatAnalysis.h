/* 
 * File:   BatAnalysis.h
 * Author: winckler
 *
 * Created on March 3, 2015, 1:49 PM
 */

#ifndef BATANALYSIS_H
#define	BATANALYSIS_H

#include "BCModel.h"
#include "IOManager.h"
class BatAnalysis 
{
public:
    BatAnalysis();
    virtual ~BatAnalysis();
    void SetModelOption(BCModel* model, IOManager* config);

private:

};

#endif	/* BATANALYSIS_H */

