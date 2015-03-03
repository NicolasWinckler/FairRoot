/* 
 * File:   OscMCPoint.h
 * Author: winckler
 *
 * Created on March 3, 2015, 1:49 AM
 */

#ifndef OSCMCPOINT_H
#define	OSCMCPOINT_H

#include <iostream> // for operator<<, basic_ostream, etc
#include "TObject.h"

#ifndef __CINT__
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#endif //__CINT__

class OscMCPoint : public  TObject
{
public:
    OscMCPoint();
    virtual ~OscMCPoint();

    double SampleSize=0.0;
    
    double NLL0=0.0;
    
    double lambda0=0.0;
    double lambda0_err=0.0;
    
    double NLL1=0.0;
    
    double lambda1=0.0;
    double lambda1_err=0.0;
    double amplitude=0.0;
    double amplitude_err=0.0;
    double omega=0.0;
    double omega_err=0.0;
    double phi=0.0;
    double phi_err=0.0;
    
    double LRT=0.0;
    
    template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& SampleSize;
            ar& NLL0;
            ar& lambda0;
            ar& lambda0_err;
            ar& NLL1;
            ar& lambda1;
            ar& lambda1_err;
            ar& amplitude;
            ar& amplitude_err;
            ar& omega;
            ar& omega_err;
            ar& phi;
            ar& phi_err;
            ar& LRT;

        }

    #ifndef __CINT__ // for BOOST serialization
    friend class boost::serialization::access;
    #endif // for BOOST serialization

    ClassDef(OscMCPoint, 1);
};

#endif	/* OSCMCPOINT_H */

