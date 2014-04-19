//
//  Analysis.cpp
//  batTuto1
//
//  Created by Nicolas Winckler on 2/20/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "Analysis.h"


#define MAX_DATE 20

Analysis::Analysis ( )
{
    
    /// Set configuration of the analysis from configuration file
    
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
    
    // define field char names that are in configfile
    fcharfield.push_back("DataName");
    fcharfield.push_back("DataPathName");
    fcharfield.push_back("OutPutDirectory");
    fcharfield.push_back("LogFileName");
    fcharfield.push_back("OutputPostpdfsM0");
    fcharfield.push_back("OutputPostpdfsM1");
    fcharfield.push_back("OutputSummaryM0");
    fcharfield.push_back("OutputSummaryM1");
    
    fcharfield.push_back("MCMCPrecision");
    
}


Analysis::~Analysis ( )
{
	
}


void Analysis::Run( )
{
    
    
    
    
    

}





void Analysis::GetBayesFactor( string filename)
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
    M1->SetIntegrationMethod(BCIntegrate::kIntCuba);
    M1->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    
    //M1->SetCubaIntegrationMethod(BCIntegrate::kCubaVegas);
    if(MCMCPrecision=="Low")      M1->MCMCSetPrecision(BCEngineMCMC::kLow);
    if(MCMCPrecision=="Medium")   M1->MCMCSetPrecision(BCEngineMCMC::kMedium);
    if(MCMCPrecision=="High")     M1->MCMCSetPrecision(BCEngineMCMC::kHigh);
    if(MCMCPrecision=="VeryHigh") M1->MCMCSetPrecision(BCEngineMCMC::kVeryHigh);
    
    BCSummaryTool * summaryM1 = new BCSummaryTool(M1);
    BCLog::OutSummary("Model M1 created");
    
    BCLog::OutSummary("Normalize M0 and M1");
    M1->Normalize();
    M0->Normalize();
    
    
    double integral0=M0->GetIntegral();
    double integral1=M1->GetIntegral();
    std::cout << "integral0 --------> " << integral0 << std::endl;
    std::cout << "integral1 --------> " << integral1 << std::endl;
    
    // run MCMC and marginalize posterior wrt. all parameters
    // and all combinations of two parameters
    BCLog::OutSummary("Marginalize M0 and M1");
    M1->MarginalizeAll();
    M0->MarginalizeAll();
    
    
    
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

   
}


////////////////////////////////////////////////////////////////// TEST



void Analysis::Test( string filename)
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


////////////////////////////////////////////////////////////////// Tuto



void Analysis::RunTuto1( )
{
	// set nicer style for drawing than the ROOT default
    BCAux::SetStyle();
    
    // open log file
    BCLog::OpenLog("log.txt");
    BCLog::SetLogLevel(BCLog::detail);
    
    // create new CountingExp object
    CountingExp * m = new CountingExp();
    
    // create background measurement data point (T, N_1)=(100, 50)
    BCDataPoint * backgroundMeasurement = new BCDataPoint(2);
    backgroundMeasurement->SetValue(0, 100);
    backgroundMeasurement->SetValue(1, 50);
    
    // add the single single measurement to the data set
    BCDataSet * dataSet = new BCDataSet();
    dataSet->AddDataPoint(backgroundMeasurement);
    
    // register the data set with the model
    m->SetDataSet(dataSet);
    
    BCLog::OutSummary("Test model created");
    
    m->SetIntegrationMethod(BCIntegrate::kIntCuba);
    m->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    
    //m->SetMarginalizationMethod(BCIntegrate::kMargDefault);
    // create a new summary tool object
    // BCSummaryTool * summary = new BCSummaryTool(m);
    
    // perform your analysis here
    
    // normalize the posterior, i.e. integrate posterior
    // over the full parameter space
    m->Normalize();
    
    // run MCMC and marginalize posterior wrt. all parameters
    // and all combinations of two parameters
    m->MarginalizeAll();
    
    // run mode finding; by default using Minuit
    //   m->FindMode();
    
    // if MCMC was run before (MarginalizeAll()) it is
    // possible to use the mode found by MCMC as
    // starting point of Minuit minimization
    m->FindMode(m->GetBestFitParameters());
    
    // draw all marginalized distributions into a PostScript file
    m->PrintAllMarginalized("CountingExp_plots.png");
    
    // print all summary plots
    //   summary->PrintParameterPlot("CountingExp_parameters.eps");
    //   summary->PrintCorrelationPlot("CountingExp_correlation.eps");
    //   summary->PrintKnowledgeUpdatePlots("CountingExp_update.ps");
    
    // calculate p-value
    //   m->CalculatePValue( m->GetBestFitParameters() );
    
    // print results of the analysis into a text file
    m->PrintResults("CountingExp_results.txt");
    
    delete dataSet;
    delete m;
    //   delete backgroundMeasurement;
    //   delete summary;
    
    BCLog::OutSummary("Test program ran successfully");
    BCLog::OutSummary("Exiting");
    
    // close log file
    BCLog::CloseLog();
}

//ClassImp(Analysis)











string Analysis::get_date()
{
   time_t now;
   char the_date[100];

   the_date[0] = '\0';

   now = time(NULL);

   if (now != -1)
   {
      strftime(the_date, 100, "%Y%m%d_%H%M%S_", gmtime(&now));
   }

   return string(the_date);
}


string Analysis::get_prefix(SidsParameters Sidspar)
{
   
   string strdate="run"+get_date();
   string prefix=Sidspar.GetName("OutPutDirectory") + strdate;
   prefix+=Sidspar.GetName("DataName");
   prefix+="_";
   prefix+="MCMCPrecision";
   prefix+=Sidspar.GetName("MCMCPrecision");
   prefix+="_";
   return prefix;
}


