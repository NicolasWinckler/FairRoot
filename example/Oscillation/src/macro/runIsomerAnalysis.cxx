
#include "IsomerAnalysis.h"

int main()
{
    string wkspDir("/Users/winckler/fair/FairRoot/example/Oscillation/workspace/");
    string filename=wkspDir+"configfile52Co_isomer.txt";
    IsomerAnalysis* myAnalysis = new IsomerAnalysis(filename);
    
    //myAnalysis->RunAnalysis();
    myAnalysis->FitOtherPeaks();
    
    
    delete myAnalysis;
    return 0;
}

