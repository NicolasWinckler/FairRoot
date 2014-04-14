// ***************************************************************
// This file was created using the CreateProject.sh script.
// CreateProject.sh is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://www.mppmu.mpg.de/bat
// ***************************************************************

#include "CountingExp.h"
#include <BAT/BCDataPoint.h>
#include <BAT/BCParameter.h>
#include <BAT/BCMath.h>
#include <cmath>


// ---------------------------------------------------------
CountingExp::CountingExp() : BCModel()
{
   // default constructor
   DefineParameters();
}

// ---------------------------------------------------------
CountingExp::CountingExp(const char * name) : BCModel(name)
{
   // constructor
   DefineParameters();
}

// ---------------------------------------------------------
CountingExp::~CountingExp()
   // default destructor
{
}

// ---------------------------------------------------------
void CountingExp::DefineParameters()
{
   // Add parameters to your model here.
   // You can then use them in the methods below by calling the
   // parameters.at(i) or parameters[i], where i is the index
   // of the parameter. The indices increase from 0 according to the
   // order of adding the parameters.

   // Allowed range for R_B is [0, 2]
   AddParameter("R_B", 0.0, 2.0);
}

// ---------------------------------------------------------
double CountingExp::LogLikelihood(const std::vector<double> &parameters)
{
   // This methods returns the logarithm of the conditional probability
   // p(data|parameters). This is where you have to define your model.

   double logprob = 0.;

   // get background measurement
   double T = GetDataPoint(0)->GetValue(0);
   double N1 = GetDataPoint(0)->GetValue(1);

   // extract value of background rate
   double R_B = parameters.at(0);

   // calculate expected counts given background rate
   double n_B = R_B * T;

   // update likelihood
   logprob += -n_B + N1 * log(n_B) - BCMath::LogFact(N1);

   return logprob;
}

// ---------------------------------------------------------
double CountingExp::LogAPrioriProbability(const std::vector<double> &parameters)
{
   // This method returns the logarithm of the prior probability for the
   // parameters p(parameters).
    double logprob = 0.0;

   // normalize flat prior with parameter ranges
   for (unsigned int i = 0; i < GetNParameters(); i++)
       logprob -= log(GetParameter(i)->GetRangeWidth());

    return 0.0;
}
// ---------------------------------------------------------

//ClassImp(CountingExp)
