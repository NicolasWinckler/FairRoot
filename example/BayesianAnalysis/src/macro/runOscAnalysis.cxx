
#include "OscillationAnalysis.h"
#include "OscRooSimulation.h"
#include <chrono>
//todo : make script


int RunOscUnbinnedBayesianAnalysis(string filename);
int RunOscBinnedBayesianAnalysis(string filename);
int RunSimulData(string configfilename);
int RunFitSimulData(string configfilename);
int ReadTree();
int TestBatchFarm();

int main()
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    wkspDir+="/Oscillation/config/";
    //string filename=wkspDir+"configfile142Pm60_4k_PBEC_2010.txt";
    //string filename=wkspDir+"configfile142PmPBBeta.txt";
    string filename=wkspDir+"configfile142PmSONY.txt";
    
    string Simfilename=wkspDir+"configfile142Pm60_Simualtion.txt";
    RunOscUnbinnedBayesianAnalysis(filename);
    //RunOscBinnedBayesianAnalysis(filename);
    //RunSimulData(filename);
    //RunFitSimulData(Simfilename);
    
    return 0;
}



int RunOscUnbinnedBayesianAnalysis(string filename)
{
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
    sim->GenerateData("SimH0ECData5000_b",5002.0,3616.0);
    sim->SaveSimData();
    
    std::cout<<"Simulation finished"<<std::endl;
    delete sim;
    return 0;
}

int RunFitSimulData(string configfilename)
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
    
    
    //create two histograms
    TH1D *h2NLLR   = new TH1D("h2NLLR","-2Log(L0/L1)",1000,-3,3);
    //read all entries and fill the histograms
    Int_t nentries = (Int_t)PullTree->GetEntries();
    for (Int_t i=0; i<nentries; i++) 
    {
          PullTree->GetEntry(i);
          h2NLLR->Fill(NLLR-5.0);
    }
    
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



int TestBatchFarm()
{
    
    
    return 0;
}


