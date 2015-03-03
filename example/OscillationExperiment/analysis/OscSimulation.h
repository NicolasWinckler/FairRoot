/* 
 * File:   OscSimulation.h
 * Author: winckler
 *
 * Created on March 2, 2015, 11:21 AM
 */

#ifndef OSCSIMULATION_H
#define	OSCSIMULATION_H

// std
#include <string>
#include <vector>
#include <cmath>

// ROOT
#include "TMath.h"
#include "TFile.h"
#include "TROOT.h"
#include "TF2.h"
#include "TPaveText.h"

// RooFit
#include "RooRealVar.h" 
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooGenericPdf.h"
#include "RooMinuit.h"
#include "RooWorkspace.h"
#include "RooRandom.h"

#include "RooUniform.h"
#include "RooGaussian.h"
#include "RooStats/ModelConfig.h"
#include "RooNumIntConfig.h"
#include "RooRealProxy.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"

// My stuff
//#include "RooMyPdfDict.h"
#include "BatAnalysis.h"
#include "OscModel.h"
#include "OscMCPoint.h"
#include "OscAnaManager.h"
#include "OscDataSet.h"
#include "RooMyAnalyticalPdf.h"
#include "RootOutFileManager.h"

using namespace RooFit;
using namespace RooStats;

class OscSimulation : public BatAnalysis
{
public:
    OscSimulation();
    OscSimulation(OscAnaManager* config);
    virtual ~OscSimulation();
    int RunSimu(int SampleSize, bool MCMC);
    int GenerateData(std::string DataName, double NumSimEvt, double PullStats=1);
    int SaveSimData(const std::string& filename, RooDataSet* DataSet);
    int SaveSimData(const std::string& filename);
    int ComputeMLEDistribution(const std::string& filename, int SampleSize, int TotStat, bool MCMC=false);
    
    int LoadSimData(const std::string& filename, const std::string& DataName);
    void SetModelOption(BCModel* model, OscAnaManager* config);
    
    
    OscMCPoint GetOscMCPoint(int indexmin, int indexmax, bool MCMC);
    OscMCPoint GetOscMCPoint(RooDataSet* roodataset);
private:
    
    int RunMCMC(RooDataSet* roodataset);
    int CutData(int indexmin, int indexmax);
    int UpdateRooParameterFromMCMC();
    void initAttributes(OscAnaManager* config);
    
    OscAnaManager* fConfig;
    OscMCPoint fMCpoint;
    OscDataSet* fDataSet;
    RooDataSet* frooData;
    RooDataSet* fReducedDataSet;
    
    RooRealVar* fx;
    RooRealVar* fx_cool;
    RooRealVar* fx_err;
    
    RooRealVar* fLambda0;// model 0 (null)

    RooRealVar* fLambda1;// model 1 (alt)
    RooRealVar* fAmp1;
    RooRealVar* fOmega1;
    RooRealVar* fPhi1;
    
    RooRealVar* fmeancool;
    RooRealVar* fsigmacool;
    RooRealVar* fmeanerr;
    RooRealVar* fsigmaerr;
    
    
    OscModel* fM1;
    RooGenericPdf* fPdf_H0;
    RooMyAnalyticalPdf* fPdf_H1s;
    RooGenericPdf* fPdf_H1b;
    RooGaussian* fCoolingDistribution;
    RooGaussian* fErrorDistribution;
    
};

#endif	/* OSCSIMULATION_H */

