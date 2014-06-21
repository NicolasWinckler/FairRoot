
#include "OscillationAnalysis.h"
#include "OscRooSimulation.h"
//todo : make script


int RunOscUnbinnedBayesianAnalysis(string filename);
int RunSimulData(string configfilename);
int RunFitSimulData(string configfilename);



int main()
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    wkspDir+="/Oscillation/config/";
    string filename=wkspDir+"configfile142Pm60_4k_PBEC_2010.txt";
    
    string Simfilename=wkspDir+"configfile142Pm60_Simualtion.txt";
    //RunOscUnbinnedBayesianAnalysis(filename);
    //RunSimulData(filename);
    RunFitSimulData(Simfilename);
    
    return 0;
}



int RunOscUnbinnedBayesianAnalysis(string filename)
{
    OscillationAnalysis* myAnalysis = new OscillationAnalysis(filename);
    myAnalysis->RunAnalysis();
    
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
    sim->GenerateData("DataTest2",10.0,3616.0);
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
    
    TDatime* da = new TDatime();
    cout<<"Pull analysis start : "<<da->GetTime()<<endl;
    
    OscRooSimulation* sim = new OscRooSimulation(configfilename);
    sim->SetOutputDirectory(outputdir);
    sim->LoadDataFromFile("DataTest2");
    sim->GetPullDistribution(3616);
    
    
    //sim->FitToH0();
    TDatime* daend = new TDatime();
    
    int endsec=daend->GetSecond();
    int endmin=daend->GetMinute();
    int endhour=daend->GetHour();
    
    int startsec=da->GetSecond();
    int starmin=da->GetMinute();
    int starthour=da->GetHour();
    
    int duration=(endhour-starthour)*3600+(endmin-starmin)*60+endsec-startsec;
    
    cout<<"Pull analysis end : "<<daend->GetTime()<<endl;
    cout<<"Duration="<<duration<<" (sec)"<<endl;
    cout<<"Duration="<< (double)duration/60.<<" (min)"<<endl;
    cout<<"Duration="<< (double)duration/3600.<<" (hour)"<<endl;
    
    delete sim;
    return 0;
}


