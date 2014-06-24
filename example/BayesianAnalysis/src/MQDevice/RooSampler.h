/* 
 * File:   RooSampler.h
 * Author: winckler
 *
 * Created on June 21, 2014, 10:03 PM
 */

#ifndef ROOSAMPLER_H
#define	ROOSAMPLER_H

#include <string>

#include "FairMQDevice.h"

struct Content {
  int id;
  double a;
  double b;
  int x;
  int y;
  int z;
};

class RooSampler: public FairMQDevice
{
  public:
    enum {
      InputFile = FairMQDevice::Last,
      EventRate,
      EventSize,
      Last
    };
    RooSampler();
    virtual ~RooSampler();
    void Log(int intervalInMs);
    void ResetEventCounter();
    virtual void SetProperty(const int key, const string& value, const int slot = 0);
    virtual string GetProperty(const int key, const string& default_ = "", const int slot = 0);
    virtual void SetProperty(const int key, const int value, const int slot = 0);
    virtual int GetProperty(const int key, const int default_ = 0, const int slot = 0);

  protected:
    int fEventSize;
    int fEventRate;
    int fEventCounter;
    virtual void Init();
    virtual void Run();
};

#endif	/* ROOSAMPLER_H */

