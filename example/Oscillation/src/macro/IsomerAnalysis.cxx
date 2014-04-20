/* 
 * File:   IsomerAnalysis.cxx
 * Author: winckler
 * 
 * Created on April 19, 2014, 11:29 PM
 */

#include "IsomerAnalysis.h"

IsomerAnalysis::IsomerAnalysis() : Analysis()
{
    
    // define field value names that are in configfile
    
    // analysis range
    fvalfield.push_back("xmin");
    fvalfield.push_back("xmax");
    fvalfield.push_back("xunit");
    
    fvalfield.push_back("mu0min");
    fvalfield.push_back("mu0max");
    fvalfield.push_back("mu1min");
    fvalfield.push_back("mu1max");
    
    fvalfield.push_back("sigma0min");
    fvalfield.push_back("sigma0max");
    fvalfield.push_back("sigma1min");
    fvalfield.push_back("sigma1max");
    
    fvalfield.push_back("weight0min");
    fvalfield.push_back("weight0max");
    //fvalfield.push_back("weight1min");
    //fvalfield.push_back("weight1max");
    
    fvalfield.push_back("BinMu0");
    fvalfield.push_back("BinMu1");
    fvalfield.push_back("BinSigma0");
    fvalfield.push_back("BinSigma1");
    fvalfield.push_back("BinWeight0");
    //fvalfield.push_back("BinWeight1");
    
    
    // parameter outputs
    fcharfield.push_back("OutputPostpdfsM0");
    fcharfield.push_back("OutputPostpdfsM1");
    fcharfield.push_back("OutputSummaryM0");
    fcharfield.push_back("OutputSummaryM1");
}

IsomerAnalysis::~IsomerAnalysis() {
}


