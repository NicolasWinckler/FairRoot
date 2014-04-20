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
}


OscModel::OscModel(SidsParameters Sidspar) : BCModel("OscModel"), fSampleMean(0.0)
{
    DefineParameters(Sidspar);
}

// ---------------------------------------------------------
OscModel::~OscModel()
// default destructor
{
}

// ---------------------------------------------------------
void OscModel::DefineParameters(SidsParameters Sidspar)
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    fxmin=Sidspar.GetValue("xmin");
    fxmax=Sidspar.GetValue("xmax");
    AddParameter("lambda1", Sidspar.GetValue("lambdamin"), Sidspar.GetValue("lambdamax"),"#lambda");
    AddParameter("amp", Sidspar.GetValue("amin"), Sidspar.GetValue("amax"),"a");
    AddParameter("omega", Sidspar.GetValue("omegamin"), Sidspar.GetValue("omegamax"),"#omega");
    AddParameter("phi", Sidspar.GetValue("phimin"), Sidspar.GetValue("phimax"),"#phi");
    GetParameter(0)->SetNbins((int)Sidspar.GetValue("BinLambda"));//lambda
    GetParameter(1)->SetNbins((int)Sidspar.GetValue("BinAmp"));//a
    GetParameter(2)->SetNbins((int)Sidspar.GetValue("BinOmega"));//omega
    GetParameter(3)->SetNbins((int)Sidspar.GetValue("BinPhi"));//phi
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
    
    return logprob;
}

// ---------------------------------------------------------

void OscModel::SetMyDataSet(BCDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
    std::cout << " Number of Data Points = " << BCModel::GetNDataPoints() << std::endl;
    std::cout << " Sample mean = " << fSampleMean << std::endl;
}



//ClassImp(OscModel)




