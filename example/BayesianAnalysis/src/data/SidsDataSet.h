//
//  SidsDataSet.h
//  
//
//  Created by Nicolas Winckler on 2/21/14.
//
//

#ifndef ____SidsDataSet__
#define ____SidsDataSet__

#include <iostream>
#include <fstream>

#include "FileReader.h"
#include "SidsParameters.h"

#include "BAT/BCDataSet.h"
#include "BAT/BCDataPoint.h"
#include "BAT/BCLog.h"

#include <TFile.h>
#include <TString.h>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

# include "TTree.h"
#include "RooRealVar.h" 
#include "RooDataSet.h"
#include "TH1D.h"



// BAT classes
//class BCDataPoint;

// ---------------------------------------------------------

//typedef std::vector<BCDataPoint*> BCDataVector;

class SidsDataSet : public BCDataSet, public FileReader
{
    public:
    
    SidsDataSet();
   
    virtual ~SidsDataSet();
    
    int FillTreeFromFileTxt(SidsParameters* Sidspar);//my stuff
    int ReadDataFromFileTxt(SidsParameters* Sidspar);//my stuff
    int ReadRooDataSet(RooDataSet* dataset);
    double GetRangeLength();
    TTree* GetTree(){return fTree;}
    TH1D* GetHisto(){return fHisto;}
    
    protected :
    
    RooDataSet* frooData;
    RooRealVar* fx;
    double fxobsmin;
    double fxobsmax;
    TTree* fTree;
    TH1D* fHisto;
    bool fXRangeAuto;
    
//    ClassDef(sidsDataSet,1);
};




#endif /* defined(____SidsDataSet__) */
