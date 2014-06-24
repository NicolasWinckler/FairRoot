/* 
 * File:   OscillationAnalysis.cxx
 * Author: winckler
 * 
 * Created on April 19, 2014, 11:38 PM
 */

#include "OscillationAnalysis.h"

OscillationAnalysis::OscillationAnalysis() : Analysis()
{
    // define field value names that are in configfile
    InitField();
    
}


OscillationAnalysis::OscillationAnalysis(string filename) : Analysis()
{
    
    // define field value names that are in configfile
    InitField();
    fConfiguration->SetExperimentalParameter(filename);
    //*
    // ----------------------------------------------------
    // define output file names and open log file
    // ----------------------------------------------------
    string prefixOutputName=get_prefix(fConfiguration);
    string file_postpdfsM0=prefixOutputName + fConfiguration->GetName("OutputPostpdfsM0");
    string file_postpdfsM1=prefixOutputName + fConfiguration->GetName("OutputPostpdfsM1");
    string file_summaryM0=prefixOutputName + fConfiguration->GetName("OutputSummaryM0");
    string file_summaryM1=prefixOutputName + fConfiguration->GetName("OutputSummaryM1");
    string file_log=prefixOutputName + fConfiguration->GetName("LogFileName");
    string file_summaryModelSelection=prefixOutputName + "SummaryModelSelection.txt";
    fxmin=fConfiguration->GetValue("xmin");
    fxmax=fConfiguration->GetValue("xmax");
    
    //string file_histroot=prefixOutputName+"histo_52Co_IMS_Data.root";
    fOutPutNames["MargeM0"]=file_postpdfsM0;
    fOutPutNames["MargeM1"]=file_postpdfsM1;
    fOutPutNames["SummaryM0"]=file_summaryM0;
    fOutPutNames["SummaryM1"]=file_summaryM1;
    fOutPutNames["Log"]=file_log;
    fOutPutNames["ModelSelection"]=file_summaryModelSelection;

    // set nicer style for drawing than the ROOT default
    BCAux::SetStyle();
    
    
    BCLog::OpenLog(fOutPutNames["Log"].c_str());
    BCLog::SetLogLevel(BCLog::detail);
    fConfiguration->PrintToBCLog();
    
    // ----------------------------------------------------
    /// Load Data and other variables
    // ----------------------------------------------------
    fDataSet = new SidsDataSet();
    fDataSet->ReadDataFromFileTxt(fConfiguration); 
    
    // ----------------------------------------------------
    /// Set Model M0
    // ----------------------------------------------------
    fM0 = new ExpModel(fConfiguration);
    fM0->SetMyDataSet(fDataSet);
    SetM0Prior();
    SetModelOption(fM0,fConfiguration);

    BCLog::OutSummary("Model M0 created");
    
    // ----------------------------------------------------
    /// Set Model M1
    // ----------------------------------------------------
    fM1 = new OscModel(fConfiguration);
    fM1->SetMyDataSet(fDataSet);
    SetM1Prior();    
    SetModelOption(fM1,fConfiguration);
    
    BCLog::OutSummary("Model M1 created");
    
    
    // ----------------------------------------------------
    /// for binned likelihood version
    // ----------------------------------------------------
    
    TH1D* hist = fDataSet->GetHisto();
    // define a model function
    

    SetTF1functions();
    // create binned likelihood
    fhM0 = new BCHistogramFitter(hist, f0);
    fhM1 = new BCHistogramFitter(hist, f1);
    SetModelOption(fhM0,fConfiguration);
    SetModelOption(fhM1,fConfiguration);
    
    // integrate function over bin (true) or use linear interpolation
    fhM0->SetFlagIntegration(true);
    fhM1->SetFlagIntegration(true);

    // set priors
    fhM0->SetPriorConstant(0);// normfactor
    //fhM0->SetPriorConstant(1);// lambda
    fhM0->SetPriorGauss(1, 0.0139, 0.0001);
    fhM0->SetPriorDelta(2,fxmin);
    fhM0->SetPriorDelta(3,fxmax);
    
    fhM1->SetPriorConstant(0);// normfactor
    fhM1->SetPriorConstant(1);// lambda
    fhM1->SetPriorConstant(2);// amp
    fhM1->SetPriorConstant(3);// omega
    fhM1->SetPriorConstant(4);// phi
    fhM1->SetPriorDelta(5,fxmin);
    fhM1->SetPriorDelta(6,fxmax);
    

    
}

