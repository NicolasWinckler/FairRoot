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
    fKeyValues.push_back("tmin");
    fKeyValues.push_back("tmax");
    fKeyValues.push_back("toffset");
    fKeyValues.push_back("tunit");
    
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
                    fParameterValues[fKeyValues[k]]=lexical_cast<double>(fFileContent[i][1]);
                    break;
                }
                else
                    if(i==fFileContent.size()-1)
                    {
                        stringstream logmsg;
                        logmsg<<"Parameter field '"<<fKeyValues[k]<<"' is not defined in configfile";
                        BCLog::OutError(logmsg.str().c_str());
                        return -1;
                    }
        }
        
        
        for(unsigned int k(0);k<fKeyNames.size();++k)
        {
            for(unsigned int i(0);i<fFileContent.size();++i)
                if(fKeyNames[k]==fFileContent[i][0])
                {
                    fParameterNames[fKeyNames[k]]=fFileContent[i][1];
                    break;
                }
                else
                    if(i==fFileContent.size()-1)
                    {
                        stringstream logmsg;
                        logmsg<<"Parameter field '"<<fKeyNames[k]<<"' is not defined in configfile";
                        BCLog::OutError(logmsg.str().c_str());
                        return -1;
                    }
        }
        
        
        if(print)
        {
            cout << "The experimental parameter set have the following field values:\n";
            
            std::map<string,string>::iterator itn = fParameterNames.begin();
            for (itn=fParameterNames.begin(); itn!=fParameterNames.end(); ++itn)
                cout << itn->first << "\t => " << itn->second << '\n';
            
            std::map<string,double>::iterator itv = fParameterValues.begin();
            for (itv=fParameterValues.begin(); itv!=fParameterValues.end(); ++itv)
                cout << itv->first << "\t \t => " << itv->second << '\n';
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

//ClassImp(SidsParameters)


