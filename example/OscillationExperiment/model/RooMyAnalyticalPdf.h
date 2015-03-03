/* 
 * File:   RooMyAnalyticalPdf.h
 * Author: winckler
 *
 * Created on June 20, 2014, 2:20 PM
 */

#ifndef ROOMYANALYTICALPDF_H
#define	ROOMYANALYTICALPDF_H

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
class RooRealVar;

class RooMyAnalyticalPdf : public RooAbsPdf 
{
    
public:
        
    RooMyAnalyticalPdf() {} ;
    RooMyAnalyticalPdf(const char *name,const char *title,RooAbsReal& _x,
                        RooAbsReal& _lambda, RooAbsReal& _amp, 
                        RooAbsReal& _omega, RooAbsReal& _phi);
    RooMyAnalyticalPdf(const RooMyAnalyticalPdf& other, const char* name);
    virtual TObject* clone(const char* newname) const { return new RooMyAnalyticalPdf(*this,newname); }
    inline virtual ~RooMyAnalyticalPdf() { };
    Int_t getAnalyticalIntegral(RooArgSet& allVars,RooArgSet& analVars,const char* rangeName=0) const;
    Double_t analyticalIntegral(Int_t code,const char* rangeName = 0) const;
    //Double_t analyticalIntegral(Int_t code) const;
    
    

protected:
    
    //Double_t evaluate(const RooDataSet* dset) const;
    Double_t evaluate() const ;
    RooRealProxy x;
    RooRealProxy lambda;
    RooRealProxy amp;
    RooRealProxy omega;
    RooRealProxy phi;
           
    
private:
    ClassDef(RooMyAnalyticalPdf,1);
};
#endif	/* ROOMYANALYTICALPDF_H */

