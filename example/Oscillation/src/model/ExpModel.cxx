//
//  ExpModel.cpp
//  BC2014Oscv0.0
//
//  Created by Nicolas Winckler on 3/9/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "ExpModel.h"

// ---------------------------------------------------------
ExpModel::ExpModel() : BCModel("ExpModel"), ftmin(0.0), ftmax(0.0), fSampleMean(0.0)
{
    // default constructor
    DefineParameters();
}


ExpModel::ExpModel(SidsParameters Sidspar) : BCModel("ExpModel"), fSampleMean(0.0)
{
    ftmin=Sidspar.GetValue("tmin");
    ftmax=Sidspar.GetValue("tmax");
    DefineParameters();
}


// ---------------------------------------------------------
ExpModel::~ExpModel()
// default destructor
{
}

// ---------------------------------------------------------
void ExpModel::DefineParameters()
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    // Allowed range for R_B is [0, 2]
    AddParameter("lambda0", 0.008, 0.020,"#lambda_{0}");
    GetParameter(0)->SetNbins(400);
    
}

// ---------------------------------------------------------
double ExpModel::LogLikelihood(const std::vector<double> &parameters)
{
    // This methods returns the logarithm of the conditional probability
    // p(data|parameters). This is where you have to define your model.
    
    double logprob = 0.;
    
    // get parameter values
    double lambda = parameters.at(0);
    
    //calculate the normalization factor analytically
    double part0=TMath::Exp(-lambda*ftmin )-TMath::Exp(-lambda*ftmax);
    double AnalyticIntegral=part0/lambda;
    
    logprob -= GetNDataPoints()*(log(AnalyticIntegral)+lambda*fSampleMean);
    
    return logprob;
}


// ---------------------------------------------------------

void ExpModel::SetDataSet2(BCDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
}





