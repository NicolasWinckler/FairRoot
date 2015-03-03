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

OscMCPoint::OscMCPoint(const OscMCPoint& orig) : TObject(),
                            SampleSize(orig.SampleSize),
                            MCMCNLL1(orig.MCMCNLL1),
                            MCMCLRT(orig.MCMCLRT),
                            MCMClambda1(orig.MCMClambda1),
                            MCMCamplitude(orig.MCMCamplitude),
                            MCMComega(orig.MCMComega),
                            MCMCphi(orig.MCMCphi),
                            NLL0(orig.NLL0),
                            lambda0(orig.lambda0),
                            lambda0_err(orig.lambda0_err),
                            NLL1(orig.NLL1),
                            lambda1(orig.lambda1),
                            lambda1_err(orig.lambda1_err),
                            amplitude(orig.amplitude),
                            amplitude_err(orig.amplitude_err),
                            omega(orig.omega),
                            omega_err(orig.omega_err),
                            phi(orig.phi),
                            phi_err(orig.phi_err),
                            LRT(orig.LRT)
{
}

OscMCPoint::~OscMCPoint() 
{
}

void OscMCPoint::Reset() 
{
    SampleSize=0.0;
    MCMCNLL1=0.0;
    MCMCLRT=0.0;
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