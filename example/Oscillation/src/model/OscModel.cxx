//
//  OscModel.cpp
//  batTuto1
//
//  Created by Nicolas Winckler on 2/20/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "OscModel.h"


// ---------------------------------------------------------
OscModel::OscModel() : BCModel("OscModel"), ftmin(0.0), ftmax(0.0), fSampleMean(0.0)
{
    // default constructor
    DefineParameters();
}


OscModel::OscModel(SidsParameters Sidspar) : BCModel(), fSampleMean(0.0)
{
    ftmin=Sidspar.GetValue("tmin");
    ftmax=Sidspar.GetValue("tmax");
    DefineParameters();
}

// ---------------------------------------------------------
OscModel::~OscModel()
// default destructor
{
}

// ---------------------------------------------------------
void OscModel::DefineParameters()
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    // Allowed range for R_B is [0, 2]
    AddParameter("lambda0", 0.008, 0.020,"#lambda_{1}");
    AddParameter("amp", -0.25,0.25,"#a_{1}");
    AddParameter("omega", 0.0,7.0,"#omega_{1}");
    AddParameter("phi", -3.14, 3.14,"#phi_{1}");
    GetParameter(0)->SetNbins(400);//lambda
    GetParameter(1)->SetNbins(400);//a
    GetParameter(2)->SetNbins(600);//omega
    GetParameter(3)->SetNbins(400);//phi
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
    double part0=TMath::Exp(-lambda*ftmin )-TMath::Exp(-lambda*ftmax);
    double factor=lambda*amp/(omega*omega+lambda*lambda);
    double part1a=(omega*TMath::Sin(omega*ftmax+phi)-lambda*TMath::Cos(omega*ftmax+phi))*TMath::Exp(-lambda*ftmax);
    double part1b=(omega*TMath::Sin(omega*ftmin+phi)-lambda*TMath::Cos(omega*ftmin+phi))*TMath::Exp(-lambda*ftmin);
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

void OscModel::SetDataSet2(BCDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
    std::cout << " GetNDataPoints() a = " << BCModel::GetNDataPoints() << std::endl;
    std::cout << " fDataSet->GetNDataPoints(); b = " << fDataSet->GetNDataPoints() << std::endl;
}



//ClassImp(OscModel)




