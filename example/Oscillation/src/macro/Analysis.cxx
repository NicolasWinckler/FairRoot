//
//  Analysis.cpp
//  batTuto1
//
//  Created by Nicolas Winckler on 2/20/14.
//  Copyright (c) 2014 Nicolas Winckler. All rights reserved.
//

#include "Analysis.h"



Analysis::Analysis ( )
{
    
    /// Set configuration of the analysis from configuration file
    
    
    // define field char names that are in configfile
    fcharfield.push_back("DataName");
    fcharfield.push_back("DataPathName");
    fcharfield.push_back("OutPutDirectory");
    fcharfield.push_back("LogFileName");
    
    fcharfield.push_back("MCMCPrecision");
    
}


Analysis::~Analysis ( )
{
	
}





string Analysis::get_date()
{
   time_t now;
   char the_date[100];

   the_date[0] = '\0';

   now = time(NULL);

   if (now != -1)
   {
      strftime(the_date, 100, "%Y%m%d_%H%M%S_", gmtime(&now));
   }

   return string(the_date);
}


string Analysis::get_prefix(SidsParameters Sidspar)
{
   
   string strdate="run"+get_date();
   string prefix=Sidspar.GetName("OutPutDirectory") + strdate;
   prefix+=Sidspar.GetName("DataName");
   prefix+="_";
   prefix+="MCMCPrecision";
   prefix+=Sidspar.GetName("MCMCPrecision");
   prefix+="_";
   return prefix;
}


