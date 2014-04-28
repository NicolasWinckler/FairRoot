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
    
}

OscillationAnalysis::~OscillationAnalysis() 
{
    delete fM0;
    delete fM1;
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
    
    bool MarginalizeDirectlyM0M1=true;
    if(MarginalizeDirectlyM0M1)
    {


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




