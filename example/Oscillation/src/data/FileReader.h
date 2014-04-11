//
//  FileReader.h
//  batTuto1
//
//  Created by Nicolas Winckler on 2/22/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#ifndef __batTuto1__FileReader__
#define __batTuto1__FileReader__

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <sstream>
#include <fstream>

#include <stdlib.h>


#include <BAT/BCLog.h>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;


using namespace std;




class FileReader
{
    public:
    
    FileReader();
    ~FileReader( );
    
    int TokenizeFileTxt(string filename);
    int ConvertStrToDouble();
    bool is_number(const string& s);


    
    protected :
    vector< vector<string> > fFileContent;
    vector< vector<double> > fRawData;
    ClassDef(FileReader,1);
};






#endif /* defined(__batTuto1__FileReader__) */