OscillationAnalysis::~OscillationAnalysis() 
{
    delete fM0;
    delete fM1;
    delete f0;
    delete f1;
    delete fhM0;
    delete fhM1;
    
    delete fConfiguration;
    delete fDataSet;
}

// --------------------------------------------------
//  Methods
// --------------------------------------------------
//*





void OscillationAnalysis::RunAnalysis()
{
    
    BCSummaryTool * summaryM0 = new BCSummaryTool(fM0);
    BCSummaryTool * summaryM1 = new BCSummaryTool(fM1);
    
   /////////////////////////////////////////////////////////////////////////////////
   // ----------------------------------------------------
   // set up model manager
   // ----------------------------------------------------

   //M0->Integrate();
   //M1->Integrate();
   

   BCModelManager * modelman = new BCModelManager();
   modelman->SetDataSet(fDataSet);
   modelman->AddModel(fM0,0.5);
   modelman->AddModel(fM1,0.5);

   
   modelman->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
   modelman->SetIntegrationMethod(BCIntegrate::kIntCuba);
   
   // Calculates the normalization of the likelihood for each model
   //modelman->GetModel(0)->Integrate();
   //modelman->GetModel(1)->Integrate();
   modelman->Integrate();
   modelman->MarginalizeAll();
   // compare models
   
   
   double bayesFact01 = modelman->BayesFactor(0,1);
   std::ostringstream buffer;
   buffer<<"Bayes Factor B01 = P(D|M0)/P(D|M1) = " << bayesFact01;
   BCLog::OutSummary(buffer.str().c_str());

   
   
   // ----------------------------------------------------
   // write output
   // ----------------------------------------------------
   modelman->PrintModelComparisonSummary();
   
   modelman->PrintModelComparisonSummary(fOutPutNames["ModelSelection"].c_str());
   
   bool printmarginalizedPdf=true;
   if(printmarginalizedPdf)
   {
        BCLog::OutSummary("******************* Write output (marginalized pdf) *******************");

        //modelman->PrintResults();//"testMy_results.txt");
        //modelman->PrintSummary("testresult.txt");

        fM0->PrintAllMarginalized(fOutPutNames["MargeM0"].c_str());
        fM1->PrintAllMarginalized(fOutPutNames["MargeM1"].c_str());

        summaryM0->PrintKnowledgeUpdatePlots(fOutPutNames["SummaryM0"].c_str());
        summaryM1->PrintKnowledgeUpdatePlots(fOutPutNames["SummaryM1"].c_str());
   }
   
   // */
}


void OscillationAnalysis::Run( )
{
    
}


double OscillationAnalysis::RunBayesTest(bool shiftMaxLikelihood )
{
    
    if(shiftMaxLikelihood)
    {
        // ----------------------------------------------------
        /// Normalize M0 and M1
        // ----------------------------------------------------
        // run MCMC to get Max likelihood, shift the likelihood with max, and then integrate
        // and all combinations of two parameters

        BCLog::OutSummary("******************* Marginalize M0 *******************");
        fM0->MarginalizeAll();
        BCLog::OutSummary("******************* Marginalize M1 *******************");
        fM1->MarginalizeAll();

        // ----------------------------------------------------
        /// Normalize M0 and M1
        // ----------------------------------------------------
        BCLog::OutSummary("******************* Normalize M0 (Normal integration) *******************");
        fM0->use_maxLogL=false;
        fM0->Normalize();
        BCLog::OutSummary("******************* Normalize M0 MAXLOGL *******************");
        fM0->use_maxLogL=true;
        fM0->Normalize();
        BCLog::OutSummary("******************* Normalize M1 (Normal integration) *******************");
        fM1->use_maxLogL=false;
        fM1->Normalize();
        BCLog::OutSummary("******************* Normalize M1 MAXLOGL *******************");
        fM1->use_maxLogL=true;
        fM1->Normalize();

        
        
    }
    
    BCModelManager * modelman = new BCModelManager();
    modelman->SetDataSet(fDataSet);
    modelman->AddModel(fM0,0.5);
    modelman->AddModel(fM1,0.5);
    modelman->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
    modelman->SetIntegrationMethod(BCIntegrate::kIntCuba);
    modelman->Integrate();
    
    fMaxLogL0=fM0->GetMaximumLogLikelihood();
    fMaxLogL1=fM1->GetMaximumLogLikelihood();

    std::ostringstream str0;
    str0<<"Max Log(L0) = "<<fMaxLogL0;
    BCLog::OutSummary(str0.str().c_str());

    std::ostringstream str1;
    str1<<"Max Log(L1) = "<<fMaxLogL1;
    BCLog::OutSummary(str1.str().c_str());

    std::ostringstream strdiff;
    strdiff<<"Max Log(L1/L0) = "<<fMaxLogL1-fMaxLogL0;
    BCLog::OutSummary(strdiff.str().c_str());
    
     
    double bayesFact01 = modelman->BayesFactor(0,1);
    std::ostringstream buffer;
    buffer<<"Bayes Factor B01 = P(D|M0)/P(D|M1) = " << bayesFact01;
    BCLog::OutSummary(buffer.str().c_str());
    if(shiftMaxLikelihood)
    {
        double CorrectedbayesFact01=bayesFact01*TMath::Exp(fMaxLogL0-fMaxLogL1);
        bayesFact01=CorrectedbayesFact01;
        std::ostringstream bufferCorrBayes;
        bufferCorrBayes<<"Corrected Bayes Factor B01 = P(D|M0)/P(D|M1) = "<< CorrectedbayesFact01;
        BCLog::OutSummary(bufferCorrBayes.str().c_str());
    }
    
    //modelman->MarginalizeAll();
    
    
    return bayesFact01;
}

