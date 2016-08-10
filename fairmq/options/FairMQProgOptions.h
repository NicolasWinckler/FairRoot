/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/*
 * File:   FairMQProgOptions.h
 * Author: winckler
 *
 * Created on March 11, 2015, 10:20 PM
 */

#ifndef FAIRMQPROGOPTIONS_H
#define FAIRMQPROGOPTIONS_H

#include <unordered_map>
#include <set>


#include "FairProgOptions.h"
#include "FairMQEventManager.h"
#include "FairMQChannel.h"


class FairMQProgOptions : public FairProgOptions , public FairMQEventManager
{
  protected:
    typedef std::unordered_map<std::string, std::vector<FairMQChannel>> FairMQMap;

  public:
    typedef boost::signals2::signal<void ()>  signal_t;
    FairMQProgOptions();
    virtual ~FairMQProgOptions();

    // parse command line and txt/INI configuration file. 
    // default parser for the mq-configuration file (JSON/XML) is called if command line key mq-config is called
    virtual void ParseAll(const int argc, char** argv, bool allowUnregistered = false);

    // external parser, store function 
    template <typename T, typename ...Args>
    int UserParser(Args &&... args)
    {
        try
        {
            Store(T().UserParser(std::forward<Args>(args)...));
        }
        catch (std::exception& e)
        {
            LOG(ERROR) << e.what();
            return 1;
        }
        return 0;
    }

    FairMQMap GetFairMQMap()
    {
        return fFairMQMap;
    }

    // to customize title of the executable help command line  
    void SetHelpTitle(const std::string& title)
    {
        fHelpTitle=title;
    }
    // to customize the executable version command line
    void SetVersion(const std::string& version)
    {
        fVersion=version;
    }

    // store key-value of type T into variable_map. 
    // If key is found in fMQKeyMap, update the FairMQChannelMap accordingly
    // Note that the fMQKeyMap is filled:
    // - if UpdateChannelMap(const FairMQMap& map) method is called 
    // - if UserParser template method is called (it is called in the ParseAll method if json or xml MQ-config files is provided)
    
    template<typename T>
    int UpdateValue(const std::string& key, const T& val)
    {

        UpdateVarMap(key,val);

        if(fMQKeyMap.count(key))
        {
            std::string channelName;
            int index = 0;
            std::string member;
            std::tie(channelName, index, member) = fMQKeyMap.at(key);

            if(std::is_same<T, int>::value || std::is_same<T, std::string>::value)
                UpdateChannelMap(channelName, index, member, val);
        }

        if(std::is_same<T, int>::value || std::is_same<T, std::string>::value)
            if(EventKeyFound(key))
                EmitUpdate(key,val);
            

        return 0;
    }

    // wrapper to hide explicit template instantiation of base class function
    template <typename F>
    void ConnectUpdateParamString(const std::string& key, F&& func) 
    {
        FairMQEventManager::Connect<EventId::UpdateParamString>(key,std::forward<F>(func));
    }

    template <typename F>
    void ConnectUpdateParamDouble(const std::string& key, F&& func) 
    {
        FairMQEventManager::Connect<EventId::UpdateParamDouble>(key,std::forward<F>(func));
    }




    template <typename F>
    void ConnectUpdateParamInt(const std::string& key, F&& func) 
    {
        FairMQEventManager::Connect<EventId::UpdateParamInt>(key,std::forward<F>(func));
    }

    template <typename T, typename F>
    void ConnectUpdateParam(const std::string& key, F&& func) 
    {
        FairMQEventManager::Connect2<EventId::UpdateParam,T>(key,std::forward<F>(func));
    }


    // replace FairMQChannelMap, and update variable map accordingly
    int UpdateChannelMap(const FairMQMap& map);

  protected:
    po::options_description fMQParserOptions;
    po::options_description fMQOptionsInCfg;
    po::options_description fMQOptionsInCmd;
    FairMQMap fFairMQMap;
    std::string fHelpTitle;
    std::string fVersion;

    bool EventKeyFound(const std::string& key)
    {
        if (
            FairMQEventManager::EventKeyFound<EventId::UpdateParamDouble>(key) || // <- this one temp?
            FairMQEventManager::EventKeyFound<EventId::UpdateParamString>(key) || 
            FairMQEventManager::EventKeyFound<EventId::UpdateParamInt>(key) 
            )
            return true;
        else
            return false;
    }



    typedef std::tuple<std::string,int,std::string> MQKey;//store key info
    std::map<std::string,MQKey> fMQKeyMap;// key=full path - val=key info

    virtual int NotifySwitchOption(); // for custom help & version printing
    void InitOptionDescription();

    // read FairMQChannelMap and insert/update corresponding values in variable map
    // create key for variable map as follow : channelName.index.memberName
    void UpdateMQValues();
    int Store(const FairMQMap& channels);


  private:

    // wrapper to hide explicit template instantiation of base class function
    void EmitUpdate(const std::string& key, const std::string& val)
    {
        Emit<EventId::UpdateParamString>(key,key,val);
    }

    void EmitUpdate(const std::string& key, double val)
    {
        Emit<EventId::UpdateParamDouble>(key,key,val);
    }

    void EmitUpdate(const std::string& key, int val)
    {
        Emit<EventId::UpdateParamInt>(key,key,val);
    }

    template<typename T>
    void EmitUpdate(const std::string& key, T val)
    {
        // dummy function for overloading of other types other types
    }
    int UpdateChannelMap(const std::string& channelName, int index, const std::string& member, const std::string& val);
    int UpdateChannelMap(const std::string& channelName, int index, const std::string& member, int val);
    // for cases other than int and string
    template<typename T>
    int UpdateChannelMap(const std::string& channelName, int index, const std::string& member, T val)
    {
        return 0;
    }
};


#endif /* FAIRMQPROGOPTIONS_H */

