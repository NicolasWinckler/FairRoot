/* 
 * File:   DataSet.h
 * Author: winckler
 *
 * Created on February 17, 2015, 9:42 AM
 */

#ifndef DATASET_H
#define	DATASET_H
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

class DataSet : public BCDataSet
{
public:
    DataSet();
    DataSet(const DataSet& orig);
    virtual ~DataSet();
    
    
    int FillTreeFromFileTxt(ParaSet* par);//my stuff
    int ReadDataFromFileTxt(ParaSet* par);//my stuff
    int ReadRooDataSet(RooDataSet* dataset);
    double GetRangeLength();
    
    
    void SetRooDataSet(RooDataSet* dataset){frooData=dataset;}
    RooDataSet* GetRooDataSet() {return frooData;}
    
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
    

};

#endif	/* DATASET_H */

