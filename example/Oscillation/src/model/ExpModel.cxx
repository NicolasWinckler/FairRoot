//
//  ExpModel.cpp
//  BC2014Oscv0.0
//
//  Created by Nicolas Winckler on 3/9/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "ExpModel.h"

// ---------------------------------------------------------
ExpModel::ExpModel() : BCModel(), ftmin(0.0), ftmax(0.0), fSampleMean(0.0)
{
    // default constructor
    DefineParameters();
}


ExpModel::ExpModel(SidsParameters Sidspar) : BCModel(), fSampleMean(0.0)
{
    ftmin=Sidspar.GetValue("tmin");
    ftmax=Sidspar.GetValue("tmax");
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
    AddParameter("lambda", 0.0, 0.01);
    
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
	
    logprob = GetNDataPoints()*(log(lambda)-lambda*fSampleMean-log(part0));
    
    return logprob;
}

// ---------------------------------------------------------
double ExpModel::LogAPrioriProbability(const std::vector<double> &parameters)
{
    // This method returns the logarithm of the prior probability for the
    // parameters p(parameters).
    double logprob = 0.0;
    
    // normalize flat prior with parameter ranges
    for (unsigned int i = 0; i < GetNParameters(); i++)
        logprob -= log(GetParameter(i)->GetRangeWidth());
    
    return 0.0;
}
// ---------------------------------------------------------

void ExpModel::SetDataSet(BCDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
}





