/* 
 * File:   TwoGaussModel.cxx
 * Author: winckler
 * 
 * Created on April 20, 2014, 1:09 AM
 */

#include "TwoGaussModel.h"

// ---------------------------------------------------------
TwoGaussModel::TwoGaussModel() : BCModel("TwoGaussModel"), fxmin(0.0), fxmax(0.0), fSampleMean(0.0)
{
    // default constructor
    DefineParameters();
}


TwoGaussModel::TwoGaussModel(SidsParameters* Sidspar) : BCModel("TwoGaussModel"), fSampleMean(0.0)
{
    DefineParameters(Sidspar);
}


// ---------------------------------------------------------
TwoGaussModel::~TwoGaussModel()
// default destructor
{
}

// ---------------------------------------------------------


void TwoGaussModel::DefineParameters(SidsParameters* Sidspar)
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    fxmin=Sidspar->GetValue("xmin");
    fxmax=Sidspar->GetValue("xmax");
    AddParameter("mu0", Sidspar->GetValue("mu0min"), Sidspar->GetValue("mu0max"),"#mu_{0} (ps)");
    AddParameter("mu1", Sidspar->GetValue("mu1min"), Sidspar->GetValue("mu1max"),"#mu_{1} (ps)");
    AddParameter("sigma0", Sidspar->GetValue("sigma0min"), Sidspar->GetValue("sigma0max"),"#sigma_{0} (ps)");
    AddParameter("sigma1", Sidspar->GetValue("sigma1min"), Sidspar->GetValue("sigma1max"),"#sigma_{1} (ps)");
    AddParameter("weight0", Sidspar->GetValue("weight0min"), Sidspar->GetValue("weight0max"),"w_{0}");
    //AddParameter("weight1", Sidspar->GetValue("weight1min"), Sidspar->GetValue("weight1max"),"w_{1}");
    
    
    GetParameter(0)->SetNbins((int)Sidspar->GetValue("BinMu0"));
    GetParameter(1)->SetNbins((int)Sidspar->GetValue("BinMu1"));
    GetParameter(2)->SetNbins((int)Sidspar->GetValue("BinSigma0"));
    GetParameter(3)->SetNbins((int)Sidspar->GetValue("BinSigma1"));
    GetParameter(4)->SetNbins((int)Sidspar->GetValue("BinWeight0"));
    //GetParameter(5)->SetNbins((int)Sidspar->GetValue("BinWeight1"));
}



// ---------------------------------------------------------
double TwoGaussModel::LogLikelihood(const std::vector<double> &parameters)
{
    // This methods returns the logarithm of the conditional probability
    // p(data|parameters). This is where you have to define your model.
    
    double logprob = 0.;
    
    // get parameter values
    double mu0 = parameters.at(0);
    double mu1 = parameters.at(1);
    double sigma0 = parameters.at(2);
    double sigma1 = parameters.at(3);
    double w0 = parameters.at(4);
    //double w1 = parameters.at(0);
    
    
    
    for (int i = 0; i < GetNDataPoints(); ++i)
    {
        // get data
        double x = GetDataPoint(i)->GetValue(0);
        Double_t gauss0=TMath::Gaus(x,mu0, sigma0,kTRUE);
        Double_t gauss1=TMath::Gaus(x,mu1, sigma1,kTRUE);
    
        logprob+=log(w0*gauss0+(1-w0)*gauss1);
    }
    
    
    
    return logprob;
}


// ---------------------------------------------------------

void TwoGaussModel::SetMyDataSet(BCDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
}





