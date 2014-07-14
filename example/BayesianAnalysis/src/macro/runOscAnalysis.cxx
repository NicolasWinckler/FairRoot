
#include "OscillationAnalysis.h"
#include "OscRooSimulation.h"
#include <chrono>
//todo : make script


int RunOscUnbinnedBayesianModelSelection(string filename);
int RunOscBinnedBayesianAnalysis(string filename);
int RunSimulData(string configfilename);
int RunMCMCFitSimulData(string configfilename);
int RunMinimizationFitSimulData(string configfilename);
int ReadTree();
int TestBatchFarm();
int RunInfoCriteria(string configfilename);
int RunOscUnbinnedMarginalization(string filename);

int RunPlotExpectedLogLikelihoodL1part();
double MinusExpectedModulationwithNorm(double *omegaphi, double *par);



int main(int argc, char *argv[])
{
    
    string Configfilename;
    string analysisoption;

    if (argc > 2)
    {
            while(--argc > 1 && **++argv == '-') switch (toupper(argv[0][1])) 
            {
            case 'S':
                    Configfilename = (--argc,*++argv);
                    break;

            case 'O':
                    analysisoption = (--argc,*++argv);
                    break;
            }

            RunInfoCriteria(Configfilename);
            //RunOscUnbinnedBayesianModelSelection(Configfilename);
            //RunOscUnbinnedMarginalization(Configfilename);

    }
    else
    {
        char* pPath;
        pPath = getenv ("ANAWSPDIR");
        string wkspDir(pPath);
        wkspDir+="/Oscillation/config/";
        /*
        //Configfilename=wkspDir+"configfile142Pm60_4k_PBEC_2010.txt";
        //Configfilename=wkspDir+"configfile142PmPBBeta.txt";
        Configfilename=wkspDir+"configfile142PmSONY.txt";*/

        string SimConfigfilename=wkspDir+"configfile142Pm60_Simualtion.txt";
        //RunOscUnbinnedBayesianModelSelection(Simfilename);
        //RunOscBinnedBayesianAnalysis(Configfilename);
        //RunSimulData(SimConfigfilename);
        //RunMCMCFitSimulData(SimConfigfilename);
        RunMinimizationFitSimulData(SimConfigfilename);
        
    }
    return 0;
}

int RunOscUnbinnedMarginalization(string filename)
{
    OscillationAnalysis* myAnalysis = new OscillationAnalysis(filename);
    myAnalysis->RunTest();
    
    return 0;
}


int RunOscUnbinnedBayesianModelSelection(string filename)
{
    auto temps1 = std::chrono::system_clock::now();
    OscillationAnalysis* myAnalysis = new OscillationAnalysis(filename);
    //myAnalysis->RunAnalysis();
    double fact1=myAnalysis->RunBayesTest(false);
    double fact2=myAnalysis->RunBayesTest(true);
    double N=myAnalysis->GetSampleSize();
    
    cout<<"------------------------------------------------------"<<endl;
    cout<<"Bayes factor without Max Likelihood shift = "<< fact1 <<endl;
    cout<<"Bayes factor with Max Likelihood shift = "<< fact2 <<endl;
    cout<<"Ratio (without/with) = "<< fact1/fact2 <<endl;
    cout<<"------------------------------------------------------"<<endl;
    cout<<"-----------------------SUMMARY------------------------"<<endl;
    cout<<"Size N  = "<< N <<endl;
    cout<<"P(M0)   = "<< myAnalysis->GetPriorM0() <<endl;
    cout<<"P(M1)   = "<< myAnalysis->GetPriorM1() <<endl;
    cout<<"-Evidence:"<<endl;
    cout<<"P(D|M0) = "<< myAnalysis->GetEvidenceM0() <<endl;
    cout<<"P(D|M1) = "<< myAnalysis->GetEvidenceM1() <<endl;
    cout<<"-Model probabilities:"<<endl;
    cout<<"P(M0|D) = "<< myAnalysis->GetProbM0()*100.0<<"%" <<endl;
    cout<<"P(M1|D) = "<< myAnalysis->GetProbM1()*100.0<<"%" <<endl;
    cout<<"-Bayes factors:"<<endl;
    cout<<"B01     = "<< myAnalysis->GetB01() <<endl;
    cout<<"B10     = "<< myAnalysis->GetB10() <<endl;
    cout<<"------------------------------------------------------"<<endl;
    
    
    auto temps2 = std::chrono::system_clock::now();
    
    auto duration= ((temps2 - temps1).count());
    cout<<"Duration="<< duration/(1000000)<<" (sec)"<<endl;
    cout<<"Duration="<< duration/60.<<" (min)"<<endl;
    cout<<"Duration="<< duration/3600.<<" (hour)"<<endl;
    cout<<"------------------------------------------------------"<<endl;
    delete myAnalysis;
    return 0;
}




