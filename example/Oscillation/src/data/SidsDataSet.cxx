//
//  SidsDataSet.cxx
//  
//
//  Created by Nicolas Winckler on 2/21/14.
//
//

#include "SidsDataSet.h"


// ---------------------------------------------------------
SidsDataSet::SidsDataSet() //: BCDataSet()
{
    // default constructor
    fBCDataVector = 0;
    fTree = 0;
}


// ---------------------------------------------------------
SidsDataSet::~SidsDataSet()
// default destructor
{
    if (fBCDataVector) {
        int ndatapoints = int(fBCDataVector->size());
        for (int i = 0; i < ndatapoints; ++i)
            delete fBCDataVector->at(i);
        fBCDataVector->clear();
        delete fBCDataVector;
    }
    
    if(fTree) delete fTree;
}


// ---------------------------------------------------------
SidsDataSet::SidsDataSet(const SidsDataSet & sidsDataSet)
{
    
    if (sidsDataSet.fBCDataVector)
    {
        if(fBCDataVector) fBCDataVector->clear();// if fBCDataVector is already initialized clear
        else fBCDataVector = new BCDataVector();// or create a new one
        
        for (int i = 0; i < int(sidsDataSet.fBCDataVector->size()); ++i)
        {
            if (sidsDataSet.fBCDataVector->at(i))
                fBCDataVector->push_back(new BCDataPoint(*(sidsDataSet.fBCDataVector->at(i))));
            else
                fBCDataVector->push_back(0);
        }
    }
    else fBCDataVector = 0;
    
    if (sidsDataSet.fTree)
    {
        if(fTree)
        {
            fTree->Reset();
            fTree->CopyEntries(sidsDataSet.fTree);
            fTree = sidsDataSet.fTree->CloneTree();
        }
        else
        {
            string treename(sidsDataSet.fTree->GetName());
            treename+="_cloned";
            fTree = new TTree(treename.c_str(),treename.c_str());
            fTree->CopyEntries(sidsDataSet.fTree);
        }
    }
    else fTree=0;
}



// ---------------------------------------------------------
SidsDataSet & SidsDataSet::operator = (const SidsDataSet & sidsDataSet)
{
    if (sidsDataSet.fBCDataVector)
    {
        if(fBCDataVector) fBCDataVector->clear();// check whether fBCDataVector is already initialized
        else fBCDataVector = new BCDataVector();
        
        for (int i = 0; i < int(sidsDataSet.fBCDataVector->size()); ++i)
        {
            if (sidsDataSet.fBCDataVector->at(i))
                fBCDataVector->push_back(new BCDataPoint(*(sidsDataSet.fBCDataVector->at(i))));
            else
                fBCDataVector->push_back(0);
        }
    }
    else fBCDataVector = 0;
    
    
    /*
    if (sidsDataSet.fTree)
    {
        if(fTree)
        {
            fTree->Reset();
            fTree->CopyEntries(sidsDataSet.fTree);
        }
        else
        {
            string treename(sidsDataSet.fTree->GetName());
            treename+="_cloned";
            fTree = new TTree(treename.c_str(),treename.c_str());
            fTree->CopyEntries(sidsDataSet.fTree);
        }
    }
    else fTree=0;
    */
    
    // return this
    return *this;
}

// ---------------------------------------------------------

unsigned int SidsDataSet::GetNDataPoints()
{
    return fBCDataVector ? fBCDataVector->size() : 0;
}

// ---------------------------------------------------------

unsigned int SidsDataSet::GetNValuesPerPoint()
{
    // check if vector exists and contains datapoints
    if (fBCDataVector && fBCDataVector->size() > 0)
        return GetDataPoint(0)->GetNValues();
    
    BCLog::OutError("SidsDataSet::GetNValuesPerPoint : Data set doesn't exist yet");
    return 0;
}

// ---------------------------------------------------------

BCDataPoint * SidsDataSet::GetDataPoint(unsigned int index)
{
    if (!fBCDataVector || GetNDataPoints()==0 )
    {
        BCLog::OutError("SidsDataSet::GetDataPoint : Dataset is empty.");
        return 0;
    }
    
    // check if index is within range. Return the data point if true ...
    if(index < GetNDataPoints())
        return fBCDataVector->at(index);
    
    // ... or give out warning and return 0 if not.
    BCLog::OutError("SidsDataSet::GetDataPoint : Index out of range. Return 0.");
    return 0;
}

