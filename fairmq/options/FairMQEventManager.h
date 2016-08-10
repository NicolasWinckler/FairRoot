/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

/*
 * File:   FairMQEventManager.h
 * Author: winckler
 *
 * Created on August 12, 2016, 13:50 PM
 */

#ifndef FAIRMQEVENTMANAGER_H
#define FAIRMQEVENTMANAGER_H

#include <map>
#include <utility>
#include <string>

#include <boost/any.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <boost/signals2/signal.hpp>




enum class EventId : uint32_t 
{
    // Place your new EventManager events here
    UpdateParam           = 0,
    Init                  = 1,
    Restart               = 2,
    Run                   = 3,
    Custom                = 4,
    UpdateParamString     = 5,
    UpdateParamDouble     = 6,
    UpdateParamInt        = 7,
    
};

namespace Events 
{

    template <EventId> struct Traits;
    template <> struct Traits<EventId::UpdateParam>       { using signal_type = boost::signals2::signal<void(const std::string&, const std::string&)>; } ;
    template <> struct Traits<EventId::UpdateParamString> { using signal_type = boost::signals2::signal<void(const std::string&, const std::string&)>; } ;
    template <> struct Traits<EventId::UpdateParamDouble> { using signal_type = boost::signals2::signal<void(const std::string&, double)>; } ;
    template <> struct Traits<EventId::UpdateParamInt>    { using signal_type = boost::signals2::signal<void(const std::string&, int)>; } ;

    //*
    template <EventId, typename ...Args> struct Traits2;
    template <> struct Traits2<EventId::UpdateParam> { using signal_type = boost::signals2::signal<void(const std::string&, const std::string&)>; } ;
    template <typename ...T> struct Traits2<EventId::UpdateParam,T...> { using signal_type = boost::signals2::signal<void(const std::string&, T...)>; } ;
    template <> struct Traits2<EventId::UpdateParamInt>    { using signal_type = boost::signals2::signal<void(const std::string&, int)>; } ;
    // */
}


class FairMQEventManager 
{
  public:
    typedef std::pair<EventId,std::string> EventKey;
    template<typename T> using EventKey2 = std::tuple<EventId,std::string,T> ;
    
    FairMQEventManager() : fEventMap() {}
    virtual ~FairMQEventManager(){}
    
    template <EventId event, typename F>
    void Connect(const std::string& key, F&& func) 
    {
        GetSlot<event>(key).connect(std::forward<F>(func));
    }

    template <EventId event, typename ValueType, typename F>
    void Connect2(const std::string& key, F&& func) 
    {
        GetSlot2<event,ValueType>(key).connect(std::forward<F>(func));
    }
    

    template <EventId event, typename... Args>
    void Emit(const std::string& key, Args&&... args) 
    {
        GetSlot<event>(key)(std::forward<Args>(args)...);
    }


    template <EventId event, typename ValueType, typename... Args>
    void Emit2(const std::string& key, Args&&... args) 
    {
        GetSlot2<event,ValueType>(key)(std::forward<Args>(args)...);
    }


    template <EventId event>
    bool EventKeyFound(const std::string& key)
    {
        if (fEventMap.find(std::pair<EventId,std::string>(event,key) ) != fEventMap.end())
            return true;
        else
            return false;
    }

  private:

    
    std::map<EventKey, boost::any> fEventMap;
    std::map<EventKey, boost::any> fEventMap2;

    template <EventId event, typename Slot = typename Events::Traits<event>::signal_type, typename SlotPtr = boost::shared_ptr<Slot> >
    Slot& GetSlot(const std::string& key) 
    {
        try 
        {
            EventKey eventKey = std::make_pair(event,key);
            if (fEventMap.find(eventKey) == fEventMap.end())
                fEventMap.emplace(eventKey, boost::make_shared<Slot>());

            return *boost::any_cast<SlotPtr>(fEventMap.at(eventKey));
        }
        catch (boost::bad_any_cast const &e) 
        {
            LOG(ERROR) << "Caught instance of boost::bad_any_cast: " << e.what() << " on event #" << static_cast<uint32_t>(event);
            abort();
        }
    }

    template <EventId event, typename T, typename Slot = typename Events::Traits2<event,T>::signal_type, typename SlotPtr = boost::shared_ptr<Slot> >
    Slot& GetSlot2(const std::string& key) 
    {
        try 
        {
            EventKey eventKey = std::make_pair(event,key);
            if (fEventMap2.find(eventKey) == fEventMap2.end())
                fEventMap2.emplace(eventKey, boost::make_shared<Slot>());

            return *boost::any_cast<SlotPtr>(fEventMap2.at(eventKey));
        }
        catch (boost::bad_any_cast const &e) 
        {
            LOG(ERROR) << "Caught instance of boost::bad_any_cast: " << e.what() << " on event #" << static_cast<uint32_t>(event);
            abort();
        }
    }
};


#endif /* FAIRMQEVENTMANAGER_H */