int RunOscBinnedBayesianAnalysis(string filename)
{
    OscillationAnalysis* myAnalysis = new OscillationAnalysis(filename);
    myAnalysis->RunBinnedAnalysis();
    
    delete myAnalysis;
    return 0;
}

int RunSimulData(string configfilename)
{
    
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    string outputdir=wkspDir+"/Oscillation/OscSimuData/";
    std::cout<<"Simulation starts"<<std::endl;
    
    OscRooSimulation* sim = new OscRooSimulation(configfilename);
    sim->SetOutputDirectory(outputdir);
    sim->GenerateData("SimH0ECData5000_b",3616.0,5002.0);
    //sim->GeneratePickupData("SimUnderH1_1000",1000.0);
    sim->SaveSimData();
    
    std::cout<<"Simulation finished"<<std::endl;
    delete sim;
    return 0;
}

int RunMCMCFitSimulData(string configfilename)
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    string outputdir=wkspDir+"/Oscillation/OscSimuData/";
    
    auto temps1 = std::chrono::system_clock::now();
    TDatime* da = new TDatime();
    cout<<"Pull analysis start : "<<da->GetTime()<<endl;
    
    OscRooSimulation* sim = new OscRooSimulation(configfilename);
    sim->SetOutputDirectory(outputdir);
    //sim->LoadDataFromFile("DataTest2");
    sim->LoadDataFromFile("SimH0ECData5000_b");
    sim->GetPullDistribution(3616);
    
    
    //sim->FitToH0();
    TDatime* daend = new TDatime();
    /*int endday=daend->GetDay();
    int endsec=daend->GetSecond();
    int endmin=daend->GetMinute();
    int endhour=daend->GetHour();
    
    int starday=da->GetDay();
    int startsec=da->GetSecond();
    int starmin=da->GetMinute();
    int starthour=da->GetHour();
    
    int daydiff=endday-starday;*/
    //int duration=((endhour+)-starthour)*3600+(endmin-starmin)*60+endsec-startsec;
    
    
    auto temps2 = std::chrono::system_clock::now();
    
    double duration= (double)((temps2 - temps1).count());
    //std::cout << (temps2 - temps1).count() << " microsecondes." << std::endl;
    
    
    cout<<"Pull analysis end : "<<daend->GetTime()<<endl;
    cout<<"Duration="<< duration/(1000000)<<" (sec)"<<endl;
    cout<<"Duration="<< duration/60.<<" (min)"<<endl;
    cout<<"Duration="<< duration/3600.<<" (hour)"<<endl;
    
    delete sim;
    return 0;
}



int RunMinimizationFitSimulData(string configfilename)
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    string outputdir=wkspDir+"/Oscillation/OscSimuData/";
    
    auto temps1 = std::chrono::system_clock::now();
    TDatime* da = new TDatime();
    cout<<"Pull analysis start : "<<da->GetTime()<<endl;
    
    OscRooSimulation* sim = new OscRooSimulation(configfilename);
    sim->SetOutputDirectory(outputdir);
    //sim->LoadDataFromFile("DataTest2");
    sim->LoadDataFromFile("SimH0ECData5000");
    sim->GetPullDistribution(3616, false);
    
    
    //sim->FitToH0();
    TDatime* daend = new TDatime();
    /*int endday=daend->GetDay();
    int endsec=daend->GetSecond();
    int endmin=daend->GetMinute();
    int endhour=daend->GetHour();
    
    int starday=da->GetDay();
    int startsec=da->GetSecond();
    int starmin=da->GetMinute();
    int starthour=da->GetHour();
    
    int daydiff=endday-starday;*/
    //int duration=((endhour+)-starthour)*3600+(endmin-starmin)*60+endsec-startsec;
    
    
    auto temps2 = std::chrono::system_clock::now();
    
    double duration= (double)((temps2 - temps1).count());
    //std::cout << (temps2 - temps1).count() << " microsecondes." << std::endl;
    
    
    cout<<"Pull analysis end : "<<daend->GetTime()<<endl;
    cout<<"Duration="<< duration/(1000000)<<" (sec)"<<endl;
    cout<<"Duration="<< duration/60.<<" (min)"<<endl;
    cout<<"Duration="<< duration/3600.<<" (hour)"<<endl;
    
    delete sim;
    return 0;
}

