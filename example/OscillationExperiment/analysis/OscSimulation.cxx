/* 
 * File:   OscSimulation.cxx
 * Author: winckler
 * 
 * Created on March 2, 2015, 11:21 AM
 */

#include "OscSimulation.h"

OscSimulation::OscSimulation(OscAnaManager* config) : BatAnalysis(),
    fConfig(nullptr),// memory handled outside
    fMCpoint(),
    fDataSet(nullptr),
    frooData(nullptr),
    fReducedDataSet(nullptr),
    fx(nullptr),
    fx_cool(nullptr),
    fx_err(nullptr),
    fLambda0(nullptr),
    fLambda1(nullptr),
    fAmp1(nullptr),
    fOmega1(nullptr),
    fPhi1(nullptr),
    fmeancool(nullptr),
    fsigmacool(nullptr),
    fmeanerr(nullptr),
    fsigmaerr(nullptr),
    fM1(nullptr),
    fPdf_H0(nullptr),
    fPdf_H1s(nullptr),
    fPdf_H1b(nullptr),
    fCoolingDistribution(nullptr),
    fErrorDistribution(nullptr)
{
    fConfig=config;
    initAttributes(config);

    
}


OscSimulation::~OscSimulation() 
{
    delete fDataSet;
    delete fx;
    delete fx_cool;
    delete fx_err;
    delete fLambda0;
    delete fLambda1;
    delete fAmp1;
    delete fOmega1;
    delete fPhi1;
    delete fmeancool;
    delete fsigmacool;
    delete fmeanerr;
    delete fsigmaerr;
    delete fM1;
    delete fPdf_H0;
    delete fPdf_H1s;
    delete fPdf_H1b;
    delete fCoolingDistribution;
    delete fErrorDistribution;
    frooData=nullptr;
}



int OscSimulation::GenerateData(std::string DataName, double NumSimEvt, double PullStats)
{
        /// simulation
        // prepare random seed
	TDatime* time = new TDatime();
	int temptime=time->GetTime();
	int randnumber = rand() % 100 + 1;
	delete time;
	int seed = TMath::Abs(randnumber*temptime);
	int randnumber2 = rand() % 10000 + 1;
	seed+=randnumber2;
	
	RooRandom::randomGenerator()->SetSeed(seed);
        double TotSimEvt=NumSimEvt*PullStats;
	frooData = fPdf_H0->generate(*fx,TotSimEvt,RooFit::Name(DataName.c_str()));
        
        return 0;
}


int OscSimulation::SaveSimData(const std::string& filename, RooDataSet* DataSet)
{
    TFile rootfile(filename.c_str(),"RECREATE");
    DataSet->Write();
    rootfile.Close();
    
    return 0;
}


int OscSimulation::SaveSimData(const std::string& filename)
{
    SaveSimData(filename,frooData);
    
    return 0;
}


int OscSimulation::LoadSimData(const std::string& filename,const std::string& DataName)
{
    std::cout<<"start Load data\n";
    
    TFile* rootfile = new TFile(filename.c_str());
    frooData = (RooDataSet*) rootfile->Get(DataName.c_str());
    
    rootfile->Close();
    delete rootfile;
    return 0;
}

int OscSimulation::CutData(int indexmin, int indexmax)
{
    //cout<<"start reduce data"<<endl;
    fReducedDataSet = (RooDataSet*) frooData->reduce(EventRange(indexmin,indexmax));
    return 0;
}



OscMCPoint OscSimulation::GetOscMCPoint(int indexmin, int indexmax, bool MCMC)
{
    //cout<<"CutData"<<endl;
    
    CutData(indexmin, indexmax);
    fMCpoint.Reset();
    // run first MCMC to be sure that the parameters are around the global minimum
    //cout<<"RunMCMC(fReducedDataSet)"<<endl;
    if(MCMC)
        RunMCMC(fReducedDataSet);
    //cout<<"UpdateRooParameterFromMCMC()"<<endl;
    if(MCMC)
        UpdateRooParameterFromMCMC();

    return GetOscMCPoint(fReducedDataSet);
}


