/* 
 * File:   OscRooSimulation.cxx
 * Author: winckler
 * 
 * Created on June 20, 2014, 12:54 PM
 */

#include "OscRooSimulation.h"

OscRooSimulation::OscRooSimulation(string filename) 
{
    RooDataSet::setDefaultStorageType(RooAbsData::Tree);
    InitField();
    fConfiguration->SetExperimentalParameter(filename);
    gROOT->Reset();
    // silence and init attributes
    RooMsgService::instance().setGlobalKillBelow(ERROR);
    initAttributes(fConfiguration);
    m_pdf_H0->verboseEval(-1);
    m_pdf_H1s->verboseEval(-1);
	
	
        
}


OscRooSimulation::~OscRooSimulation() 
{
    delete m_x;
    delete m_lambda0;
    delete m_lambda1;
    delete m_amp1;
    delete m_omega1;
    delete m_phi1;
    delete m_pdf_H0;
    delete m_pdf_H1s;
    
}



int OscRooSimulation::GenerateData(string DataName, double NumSimEvt, double PullStats)
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
	m_dataSim = m_pdf_H0->generate(*m_x,TotSimEvt,Name(DataName.c_str()));// simu under H0
	//m_DataSim = m_pdf_H1s->generate(*m_x,NumSimEvt);// simu under H1
        
        
        
        return 0;
}


int OscRooSimulation::SaveSimData(string OutputDir)
{
        
        // save data
        string filename=OutputDir+(string)m_dataSim->GetName();
        filename+=".root";
        TFile rootfile(filename.c_str(),"RECREATE");
        m_dataSim->Write();
        
        
        rootfile.Close();
        
        return 0;
}



int OscRooSimulation::LoadDataFromFile(string DataName)
{
    
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    string outputdir=wkspDir+"/Oscillation/OscSimuData/";
    string filename=outputdir+DataName;
    filename+=".root";
    TFile* rootfile = new TFile(filename.c_str());
    //RooWorkspace* w = (RooWorkspace*) f->Get("w") ;
    m_dataSim= (RooDataSet*) rootfile->Get(DataName.c_str());
    
    rootfile->Close();
    delete rootfile;
    return 0;
}

int OscRooSimulation::CutData(int indexmin, int indexmax)
{
    fReducedDataSet = (RooDataSet*) m_dataSim->reduce(EventRange(indexmin,indexmax));
    return 0;
}



int OscRooSimulation::RunMCMC(RooDataSet* roodataset)
{
    // set nicer style for drawing than the ROOT default
    BCAux::SetStyle();
    
    BCLog::SetLogLevel(BCLog::error);
    fMCMCdone=false;
    
    // ----------------------------------------------------
    /// Load Data and other variables
    // ----------------------------------------------------
    SidsDataSet* DataSet = new SidsDataSet();
    DataSet->ReadRooDataSet(roodataset);
    
    // ----------------------------------------------------
    /// Set Model M1
    // ----------------------------------------------------
    OscModel* fM1 = new OscModel(fConfiguration);
    fM1->SetSimulation(true);
    fM1->SetMyDataSet(DataSet);
    //prior
    fM1->SetPriorConstant(0);
    fM1->SetPriorConstant(1);
    fM1->SetPriorConstant(2);
    fM1->SetPriorConstant(3);
    SetModelOption(fM1,fConfiguration);
    
    //BCLog::OutSummary("Model M1 created");
    fM1->MarginalizeAll();
    
    vector<double> MLE_Values=fM1->GetMCMCMLEValue();
    
    if(MLE_Values.size()==4)
    {
        m_lambdatot=MLE_Values[0];
        m_amp=MLE_Values[1];
        m_omega=MLE_Values[2];
        m_phi=MLE_Values[3];
        fMCMCdone=true;
    }
    else
    {
        BCLog::OutSummary("ERROR : MCMC MLE values not obtained from current run");
        
    }
    
    
    delete DataSet;
    delete fM1;
    
    return 0;
}



int OscRooSimulation::UpdateRooParameterFromMCMC()
{
    if(fMCMCdone)
    {
        m_amp1->setVal(m_amp);
        m_omega1->setVal(m_omega);
        m_phi1->setVal(m_phi);
        m_lambda1->setVal(m_lambdatot);
    }
    return 0;
}


int OscRooSimulation::FitToH1()
{
	
    
    m_pdf_H1s->fitTo(*fReducedDataSet,PrintLevel(-1));    
    return 0;
}


int OscRooSimulation::FitToH0()
{
	m_pdf_H0->fitTo(*m_dataSim);
        
        return 0;
}