int ReadTree()
{
    //read the Tree generated by tree1w and fill two histograms
    //note that we use "new" to create the TFile and TTree objects,
    //to keep them alive after leaving this function.
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    string outputdir=wkspDir+"/Oscillation/OscSimuData/";
    
    //int pullstat=(int)(Ntot/SampleSize);
        
    TString treename="PullDistribution_";
    TString filename=outputdir;
    filename+=treename;
    filename+=".root";

    
    TFile* PullFile = new TFile(filename);
    TTree* PullTree= (TTree*)PullFile->Get(treename);
    
    int N;
    double NLL0;
    double lambda0;
    double NLL1;
    double lambda;
    double amp;
    double omega;
    double phi;
    double erN;
    double erNLL0;
    double erlambda0;
    double erNLL1;
    double erlambda;
    double eramp;
    double eromega;
    double erphi;
    double NLLR;
    
    
    PullTree->SetBranchAddress("N",&N);
    PullTree->SetBranchAddress("NLL0",&NLL0);
    PullTree->SetBranchAddress("lambda0",&lambda0);
    PullTree->SetBranchAddress("NLL1",&NLL1);
    PullTree->SetBranchAddress("lambda",&lambda);
    PullTree->SetBranchAddress("amp",&amp);
    PullTree->SetBranchAddress("omega",&omega);
    PullTree->SetBranchAddress("phi",&phi);
    // errors
    PullTree->SetBranchAddress("erN",&erN);	
    PullTree->SetBranchAddress("erlambda0",&erlambda0);
    PullTree->SetBranchAddress("erlambda",&erlambda);
    PullTree->SetBranchAddress("eramp",&eramp);
    PullTree->SetBranchAddress("eromega",&eromega);
    PullTree->SetBranchAddress("erphi",&erphi);
    PullTree->SetBranchAddress("NLLR",&NLLR);
    
    double TrueLambda0=0.0139;
    double TrueLambda1=0.0139;
    double TrueAmp=0.0;
    double TrueOmega=0.0;
    double TruePhi=0.0;
    
    //create two histograms
    TH1D *h2NLLR   = new TH1D("h2NLLR","-2Log(L0/L1)",10000,-1.0,40.0);
    TH1D *hPULLlambda0   = new TH1D("PullDistLambda0","PullDistLambda0",10000,-10.0,10.0);
    TH1D *hPULLlambda1   = new TH1D("PullDistLambda1","PullDistLambda1",10000,-10.0,10.0);
    TH1D *hPULLAmp   = new TH1D("PullDistAmp","PullDistAmp",10000,0.0,10.0);
    TH1D *hBiasAmp   = new TH1D("Amp","Amp",10000,0.0,1.0);
    TH1D *hPULLOmega   = new TH1D("PullDistOmega","PullDistOmega",100000,0.0,1000.0);
    TH1D *hPULLPhi   = new TH1D("PullDistPhi","PullDistPhi",10000,0.0,70.0);
    
    
    //read all entries and fill the histograms
    Int_t nentries = (Int_t)PullTree->GetEntries();
    for (Int_t i=0; i<nentries; i++) 
    {
          PullTree->GetEntry(i);
          
          //-2Log(LRT)
          h2NLLR->Fill(NLLR);
          
          // pull lambda0
          
          double pullLambda0=(lambda0-TrueLambda0)/erlambda0;
          hPULLlambda0->Fill(pullLambda0);
          
          // pull lambda1
          
          double pullLambda1=(lambda-TrueLambda1)/erlambda;
          hPULLlambda1->Fill(pullLambda1);
          
          // pull |a|
          
          double pullAmp=(fabs(amp)-TrueAmp)/eramp;
          hPULLAmp->Fill(pullAmp);
          
          // bias |a|
          pullAmp=fabs(amp);
          hBiasAmp->Fill(pullAmp);
          
          // pull omega
          
          double pullOmega=(omega-TrueOmega)/eromega;
          hPULLOmega->Fill(pullOmega);
          
          // pull phi
          
          double pullPhi=(phi-TruePhi)/erphi;
          hPULLPhi->Fill(pullPhi);
          
    }
    
    double Entries=h2NLLR->GetEntries();
    if(Entries!=0)
        h2NLLR->Scale(1.0/Entries);
    
    /*
    Entries=hPULLlambda0->GetEntries();
    if(Entries!=0)
        hPULLlambda0->Scale(1.0/Entries);
        
    Entries=hPULLlambda1->GetEntries();
    if(Entries!=0)
        hPULLlambda1->Scale(1.0/Entries);
        
    Entries=hPULLAmp->GetEntries();
    if(Entries!=0)
        hPULLAmp->Scale(1.0/Entries);
        
    Entries=hPULLOmega->GetEntries();
    if(Entries!=0)
        hPULLOmega->Scale(1.0/Entries);
        
    Entries=hPULLPhi->GetEntries();
    if(Entries!=0)
        hPULLPhi->Scale(1.0/Entries);
    
    PullFile->Close();
        */
    
    h2NLLR->GetXaxis()->SetTitle("-2Log(L_{0}/L_{L1})");
    hPULLlambda0->GetXaxis()->SetTitle("(#hat{#lambda_{0}}-#lambda_{0})/#sigma_{#lambda_{0}}");
    hPULLlambda1->GetXaxis()->SetTitle("(#hat{#lambda_{1}}-#lambda_{1})/#sigma_{#lambda_{1}}");
    hPULLAmp->GetXaxis()->SetTitle("#hat{a}/#sigma_{a}");
    hBiasAmp->GetXaxis()->SetTitle("#hat{a}");
    hPULLOmega->GetXaxis()->SetTitle("#hat{#omega}/#sigma_{#omega}");
    hPULLPhi->GetXaxis()->SetTitle("#hat{#phi}/#sigma_{#phi}");
    
    
    TString Outfilename(outputdir);
    Outfilename+="Histo_";
    Outfilename+=treename;
    Outfilename+=".root";
    TFile histofile(Outfilename,"RECREATE");
    h2NLLR->Write();
    hPULLlambda0->Write();
    hPULLlambda1->Write();
    hPULLAmp->Write();
    hPULLOmega->Write();
    hPULLPhi->Write();
    hBiasAmp->Write();

    histofile.Close();
    
    /*
    //We do not close the file. We want to keep the generated histograms
    //we open a browser and the TreeViewer
    if (gROOT->IsBatch()) return;
    new TBrowser ();
    t1->StartViewer();

    //In the browser, click on "ROOT Files", then on "tree1.root"
    //You can click on the histogram icons in the right panel to draw
    //them in the TreeViewer, follow the instructions in the Help.
    */
    return 0;
}