OscMCPoint OscSimulation::GetOscMCPoint(RooDataSet* roodataset)
{
    /// perform fit of H0 and H1
    fPdf_H0->fitTo(*roodataset,PrintLevel(-1));
    RooAbsReal* NLL0= fPdf_H0->createNLL(*roodataset);

    fPdf_H1s->fitTo(*roodataset,PrintLevel(-1));
    RooAbsReal* NLL1= fPdf_H1s->createNLL(*roodataset);
	
    
    /// Fill Payload
    // sample size
    fMCpoint.SampleSize=roodataset->numEntries();
    
    // H0 data
    fMCpoint.NLL0=NLL0->getValV();
    
    fMCpoint.lambda0=fLambda0->getValV();
    fMCpoint.lambda0_err=fLambda0->getError();
    
    //H1 data
    fMCpoint.NLL1=NLL1->getValV();
    
    fMCpoint.lambda1=fLambda1->getValV();
    fMCpoint.lambda1_err=fLambda1->getError();
    
    fMCpoint.amplitude=fAmp1->getValV();
    fMCpoint.amplitude_err=fAmp1->getError();
    
    fMCpoint.omega=fOmega1->getValV();
    fMCpoint.omega_err=fOmega1->getError();
    
    fMCpoint.phi=fPhi1->getValV();
    fMCpoint.phi_err=fPhi1->getError();
    
    //LRT
    fMCpoint.LRT=2.0*(NLL0->getValV()-NLL1->getValV());
    fMCpoint.MCMCLRT=2.0*(NLL0->getValV()-fMCpoint.MCMCNLL1);
    return fMCpoint;
}


int OscSimulation::ComputeMLEDistribution(const std::string& filename, int SampleSize, int Iteration, bool MCMC)
{
    std::vector<OscMCPoint> Distribution;
    int Ntot=frooData->numEntries();
    int NStat=Iteration*SampleSize;
    
    if( NStat< Ntot)
    {        
        int nextprint=1000;
        int counter=0;
        for(unsigned int i=0 ; i<=NStat ;i+=SampleSize)
        {
            int indexmin=i;
            int indexmax=i+SampleSize;
            //cout<<"index = "<<i<<endl;
            if(indexmax<Ntot)
            {
                OscMCPoint MCpoint=GetOscMCPoint(indexmin, indexmax, MCMC);
                Distribution.push_back(MCpoint);
                if(counter==nextprint)
                {
                    std::cout<<"____________________ Number of run so far ="<<nextprint<<std::endl;
                    nextprint+=1000;
                }
                counter++;
            }
        }
        
    }
    else
    {
        std::cout<<"[ERROR] Total statistics must be a multiple of the given sample size\n";
        return -1;
    }
    
    
    
    std::cout<<"Distribution computed with "<< Distribution.size()<<" iterations\n";
    
    
    // Temp for test
    std::vector<std::string> treename=fConfig->GetPar< std::vector<std::string> >("sim.file.output.tree");
    std::vector<std::string> branchname=fConfig->GetPar< std::vector<std::string> >("sim.file.output.branch");
    std::string classname="OscMCPoint";
    
    
    std::cout<<"Distribution will be saved to file "<< filename <<"\n";
    
    RootOutFileManager<OscMCPoint>* OutMan = new RootOutFileManager<OscMCPoint>();
    OutMan->SetFileProperties(filename,treename[0],branchname[0],classname,std::string("RECREATE"),true);
    OutMan->InitOutFile();
    OutMan->AddToFile(Distribution);
    delete OutMan;
    std::cout<<"Distribution saved to file "<< filename <<"\n";
    return 0;
}



int OscSimulation::RunMCMC(RooDataSet* roodataset)
{
    
    //cout<<"start run MCMC"<<endl;
    std::cout<<"LRT::RunMCMC - OK0\n";
    
    // ----------------------------------------------------
    /// Set Model M1
    // ----------------------------------------------------
    OscModel* ModelM1 = new OscModel(fConfig);
    ModelM1->SetSimulation(true);
    
    //prior
    ModelM1->SetPriorConstant(0);
    ModelM1->SetPriorConstant(1);
    ModelM1->SetPriorConstant(2);
    ModelM1->SetPriorConstant(3);
    SetModelOption(ModelM1,fConfig);
    
    OscDataSet* DataSet = new OscDataSet(fConfig);
    DataSet->ConvertRooToBCDataset(roodataset);
    ModelM1->SetMyDataSet(DataSet);
    ModelM1->MarginalizeAll();
    ModelM1->GetMCMCMLEValue(fMCpoint);
    delete DataSet;
    return 0;
}


