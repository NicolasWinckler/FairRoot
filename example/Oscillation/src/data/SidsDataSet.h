//
//  SidsDataSet.h
//  
//
//  Created by Nicolas Winckler on 2/21/14.
//
//

#ifndef ____SidsDataSet__
#define ____SidsDataSet__

#include <iostream>
#include <fstream>

#include "FileReader.h"
#include "SidsParameters.h"

#include "BAT/BCDataSet.h"
#include "BAT/BCDataPoint.h"
#include "BAT/BCLog.h"

#include <TFile.h>
#include <TString.h>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

# include "TTree.h"




// BAT classes
//class BCDataPoint;

// ---------------------------------------------------------

//typedef std::vector<BCDataPoint*> BCDataVector;

class SidsDataSet : public BCDataSet, public FileReader
{
    public:
    
    SidsDataSet();
    SidsDataSet(const SidsDataSet & sidsDataSet);// modified for tree copy

    virtual ~SidsDataSet();
    
    SidsDataSet & operator = (const SidsDataSet & sidsDataSet);// modified for tree copy
    SidsDataSet & operator + ( SidsDataSet & sidsDataSet);// modified for tree copy
    
    int FillTreeFromFileTxt(SidsParameters Sidspar);//my stuff
    int ReadDataFromFileTxt(SidsParameters Sidspar);//my stuff
    
    unsigned int GetNDataPoints();
    unsigned int GetNValuesPerPoint();
    BCDataPoint * GetDataPoint(unsigned int index);
    std::vector<double> GetDataComponents(int index);
    
    int ReadDataFromFile(const char * filename, const char * treename, const char * branchnames)
    { return this -> ReadDataFromFileTree(filename, treename, branchnames); };
    
    int ReadDataFromFile(const char * filename, int nvariables)
    { return this -> ReadDataFromFileTxt(filename, nvariables); };
    
    int ReadDataFromFileTree(const char * filename, const char * treename, const char * branchnames);
    int ReadDataFromFileTxt(const char * filename, int nvariables);
    
    void AddDataPoint(BCDataPoint * datapoint);
    
    void Reset();
    void Dump();
    
    private :
    
    TTree* fTree;
    string branchname;
    BCDataVector *fBCDataVector;
    
    ClassDef(sidsDataSet,1);
};




#endif /* defined(____SidsDataSet__) */
