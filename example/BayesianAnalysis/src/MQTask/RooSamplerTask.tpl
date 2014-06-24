/* 
 * File:   RooSamplerTask.tpl
 * Author: winckler
 *
 * Created on June 21, 2014, 10:39 PM
 */

#ifndef ROOSAMPLERTASK_TPL
#define	ROOSAMPLERTASK_TPL


////////// Base template class <T1,T2>



template <typename T1, typename T2> 
RooSamplerTask<T1,T2>::RooSamplerTask() : FairMQSamplerTask("Load class T1")
{
    

    
    
}


template <typename T1, typename T2> 
RooSamplerTask<T1,T2>::~RooSamplerTask() 
{
    if(fDigiVector.size()>0) 
        fDigiVector.clear();
}
//*
template <typename T1, typename T2> 
InitStatus RooSamplerTask<T1,T2>::Init()
{
    FairRootManager* ioman = FairRootManager::Instance();
    fCBMTimeSlice = reinterpret_cast<CbmTimeSlice*>(ioman->GetObject(fBranch.c_str())) ;

    return kSUCCESS;
}


template <typename T1, typename T2> 
void RooSamplerTask<T1,T2>::Exec(Option_t* opt) 
{         
        std::ostringstream buffer;
        T2 OutputArchive(buffer);
            //*
        
        std::cout<<"Event Index = "<< fEventIndex <<std::endl;
        // get start time and endtime(=startime + duration)
        
        //fMicroSliceContent
        OutputArchive << fDigiVector;
        int size=buffer.str().length();
        fOutput = fTransportFactory->CreateMessage(size);
        std::memcpy(fOutput->GetData(), buffer.str().c_str(), size);

        
        // delete the vector content
        if(fDigiVector.size()>0) 
            fDigiVector.clear();
    
    fStsData.clear();
    fMuchData.clear();
    fMicroSliceContent.clear();
}


#endif	/* ROOSAMPLERTASK_TPL */

