/* 
 * File:   OscRooSimulation.cxx
 * Author: winckler
 * 
 * Created on June 20, 2014, 12:54 PM
 */

#include "OscRooSimulation.h"

OscRooSimulation::OscRooSimulation(SidsParameters* Sidspar, double NumSimEvt) 
{
        gROOT->Reset();
	// silence and init attributes
	RooMsgService::instance().setGlobalKillBelow(ERROR);
	initAttributes(Sidspar);
	m_pdf_H0->verboseEval(-1);
	m_pdf_H1s->verboseEval(-1);
	
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
        
	m_dataSim = m_pdf_H0->generate(*m_x,NumSimEvt);
	//m_DataSim = m_pdf_H1s->generate(*m_x,NumSimEvt);
        
}


OscRooSimulation::~OscRooSimulation() 
{
    
}


void OscRooSimulation::FitToH1()
{
	m_pdf_H1s->fitTo(*m_dataSim,PrintLevel(-1));
}



vector< vector<double> > OscRooSimulation::GetMLEofM1( )
{
	vector<double> Vector;
	vector<double> Vectorerr;
	vector< vector<double> > V;
	
	m_pdf_H0->fitTo(*m_dataSim,PrintLevel(-1));
	RooAbsReal* NLL0= m_pdf_H0->createNLL(*m_dataSim);
	
	m_pdf_H1s->fitTo(*m_dataSim,PrintLevel(-1));
	RooAbsReal* NLL1= m_pdf_H1s->createNLL(*m_dataSim);
	
	Vector.push_back(m_dataSim->sumEntries());	//   [0]
	
	Vector.push_back(NLL0->getValV());		//   [1]
	Vector.push_back(m_lambda0->getValV());		//   [2]
	
	Vector.push_back(NLL1->getValV());		//   [3]
	Vector.push_back(m_lambda1->getValV());		//   [4]
	Vector.push_back(m_amp1->getValV());		//   [5]
	Vector.push_back(m_omega1->getValV());		//   [6]
	Vector.push_back(m_phi1->getValV());		//   [7]
	
							//   [0]
	Vectorerr.push_back(TMath::Sqrt(m_dataSim->sumEntries()));
	
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



void OscRooSimulation::initAttributes(SidsParameters* Sidspar)
{
	/// Import parameters from configfile
	// Range of the Analysis
	
        
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


/*
/// ////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////   RooFit Analytical PDF //////////////////////////////////////
/// ////////////////////////////////////////////////////////////////////////////////////


ClassImp(RooMyAnalyticalPdf)
// Constructor
RooMyAnalyticalPdf::RooMyAnalyticalPdf(const char *name,const char *title,RooAbsReal& _x,
	RooAbsReal& _lambda, RooAbsReal& _amp, RooAbsReal& _omega, RooAbsReal& _phi) : 
		 RooAbsPdf(name,title),
		x("x","Dependent",this,_x),
		lambda("lambda","Lambda",this,_lambda),
		amp("amp","Amp",this,_amp),
		omega("omega","Omega",this,_omega),
		phi("phi","Phi",this,_phi)
{
}
// Copy constructor
RooMyAnalyticalPdf::RooMyAnalyticalPdf(const RooMyAnalyticalPdf& other, const char* name) : RooAbsPdf(other,name),
		x("x",this,other.x),lambda("lambda",this,other.lambda),
		amp("amp",this,other.amp),omega("omega",this,other.omega),
		phi("phi",this,other.phi)
{
}
// Implementation of value calculation
//Double_t RooMyAnalyticalPdf::evaluate(const RooDataSet* dset) const
Double_t RooMyAnalyticalPdf::evaluate() const
{
	Double_t exp=TMath::Exp(-lambda*x);
	Double_t cos=1+amp*TMath::Cos(omega*x+phi);
	
	return cos*exp;
}



//////////////
Int_t RooMyAnalyticalPdf::getAnalyticalIntegral(RooArgSet& allVars,RooArgSet& analVars,const char* ) const 
{
	
	//if (matchArgs(allVars, analVars, x,amp,omega,phi)) return 4;
	//if (matchArgs(allVars, analVars, x,amp,omega)) return 3;
	//if (matchArgs(allVars, analVars, x,amp)) return 2;
	
	if (matchArgs(allVars, analVars, x)) return 1;
	return 0;
}

Double_t RooMyAnalyticalPdf::analyticalIntegral(Int_t code, const char* rangeName) const 
{
	//assert(code==1 || code==2) ;
	Double_t valueOfIntegral;
	if(code == 1)
	{
	//calculation of integral for variables from code 1
		Double_t part0=TMath::Exp(-lambda*x.min())-TMath::Exp(-lambda*x.max());
		Double_t factor=lambda*amp/(omega*omega+lambda*lambda);
		Double_t part1a=(omega*TMath::Sin(omega*x.max()+phi)-lambda*TMath::Cos(omega*x.max()+phi))*TMath::Exp(-lambda*x.max());
		Double_t part1b=(omega*TMath::Sin(omega*x.min()+phi)-lambda*TMath::Cos(omega*x.min()+phi))*TMath::Exp(-lambda*x.min());
		valueOfIntegral=(part0+factor*(part1a-part1b))/lambda;
	} 
	
	return valueOfIntegral;
}
*/


