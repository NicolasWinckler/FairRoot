
#include "IsomerAnalysis.h"
#include <stdlib.h>
//todo : make script
int main()
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    wkspDir+="/Isomer/config/";
    string filename=wkspDir+"Config52Co.txt";
    
    IsomerAnalysis* myAnalysis = new IsomerAnalysis(filename);
    myAnalysis->RunAnalysis();
    //myAnalysis->FitOtherPeaks();
    //myAnalysis->TempTest();
    
    delete myAnalysis;
    return 0;
}

