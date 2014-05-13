//
//  SidsParameters.cpp
//
//
//  Created by Nicolas Winckler on 2/22/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "SidsParameters.h"




SidsParameters::SidsParameters()
{
    fKeyValues.push_back("xmin");
    fKeyValues.push_back("xmax");
    fKeyValues.push_back("xoffset");
    fKeyValues.push_back("xunit");
    
    fKeyNames.push_back("DataName");
    fKeyNames.push_back("DataFileName");
    fKeyNames.push_back("LogFileName");

}


SidsParameters::SidsParameters(vector<string> keyvalues, vector<string> keynames)
{
    fKeyValues=keyvalues;
    fKeyNames=keynames;
}

SidsParameters::~SidsParameters( )
{
    
}
    
    

int SidsParameters::SetExperimentalParameter(string filename, bool print)
{
    /// Tokenize the given file and if successfull, for each row in file, compare first cell
    /// with all given keys. If cell and key are equal, the second cell is tored in value or name fields.
    if(FileReader::TokenizeFileTxt(filename)==0)
    {
        for(unsigned int k(0);k<fKeyValues.size();++k)
        {
            for(unsigned int i(0);i<fFileContent.size();++i)
                if(fKeyValues[k]==fFileContent[i][0])
                {
                    if(fFileContent[i].size()>1)
                    {
                        fParameterValues[fKeyValues[k]]=lexical_cast<double>(fFileContent[i][1]);
                        break;
                    }
                    
                }
                else
                    if(i==fFileContent.size()-1)
                    {
                        stringstream logmsg;
                        logmsg<<"Parameter field '"<<fKeyValues[k]<<"' is not defined in configfile. Value set to zero.";
                        BCLog::OutError(logmsg.str().c_str());
                        fParameterValues[fKeyValues[k]]=0.0;
                        //return -1;
                    }
        }
        
        
        for(unsigned int k(0);k<fKeyNames.size();++k)
        {
            for(unsigned int i(0);i<fFileContent.size();++i)
                if(fKeyNames[k]==fFileContent[i][0])
                {
                    if(fFileContent[i].size()>1)
                    {
                        fParameterNames[fKeyNames[k]]=fFileContent[i][1];
                        break;
                    }
                }
                else
                    if(i==fFileContent.size()-1)
                    {
                        stringstream logmsg;
                        logmsg<<"Parameter field '"<<fKeyNames[k]<<"' is not defined in configfile. Value Set to Undefined";
                        BCLog::OutError(logmsg.str().c_str());
                        fParameterNames[fKeyNames[k]]="Undefined";
                        //return -1;
                    }
        }
        
        
        if(print)
        {
            cout << "The experimental parameter set have the following field values:\n";
            
            std::map<string,string>::iterator itn = fParameterNames.begin();
            for (itn=fParameterNames.begin(); itn!=fParameterNames.end(); ++itn)
                cout << itn->first << "\t \t \t => " << itn->second << '\n';
            
            std::map<string,double>::iterator itv = fParameterValues.begin();
            for (itv=fParameterValues.begin(); itv!=fParameterValues.end(); ++itv)
                cout << itv->first << "\t \t \t \t => " << itv->second << '\n';
        }
    }
    return 0;
}



double SidsParameters::GetValue(string key)
{
    double value=fParameterValues[key];    
    
    return value;
}



string SidsParameters::GetName(string key)
{
    string name=fParameterNames[key];
    
    return name;
}


void SidsParameters::SetValue(string key, double Value)
{
    fParameterValues[key]=Value;
}


void SidsParameters::SetName(string key, string Value)
{
    fParameterNames[key]=Value;
}


int SidsParameters::PrintToBCLog()
{
    BCLog::OutSummary("The experimental parameter set have the following field values:");
    
    std::map<string,string>::iterator itn = fParameterNames.begin();
    for (itn=fParameterNames.begin(); itn!=fParameterNames.end(); ++itn)
    {
        std::ostringstream bufferc;
        bufferc << itn->first << "\t \t \t => " << itn->second;
        BCLog::OutSummary(bufferc.str().c_str());
    }
    
    
    std::map<string,double>::iterator itv = fParameterValues.begin();
    for (itv=fParameterValues.begin(); itv!=fParameterValues.end(); ++itv)
    {
        std::ostringstream bufferv;
        bufferv << itv->first << "\t \t \t  => " << itv->second;
        BCLog::OutSummary(bufferv.str().c_str());
    }
    return 0;
}
//ClassImp(SidsParameters)


