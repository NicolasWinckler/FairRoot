
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
    //RunOscUnbinnedBayesianAnalysis(filename);
    RunSimulData(filename);
    //RunFitSimulData(filename);
    
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
    OscRooSimulation* sim = new OscRooSimulation(configfilename);
    sim->GenerateData("DataTest2",10.0,3616.0);
    sim->SaveSimData(outputdir);
    
    std::cout<<"Simulation finished"<<std::endl;
    delete sim;
    return 0;
}

int RunFitSimulData(string configfilename)
{
    OscRooSimulation* sim = new OscRooSimulation(configfilename);
    sim->LoadDataFromFile("DataTest");
    sim->FitToH0();
    
    
    delete sim;
    return 0;
}


