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

        if(fVarMap.count(key))
        {
            // update variable map
            UpdateVarMap(key,val);

            // update FairMQChannel map, check first if data are int or string
            if(std::is_same<T, int>::value || std::is_same<T, std::string>::value)
                if(fMQKeyMap.count(key))
                {
                    std::string channelName;
                    int index = 0;
                    std::string member;
                    std::tie(channelName, index, member) = fMQKeyMap.at(key);
                    UpdateChannelMap(channelName, index, member, val);
                }

            // execute stored function of a given key if exist
            //if(std::is_same<T, int>::value || std::is_same<T, std::string>::value)//if one wants to restrict type
            if(EventKeyFound(key))
                EmitUpdate<T>(key,val);

            return 0;
        }
        else
        {

            LOG(ERROR)  <<"UpdatedValue failed because the provided key '"
                        <<key
                        <<"' is not found in the variable map";
        }
        return 0;
    }

    
    template <typename T, typename F>
    void Subscribe(const std::string& key, F&& func) 
    {
        if(fVarMap.count(key))
            FairMQEventManager::Connect<EventId::UpdateParam,T>(key,std::forward<F>(func));
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
            FairMQEventManager::EventKeyFound<EventId::UpdateParam>(key) 
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

    template<typename T>
    void EmitUpdate(const std::string& key, T val)
    {
        Emit<EventId::UpdateParam,T>(key,key,val);
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