int OscSimulation::UpdateRooParameterFromMCMC()
{
    fAmp1->setVal(fMCpoint.MCMCamplitude);
    fOmega1->setVal(fMCpoint.MCMComega);
    fPhi1->setVal(fMCpoint.MCMCphi);
    fLambda1->setVal(fMCpoint.MCMClambda1);

    return 0;
}


void OscSimulation::initAttributes(OscAnaManager* config)
{
    /// Import parameters from configfile
    // Range of the Analysis

    double fxmin=config->GetPar<double>("obs.xmin");
    double fxmax=config->GetPar<double>("obs.xmax");
    double flambdamin=config->GetPar<double>("par.lambda.min");
    double flambdamax=config->GetPar<double>("par.lambda.max");
    double famin=config->GetPar<double>("par.amp.min");
    double famax=config->GetPar<double>("par.amp.max");
    double fomegamin=config->GetPar<double>("par.omega.min");
    double fomegamax=config->GetPar<double>("par.omega.max");
    double fphimin=config->GetPar<double>("par.phi.min");
    double fphimax=config->GetPar<double>("par.phi.max");

    // Model parameters for simulation or/and Hypothesis test
    double LambdaInit=config->GetPar<double>("par.lambda.init");
    double PhiInit=config->GetPar<double>("par.phi.init");
    double OmegaInit=config->GetPar<double>("par.omega.init");
    double AmpInit=config->GetPar<double>("par.amp.init");

    /// //////////  Define random variable and parameters ////////////
    //Define observable (rdv)
    fx = new RooRealVar("x","x",fxmin,fxmax);

    //Define null parameters
    fLambda0 = new RooRealVar("#lambda_{0}","#lambda_{0}",LambdaInit,flambdamin,flambdamax);

    //Define alt parameters
    fLambda1 = new RooRealVar("#lambda_{1}","#lambda_{1}",LambdaInit,flambdamin,flambdamax);
    fAmp1 = new RooRealVar("a_{1}","a_{1}",AmpInit,famin,famax);
    fOmega1 = new RooRealVar("#omega_{1}","#omega_{1}",OmegaInit,fomegamin,fomegamax);
    fPhi1 = new RooRealVar("#phi_{1}","#phi_{1}",PhiInit,fphimin,fphimax);


    /// //////////  Define PDF ////////////
    //Define null model H0
    fPdf_H0 = new RooGenericPdf("H0","#lambda_{0}*exp(-#lambda_{0}*x)",RooArgSet(*fx,*fLambda0));

    // Define alt model with analytical integration over x (time)
    fPdf_H1s = new RooMyAnalyticalPdf("H1s","H1s",*fx,*fLambda1,*fAmp1,*fOmega1,*fPhi1);

    fPdf_H1b = new RooGenericPdf("H1","#lambda_{1}*(#lambda_{1}*#lambda_{1}+#omega_{1}*#omega_{1})/(#lambda_{1}*#lambda_{1}*(1+a_{1})+#omega_{1}*#omega_{1})*(1+a_{1}*cos(#omega_{1}*x+#phi_{1}))*exp(-#lambda_{1}*x)"
            ,RooArgSet(*fx,*fAmp1,*fOmega1,*fPhi1,*fLambda1));

    fmeancool = new RooRealVar("#mu_{cool}","#mu_{cool}",0.565);
    fsigmacool = new RooRealVar("#sigma_{cool}","#sigma_{cool}",0.174);
    fmeanerr = new RooRealVar("#mu_{err}","#mu_{err}",0.0);
    fsigmaerr = new RooRealVar("#sigma_{err}","#sigma_{err}",0.064);



    fx_cool = new RooRealVar("xcool","xcool",0.0,3.0);
    fx_err =new RooRealVar("xerr","xerr",-1,1);


    //RooGaussian gauss("gauss","gaussian PDF",x,mean,sigma);
    fCoolingDistribution = new RooGaussian("CoolDist","CoolDist",*fx_cool,*fmeancool,*fsigmacool);
    fErrorDistribution = new RooGaussian("ErrDist","ErrDist",*fx_err,*fmeanerr,*fsigmaerr);

}

