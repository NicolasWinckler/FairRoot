
#include "Analysis.h"

int main()
{
    string wkspDir("/Users/winckler/git/AnalysisProject/BC2014Oscv0.0/workspace/");
    string filename=wkspDir+"configfile142Pm60_4k_PBEC_2010.txt";
    Analysis myAnalysis;
    
    myAnalysis.GetBayesFactor(filename);
    
    return 0;
}
