/* 
 * File:   BatAnalysis.cxx
 * Author: winckler
 * 
 * Created on March 3, 2015, 1:49 PM
 */

#include "BatAnalysis.h"

BatAnalysis::BatAnalysis() {
}


BatAnalysis::~BatAnalysis() {
}


void BatAnalysis::SetModelOption(BCModel* model, IOManager* config)
{
    
    double relprecision=config->GetPar<double>("bat.CubaRelPrecision");
    double absprecision=config->GetPar<double>("bat.CubaAbsPrecision");
    double niterationsmin=(double)config->GetPar<int>("bat.CubaNIterationsMin");
    double niterationsmax=(double)config->GetPar<int>("bat.CubaNIterationsMax");
    
    //model->SetNIterationsPrecisionCheck (int niterations)
    //model->SetNIterationsOutput (int niterations)

    //*
    model->SetNIterationsMin(niterationsmin);
    model->SetNIterationsMax(niterationsmax);
    
    model->SetRelativePrecision(relprecision);
    model->SetAbsolutePrecision(absprecision);
    //*/
    
    
    std::string IntegrationMethod=config->GetPar<std::string>("bat.IntegrationMethod");
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
    
    
    std::string MarginalizationMethod=config->GetPar<std::string>("bat.MarginalizationMethod");
    if(IntegrationMethod=="Default") model->SetMarginalizationMethod(BCIntegrate::kMargDefault);
    if(IntegrationMethod=="Metropolis") model->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    if(IntegrationMethod=="MonteCarlo") model->SetMarginalizationMethod(BCIntegrate::kMargMonteCarlo);
    if(IntegrationMethod=="Grid") model->SetMarginalizationMethod(BCIntegrate::kMargGrid);
        
    std::string MCMCPrecision=config->GetPar<std::string>("bat.MCMCPrecision");
    if(MCMCPrecision=="Low")      model->MCMCSetPrecision(BCEngineMCMC::kLow);
    if(MCMCPrecision=="Medium")   model->MCMCSetPrecision(BCEngineMCMC::kMedium);
    if(MCMCPrecision=="High")     model->MCMCSetPrecision(BCEngineMCMC::kHigh);
    if(MCMCPrecision=="VeryHigh") model->MCMCSetPrecision(BCEngineMCMC::kVeryHigh);
    
}