void OscillationAnalysis::RunTest( )
{
    
    
    BCSummaryTool * summaryM0 = new BCSummaryTool(fM0);
    BCSummaryTool * summaryM1 = new BCSummaryTool(fM1);
    
    bool MarginalizeDirectlyM0M1=true;
    if(MarginalizeDirectlyM0M1)
    {

        fM0->Normalize();
        // ----------------------------------------------------
        /// Normalize M0 and M1
        // ----------------------------------------------------
        // run MCMC and marginalize posterior wrt. all parameters
        // and all combinations of two parameters

        BCLog::OutSummary("******************* Marginalize M0 *******************");
        fM0->MarginalizeAll();
        BCLog::OutSummary("******************* Marginalize M1 *******************");
        fM1->MarginalizeAll();

                // ----------------------------------------------------
        /// Normalize M0 and M1
        // ----------------------------------------------------
        BCLog::OutSummary("******************* Normalize M0 *******************");
        fM0->use_maxLogL=false;
        fM0->Normalize();
        BCLog::OutSummary("******************* Normalize M0 MAXLOGL *******************");
        fM0->use_maxLogL=true;
        fM0->Normalize();
        BCLog::OutSummary("******************* Normalize M1 *******************");
        fM1->use_maxLogL=false;
        fM1->Normalize();
        BCLog::OutSummary("******************* Normalize M1 MAXLOGL *******************");
        fM1->use_maxLogL=true;
        fM1->Normalize();

        
        
    }

   /////////////////////////////////////////////////////////////////////////////////
   // ----------------------------------------------------
   // set up model manager
   // ----------------------------------------------------

   //M0->Integrate();
   //M1->Integrate();
   

   BCModelManager * modelman = new BCModelManager();
   modelman->SetDataSet(fDataSet);
   modelman->AddModel(fM0,0.5);
   modelman->AddModel(fM1,0.5);

   
   modelman->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
   modelman->SetIntegrationMethod(BCIntegrate::kIntCuba);
   
   // Calculates the normalization of the likelihood for each model
   //modelman->GetModel(0)->Integrate();
   //modelman->GetModel(1)->Integrate();
   modelman->Integrate();
   modelman->MarginalizeAll();
   // compare models
   

    fMaxLogL0=fM0->GetMaximumLogLikelihood();
    fMaxLogL1=fM1->GetMaximumLogLikelihood();

    std::ostringstream str0;
    str0<<"Max Log(L0) = "<<fMaxLogL0;
    BCLog::OutSummary(str0.str().c_str());

    std::ostringstream str1;
    str1<<"Max Log(L1) = "<<fMaxLogL1;
    BCLog::OutSummary(str1.str().c_str());

    std::ostringstream strdiff;
    str1<<"Max Log(L1/L0) = "<<fMaxLogL1-fMaxLogL0;
    BCLog::OutSummary(str1.str().c_str());
   
   double bayesFact01 = modelman->BayesFactor(0,1);
   std::ostringstream buffer;
   buffer<<"Bayes Factor B01 = P(D|M0)/P(D|M1) = " << bayesFact01;
   BCLog::OutSummary(buffer.str().c_str());

   
   
   // ----------------------------------------------------
   // write output
   // ----------------------------------------------------
   modelman->PrintModelComparisonSummary();
   
   modelman->PrintModelComparisonSummary(fOutPutNames["ModelSelection"].c_str());
   
   bool printmarginalizedPdf=true;
   if(printmarginalizedPdf)
   {
        BCLog::OutSummary("******************* Write output (marginalized pdf) *******************");

        //modelman->PrintResults();//"testMy_results.txt");
        //modelman->PrintSummary("testresult.txt");

        fM0->PrintAllMarginalized(fOutPutNames["MargeM0"].c_str());
        fM1->PrintAllMarginalized(fOutPutNames["MargeM1"].c_str());

        summaryM0->PrintKnowledgeUpdatePlots(fOutPutNames["SummaryM0"].c_str());
        summaryM1->PrintKnowledgeUpdatePlots(fOutPutNames["SummaryM1"].c_str());
   }
    
    

}


