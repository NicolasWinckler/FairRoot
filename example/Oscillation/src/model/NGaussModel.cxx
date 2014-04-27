/* 
 * File:   NGaussModel.cxx
 * Author: winckler
 * 
 * Created on April 26, 2014, 9:41 PM
 */

#include "NGaussModel.h"

// ---------------------------------------------------------
NGaussModel::NGaussModel() : BCModel("NGaussModel"), fxmin(0.0), fxmax(0.0), fSampleMean(0.0)
{
    // default constructor
    DefineParameters();
}


NGaussModel::NGaussModel(SidsParameters* Sidspar, int N) : BCModel("NGaussModel"), fSampleMean(0.0)
{

        // Build two Gaussian PDFs
    int iii=0;
    std::ostringstream meanName0;
        meanName0<<"mu0"<<iii+1;
    
        fXvar = new RooRealVar("x","x",-10,10);
    //RooRealVar x("x","x",-10,10) ;
    
    RooRealVar mean1(meanName0.str().c_str(),meanName0.str().c_str(),0) ;
    //RooRealVar mean1("mean1","mean of gaussian 1",0) ;
    RooRealVar mean2("mean2","mean of gaussian 2",0) ;
    RooRealVar sigma1("sigma1","width of 1gaussians",1) ;
    RooGaussian gauss1("gauss1","gaussian PDF",*fXvar,mean1,sigma1) ;
    RooGaussian gauss2("gauss2","gaussian PDF",*fXvar,mean2,sigma1) ;
    // Add the components
    RooRealVar g1frac("g1frac","fraction of gauss1",0.5);
    RooRealVar g2frac("g2frac","fraction of gauss2",0.5);
    
    //fCoefList.add(g1frac);
    //fCoefList.add(g2frac);
    
    //fGaussiansList.add(gauss1);
    //fGaussiansList.add(gauss2);
    //RooAddPdf sum("sum","g1+g2",RooArgList(gauss1,gauss2),RooArgList(g1frac,g2frac));
    //RooAddPdf* sum;
    
    //sum= new RooAddPdf("sum","g1+g2",RooArgList(gauss1,gauss2),RooArgList(g1frac,g2frac));
    std::cout<<"+++++++++++++++++++++++++++++++ ->ok0" <<std::endl;
    for(int i=0;i<2;i++)
    {
        std::ostringstream meanName;
        meanName<<"mu"<<i+1;
        std::ostringstream widthName;
        widthName<<"sigma"<<i+1;
        std::ostringstream coefName;
        coefName<<"w"<<i+1;
        std::ostringstream GaussName;
        GaussName<<"Gaussian"<<i+1;
        std::cout<<"+++++++++++++++++++++++++++++++ ->ok1" <<std::endl;
        
        RooRealVar mean(meanName.str().c_str(),meanName.str().c_str(),0) ;
        RooRealVar sigma(widthName.str().c_str(),widthName.str().c_str(),1) ;
        RooRealVar coef(coefName.str().c_str(),coefName.str().c_str(),0.5) ;
        
        fMeanSet.push_back(mean);
        fWidthSet.push_back(sigma);
        fWeightSet.push_back(coef);
        //RooGaussian gauss(GaussName.str().c_str(),GaussName.str().c_str(),x,fMeanSet[i],fWidthSet[i]) ;
        std::cout<<"+++++++++++++++++++++++++++++++ ->ok2" <<std::endl;
        std::cout<<"fMeanSet "<<fMeanSet[i].getValV() <<std::endl;
        std::cout<<"fWidthSet "<<fWidthSet[i].getValV() <<std::endl;
        std::cout<<"fWeightSet "<<fWeightSet[i].getValV() <<std::endl;
        fXvar->setVal(1.0);
        RooArgSet obs(*fXvar) ;
        
        fGaussianSet.push_back(
        new RooGaussian (GaussName.str().c_str(),GaussName.str().c_str(),*fXvar,fMeanSet[i],fWidthSet[i]));
        std::cout<<"gauss "<< fGaussianSet[i]->getVal(&obs) <<std::endl;
        
        
        std::cout<<"+++++++++++++++++++++++++++++++ ->ok3" <<std::endl;
        fCoefList.add(fWeightSet[i]);
        fGaussiansList.add(*fGaussianSet[i]);
    std::cout<<"+++++++++++++++++++++++++++++++ ->ok4" <<std::endl;
    }
    std::cout<<"+++++++++++++++++++++++++++++++ ->ok" <<std::endl;

    std::cout<<"+++++++++++++++++++++++++++++++ -> VALUE of PDF is = "<<fWidthSet[0].getValV() <<std::endl;

    //sum= new RooAddPdf("sum","g1+g2",fGaussiansList,fCoefList);
    RooAddPdf sum("sum","g1+g2",fGaussiansList,fCoefList);
    
    fXvar->setVal(1.0);
    RooArgSet obs(*fXvar) ;
    Double_t val = sum.getVal(&obs) ;
    std::cout<<"+++++++++++++++++++++++++++++++ -> VALUE of PDF is = "<<val<<std::endl;
    //*/
    //delete sum;
    //DefineParameters(Sidspar, N);
}


// ---------------------------------------------------------
NGaussModel::~NGaussModel()
// default destructor
{
    //delete fXvar;
    //delete fNGaussModel;
}

// ---------------------------------------------------------


