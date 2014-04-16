//
//  SidsDataSet.cxx
//  
//
//  Created by Nicolas Winckler on 2/21/14.
//
//

#include "SidsDataSet.h"


// ---------------------------------------------------------
SidsDataSet::SidsDataSet() : BCDataSet(), FileReader()
{
    // default constructor
}


// ---------------------------------------------------------
SidsDataSet::~SidsDataSet()
// default destructor
{
    
}






////////////////////// My dedicated stuff/////////////////////////////////


// ---------------------------------------------------------
int SidsDataSet::FillTreeFromFileTxt(SidsParameters Sidspar)
{
	
    
    /////////////////////////////////////////////////////////////////////////
	/// Get experimental parameter
    /////////////////////////////////////////////////////////////////////////

    string DataName=Sidspar.GetName("DataName");
    string filename=Sidspar.GetName("DataPathName");
	double t0=Sidspar.GetValue("tmin");
	double tf=Sidspar.GetValue("tmax");
	double xunit=Sidspar.GetValue("tunit");             // minimum difference between 2 events
	double XvarOffset=Sidspar.GetValue("toffset");      //0.5*xunit;// to center in the middle of a bin
	double Xvar;                                        //random variable
    /////////////////////////////////////////////////////////////////////////
	/// Tokenize file content and convert string cells into double cells
    /////////////////////////////////////////////////////////////////////////
    
    if(FileReader::TokenizeFileTxt(filename)==0)
        FileReader::ConvertStrToDouble();
	
	
	/////////////////////////////////////////////////////////////////////////
	/// fill TTree
	/////////////////////////////////////////////////////////////////////////
	
    fTree = new TTree(DataName.c_str(),DataName.c_str());
    
    if(fRawData.size()==0)
    {
        BCLog::OutError("Raw data array has zero row");
        return -1;
    }
    
    fTree->Branch("x",&Xvar);
    for(unsigned int i(0); i<fRawData.size();++i)
        for(unsigned int j(0); j<fRawData[i].size();++j)
        {
            Xvar=xunit*fRawData[i][j]-XvarOffset;
            if(Xvar>=t0-XvarOffset && Xvar<=tf-XvarOffset)
                fTree->Fill();		// fill Tree if Xvar is in defined range
        }
    //fTree->StartViewer();
    fTree->ResetBranchAddresses();
	
    return 0;
}





int SidsDataSet::ReadDataFromFileTxt(SidsParameters Sidspar)
{
	
    
    /////////////////////////////////////////////////////////////////////////
	/// Get experimental parameter
    /////////////////////////////////////////////////////////////////////////
    
    string DataName=Sidspar.GetName("DataName");
    string filename=Sidspar.GetName("DataPathName");
	double t0=Sidspar.GetValue("tmin");
	double tf=Sidspar.GetValue("tmax");
	double xunit=Sidspar.GetValue("tunit");             // minimum difference between 2 events
	double XvarOffset=Sidspar.GetValue("toffset");      //0.5*xunit;// to center in the middle of a bin
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
        
        BCLog::OutDetail("SidsDataSet::ReadDataFromFileTxt : Overwrite existing data.");
    }
    
    std::cout << " GetNDataPoints() = " << GetNDataPoints() << std::endl;
    // create temporary vector with data and assign some zeros.
    
    
    
    fTree = new TTree(DataName.c_str(),DataName.c_str());
    
    if(fRawData.size()==0)
    {
        BCLog::OutError("Raw data array has zero row");
        return -1;
    }
    
    fTree->Branch("x",&Xvar);
    for(unsigned int i(0); i<fRawData.size();++i)
        for(unsigned int j(0); j<fRawData[i].size();++j)
        {
            
            
            Xvar=xunit*fRawData[i][j]-XvarOffset;
            
            if(Xvar>=t0-XvarOffset && Xvar<=tf-XvarOffset)
            {
                std::vector<double> data;//to comply to the standard BAT dataset
                data.push_back(Xvar);// put other pushback if other variables e.g. (x,y,z,t)
                // add data point to this data set.
                AddDataPoint(new BCDataPoint(data));
                
                fTree->Fill();		// fill Tree if Xvar is in defined range
                
            }
        }
    //fTree->StartViewer();
    fTree->ResetBranchAddresses();
	std::cout << " GetNDataPoints() 2 = " << GetNDataPoints() << std::endl;
    return 0;
}



//ClassImp(sidsDataSet)
