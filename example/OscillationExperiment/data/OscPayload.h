/* 
 * File:   OscPayload.h
 * Author: winckler
 *
 * Created on March 2, 2015, 11:28 AM
 */

#ifndef OSCPAYLOAD_H
#define	OSCPAYLOAD_H

struct OscPayload
{
    OscPayload() = default;
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
    
    
};

#endif	/* OSCPAYLOAD_H */

