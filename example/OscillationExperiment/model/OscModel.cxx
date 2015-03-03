//
//  OscModel.cpp
//  batTuto1
//
//  Created by Nicolas Winckler on 2/20/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "OscModel.h"


// ---------------------------------------------------------
OscModel::OscModel() : BCModel("OscModel"), fxmin(0.0), fxmax(0.0), fSampleMean(0.0)
{
    // default constructor
    DefineParameters();
    fsimulation_on=false;
    fMLE_lambda=0.0;
    fMLE_amp=0.0;
    fMLE_omega=0.0;
    fMLE_phi=0.0;
}


OscModel::OscModel(OscAnaManager* config) : BCModel("OscModel"), fSampleMean(0.0)
{
    DefineParameters(config);
    fsimulation_on=false;
    fMLE_lambda=0.0;
    fMLE_amp=0.0;
    fMLE_omega=0.0;
    fMLE_phi=0.0;
    
}

// ---------------------------------------------------------
OscModel::~OscModel()
// default destructor
{
}

// ---------------------------------------------------------
void OscModel::DefineParameters(OscAnaManager* config)
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    fxmin=config->GetPar<double>("obs.xmin");
    fxmax=config->GetPar<double>("obs.xmax");
    AddParameter("lambda1", config->GetPar<double>("par.lambda.min"), config->GetPar<double>("par.lambda.max"),"#lambda");
    AddParameter("amp", config->GetPar<double>("par.amp.min"), config->GetPar<double>("par.amp.max"),"a");
    AddParameter("omega", config->GetPar<double>("par.omega.min"), config->GetPar<double>("par.omega.max"),"#omega");
    AddParameter("phi", config->GetPar<double>("par.phi.min"), config->GetPar<double>("par.phi.max"),"#phi");
    GetParameter(0)->SetNbins((int)config->GetPar<int>("par.lambda.bin"));//lambda
    GetParameter(1)->SetNbins((int)config->GetPar<int>("par.amp.bib"));//a
    GetParameter(2)->SetNbins((int)config->GetPar<int>("par.omega.bin"));//omega
    GetParameter(3)->SetNbins((int)config->GetPar<int>("par.phi.bin"));//phi

    fMaxLogL = -1e99;
    use_maxLogL = false;
}



// ---------------------------------------------------------
void OscModel::DefineParameters()
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    AddParameter("lambda1", 0.008, 0.020,"#lambda");
    AddParameter("amp", -0.25,0.25,"a");
    AddParameter("omega", 0.0,7.0,"#omega");
    AddParameter("phi", -3.14, 3.14,"#phi");
    GetParameter(0)->SetNbins(200);//lambda
    GetParameter(1)->SetNbins(200);//a
    GetParameter(2)->SetNbins(200);//omega
    GetParameter(3)->SetNbins(200);//phi
    fMaxLogL = -1e99;
    use_maxLogL = false;
}

// ---------------------------------------------------------
double OscModel::LogLikelihood(const std::vector<double> &parameters)
{
    // This methods returns the logarithm of the conditional probability
    // p(data|parameters). This is where you have to define your model.
    
    double logprob = 0.;
    
    // get parameter values
    double lambda = parameters.at(0);
    double amp = parameters.at(1);
    double omega = parameters.at(2);
    double phi = parameters.at(3);
    
    
    //calculate the normalization factor analytically
    double part0=TMath::Exp(-lambda*fxmin )-TMath::Exp(-lambda*fxmax);
    double factor=lambda*amp/(omega*omega+lambda*lambda);
    double part1a=(omega*TMath::Sin(omega*fxmax+phi)-lambda*TMath::Cos(omega*fxmax+phi))*TMath::Exp(-lambda*fxmax);
    double part1b=(omega*TMath::Sin(omega*fxmin+phi)-lambda*TMath::Cos(omega*fxmin+phi))*TMath::Exp(-lambda*fxmin);
    double AnalyticIntegral=(part0+factor*(part1a-part1b))/lambda;      // normalisation factor of the pdf M1
	
    //std::cout << " GetNDataPoints() = " << BCModel::GetNDataPoints() << std::endl;
    //std::cout << " fDataSet->GetNDataPoints(); b = " << fDataSet->GetNDataPoints() << std::endl;
    for (int i = 0; i < GetNDataPoints(); ++i)
    {
        // get data
        double t = GetDataPoint(i)->GetValue(0);
            //std::cout << " GetDataPoint(i)->GetValue(0) = " << GetDataPoint(i)->GetValue(0) << std::endl;

        // calculate Sum log(1+a cos(wt+phi))
        double coswt=TMath::Cos(omega*t+phi);
        logprob += log(1+amp*coswt);                                    //cos statistics
    }
    // update likelihood
    logprob -= GetNDataPoints()*(log(AnalyticIntegral)+lambda*fSampleMean);
    
    // find MLE via MCMC (dedicated for simulation and bias study)
    if(fsimulation_on)
    {
        if(fMaxLogL<logprob)
        {
            fMaxLogL = logprob;
            fMLE_lambda=lambda;
            fMLE_amp=amp;
            fMLE_omega=omega;
            fMLE_phi=phi;
        }
    }
    if(fMaxLogL<logprob && use_maxLogL== false)
        fMaxLogL = logprob;
    
    if(use_maxLogL==true)
        logprob += -fMaxLogL;
   
    
    
    return logprob;
}

// ---------------------------------------------------------

void OscModel::SetMyDataSet(BCDataSet* dataset, double unit)
{
    if(GetNDataPoints()>0)
        GetDataSet()->Reset();
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
    std::cout << " Number of Data Points = " << BCModel::GetNDataPoints() << std::endl;
    std::cout << " Sample mean = " << fSampleMean << std::endl;
}

std::vector<double> OscModel::GetMCMCMLEValue()
{
    
    std::vector<double> MLE;
    MLE.push_back(fMLE_lambda);
    MLE.push_back(fMLE_amp);
    MLE.push_back(fMLE_omega);
    MLE.push_back(fMLE_phi);
    
    return MLE;
}


//ClassImp(OscModel)




