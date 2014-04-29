
#include "OscillationAnalysis.h"
//todo : make script
int main()
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    wkspDir+="/Oscillation/config/";
    string filename=wkspDir+"configfile142Pm60_4k_PBEC_2010.txt";
    
    OscillationAnalysis* myAnalysis = new OscillationAnalysis(filename);
    myAnalysis->RunAnalysis();
    
    delete myAnalysis;
    return 0;
}
