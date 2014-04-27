/* 
 * File:   IsomerAnalysis.cxx
 * Author: winckler
 * 
 * Created on April 19, 2014, 11:29 PM
 */

#include "IsomerAnalysis.h"

IsomerAnalysis::IsomerAnalysis() : Analysis()
{
    
    // define field value names that are in configfile
    InitField();
    
}

IsomerAnalysis::IsomerAnalysis(string filename) : Analysis()
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
    //string file_histroot=prefixOutputName+"histo_52Co_IMS_Data.root";
    fOutPutNames["MargeM0"]=file_postpdfsM0;
    fOutPutNames["MargeM1"]=file_postpdfsM1;
    fOutPutNames["SummaryM0"]=file_summaryM0;
    fOutPutNames["SummaryM1"]=file_summaryM1;
    fOutPutNames["Log"]=file_log;
    fOutPutNames["ModelSelection"]=file_summaryModelSelection;

    fOutPutNames["OutputCorrelationM1"]=prefixOutputName+fConfiguration->GetName("OutputCorrelationM1");
    fOutPutNames["OutputResultsM1"]=prefixOutputName+fConfiguration->GetName("OutputResultsM1");
    fOutPutNames["OutputParameterScaledM1"]=prefixOutputName+fConfiguration->GetName("OutputParameterScaledM1");
    
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
    fM0 = new OneGaussModel(fConfiguration);
    fM0->SetMyDataSet(fDataSet);
    SetM0Prior();
    SetModelOption(fM0,fConfiguration);

    BCLog::OutSummary("Model M0 created");
    
    // ----------------------------------------------------
    /// Set Model M1
    // ----------------------------------------------------
    fM1 = new TwoGaussModel(fConfiguration);
    fM1->SetMyDataSet(fDataSet);
    SetM1Prior();    
    SetModelOption(fM1,fConfiguration);
    
    BCLog::OutSummary("Model M1 created");
    
}

IsomerAnalysis::~IsomerAnalysis() 
{
    delete fM0;
    delete fM1;
    delete fConfiguration;
    delete fDataSet;
    
}



void IsomerAnalysis::RunAnalysis()
{
    
    //*
    BCSummaryTool * summaryM0 = new BCSummaryTool(fM0);
    BCSummaryTool * summaryM1 = new BCSummaryTool(fM1);
    
    bool MarginalizeDirectlyM0M1=false;
    if(MarginalizeDirectlyM0M1)
    {
        // ----------------------------------------------------
        /// Normalize M0 and M1
        // ----------------------------------------------------

        BCLog::OutSummary("******************* Normalize M0 *******************");
        fM0->Normalize();
        BCLog::OutSummary("******************* Normalize M1 *******************");
        fM1->Normalize();

        // ----------------------------------------------------
        /// Normalize M0 and M1
        // ----------------------------------------------------
        // run MCMC and marginalize posterior wrt. all parameters
        // and all combinations of two parameters

        BCLog::OutSummary("******************* Marginalize M0 *******************");
        fM0->MarginalizeAll();
        BCLog::OutSummary("******************* Marginalize M1 *******************");
        fM1->MarginalizeAll();

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
            /// run mode finding; by default using Minuit
         //   m->SetOptimizationMethod(BCIntegrate::kOptDefault);
         //   m->SetOptimizationMethod(BCIntegrate::kOptMinuit);
         //   m->SetOptimizationMethod(BCIntegrate::kOptSimAnn);
         //   m->SetOptimizationMethod(BCIntegrate::kOptMetropolis);
        fM1->FindMode();

            /// if MCMC was run before (MarginalizeAll()) it is
            /// possible to use the mode found by MCMC as
            /// starting point of Minuit minimization
        fM1->FindMode(fM1->GetBestFitParameters());

            /// draw all marginalized distributions into a PostScript file
         //   m->PrintAllMarginalized("testMy_plots.pdf");

            /// print individual histograms
         //   m->GetMarginalized("x")->Print("x.pdf");
         //   m->GetMarginalized("y")->Print("y.pdf");
         //   m->GetMarginalized("x", "y")->Print("xy.pdf");

            /// print all summary plots
         summaryM1->PrintParameterPlot(fOutPutNames["OutputParameterScaledM1"].c_str());
         summaryM1->PrintCorrelationPlot(fOutPutNames["OutputCorrelationM1"].c_str());
         //   summary->PrintKnowledgeUpdatePlots("testMy_update.pdf");

            /// calculate p-value
        fM1->CalculatePValue( fM1->GetBestFitParameters() );

            /// print results of the analysis into a text file
        fM1->PrintResults(fOutPutNames["OutputResultsM1"].c_str());
        
        summaryM0->PrintKnowledgeUpdatePlots(fOutPutNames["SummaryM0"].c_str());
        summaryM1->PrintKnowledgeUpdatePlots(fOutPutNames["SummaryM1"].c_str());
   }
   
   // */
}







