/* 
 * File:   IsomerAnalysis.cxx
 * Author: winckler
 * 
 * Created on April 19, 2014, 11:29 PM
 */

#include "IsomerAnalysis.h"

IsomerAnalysis::IsomerAnalysis() : Analysis(), fMaxLogL0(0.0), fMaxLogL1(0.0), fLogLRatio(0.0)
{
    
    // define field value names that are in configfile
    InitField();
    
}

IsomerAnalysis::IsomerAnalysis(string filename) : Analysis(), fMaxLogL0(0.0), fMaxLogL1(0.0), fLogLRatio(0.0)
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
    
    fOutPutNames["OutPutRootHisto"]=prefixOutputName+"histogram.root";
    // set nicer style for drawing than the ROOT default
    BCAux::SetStyle();
    
    BCLog::OpenLog(fOutPutNames["Log"].c_str());
    BCLog::SetLogLevel(BCLog::detail);
    fConfiguration->PrintToBCLog();
    
    
    fM0prior=fConfiguration->GetName("M0PriorSet");
    fM1prior=fConfiguration->GetName("M1PriorSet");
    BCLog::OutSummary("Prior Set for M0 :");
    BCLog::OutSummary(fM0prior.c_str());
    BCLog::OutSummary("Prior Set for M1 :");
    BCLog::OutSummary(fM1prior.c_str());
    //fM0prior="Constant";
    //fM1prior="Constant";
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




void IsomerAnalysis::FitOnePeak()
{
    fM0->RooFitToData(false);
    //string filename=fOutPutNames["OutPutRootHisto"];
    //SaveDataHistogram(filename);
    
}



