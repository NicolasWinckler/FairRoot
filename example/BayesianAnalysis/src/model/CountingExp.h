// ***************************************************************
// This file was created using the CreateProject.sh script.
// CreateProject.sh is part of Bayesian Analysis Toolkit (BAT).
// BAT can be downloaded from http://www.mppmu.mpg.de/bat
// ***************************************************************

#ifndef __BAT__COUNTINGEXP__H
#define __BAT__COUNTINGEXP__H

#include <BAT/BCModel.h>

// This is a CountingExp header file.
// Model source code is located in file CountingExp/CountingExp.cxx
class BCDataPoint;
// ---------------------------------------------------------
class CountingExp : public BCModel
{
   public:

      // Constructors and destructor
      CountingExp();
      CountingExp(const char * name);
      ~CountingExp();

      // Methods to overload, see file CountingExp.cxx
      void DefineParameters();
      double LogAPrioriProbability(const std::vector<double> &parameters);
      double LogLikelihood(const std::vector<double> &parameters);
      // void MCMCIterationInterface();
//      ClassDef(CountingExp,1);
};
// ---------------------------------------------------------

#endif