void NGaussModel::DefineParameters(SidsParameters* Sidspar, int N)
{
    // Add parameters to your model here.
    // You can then use them in the methods below by calling the
    // parameters.at(i) or parameters[i], where i is the index
    // of the parameter. The indices increase from 0 according to the
    // order of adding the parameters.
    
    /// My parameters
    fxmin=Sidspar->GetValue("xmin");
    fxmax=Sidspar->GetValue("xmax");
    fmumin=Sidspar->GetValue("mu0min");
    fmumax=Sidspar->GetValue("mu0max");
    fsigmamin=Sidspar->GetValue("sigma0min");
    fsigmamax=Sidspar->GetValue("sigma0max");
    fcoefmin=Sidspar->GetValue("weight0min");
    fcoefmax=Sidspar->GetValue("weight0max");
    
    
    /// Roofit parameters
    
    fXvar = new RooRealVar("x","x",fxmin,fxmax);
    int kpar=0;
    for(int i=0;i<N;i++)
    {
        std::ostringstream meanName;
        meanName<<"#mu_{"<<i+1<<"}";
        std::ostringstream widthName;
        widthName<<"#sigma_{"<<i+1<<"}";
        std::ostringstream coefName;
        coefName<<"w_{"<<i+1<<"}";
        std::ostringstream GaussName;
        GaussName<<"Gaussian"<<i+1;
        
        fMeanSet.push_back(RooRealVar(meanName.str().c_str(),meanName.str().c_str(),fmumin,fmumax));
        fWidthSet.push_back(RooRealVar(widthName.str().c_str(),widthName.str().c_str(),fsigmamin,fsigmamax) );
        fWeightSet.push_back(RooRealVar(coefName.str().c_str(),coefName.str().c_str(),fcoefmin,fcoefmax) );
        
        RooGaussian gauss(GaussName.str().c_str(),GaussName.str().c_str(),*fXvar,fMeanSet.at(i),fWidthSet.at(i)) ;
        fCoefList.add(RooRealVar(coefName.str().c_str(),coefName.str().c_str(),0.5,fcoefmin,fcoefmax));
        fGaussiansList.add(gauss);
        
        fMeanList.add(RooRealVar(meanName.str().c_str(),meanName.str().c_str(),fmumin,fmumax));
        fWidthList.add(RooRealVar(widthName.str().c_str(),widthName.str().c_str(),fsigmamin,fsigmamax) );
        //int index=fMeanSet.index(meanName.str().c_str());
        
        //RooGaussian gauss(GaussName.str().c_str(),GaussName.str().c_str(),*fXvar,fMeanSet[meanName.str().c_str()],fWidthSet[widthName.str().c_str()]) ;
        //RooGaussian gauss(GaussName.str().c_str(),GaussName.str().c_str(),*fXvar,fMeanSet.at(i),fWidthSet.at(i)) ;
        
        //RooRealVar mean1("mean1","mean of gaussian 1",0) ;
        //RooRealVar sigma("sigma","width of gaussians",1) ;
        //RooGaussian gauss(GaussName.str().c_str(),GaussName.str().c_str(),*fXvar,mean1,sigma) ;
        
        //RooGaussian gauss();
        //std::cout<<"+++++++++++++++++++++++++++++++ -> VALUE of string is = "<<coefName.str()<<std::endl;
        
        /// BAT parameters
        
        AddParameter(meanName.str().c_str(), Sidspar->GetValue("mu0min"), Sidspar->GetValue("mu0max"),meanName.str().c_str());
        AddParameter(widthName.str().c_str(), Sidspar->GetValue("sigma0min"), Sidspar->GetValue("sigma0max"),widthName.str().c_str());
        AddParameter(coefName.str().c_str(), Sidspar->GetValue("weight0min"), Sidspar->GetValue("weight0max"),coefName.str().c_str());
        GetParameter(kpar)->SetNbins((int)Sidspar->GetValue("BinMu0"));
        kpar++;
        GetParameter(kpar)->SetNbins((int)Sidspar->GetValue("BinSigma0"));
        kpar++;
        GetParameter(kpar)->SetNbins((int)Sidspar->GetValue("BinWeight0"));
        kpar++;
        
    }
    std::cout<<"OK0 "<<std::endl;
    
    fNGaussModel = new RooAddPdf("NGaussianPdf","NGaussianPdf",fGaussiansList,fCoefList);
    //fNGaussModel = new RooAddPdf("NGaussianPdf","NGaussianPdf",fGaussiansList,fCoefList,kTRUE);
    std::cout<<"OK1 "<<std::endl;
    
}



// ---------------------------------------------------------
double NGaussModel::LogLikelihood(const std::vector<double> &parameters)
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
    
        
        fXvar->setVal(1.0);
        RooArgSet obs(*fXvar) ;
        Double_t val = fNGaussModel->getVal(&obs) ;
        
        
        logprob+=log(gauss0);
    }
    
    
    
    return logprob;
}


// ---------------------------------------------------------

void NGaussModel::SetMyDataSet(BCDataSet* dataset, double unit)
{
    BCModel::SetDataSet(dataset);
    double sum=0.0;
    for (int i = 0; i < GetNDataPoints(); ++i)
        sum+=GetDataPoint(i)->GetValue(0);
    fSampleMean=unit*sum/((double)GetNDataPoints());
}





