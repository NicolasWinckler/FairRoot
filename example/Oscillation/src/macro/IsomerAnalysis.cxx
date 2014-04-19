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
    fvalfield.push_back("tmin");
    fvalfield.push_back("tmax");
    fvalfield.push_back("toffset");
    fvalfield.push_back("tunit");
    
    // parameter ranges
    fvalfield.push_back("lambdamin");
    fvalfield.push_back("lambdamax");
    fvalfield.push_back("amin");
    fvalfield.push_back("amax");
    fvalfield.push_back("omegamin");
    fvalfield.push_back("omegamax");
    fvalfield.push_back("phimin");
    fvalfield.push_back("phimax");
    
    // parameter binning
    fvalfield.push_back("BinLambda");
    fvalfield.push_back("BinAmp");
    fvalfield.push_back("BinOmega");
    fvalfield.push_back("BinPhi");
    
    
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
    
    BCLog::OpenLog(file_log.c_str());
    BCLog::SetLogLevel(BCLog::detail);
    
    // ----------------------------------------------------
    /// Load Data and other variables
    // ----------------------------------------------------
    SidsDataSet* ECData = new SidsDataSet();
    ECData->ReadDataFromFileTxt(configPar);
    string MCMCPrecision=configPar.GetName("MCMCPrecision");
    
    // ----------------------------------------------------
    /// Set Model M0
    // ----------------------------------------------------
    ExpModel* M0 = new ExpModel(configPar);
    M0->SetMyDataSet(ECData);
    M0->SetPriorConstant(0);
    M0->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    
    if(MCMCPrecision=="Low")      M0->MCMCSetPrecision(BCEngineMCMC::kLow);
    if(MCMCPrecision=="Medium")   M0->MCMCSetPrecision(BCEngineMCMC::kMedium);
    if(MCMCPrecision=="High")     M0->MCMCSetPrecision(BCEngineMCMC::kHigh);
    if(MCMCPrecision=="VeryHigh") M0->MCMCSetPrecision(BCEngineMCMC::kVeryHigh);
    
    BCSummaryTool * summaryM0 = new BCSummaryTool(M0);
    BCLog::OutSummary("Model M0 created");
    
    // ----------------------------------------------------
    /// Set Model M1
    // ----------------------------------------------------
    OscModel* M1 = new OscModel(configPar);
    M1->SetMyDataSet(ECData);
    
    M1->SetPriorConstant(0);
    M1->SetPriorConstant(1);
    M1->SetPriorConstant(2);
    M1->SetPriorConstant(3);
    //M1->SetPriorDelta(2, 0.88);
    //M1->SetPriorDelta(3, 2.4);
    BCLog::OutSummary("******************* Set integration and marginalization method for M1 *******************");
    M1->SetIntegrationMethod(BCIntegrate::kIntCuba);
    M1->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    //M1->SetNIterationsMin(1000000);
    //M1->SetNIterationsMax(10000000);
    //M1->SetCubaIntegrationMethod(BCIntegrate::kCubaVegas);
    if(MCMCPrecision=="Low")      M1->MCMCSetPrecision(BCEngineMCMC::kLow);
    if(MCMCPrecision=="Medium")   M1->MCMCSetPrecision(BCEngineMCMC::kMedium);
    if(MCMCPrecision=="High")     M1->MCMCSetPrecision(BCEngineMCMC::kHigh);
    if(MCMCPrecision=="VeryHigh") M1->MCMCSetPrecision(BCEngineMCMC::kVeryHigh);
    
    BCSummaryTool * summaryM1 = new BCSummaryTool(M1);
    BCLog::OutSummary("Model M1 created");
    
    BCLog::OutSummary("******************* Normalize M0 and M1 *******************");

    BCLog::OutSummary("");
    M1->Normalize();
    M0->Normalize();
    
    
    double integral0=M0->GetIntegral();
    double integral1=M1->GetIntegral();
    std::cout << "integral0 --------> " << integral0 << std::endl;
    std::cout << "integral1 --------> " << integral1 << std::endl;
    
    // run MCMC and marginalize posterior wrt. all parameters
    // and all combinations of two parameters
    BCLog::OutSummary("******************* Marginalize M0 and M1 *******************");
    M1->MarginalizeAll();
    M0->MarginalizeAll();
    
    
    
   // ----------------------------------------------------
   // set up model manager
   // ----------------------------------------------------

   //M0->Integrate();
   //M1->Integrate();
   integral0=M0->GetIntegral();
   integral1=M1->GetIntegral();
   std::cout << "integral0 --------> " << integral0 << std::endl;
   std::cout << "integral1 --------> " << integral1 << std::endl;
   
   
}