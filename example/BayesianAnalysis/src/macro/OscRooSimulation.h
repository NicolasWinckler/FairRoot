/* 
 * File:   OscRooSimulation.h
 * Author: winckler
 *
 * Created on June 20, 2014, 12:54 PM
 */

#ifndef OSCROOSIMULATION_H
#define	OSCROOSIMULATION_H


#include <string>
#include <vector>

#include "TPaveText.h"
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

#include "TFile.h"
#include "TROOT.h"
#include <TF2.h>
#include "RooAbsData.h"

#include "RooStats/LikelihoodInterval.h"

#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/AsymptoticCalculator.h"
#include "RooStats/HybridCalculator.h"
#include "RooStats/FrequentistCalculator.h"
#include "RooStats/ToyMCSampler.h"
#include "RooStats/HypoTestPlot.h"
#include "RooStats/NumEventsTestStat.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/SimpleLikelihoodRatioTestStat.h"
#include "RooStats/RatioOfProfiledLikelihoodsTestStat.h"
#include "RooStats/MaxLikelihoodEstimateTestStat.h"
#include "RooStats/HypoTestInverter.h"
#include "RooStats/HypoTestInverterResult.h"
#include "RooStats/HypoTestInverterPlot.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/SimpleInterval.h"



#include "TMath.h"
#include <cmath>
#include "SidsParameters.h"
//#include "RooMyAnalyticalPdf.h"
#include "RooMyPdfDict.h"
#include "OscModel.h"

#include "Analysis.h"

using namespace RooFit;
using namespace RooStats;

class RooMyAnalyticalPdf;



class OscRooSimulation : public Analysis
{
public:
    OscRooSimulation(string filename);
    virtual ~OscRooSimulation();
    
    int FitToH1();
    int FitToH0();
    int GenerateData(string DataName, double NumSimEvt, double PullStats=10000);
    int LoadDataFromFile(string DataName);
    int CutData(int indexmin, int indexmax);
    int SaveSimData();
    vector< vector<double> > GetSetOfMLE(RooDataSet* roodataset);
    int GetPullDistribution(int SampleSize);
    int GetPullDistributionBatchFarm(string DataName, int SampleSize, int PullStats=1000);
    void SetOutputDirectory(string ouptdir)
        {foutputdir=ouptdir;}
    
private:

    bool fMCMCdone;
    SidsParameters* fConfiguration;
    double fxmin;
    double fxmax;
    double flambdamin;
    double flambdamax;
    double famin;
    double famax;
    double fomegamin;
    double fomegamax;
    double fphimin;
    double fphimax;

    string foutputdir;
    
    double m_lambdatot;
    double m_phi;
    double m_omega;
    double m_amp;

    RooRealVar* m_x;
    RooDataSet* m_dataSim;
    RooDataSet* fReducedDataSet;

    //RooDataSet* m_DataSimH0;
    //RooDataSet* m_DataSimH1;

    RooRealVar* m_lambda0;// model 0 (null)

    RooRealVar* m_lambda1;// model 1 (alt)
    RooRealVar* m_amp1;
    RooRealVar* m_omega1;
    RooRealVar* m_phi1;

    double m_AIC0;
    double m_AIC1;
    double m_AIC2;
    double m_AIC3;

    double m_BIC0;
    double m_BIC1;
    double m_BIC2;
    double m_BIC3;


    // define pdfs
    RooGenericPdf* m_pdf_H0;
    RooMyAnalyticalPdf* m_pdf_H1s;

    int InitField();
    void initAttributes(SidsParameters* Sidspar);
    int UpdateRooParameterFromMCMC();
    int RunMCMC(RooDataSet* roodataset);
        
};

#endif	/* OSCROOSIMULATION_H */

