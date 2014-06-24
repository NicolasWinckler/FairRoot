//
//  Analysis.cpp
//  batTuto1
//
//  Created by Nicolas Winckler on 2/20/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "Analysis.h"



Analysis::Analysis ( )
{
    
    /// Set configuration of the analysis from configuration file
    
    
    // define field char names that are in configfile
    fcharfield.push_back("DataName");
    fcharfield.push_back("DataPathName");
    fcharfield.push_back("OutPutDirectory");
    fcharfield.push_back("LogFileName");
    
    fcharfield.push_back("XRangeOption");
    
    fcharfield.push_back("MCMCPrecision");
    fcharfield.push_back("IntegrationMethod");
    fcharfield.push_back("MarginalizationMethod");
    
    
    fvalfield.push_back("CubaRelPrecision");
    fvalfield.push_back("CubaAbsPrecision");
    fvalfield.push_back("CubaNIterationsMin");
    fvalfield.push_back("CubaNIterationsMax");
    
}


Analysis::~Analysis ( )
{
	
}



void Analysis::SetModelOption(BCModel* model, SidsParameters* Sidspar)
{
    
    double relprecision=Sidspar->GetValue("CubaRelPrecision");
    double absprecision=Sidspar->GetValue("CubaAbsPrecision");
    double niterationsmin=Sidspar->GetValue("CubaNIterationsMin");
    double niterationsmax=Sidspar->GetValue("CubaNIterationsMax");
    
    //model->SetNIterationsPrecisionCheck (int niterations)
    //model->SetNIterationsOutput (int niterations)

    //*
    model->SetNIterationsMin(niterationsmin);
    model->SetNIterationsMax(niterationsmax);
    
    model->SetRelativePrecision(relprecision);
    model->SetAbsolutePrecision(absprecision);
    //*/
    
    
    string IntegrationMethod=Sidspar->GetName("IntegrationMethod");
    if(IntegrationMethod=="Default")    model->SetIntegrationMethod(BCIntegrate::kIntDefault); 
    if(IntegrationMethod=="Cuba")       model->SetIntegrationMethod(BCIntegrate::kIntCuba); 
    if(IntegrationMethod=="MonteCarlo") model->SetIntegrationMethod(BCIntegrate::kIntMonteCarlo); 
    if(IntegrationMethod=="Grid")       model->SetIntegrationMethod(BCIntegrate::kIntGrid); 
    
    if(IntegrationMethod=="CubaVegas")
    {
        
        BCCubaOptions::Vegas VegasOption = model->GetCubaVegasOptions(); 
        
        model->SetCubaOptions(VegasOption); 
        //model->IntegrateCuba(BCIntegrate::kCubaVegas);
        
        
    }
    
    
    string MarginalizationMethod=Sidspar->GetName("MarginalizationMethod");
    if(IntegrationMethod=="Default") model->SetMarginalizationMethod(BCIntegrate::kMargDefault);
    if(IntegrationMethod=="Metropolis") model->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    if(IntegrationMethod=="MonteCarlo") model->SetMarginalizationMethod(BCIntegrate::kMargMonteCarlo);
    if(IntegrationMethod=="Grid") model->SetMarginalizationMethod(BCIntegrate::kMargGrid);
        
    string MCMCPrecision=Sidspar->GetName("MCMCPrecision");
    if(MCMCPrecision=="Low")      model->MCMCSetPrecision(BCEngineMCMC::kLow);
    if(MCMCPrecision=="Medium")   model->MCMCSetPrecision(BCEngineMCMC::kMedium);
    if(MCMCPrecision=="High")     model->MCMCSetPrecision(BCEngineMCMC::kHigh);
    if(MCMCPrecision=="VeryHigh") model->MCMCSetPrecision(BCEngineMCMC::kVeryHigh);
    
}

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


string Analysis::get_prefix(SidsParameters* Sidspar)
{
   
   string strdate="run"+get_date();
   string prefix=Sidspar->GetName("OutPutDirectory") + strdate;
   prefix+=Sidspar->GetName("DataName");
   prefix+="_";
   prefix+="MCMCPrecision";
   prefix+=Sidspar->GetName("MCMCPrecision");
   prefix+="_";
   return prefix;
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