vector< vector<double> > OscRooSimulation::GetSetOfMLE()
{
	vector<double> Vector;
	vector<double> Vectorerr;
	vector< vector<double> > V;
	
	m_pdf_H0->fitTo(*fReducedDataSet,PrintLevel(-1));
	RooAbsReal* NLL0= m_pdf_H0->createNLL(*fReducedDataSet);
	
	m_pdf_H1s->fitTo(*fReducedDataSet,PrintLevel(-1));
	RooAbsReal* NLL1= m_pdf_H1s->createNLL(*fReducedDataSet);
	
	Vector.push_back(fReducedDataSet->numEntries());	//   [0]
	
	Vector.push_back(NLL0->getValV());		//   [1]
	Vector.push_back(m_lambda0->getValV());		//   [2]
	
	Vector.push_back(NLL1->getValV());		//   [3]
	Vector.push_back(m_lambda1->getValV());		//   [4]
	Vector.push_back(m_amp1->getValV());		//   [5]
	Vector.push_back(m_omega1->getValV());		//   [6]
	Vector.push_back(m_phi1->getValV());		//   [7]
	
							//   [0]
	Vectorerr.push_back(TMath::Sqrt(fReducedDataSet->numEntries()));
	
	Vectorerr.push_back(0.0);			//   [1]
	Vectorerr.push_back(m_lambda0->getError());	//   [2]
	
	Vectorerr.push_back(0.0);			//   [3]
	Vectorerr.push_back(m_lambda1->getError());	//   [4]
	Vectorerr.push_back(m_amp1->getError());	//   [5]
	Vectorerr.push_back(m_omega1->getError());	//   [6]
	Vectorerr.push_back(m_phi1->getError());	//   [7]
	
	V.push_back(Vector);
	V.push_back(Vectorerr);
	return V;
}



int OscRooSimulation::GetPullDistribution()
{
    
    int indexmin=0;
    int indexmax=10;
    CutData(indexmin, indexmax);
    RunMCMC(fReducedDataSet);
    UpdateRooParameterFromMCMC();
    GetSetOfMLE();
    
    return 0;
}


int OscRooSimulation::InitField()
{
    // define field value names that are in configfile
    
    // analysis range
    fvalfield.push_back("xmin");
    fvalfield.push_back("xmax");
    fvalfield.push_back("xoffset");
    fvalfield.push_back("xunit");
    
    // parameter ranges
    fvalfield.push_back("lambdamin");
    fvalfield.push_back("lambdamax");
    fvalfield.push_back("amin");
    fvalfield.push_back("amax");
    fvalfield.push_back("omegamin");
    fvalfield.push_back("omegamax");
    fvalfield.push_back("phimin");
    fvalfield.push_back("phimax");
    
    // parameter binning
    fvalfield.push_back("BinLambda");
    fvalfield.push_back("BinAmp");
    fvalfield.push_back("BinOmega");
    fvalfield.push_back("BinPhi");
    
    // parameter initialization
    fvalfield.push_back("lambdaInit");
    fvalfield.push_back("phiInit");
    fvalfield.push_back("omegaInit");
    fvalfield.push_back("ampInit");
    
    
    // parameter outputs
    fcharfield.push_back("OutputPostpdfsM0");
    fcharfield.push_back("OutputPostpdfsM1");
    fcharfield.push_back("OutputSummaryM0");
    fcharfield.push_back("OutputSummaryM1");
    
    fConfiguration = new SidsParameters(fvalfield,fcharfield);
    return 0;
}

void OscRooSimulation::initAttributes(SidsParameters* Sidspar)
{
	/// Import parameters from configfile
	// Range of the Analysis
	
        fMCMCdone=false;
        fxmin=Sidspar->GetValue("xmin");
        fxmax=Sidspar->GetValue("xmax");
        flambdamin=Sidspar->GetValue("lambdamin");
        flambdamax=Sidspar->GetValue("lambdamax");
        famin=Sidspar->GetValue("amin");
        famax=Sidspar->GetValue("amax");
        fomegamin=Sidspar->GetValue("omegamin");
        fomegamax=Sidspar->GetValue("omegamax");
        fphimin=Sidspar->GetValue("phimin");
        fphimax=Sidspar->GetValue("phimax");
	
	// Model parameters for simulation or/and Hypothesis test
	m_lambdatot=Sidspar->GetValue("lambdaInit");
        m_phi=Sidspar->GetValue("phiInit");
        m_omega=Sidspar->GetValue("omegaInit");
        m_amp=Sidspar->GetValue("ampInit");
	
	
	
	/// //////////  Define random variable and parameters ////////////
	//Define observable (rdv)
	m_x = new RooRealVar("x","x",fxmin,fxmax);
	
	//Define null parameters
	m_lambda0 = new RooRealVar("#lambda_{0}","#lambda_{0}",m_lambdatot,flambdamin,flambdamax);
	
	//Define alt parameters
        m_lambda1 = new RooRealVar("#lambda_{1}","#lambda_{1}",m_lambdatot,flambdamin,flambdamax);
	m_amp1 = new RooRealVar("a_{1}","a_{1}",m_amp,famin,famax);
	m_omega1 = new RooRealVar("#omega_{1}","#omega_{1}",m_omega,fomegamin,fomegamax);
	m_phi1 = new RooRealVar("#phi_{1}","#phi_{1}",m_phi,fphimin,fphimax);
	
	
	/// //////////  Define PDF ////////////
	//Define null model H0
	m_pdf_H0 = new RooGenericPdf("H0","#lambda_{0}*exp(-#lambda_{0}*x)",RooArgSet(*m_x,*m_lambda0));
	
	// Define alt model with analytical integration over x (time)
	m_pdf_H1s = new RooMyAnalyticalPdf("H1s","H1s",*m_x,*m_lambda1,*m_amp1,*m_omega1,*m_phi1);
	
	
}

