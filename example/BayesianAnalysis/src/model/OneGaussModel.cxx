/* 
 * File:   OneGaussModel.cxx
 * Author: winckler
 * 
 * Created on April 20, 2014, 2:55 AM
 */

#include "OneGaussModel.h"

// ---------------------------------------------------------
OneGaussModel::OneGaussModel() : BCModel("OneGaussModel"), fxmin(0.0), fxmax(0.0), fSampleMean(0.0)
{
    // default constructor
    DefineParameters();
}


OneGaussModel::OneGaussModel(SidsParameters* Sidspar) : BCModel("OneGaussModel"), fSampleMean(0.0)
{
    DefineParameters(Sidspar);
}


// ---------------------------------------------------------
OneGaussModel::~OneGaussModel()
// default destructor
{
}

// ---------------------------------------------------------


void OneGaussModel::DefineParameters(SidsParameters* Sidspar)
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    fxmin=Sidspar->GetValue("xmin");
    fxmax=Sidspar->GetValue("xmax");
    AddParameter("mu00", Sidspar->GetValue("mu0min"), Sidspar->GetValue("mu0max"),"#mu_{0}");
    AddParameter("sigma00", Sidspar->GetValue("sigma0min"), Sidspar->GetValue("sigma0max"),"#sigma_{0}");
    
    
    GetParameter(0)->SetNbins((int)Sidspar->GetValue("BinMu0"));
    GetParameter(1)->SetNbins((int)Sidspar->GetValue("BinSigma0"));
    //GetParameter(5)->SetNbins((int)Sidspar->GetValue("BinWeight1"));
}



// ---------------------------------------------------------
double OneGaussModel::LogLikelihood(const std::vector<double> &parameters)
{
    // This methods returns the logarithm of the conditional probability
    // p(data|parameters). This is where you have to define your model.
    
    double logprob = 0.;
    
    // get parameter values
    double mu0 = parameters.at(0);
    double sigma0 = parameters.at(1);
    //double w1 = parameters.at(0);
    
    
    
    for (int i = 0; i < GetNDataPoints(); ++i)
    {
        // get data
        double x = GetDataPoint(i)->GetValue(0);
        Double_t gauss0=TMath::Gaus(x,mu0, sigma0,kTRUE);
    
        logprob+=log(gauss0);
    }
    
    
    
    return logprob;
}


// ---------------------------------------------------------

void OneGaussModel::SetMyDataSet(BCDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
}