// ---------------------------------------------------------
std::vector<double> SidsDataSet::GetDataComponents( int index)
{
    unsigned int N = GetNDataPoints();
    std::vector<double> components( N , 0.0 );
    
    BCDataPoint* point=0;
    for (unsigned int i = 0; i < N; ++i) {
        //rely on index checking in Get... methods
        point = GetDataPoint(i);
        components[i] = point->GetValue(index);
    }
    
    return components;
}



// ---------------------------------------------------------

void SidsDataSet::AddDataPoint(BCDataPoint * datapoint)
{
    
    // check if memory for the vector has been allocated and
    // allocate if not.
    if (fBCDataVector == 0)
        fBCDataVector = new BCDataVector();
    
    // add data point to the data set.
    fBCDataVector->push_back(datapoint);
    
}

// ---------------------------------------------------------

void SidsDataSet::Reset()
{
    
    // if memory has been allocated to the data set
    // clear the content.
    if (fBCDataVector != 0)
        fBCDataVector->clear();
    
}

// ---------------------------------------------------------

void SidsDataSet::Dump()
{
    if (!fBCDataVector) {
        BCLog::OutError("SidsDataSet::Dump : Data set is empty. Nothing to dump.");
        return;
    }
    
    BCLog::OutSummary("Data set summary:");
    BCLog::OutSummary(Form("Number of points           : %d", int(fBCDataVector->size())));
    BCLog::OutSummary(Form("Number of values per point : %d", GetDataPoint(0)->GetNValues()));
    unsigned int n = GetDataPoint(0)->GetNValues();
    for (unsigned int i=0; i< fBCDataVector->size(); i++) {
        BCLog::OutSummary(Form("Data point %5d :  ", i));
        for (unsigned int j=0; j<n; j++)
            BCLog::OutSummary(Form("%d : %12.5g", j, GetDataPoint(i)->GetValue(j)));
    }
}


// ---------------------------------------------------------

int SidsDataSet::ReadDataFromFileTree(const char * filename, const char * treename, const char * branchnames)
{
    // open root file
    TFile * file = TFile::Open(filename, "READ");
    
    // check if file is open and warn if not.
    if (!file->IsOpen())
    {
        BCLog::OutError(Form("SidsDataSet::ReadDataFromFileTree : Could not open file %s.", filename));
        return -1;
    }
    
    // get tree
    TTree * tree = (TTree*) file->Get(treename);
    
    // check if tree is there and warn if not.
    if (!tree)
    {
        BCLog::OutError(Form("SidsDataSet::ReadDataFromFileTree : Could not find TTree %s.", treename));
        
        // close file
        file->Close();
        
        return -1;
    }
    
    // if data set contains data, clear data object container ...
    if (fBCDataVector != 0)
    {
        fBCDataVector->clear();
        
        BCLog::OutDetail("SidsDataSet::ReadDataFromFileTree : Overwrite existing data.");
    }
    
    // ... or allocate memory for the vector if not.
    else
        fBCDataVector = new BCDataVector();
    
    // get branch names.
    
    // first, copy the branchnames into a std::string.
    std::string branches(branchnames);
    
    // define a vector of std::strings which contain the tree names.
    std::vector<std::string> * branchnamevector = new std::vector<std::string>;
    
    // the names are supposed to be separated by commas. find first comma
    // entry in the string.
    int temp_index = branches.find_first_of(",");
    
    // reset number of branches
    int nbranches = 0;
    
    // repeat until the is nothing left in the string.
    while(branches.size() > 0)
    {
        // temporary string which contains the name of the current branch
        std::string branchname;
        
        // get current branch name
        
        // if there is no comma the current branchname corresponds to the whole string, ...
        if (temp_index == -1)
            branchname = branches;
        
        // ... if there is a comma, copy that part of the string into the current branchname.
        else
            branchname.assign(branches, 0, temp_index);
        
        // write branch name to a vector
        branchnamevector->push_back(branchname);
        
        // increase the number of branches found
        nbranches++;
        
        // cut remaining string with branchnames
        
        // if there is no comma left empty the string, ...
        if (temp_index == -1)
            branches = "";
        
        // ... if there is a comma remove the current branchname from the string.
        else
            branches.erase(0, temp_index + 1);
        
        // find the next comma
        temp_index = branches.find_first_of(",");
    }
    
    // create temporary vector with data and assign some zeros.
    std::vector<double> data;
    data.assign(nbranches, 0.0);
    
    // set the branch address.
    for (int i = 0; i < nbranches; i++)
        tree->SetBranchAddress(branchnamevector->at(i).data(), &data.at(i));
    
    // calculate maximum number of entries
    int nentries = tree->GetEntries();
    
    // check if there are any events in the tree and close file if not.
    if (nentries <= 0)
    {
        BCLog::OutError(Form("SidsDataSet::ReadDataFromFileTree : No events in TTree %s.", treename));
        
        // close file
        file->Close();
        
        return -1;
    }
    
    // loop over entries
    for (int ientry = 0; ientry < nentries; ientry++)
    {
        // get entry
        tree->GetEntry(ientry);
        
        // create data object
        BCDataPoint * datapoint = new BCDataPoint(nbranches);
        
        // copy data
        
        for (int i = 0; i < nbranches; i++)
            datapoint->SetValue(i, data.at(i));
        
        // add data point to this data set.
        AddDataPoint(datapoint);
    }
    
    // close file
    file->Close();
    
    // remove file pointer.
    if (file)
        delete file;
    
    return 0;
    
}