void OscillationAnalysis::RunBinnedAnalysis()
{

    

    BCModelManager * modelman = new BCModelManager();
    //modelman->SetDataSet(fDataSet);
    modelman->AddModel(fhM0,0.5);//BCHistogramFitter for M0
    modelman->AddModel(fhM1,0.5);//BCHistogramFitter for M1
    modelman->Integrate();
    /*modelman->MarginalizeAll();
    // compare models
   
   
    double bayesFact01 = modelman->BayesFactor(0,1);
    std::ostringstream buffer;
    buffer<<"Bayes Factor B01 = P(D|M0)/P(D|M1) = " << bayesFact01;
    BCLog::OutSummary(buffer.str().c_str());

   
   
   // ----------------------------------------------------
   // write output
   // ----------------------------------------------------
   modelman->PrintModelComparisonSummary();
   
   modelman->PrintModelComparisonSummary(fOutPutNames["ModelSelection"].c_str());
   
   fhM0->PrintAllMarginalized(fOutPutNames["MargeM0"].c_str());
   fhM1->PrintAllMarginalized(fOutPutNames["MargeM1"].c_str());
   
    /*
    // perform fit
    fhM1->Fit();

    double pvalue, pvalueCorrected;
    std::vector<double> init (4);
    init[0] = mean; init[1] = sigma; init[2] = 150; init[3] = 1;
    fhM1->FindMode(init);
    fhM1->CalculatePValueFast(fhM1->GetBestFitParameters());
    pvalue = fhM1->GetPValue();
    pvalueCorrected = fhM1->GetPValueNDoF();

    cout << "Pvalue " << pvalue
        << ", corrected " << pvalueCorrected
        << endl;

    // print marginalized distributions
    fhM1->PrintAllMarginalized("distributions.pdf");

    // print data and fit
    TCanvas* c1 = new TCanvas("c1");
    fhM1->DrawFit("", true); // draw with a legend
    c1->Print("fit.pdf");
    */
}

//ClassImp(Analysis)


// */


void OscillationAnalysis::SetM0Prior()
{
    fM0->SetPriorConstant(0);
}


void OscillationAnalysis::SetM1Prior()
{
    fM1->SetPriorConstant(0);
    fM1->SetPriorConstant(1);
    fM1->SetPriorConstant(2);
    fM1->SetPriorConstant(3);
}



double OscillationAnalysis::fitffunction0(double *t,double *par)
{
    //par[0] A
    //par[1] lambda
    //par[2] xmin
    //par[3] xmax
    double integral=TMath::Exp(-par[1]*par[2] )-TMath::Exp(-par[1]*par[3]);
    double value = par[0]*TMath::Exp(-par[1]*t[0])/integral;
    return value;
}

