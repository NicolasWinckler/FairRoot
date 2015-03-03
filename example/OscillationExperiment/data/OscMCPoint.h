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
    OscMCPoint(const OscMCPoint& orig);
    virtual ~OscMCPoint();
    void Reset();

    double SampleSize=0.0;
    
    
    ///MC MC data
    double MCMCNLL1=0.0;
    double MCMCLRT=0.0;
    double MCMClambda1=0.0;
    double MCMCamplitude=0.0;
    double MCMComega=0.0;
    double MCMCphi=0.0;
    
    /// Minimization data
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
            ar& MCMCNLL1;
            ar& MCMCLRT;
            ar& MCMClambda1;
            ar& MCMCamplitude;
            ar& MCMComega;
            ar& MCMCphi;

        }

    #ifndef __CINT__ // for BOOST serialization
    friend class boost::serialization::access;
    #endif // for BOOST serialization

    ClassDef(OscMCPoint, 1);
};

#endif	/* OSCMCPOINT_H */

