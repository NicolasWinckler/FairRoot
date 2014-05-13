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
    delete fx;
    delete fmean;
    delete fsigma;
    delete fRooModel;
    delete frooData;
    delete ftree;
}

// ---------------------------------------------------------


void OneGaussModel::DefineParameters(SidsParameters* Sidspar)
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    string dataname=Sidspar->GetName("DataName");
    string meanName("mu00_");
    meanName+=dataname;
    string sigmaName("sigma00_");
    sigmaName+=dataname;
    string ModelName("Gauss0_");
    ModelName+=dataname;
    
    fxmin=Sidspar->GetValue("xmin");
    fxmax=Sidspar->GetValue("xmax");
    
    fx = new RooRealVar("x","x",fxmin,fxmax);
    double mu_min=Sidspar->GetValue("mu0min");
    double mu_max=Sidspar->GetValue("mu0max");
    double mu_ = (mu_max-mu_min)/2.0;
    
    double sigma_min=Sidspar->GetValue("sigma0min");
    double sigma_max=Sidspar->GetValue("sigma0max");
    double sigma_ = (sigma_max-sigma_min)/2.0;
    
    
    // BAT par
    AddParameter("mu00", mu_min, mu_max,"#mu_{0}");
    AddParameter("sigma00", sigma_min, sigma_max,"#sigma_{0}");
    
    
    GetParameter(0)->SetNbins((int)Sidspar->GetValue("BinMu0"));
    GetParameter(1)->SetNbins((int)Sidspar->GetValue("BinSigma0"));
    //GetParameter(5)->SetNbins((int)Sidspar->GetValue("BinWeight1"));
    fMaxLogL = -1e99;
    use_maxLogL = false;
    
    
    
    
    
    // RooFit par
    fmean = new RooRealVar("mean","mean value",mu_,mu_min,mu_max) ;
    fsigma = new RooRealVar("sigma","width",sigma_,sigma_min,sigma_max) ;
    fRooModel = new RooGaussian("gauss","gaussian PDF",*fx,*fmean,*fsigma) ;
    
    std::cout<<"ok1"<<std::endl;
    
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
    
    
    
    
    if(fMaxLogL<logprob && use_maxLogL== false)
        fMaxLogL = logprob;
    
    if(use_maxLogL==true)
        logprob += -fMaxLogL;
    
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


void OneGaussModel::SetMyDataSet(SidsDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);std::cout<<"ok2"<<std::endl;
    ftree=dataset->GetTree();
    
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
    //frooData = new RooDataSet("frooData","dataset with x",ftree,*fx);
    std::cout<<"ok2b"<<std::endl;
    frooData = fRooModel->generate(*fx,1000);
    std::cout<<"ok3"<<std::endl;
}


void OneGaussModel::RooFitToData(bool print)
{
    fRooModel->fitTo(*frooData);
    std::cout<<"ok4"<<std::endl;
    /*
    if(print) fRooModel->fitTo(*frooData);
    else fRooModel->fitTo(*frooData,RooFit::PrintLevel(-1));
    */bool draw=true;
    if(draw)
    {
            RooPlot* xframe = fx->frame();
            frooData->plotOn(xframe,RooFit::Binning(100)) ;
            fRooModel->plotOn(xframe,RooFit::LineColor(kRed));
            TCanvas *cmodel = new TCanvas();
            xframe->Draw();
    }std::cout<<"ok5"<<std::endl;
    // */
}