void IsomerAnalysis::RunAnalysis()
{
    
    //*
    BCSummaryTool * summaryM0 = new BCSummaryTool(fM0);
    BCSummaryTool * summaryM1 = new BCSummaryTool(fM1);
    BCModelManager * modelman = new BCModelManager();
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
    
    
    bool modelmantrue=true;
    if(modelmantrue)
    {
        
        /////////////////////////////////////////////////////////////////////////////////
        // ----------------------------------------------------
        // set up model manager
        // ----------------------------------------------------

        //M0->Integrate();
        //M1->Integrate();



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


         // calculate p-value
         fM1->CalculatePValue( fM1->GetBestFitParameters() );
         fM0->CalculatePValue( fM0->GetBestFitParameters() );


         // ----------------------------------------------------
         // write output
         // ----------------------------------------------------

         modelman->PrintModelComparisonSummary();
        
        
    }
   
    
   
   
   
    bool printmarginalizedPdf=true;
    if(printmarginalizedPdf)
    {
        BCLog::OutSummary("******************* Write output (marginalized pdf) *******************");

         
         
         fMaxLogL0=fM0->GetMaximumLogLikelihood();
         fMaxLogL1=fM1->GetMaximumLogLikelihood();

         std::ostringstream str0;
         str0<<"Max Log(L0) = "<<fMaxLogL0;
         BCLog::OutSummary(str0.str().c_str());

         std::ostringstream str1;
         str1<<"Max Log(L1) = "<<fMaxLogL1;
         BCLog::OutSummary(str1.str().c_str());
        
        modelman->PrintModelComparisonSummary(fOutPutNames["ModelSelection"].c_str());
        
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
    
    if(fM0prior=="Constant")
    {
        fM0->SetPriorConstant(0);
        fM0->SetPriorConstant(1);
    }
    
    
    if(fM0prior=="Gauss")
    {
        double mean=fConfiguration->GetValue("M0SigmaPriorGMeanVal");
        double sigma=fConfiguration->GetValue("M0SigmaPriorGSigmaVal");
        fM0->SetPriorConstant(0);
        fM0->SetPriorGauss(1, mean, sigma);
    }
    
}


void IsomerAnalysis::SetM1Prior()
{
    
    if(fM1prior=="Constant")
    {
        fM1->SetPriorConstant(0);// mu0
        fM1->SetPriorConstant(1);// mu1
        fM1->SetPriorConstant(2);// sigma0
        fM1->SetPriorConstant(3);// sigma1
        fM1->SetPriorConstant(4);// weight0
    }
    
    if(fM1prior=="Gauss")
    {
        double mean1=fConfiguration->GetValue("M1Sigma1PriorGMeanVal");
        double sigma1=fConfiguration->GetValue("M1Sigma1PriorGSigmaVal");
        
        double mean2=fConfiguration->GetValue("M1Sigma2PriorGMeanVal");
        double sigma2=fConfiguration->GetValue("M1Sigma2PriorGSigmaVal");
        
        fM1->SetPriorConstant(0);
        fM1->SetPriorConstant(1);
        fM1->SetPriorGauss(2, mean1, sigma1);
        fM1->SetPriorGauss(3, mean2, sigma2);
        fM1->SetPriorConstant(4);
    }
    
    
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


void IsomerAnalysis::SaveDataHistogram( string filename)
{
    TFile fileHisto(filename.c_str(), "new");
    TH1D h1(filename.c_str(), filename.c_str(), 100, fConfiguration->GetValue("xmin"),fConfiguration->GetValue("xmax"));
    
    for(unsigned int i=0; i < fDataSet->GetNDataPoints(); ++i)
    {
        double xi = fDataSet->GetDataPoint(i)->GetValue(0);
        std::cout<<"xi="<<xi<<std::endl;
        h1.Fill(xi);
    }
    h1.Write();
    fileHisto.Close();
}

void IsomerAnalysis::FitOtherPeaks()
{
    NGaussModel* fgN = new NGaussModel(fConfiguration,2);
    // 40sc,42ti,44v,46cr,48mn,50fe,52co,54ni,56cu (9+1 peaks))
    //(low statistics)
    
    
    //higher statistics -> 19Ne, 21Na, 23Mg, 25Al, 27Si, 29P, 31S
    
    
    
    
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
    
    fvalfield.push_back("M0SigmaPriorGMeanVal");
    fvalfield.push_back("M0SigmaPriorGSigmaVal");
    fvalfield.push_back("M1Sigma1PriorGMeanVal");
    fvalfield.push_back("M1Sigma1PriorGSigmaVal");
    fvalfield.push_back("M1Sigma2PriorGMeanVal");
    fvalfield.push_back("M1Sigma2PriorGSigmaVal");
    
    // parameter outputs
    fcharfield.push_back("OutputPostpdfsM0");
    fcharfield.push_back("OutputPostpdfsM1");
    fcharfield.push_back("OutputSummaryM0");
    fcharfield.push_back("OutputSummaryM1");
    fcharfield.push_back("OutputCorrelationM1");
    fcharfield.push_back("OutputResultsM1");
    fcharfield.push_back("OutputParameterScaledM1");
    
    fcharfield.push_back("M0PriorSet");
    fcharfield.push_back("M1PriorSet");
    
    fConfiguration = new SidsParameters(fvalfield,fcharfield);
    return 0;
}




void IsomerAnalysis::TempTest()
{
    
    //*
    
    bool MarginalizeDirectlyM0M1=true;
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

        
        
        
        ////////////// TEST1 /////////////////////
        bool testNormalization=false;
        if(testNormalization)
        {
            BCModelManager * modelman0 = new BCModelManager();
            modelman0->SetDataSet(fDataSet);
            modelman0->AddModel(fM0,0.5);
            modelman0->AddModel(fM1,0.5);


            modelman0->SetMarginalizationMethod(BCIntegrate::kMargMetropolis);
            modelman0->SetIntegrationMethod(BCIntegrate::kIntCuba);

            // Calculates the normalization of the likelihood for each model
            //modelman->GetModel(0)->Integrate();
            //modelman->GetModel(1)->Integrate();
            modelman0->Integrate();
            modelman0->MarginalizeAll();

            double bayesFact01bis = modelman0->BayesFactor(0,1);
             std::ostringstream buffer0;
             buffer0<<"1st Bayes Factor B01 = P(D|M0)/P(D|M1) = " << bayesFact01bis;
             BCLog::OutSummary(buffer0.str().c_str());

             modelman0->PrintModelComparisonSummary();
             
             //delete modelman0;
        }
        ////////////// TEST1 END /////////////////////
        
   
        fMaxLogL0=fM0->GetMaximumLogLikelihood();
        fMaxLogL1=fM1->GetMaximumLogLikelihood();

        std::ostringstream str00;
        str00<<"Max Log(L0) = "<<fMaxLogL0;
        BCLog::OutSummary(str00.str().c_str());

        std::ostringstream str01;
        str01<<"Max Log(L1) = "<<fMaxLogL1;
        BCLog::OutSummary(str01.str().c_str());
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
   
   
    fB01 = modelman->BayesFactor(0,1);
    std::ostringstream buffer;
    buffer<<"Bayes Factor B01 = P(D|M0)/P(D|M1) = " << fB01;
    BCLog::OutSummary(buffer.str().c_str());

   
    double CorrectedbayesFact01=fB01*TMath::Exp(-fMaxLogL0/fMaxLogL1);
   
    std::ostringstream bufferCorrBayes;
    bufferCorrBayes<<"Corrected Bayes Factor B01 = P(D|M0)/P(D|M1) = "<< CorrectedbayesFact01;
    BCLog::OutSummary(bufferCorrBayes.str().c_str());
   
   double invCorrectedbayesFact01=fB01*TMath::Exp(fMaxLogL0/fMaxLogL1);
   
    std::ostringstream bufferCorrBayesinv;
    bufferCorrBayesinv<<"Inversed corr Bayes Factor B01 = P(D|M0)/P(D|M1) = "<< invCorrectedbayesFact01;
    BCLog::OutSummary(bufferCorrBayesinv.str().c_str());
   
   
   
    // ----------------------------------------------------
    // write output
    // ----------------------------------------------------
    modelman->PrintModelComparisonSummary();
   
    
   
   // */
}