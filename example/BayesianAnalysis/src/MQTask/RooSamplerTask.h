/* 
 * File:   RooSamplerTask.h
 * Author: winckler
 *
 * Created on June 21, 2014, 10:38 PM
 */

#ifndef ROOSAMPLERTASK_H
#define	ROOSAMPLERTASK_H

#include "FairMQSamplerTask.h"

#include "FairMQLogger.h"
#include <boost/timer/timer.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>

#include <iostream> 
#include <iterator>
#include <algorithm>
#include <cassert>


#if __cplusplus >= 201103L
#include <type_traits>
#include <array>

#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////


////////// Base template header <T1,T2>
template <typename T1, typename T2>
class RooSamplerTask : public FairMQSamplerTask
{ 
public : 
 
    RooSamplerTask();
    virtual ~RooSamplerTask();
    virtual InitStatus Init();
    virtual void Exec(Option_t* opt);
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & fDigiVector;
    }
    
    
private :
    friend class boost::serialization::access;
    vector<T1> fDigiVector;
    
}; 





////////// Template implementation is in RooSamplerTask.tpl :
#include "RooSamplerTask.tpl"

#endif	/* ROOSAMPLERTASK_H */

