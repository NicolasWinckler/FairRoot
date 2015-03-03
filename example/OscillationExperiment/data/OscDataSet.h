/* 
 * File:   OscDataSet.h
 * Author: winckler
 *
 * Created on March 2, 2015, 12:22 PM
 */

#ifndef OSCDATASET_H
#define	OSCDATASET_H



#include <iostream>
#include <fstream>

#include "FileReader.h"

#include "BAT/BCDataSet.h"
#include "BAT/BCDataPoint.h"
#include "BAT/BCLog.h"

#include <TFile.h>
#include <TString.h>

# include "TTree.h"
#include "RooRealVar.h" 
#include "RooDataSet.h"
#include "TH1D.h"

#include "OscAnaManager.h"

class OscDataSet : public BCDataSet , public FileReader
{
public:
    OscDataSet(OscAnaManager* config);
    virtual ~OscDataSet();
    
    int ConvertRooToBCDataset(RooDataSet* dataset, const std::string& ObsName="x");
    int ReadDataFromFileTxt(OscAnaManager* config);
    int ReadDataFromFileRoot(OscAnaManager* config);
    
    RooDataSet* GetRooData(){return frooData;}
    TTree* GetTree(){return fTree;}
    TH1D* GetHisto(){return fHisto;}
    
private:

    void initAttributes(OscAnaManager* config);
    RooDataSet* frooData;
    RooRealVar* fx;
    double fxmin;
    double fxmax;
    TTree* fTree;
    TH1D* fHisto;
    bool fXRangeAuto;
    bool fDigitize;
    double fXResolution;
};

#endif	/* OSCDATASET_H */

