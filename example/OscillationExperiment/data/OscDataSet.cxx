/* 
 * File:   OscDataSet.cxx
 * Author: winckler
 * 
 * Created on March 2, 2015, 12:22 PM
 */

#include "OscDataSet.h"

OscDataSet::OscDataSet(OscAnaManager* config) : BCDataSet(), FileReader(),
                        fxmin(0.),
                        fxmax(0.),
                        fXRangeAuto(false),
                        fDigitize(false),
                        fXResolution(0.032)
    {
        
    }

OscDataSet::~OscDataSet() {
}



int OscDataSet::ReadDataFromFileTxt(OscAnaManager* config)
{
    std::string filename=config->GetPar<std::string>("input.data.file.name");
    double x0=config->GetPar<double>("obs.xmin");
    double xf=config->GetPar<double>("obs.xmax");
    double xunit=config->GetPar<double>("obs.xunit");             
    double XvarOffset=config->GetPar<double>("obs.xoffset");
    double Xvar;                                        //random variable
    
    
    /// Tokenize file content and convert string cells into double cells    
    if(FileReader::TokenizeFileTxt(filename)==0)
        FileReader::ConvertStrToDouble();
    
    // if data set contains data, clear data object container ...
    if (GetNDataPoints () != 0)
    {
        Reset ();
        
        BCLog::OutDetail("OscDataSet::ReadDataFromFileTxt : Overwrite existing data.");
    }
        
    if(fRawData.size()==0)
    {
        BCLog::OutError("Raw data array has zero row");
        return -1;
    }
    
    
    /// fill fBCDataVector
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
            }
        }
    
    
    return 0;
}


int OscDataSet::ReadDataFromFileRoot(OscAnaManager* config)
{
    std::string filename=config->GetPar<std::string>("input.data.file.name");
    std::string branchname=config->GetPar<std::string>("input.data.file.branch.name");
    std::string treename=config->GetPar<std::string>("input.data.file.tree.name");
    
    double x0=config->GetPar<double>("obs.xmin");
    double xf=config->GetPar<double>("obs.xmax");
    double xunit=config->GetPar<double>("obs.xunit");             
    double XvarOffset=config->GetPar<double>("obs.xoffset");
    double Xvar;
    
    
    
    // if data set contains data, clear data object container ...
    if (GetNDataPoints () != 0)
    {
        Reset();
        BCLog::OutDetail("OscDataSet::ReadDataFromFileRoot : Overwrite existing data.");
    }
    
    
    
    return 0;
}



int OscDataSet::ConvertRooToBCDataset(RooDataSet* dataset, const std::string& ObsName)
{
    double Xvar=0.0;
    for(unsigned int i(0); i<dataset->numEntries();++i)
    {
        const RooArgSet* event = dataset->get(i);
        Xvar=event->getRealValue(ObsName.c_str());

        if(fDigitize)
        {
            int xint=(int)(Xvar/fXResolution);
            Xvar=(double)(xint)*fXResolution;
        }

        std::vector<double> data;//to comply to the standard BAT dataset
        data.push_back(Xvar);// put other pushback if other variables e.g. (x,y,z,t)
        // add data point to this data set.
        //cout<<"Data = "<<Xvar<<endl;
        AddDataPoint(new BCDataPoint(data));                
    }
    
    return 0;
}

void OscDataSet::initAttributes(OscAnaManager* config)
{
    /// Import parameters from configfile
    // Range of the Analysis

    fxmin=config->GetPar<double>("obs.xmin");
    fxmax=config->GetPar<double>("obs.xmax");

}