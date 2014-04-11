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
    /// Set configuration of the analysis from configuration file
    // define field value names that are in configfile
    vector <string> valfield;
    valfield.push_back("tmin");
    valfield.push_back("tmax");
    valfield.push_back("toffset");
    valfield.push_back("tunit");
    // define field char names that are in configfile
    vector <string> charfield;
    charfield.push_back("DataName");
    charfield.push_back("DataPathName");
    charfield.push_back("LogFileName");
    // initialize the configpar
    SidsParameters configPar(valfield,charfield);
    // load values from file
    configPar.SetExperimentalParameter(filename,true);
    
    // open log file
    string LogFileName=configPar.GetName("LogFileName");
    BCLog::OpenLog(LogFileName.c_str());
    BCLog::SetLogLevel(BCLog::detail);
    
    /// Set Data
    SidsDataSet* ECData = new SidsDataSet();
    ECData->ReadDataFromFileTxt(configPar);
    
    /// Set Model M0
    
    /// Set Model M1
    OscModel* M1 = new OscModel();
    M1->SetDataSet(ECData);
    BCLog::OutSummary("Model M1 created");
    
}




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
    
    //m->SetIntegrationMethod(BCIntegrate::kIntCuba);
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











