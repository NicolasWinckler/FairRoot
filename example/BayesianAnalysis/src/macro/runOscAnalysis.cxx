
#include "OscillationAnalysis.h"
//todo : make script


int RunOscUnbinnedBayesianAnalysis(string filename);

int main()
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    wkspDir+="/Oscillation/config/";
    string filename=wkspDir+"configfile142Pm60_4k_PBEC_2010.txt";
    RunOscUnbinnedBayesianAnalysis(filename);
    
    return 0;
}



int RunOscUnbinnedBayesianAnalysis(string filename)
{
    OscillationAnalysis* myAnalysis = new OscillationAnalysis(filename);
    myAnalysis->RunAnalysis();
    
    delete myAnalysis;
    return 0;
}