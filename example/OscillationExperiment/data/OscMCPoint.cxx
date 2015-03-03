/* 
 * File:   OscMCPoint.cxx
 * Author: winckler
 * 
 * Created on March 3, 2015, 1:49 AM
 */

#include "OscMCPoint.h"
ClassImp(OscMCPoint);
OscMCPoint::OscMCPoint() : TObject()
{
}


OscMCPoint::~OscMCPoint() 
{
}

void OscMCPoint::Reset() 
{
    SampleSize=0.0;
    MCMCNLL1=0.0;
    MCMClambda1=0.0;
    MCMCamplitude=0.0;
    MCMComega=0.0;
    MCMCphi=0.0;
    NLL0=0.0;
    lambda0=0.0;
    lambda0_err=0.0;
    NLL1=0.0;
    lambda1=0.0;
    lambda1_err=0.0;
    amplitude=0.0;
    amplitude_err=0.0;
    omega=0.0;
    omega_err=0.0;
    phi=0.0;
    phi_err=0.0;
    LRT=0.0;
}