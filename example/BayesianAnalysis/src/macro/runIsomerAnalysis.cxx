
#include "IsomerAnalysis.h"
#include <stdlib.h>
#include "TApplication.h"
//todo : make script
int main(int argc, char *argv[])
{
    char* pPath;
    pPath = getenv ("ANAWSPDIR");
    string wkspDir(pPath);
    wkspDir+="/Isomer/config/";
    //string filename=wkspDir+"Config52Co.txt";
    string filename=wkspDir+"Config27Si.txt";
    
    IsomerAnalysis* myAnalysis = new IsomerAnalysis(filename);
    //myAnalysis->RunAnalysis();
    //myAnalysis->FitOtherPeaks();
    //myAnalysis->TempTest();
    
    
    bool draw=true;
    TApplication app("App", &argc, argv);
    myAnalysis->FitOnePeak();
    
    
    if(draw) app.Run();
    delete myAnalysis;
    return 0;
}

