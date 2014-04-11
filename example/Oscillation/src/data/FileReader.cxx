//
//  FileReader.cxx
//  batTuto1
//
//  Created by Nicolas Winckler on 2/22/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "FileReader.h"



// ---------------------------------------------------------
FileReader::FileReader()
{
    // default constructor
    
}


// ---------------------------------------------------------
FileReader::~FileReader()
// default destructor
{
}

/////////////////////////////////////////////////////////////////////////
/// Read, tokenize and store file content
/////////////////////////////////////////////////////////////////////////
int FileReader::TokenizeFileTxt(string filename)
{
	fFileContent.clear();
    string line;
    ifstream InputFile(filename.c_str());
    
    if (!InputFile.is_open())
    {
        stringstream logmsg;
        logmsg<<"File "<< filename.c_str() <<" cannot be read";
        BCLog::OutError(logmsg.str().c_str());
        return -1;
    }
    
    while (getline(InputFile, line))
    {
        if(!line.empty())
        {
            vector<string> stringArray;
            typedef tokenizer<char_separator<char> > tokenizer;
            char_separator<char> sep("\t ;,|" );
            tokenizer tokens(line,sep);
            tokenizer::iterator tok_iter = tokens.begin();
            tok_iter = tokens.begin();
            for(; tok_iter != tokens.end();++tok_iter)
                stringArray.push_back(lexical_cast<string>((*tok_iter).c_str()));
            fFileContent.push_back(stringArray);
        }
    }
    InputFile.close();
	
    return 0;
}

/////////////////////////////////////////////////////////////////////////
/// if numerical value, convert file content into double  and fill matrix
/////////////////////////////////////////////////////////////////////////
int FileReader::ConvertStrToDouble()
{
	fRawData.clear();
    if(fFileContent.size()==0)
    {
        BCLog::OutError("File content array has zero row");
        return -1;
    }
    
	for(unsigned int i(0);i<fFileContent.size();++i)
    {
		vector<double> row;
		for(unsigned int j(0);j<fFileContent[i].size();++j)
		{
			if(is_number(fFileContent[i][j]))
				row.push_back(lexical_cast<double>(fFileContent[i][j]));
			else
			{
				stringstream logmsg;
                logmsg<<"Cell content '"<<fFileContent[i][j]<<"' is not a number Cell[i,j]=["<<i<<","<<j<<"]. Element is discarded.";
                BCLog::OutWarning(logmsg.str().c_str());
			}
		}
		if(row.size()>0)
            fRawData.push_back(row);
	}
	
    return 0;
}


/////////////////////////////////////////////////////////////////////////
/// return true if it is a number
/////////////////////////////////////////////////////////////////////////
bool FileReader::is_number(const string& s)
{
    bool is_a_number = false;
    try
    {
        lexical_cast<double>(s);
        is_a_number = true;
    }
    catch(bad_lexical_cast &)
    {
        // if it throws, it's not a number.
    }
    return is_a_number;
}

ClassImp(FileReader)