int RunInfoCriteria(string configfilename)
{
    
    OscillationAnalysis* myAnalysis = new OscillationAnalysis(configfilename);
    //myAnalysis->RunAnalysis();
    
    myAnalysis->GetInformationCriteriaFromMCMC();
            
            
    return 0;
}

int TestBatchFarm()
{
    
    
    return 0;
}



int RunPlotExpectedLogLikelihoodL1part()
{
    
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    string outputdir=wkspDir+"/Oscillation/OscSimuData/";
    TF2 *f2 = new TF2("f2",MinusExpectedModulationwithNorm,0.0,1,0,2*pi, 2);
	
    f2->SetNpx(500);
    f2->SetNpy(500);
    f2->SetParameters(0.0138,0.1);
    
    double omegamin=0;
    double phimin=0;
    f2->GetMinimumXY(omegamin,phimin);
	
    cout<<"omega min = "<<omegamin<<endl;
    cout<<"phi min = "<<phimin<<endl;
    
    TH2D* histo=(TH2D*)f2->CreateHistogram();
    histo->GetXaxis()->SetTitle("#omega");
    histo->GetYaxis()->SetTitle("#phi");
    
    
    TString Outfilename(outputdir);
    Outfilename+="Histo_EXPECTEDLRT";
    Outfilename+=".root";
    TFile histofile(Outfilename,"RECREATE");
    histo->Write();

    histofile.Close();
    
}        


double MinusExpectedModulationwithNorm(double *omegaphi, double *par)
{
	// \frac{a}{1+omega^2/lambda} 
	//[ (omega/lambda sin(omega t + phi)
	//- cos(omega t + phi) ) e^{-lambda t]_{t_{min}}^{t_{max}}
	// par 0 -> lambda, 1->a, 2->tmin, 3->tmax
	// omegaphi 0-> omega, 1->phi
        /*
	double val=0.0;
	double r=1.0;
	if(par[0]!=0) r=omegaphi[0]/par[0];
	else cout<<"*** Warning ***: lambda is equal to zero"<<endl;
	double fact=par[1]/(1.0+omegaphi[0]*r);
	double partmax=TMath::Exp(-par[0]*par[3])*(r*TMath::Sin(omegaphi[0]*par[3]+omegaphi[1])-TMath::Cos(omegaphi[0]*par[3]+omegaphi[1]));
	double partmin=TMath::Exp(-par[0]*par[2])*(r*TMath::Sin(omegaphi[0]*par[2]+omegaphi[1])-TMath::Cos(omegaphi[0]*par[2]+omegaphi[1]));
	
	double DataPart=fact*(partmax-partmin);
	
	
	double pa=TMath::Log(omegaphi[0]*omegaphi[0]+par[0]*par[0]);
	double pb=TMath::Log(omegaphi[0]*omegaphi[0]+(1+par[1])*par[0]*par[0]);
	double NormPart=pa-pb;
	//NormPart=0.0;
	//DataPart=0.0;
	val=NormPart+DataPart;
	*/
    
        double val=0.0;
        if(par[0]!=0) r=omegaphi[0]/par[0];
        
        val=TMath::Cos(omegaphi[1])*par[1]/(1.0+omegaphi[0]*r);
    
	return val;
}