// ---------------------------------------------------------

int SidsDataSet::ReadDataFromFileTxt(const char * filename, int nbranches)
{
    // open text file.
    std::fstream file;
    file.open(filename, std::fstream::in);
    
    // check if file is open and warn if not.
    if (!file.is_open())
    {
        BCLog::OutError(Form("SidsDataSet::ReadDataFromFileText : Could not open file %s.", filename));
        return -1;
    }
    
    // if data set contains data, clear data object container ...
    if (fBCDataVector != 0)
    {
        fBCDataVector->clear();
        
        BCLog::OutDetail("SidsDataSet::ReadDataFromFileTxt : Overwrite existing data.");
    }
    
    // ... or allocate memory for the vector if not.
    else
        fBCDataVector = new BCDataVector();
    
    // create temporary vector with data and assign some zeros.
    std::vector<double> data;
    data.assign(nbranches, 0.0);
    
    // reset counter
    int nentries = 0;
    
    // read data and create data points.
    while (!file.eof())
    {
        // read data from file
        int i=0;
        while(file >> data[i])
        {
            if (i==nbranches-1)
                break;
            i++;
        }
        
        // create data point.
        if(i == nbranches-1)
        {
            BCDataPoint * datapoint = new BCDataPoint(nbranches);
            
            // copy data into data point
            for (int i = 0; i < nbranches; i++)
                datapoint->SetValue(i, data.at(i));
            
            // add data point to this data set.
            AddDataPoint(datapoint);
            
            // increase counter
            nentries++;
        }
    }
    
    // check if there are any events in the tree and close file if not.
    if (nentries <= 0)
    {
        BCLog::OutError(Form("SidsDataSet::ReadDataFromFileText : No events in the file %s.", filename));
        
        // close file
        file.close();
        
        return -1;
    }
    
    // close file
    file.close();
    
    return 0;
    
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
    if (fBCDataVector != 0)
    {
        fBCDataVector->clear();
        
        BCLog::OutDetail("SidsDataSet::ReadDataFromFileTxt : Overwrite existing data.");
    }
    
    // ... or allocate memory for the vector if not.
    else
        fBCDataVector = new BCDataVector();
    
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
	
    return 0;
}


// ---------------------------------------------------------
SidsDataSet & SidsDataSet::operator + ( SidsDataSet & sidsDataSet)
{
    if (sidsDataSet.fBCDataVector)
    {
        for (int i = 0; i < sidsDataSet.GetNDataPoints(); ++i)
            AddDataPoint(sidsDataSet.GetDataPoint(i));
        
    }
    //else fBCDataVector = 0;
    
    
    /*
    if (sidsDataSet.fTree)
    {
        if(fTree)
        {
            double value;
            sidsDataSet.fTree->SetBranchAddress("x",&value);
            fTree->SetBranchAddress("x",&value);
            unsigned int nentries = sidsDataSet.fTree->GetEntries();
            for(unsigned int i(0); i<nentries; ++i)
            {
                sidsDataSet.fTree->GetEntry(i);
                fTree->Fill();
            }
        }
        else
        {
            
        }
    }
    else fTree=0;
    */
    
    // return this
    return *this;
}


//ClassImp(sidsDataSet)
