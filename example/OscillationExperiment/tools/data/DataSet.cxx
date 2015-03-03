/* 
 * File:   DataSet.cxx
 * Author: winckler
 * 
 * Created on February 17, 2015, 9:42 AM
 */

#include "DataSet.h"

DataSet::DataSet() {
}

DataSet::DataSet(const DataSet& orig) {
}

DataSet::~DataSet() {
}



////////////////////// My dedicated stuff/////////////////////////////////

double DataSet::GetRangeLength()
{
    double range=fxobsmax-fxobsmin;
    return range;
}

// ---------------------------------------------------------
int DataSet::FillTreeFromFileTxt(SidsParameters *Sidspar)
{
	
    
    return 0;
}





int DataSet::ReadDataFromFileTxt(SidsParameters* Sidspar)
{
	
    fXRangeAuto=false;
    /////////////////////////////////////////////////////////////////////////
	/// Get experimental parameter
    /////////////////////////////////////////////////////////////////////////
    
    if(Sidspar->GetName("XRangeOption")=="XRangeAuto") 
        fXRangeAuto=true;
    string DataName=Sidspar->GetName("DataName");
    string filename=Sidspar->GetName("DataPathName");
    double x0=Sidspar->GetValue("xmin");
    double xf=Sidspar->GetValue("xmax");
    double xunit=Sidspar->GetValue("xunit");             
    double XvarOffset=Sidspar->GetValue("xoffset");
    double Xvar;                                        //random variable
    /////////////////////////////////////////////////////////////////////////
	/// Tokenize file content and convert string cells into double cells
    /////////////////////////////////////////////////////////////////////////
    
    if(FileReader::TokenizeFileTxt(filename)==0)
        FileReader::ConvertStrToDouble();
	
	
	/////////////////////////////////////////////////////////////////////////
	/// fill TTree and fBCDataVector
	/////////////////////////////////////////////////////////////////////////
	// if data set contains data, clear data object container ...
    if (GetNDataPoints () != 0)
    {
        Reset ();
        
        BCLog::OutDetail("DataSet::ReadDataFromFileTxt : Overwrite existing data.");
    }
    
    // create temporary vector with data and assign some zeros.
    
    
    
    fTree = new TTree(DataName.c_str(),DataName.c_str());
    fHisto= new TH1D("DataHistogram","DataHistogram",100,x0,xf);
    if(fRawData.size()==0)
    {
        BCLog::OutError("Raw data array has zero row");
        return -1;
    }
    
    
    for(unsigned int i(0); i<fRawData.size();++i)
        for(unsigned int j(0); j<fRawData[i].size();++j)
        {

            Xvar=xunit*(fRawData[i][j]-XvarOffset);
            //get min and max
            if(Xvar<fxobsmin) 
                    fxobsmin=Xvar;
            if(Xvar>fxobsmax) 
                fxobsmax=Xvar;
        }
    
    
    if(fXRangeAuto)
    {
        double delta=(fxobsmax-fxobsmin)*0.05;// 5% on each side
        x0=fxobsmin-delta;
        xf=fxobsmax+delta;
        Sidspar->SetValue("xmin",x0);
        Sidspar->SetValue("xmax",xf);
    }
    
    fHisto= new TH1D("DataHistogram","DataHistogram",10000,x0,xf);
    fTree->Branch("x",&Xvar);
    for(unsigned int i(0); i<fRawData.size();++i)
        for(unsigned int j(0); j<fRawData[i].size();++j)
        {
            
            Xvar=xunit*(fRawData[i][j]-XvarOffset);
            
           
            
            if(Xvar>=x0 && Xvar<=xf)
            {
                std::vector<double> data;//to comply to the standard BAT dataset
                data.push_back(Xvar);// put other pushback if other variables e.g. (x,y,z,t)
                // add data point to this data set.
                //cout<<"Data = "<<Xvar<<endl;
                AddDataPoint(new BCDataPoint(data));
                
                fTree->Fill();		// fill Tree if Xvar is in defined range
                fHisto->Fill(Xvar);
            }
        }
    //fTree->StartViewer();
    fTree->ResetBranchAddresses();
    
    //fx = new RooRealVar("x","x",x0,xf);
    //frooData = new RooDataSet("frooData","dataset with x",fTree,*fx);
    
    return 0;
}


int DataSet::ReadRooDataSet(RooDataSet* dataset)
{
    double Xvar;
    bool digitize=true;
    for(unsigned int i(0); i<dataset->numEntries();++i)
        {
            const RooArgSet* event = dataset->get(i) ; 
            Xvar=event->getRealValue("x");
            
            if(digitize)
            {
                int xint=(int)(Xvar/0.032);
                Xvar=(double)(xint)*0.032;
                
            }
            
            std::vector<double> data;//to comply to the standard BAT dataset
            data.push_back(Xvar);// put other pushback if other variables e.g. (x,y,z,t)
            // add data point to this data set.
            //cout<<"Data = "<<Xvar<<endl;
            AddDataPoint(new BCDataPoint(data));                
        }
}