void IsomerAnalysis::SetM0Prior()
{
    fM0->SetPriorConstant(0);
    fM0->SetPriorConstant(1);
}


void IsomerAnalysis::SetM1Prior()
{
    fM1->SetPriorConstant(0);
    fM1->SetPriorConstant(1);
    fM1->SetPriorConstant(2);
    fM1->SetPriorConstant(3);
    fM1->SetPriorConstant(4);
}

void IsomerAnalysis::SaveDataHistogram( string filename, BCDataSet* Data)
{
    TFile fileHisto(filename.c_str(), "new");
    TH1D h1("h52Co", "histogram of the 52Co IMS-Data", 100, 609.05, 609.07);
    
    for(unsigned int i=0; i < Data->GetNDataPoints(); ++i)
    {
        double xi = Data->GetDataPoint(i)->GetValue(0);
        //std::cout<<"xi="<<xi<<std::endl;
        h1.Fill(xi);
    }
    h1.Write();
    fileHisto.Close();
}



void IsomerAnalysis::FitOtherPeaks()
{
    NGaussModel* fgN = new NGaussModel(fConfiguration,2);
    // 40sc,42ti,44v,46cr,48mn,50fe,52co,54ni,56cu (9+1 peaks))
    
    
    
    
    
    
    delete fgN;
    
}





int IsomerAnalysis::InitField()
{
    // define field value names that are in configfile
    // analysis range
    fvalfield.push_back("xmin");
    fvalfield.push_back("xmax");
    fvalfield.push_back("xunit");
    fvalfield.push_back("xoffset");
    
    fvalfield.push_back("mu0min");
    fvalfield.push_back("mu0max");
    fvalfield.push_back("mu1min");
    fvalfield.push_back("mu1max");
    
    fvalfield.push_back("sigma0min");
    fvalfield.push_back("sigma0max");
    fvalfield.push_back("sigma1min");
    fvalfield.push_back("sigma1max");
    
    fvalfield.push_back("weight0min");
    fvalfield.push_back("weight0max");
    fvalfield.push_back("weight1min");
    fvalfield.push_back("weight1max");
    
    fvalfield.push_back("BinMu0");
    fvalfield.push_back("BinMu1");
    fvalfield.push_back("BinSigma0");
    fvalfield.push_back("BinSigma1");
    fvalfield.push_back("BinWeight0");
    fvalfield.push_back("BinWeight1");
    
    
    // parameter outputs
    fcharfield.push_back("OutputPostpdfsM0");
    fcharfield.push_back("OutputPostpdfsM1");
    fcharfield.push_back("OutputSummaryM0");
    fcharfield.push_back("OutputSummaryM1");
    fcharfield.push_back("OutputCorrelationM1");
    fcharfield.push_back("OutputResultsM1");
    fcharfield.push_back("OutputParameterScaledM1");
    
    
    fConfiguration = new SidsParameters(fvalfield,fcharfield);
    return 0;
}
