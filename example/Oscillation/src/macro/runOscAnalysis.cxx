
#include "Analysis.h"

int main()
{
    string wkspDir("/Users/winckler/fair/FairRoot/example/Oscillation/workspace/");
    string filename=wkspDir+"configfile142Pm60_4k_PBEC_2010.txt";
    Analysis myAnalysis;
    
    myAnalysis.GetBayesFactor(filename);
    //myAnalysis.RunTuto1( );
    return 0;
}