void IsomerAnalysis::MarginalizeAllVariables( string filename)
{
    
    // set nicer style for drawing than the ROOT default
    BCAux::SetStyle();
    
    // initialize the configpar
    SidsParameters configPar(fvalfield,fcharfield);
    // load values from file
    configPar.SetExperimentalParameter(filename,true);
    
    // ----------------------------------------------------
    // define output file names and open log file
    // ----------------------------------------------------
    string prefixOutputName=get_prefix(configPar);
    string file_postpdfsM0=prefixOutputName + configPar.GetName("OutputPostpdfsM0");
    string file_postpdfsM1=prefixOutputName + configPar.GetName("OutputPostpdfsM1");
    string file_summaryM0=prefixOutputName + configPar.GetName("OutputSummaryM0");
    string file_summaryM1=prefixOutputName + configPar.GetName("OutputSummaryM1");
    string file_log=prefixOutputName + configPar.GetName("LogFileName");
    string file_histroot=prefixOutputName+"histo_52Co_IMS_Data.root";
    
    BCLog::OpenLog(file_log.c_str());
    BCLog::SetLogLevel(BCLog::detail);
    
    // ----------------------------------------------------
    /// Load Data and other variables
    // ----------------------------------------------------
    SidsDataSet* ECData = new SidsDataSet();
    ECData->ReadDataFromFileTxt(configPar);
    string MCMCPrecision=configPar.GetName("MCMCPrecision");
    
    
    
    //*
    // ----------------------------------------------------
    /// Set Model M0
    // ----------------------------------------------------
    OneGaussModel* M0 = new OneGaussModel(configPar);
    M0->SetMyDataSet(ECData);
    M0->SetPriorConstant(0);
    M0->SetPriorConstant(1);
    
    
    //M0->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    //if(MCMCPrecision=="Low")      M0->MCMCSetPrecision(BCEngineMCMC::kLow);
    //if(MCMCPrecision=="Medium")   M0->MCMCSetPrecision(BCEngineMCMC::kMedium);
    //if(MCMCPrecision=="High")     M0->MCMCSetPrecision(BCEngineMCMC::kHigh);
    //if(MCMCPrecision=="VeryHigh") M0->MCMCSetPrecision(BCEngineMCMC::kVeryHigh);
    
    SetModelOption(M0,configPar);
    BCSummaryTool * summaryM0 = new BCSummaryTool(M0);
    BCLog::OutSummary("Model M0 created");
    
    // ----------------------------------------------------
    /// Set Model M1
    // ----------------------------------------------------
    TwoGaussModel* M1 = new TwoGaussModel(configPar);
    M1->SetMyDataSet(ECData);
    
    M1->SetPriorConstant(0);
    M1->SetPriorConstant(1);
    M1->SetPriorConstant(2);
    M1->SetPriorConstant(3);
    M1->SetPriorConstant(4);
    
    BCLog::OutSummary("******************* Set integration and marginalization method for M1 *******************");
    //M1->SetIntegrationMethod(BCIntegrate::kIntCuba);
    //M1->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    //M1->SetNIterationsMin(1000000);
    //M1->SetNIterationsMax(10000000);
    //M1->SetCubaIntegrationMethod(BCIntegrate::kCubaVegas);
    //if(MCMCPrecision=="Low")      M1->MCMCSetPrecision(BCEngineMCMC::kLow);
    //if(MCMCPrecision=="Medium")   M1->MCMCSetPrecision(BCEngineMCMC::kMedium);
    //if(MCMCPrecision=="High")     M1->MCMCSetPrecision(BCEngineMCMC::kHigh);
    //if(MCMCPrecision=="VeryHigh") M1->MCMCSetPrecision(BCEngineMCMC::kVeryHigh);
    
    SetModelOption(M1,configPar);
    
    BCSummaryTool * summaryM1 = new BCSummaryTool(M1);
    BCLog::OutSummary("Model M1 created");
    
    BCLog::OutSummary("******************* Normalize M0 and M1 *******************");

    BCLog::OutSummary("");
    M0->Normalize();
    M1->Normalize();
    
    
    
    double integral0=M0->GetIntegral();
    double integral1=M1->GetIntegral();
    std::cout << "integral0 --------> " << integral0 << std::endl;
    std::cout << "integral1 --------> " << integral1 << std::endl;
    
    // run MCMC and marginalize posterior wrt. all parameters
    // and all combinations of two parameters
    BCLog::OutSummary("******************* Marginalize M0 and M1 *******************");
    M0->MarginalizeAll();
    M1->MarginalizeAll();
    
    
    
    
   // ----------------------------------------------------
   // set up model manager
   // ----------------------------------------------------

   //M0->Integrate();
   //M1->Integrate();
   integral0=M0->GetIntegral();
   integral1=M1->GetIntegral();
   std::cout << "integral0 --------> " << integral0 << std::endl;
   std::cout << "integral1 --------> " << integral1 << std::endl;
   
    
   // ----------------------------------------------------
   // set up model manager
   // ----------------------------------------------------   

   BCModelManager * modelman = new BCModelManager();
   modelman->SetDataSet(ECData);
   modelman->AddModel(M0,0.5);
   modelman->AddModel(M1,0.5);

   
   modelman->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
   modelman->SetIntegrationMethod(BCIntegrate::kIntCuba);
   
   
   // Calculates the normalization of the likelihood for each model
   //modelman->GetModel(0)->Integrate();
   //modelman->GetModel(1)->Integrate();
   modelman->Integrate();
   modelman->MarginalizeAll();
   // compare models
   double bayesFact01 = modelman->BayesFactor(0,1);

   std::cout << std::endl << "Bayes Factor P(D|M0)/P(D|M1) = " << bayesFact01 << std::endl;
    //
   
   //modelman->PrintResults();//"testMy_results.txt");
   //modelman->PrintSummary("testresult.txt");
  
   
   /// write output
   
   
   M0->PrintAllMarginalized(file_postpdfsM0.c_str());
   M1->PrintAllMarginalized(file_postpdfsM1.c_str());

   summaryM0->PrintKnowledgeUpdatePlots(file_summaryM0.c_str());
   summaryM1->PrintKnowledgeUpdatePlots(file_summaryM1.c_str());

   
   // */
}




void IsomerAnalysis::SaveDataHistogram( string filename, BCDataSet* Data)
{
    TFile fileHisto(filename.c_str(), "new");
    TH1D h1("h52Co", "histogram of the 52Co IMS-Data", 100, 609.05, 609.07);
    
    for(unsigned int i=0; i < Data->GetNDataPoints(); ++i)
    {
        double xi = Data->GetDataPoint(i)->GetValue(0);
        //std::cout<<"xi="<<xi<<std::endl;
        h1.Fill(xi);
    }
    h1.Write();
    fileHisto.Close();
}