double OscillationAnalysis::fitffunction1(double *t,double *par)//lambda 0 , lambda_EC 1, lambdaloss 2, Nom 3, Nod 4
{
    //par[0] A
    //par[1] lambda
    //par[2] amplitude
    //par[3] omega
    //par[4] phase
    //par[5] xmin
    //par[6] xmax
    double part0=TMath::Exp(-par[1]*par[5] )-TMath::Exp(-par[1]*par[6]);
    double factor=par[1]*par[2]/(par[3]*par[3]+par[1]*par[1]);
    double part1a=(par[3]*TMath::Sin(par[3]*par[6]+par[4])-par[1]*TMath::Cos(par[3]*par[6]+par[4]))*TMath::Exp(-par[1]*par[6]);
    double part1b=(par[3]*TMath::Sin(par[3]*par[5]+par[4])-par[1]*TMath::Cos(par[3]*par[5]+par[4]))*TMath::Exp(-par[1]*par[5]);
    double Integral=(part0+factor*(part1a-part1b))/par[1];      // normalisation factor of the pdf M1
	
    if(Integral==0)
    {
        Integral=1.0;
        cout<<"Warning analytical integral is zero. Set to 1.0"<<endl;
    }
    
    double cospart= 1+par[2]*TMath::Cos(par[3]*t[0]+par[4]);
    double exppart=par[0]*TMath::Exp(-par[1]*t[0]);
    double value=cospart*exppart/Integral;
    
    return value;
}



void OscillationAnalysis::SetTF1functions()
{
    double NormFactMin=3000.0;
    double NormFactMax=4000.0;
    
    //Set exp model
    f0 = new TF1("f0",fitffunction0,fxmin,fxmin,4);
    
    f0->SetParName(0,"NormFactor");
    f0->SetParName(1,"lambda");
    f0->SetParName(2,"xmin");
    f0->SetParName(3,"xmax");
    
    f0->SetParLimits(0,0.0,NormFactMax);                        // A
    f0->SetParLimits(1,fConfiguration->GetValue("lambdamin"),
            fConfiguration->GetValue("lambdamax"));             // lambda
    f0->FixParameter(2,fxmin);                                  // xmin
    f0->FixParameter(3,fxmax);                                  // xmax
    
    //Set osc model
    f1 = new TF1("f1",fitffunction1,fxmin,fxmax,7);
    
    f1->SetParName(0,"NormFactor");
    f1->SetParName(1,"lambda");
    f1->SetParName(2,"amp");
    f1->SetParName(3,"omega");
    f1->SetParName(4,"phi");
    f1->SetParName(5,"xmin");
    f1->SetParName(6,"xmax");
    
    f1->SetParLimits(0,0.0,NormFactMax);                        // A
    f1->SetParLimits(1,fConfiguration->GetValue("lambdamin"),
            fConfiguration->GetValue("lambdamax"));             // lambda
    f1->SetParLimits(2,fConfiguration->GetValue("amin"), 
            fConfiguration->GetValue("amax"));                  // amp
    f1->SetParLimits(3,fConfiguration->GetValue("omegamin"), 
            fConfiguration->GetValue("omegamax"));              // omega
    f1->SetParLimits(4,fConfiguration->GetValue("phimin"), 
            fConfiguration->GetValue("phimax"));                // phi
    f1->FixParameter(5,fxmin);                                  // xmin
    f1->FixParameter(6,fxmax);                                  // xmax
}

int OscillationAnalysis::InitField()
{
    // define field value names that are in configfile
    
    // analysis range
    fvalfield.push_back("xmin");
    fvalfield.push_back("xmax");
    fvalfield.push_back("xoffset");
    fvalfield.push_back("xunit");
    
    // parameter ranges
    fvalfield.push_back("lambdamin");
    fvalfield.push_back("lambdamax");
    fvalfield.push_back("amin");
    fvalfield.push_back("amax");
    fvalfield.push_back("omegamin");
    fvalfield.push_back("omegamax");
    fvalfield.push_back("phimin");
    fvalfield.push_back("phimax");
    
    // parameter binning
    fvalfield.push_back("BinLambda");
    fvalfield.push_back("BinAmp");
    fvalfield.push_back("BinOmega");
    fvalfield.push_back("BinPhi");
    
    
    // parameter outputs
    fcharfield.push_back("OutputPostpdfsM0");
    fcharfield.push_back("OutputPostpdfsM1");
    fcharfield.push_back("OutputSummaryM0");
    fcharfield.push_back("OutputSummaryM1");
    
    fConfiguration = new SidsParameters(fvalfield,fcharfield);
    return 0;
}

unsigned int OscillationAnalysis::GetSampleSize()
{
    fDataSet->GetNDataPoints();
}


