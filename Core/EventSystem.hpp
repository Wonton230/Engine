#pragma once
#include "Engine/Core/NamedStrings.hpp"
#include <vector>
#include <string>
#include <map>

typedef NamedStrings EventArgs;
typedef bool (EventCallbackFunction)(EventArgs& args);

struct EventSubscription
{
	EventSubscription(EventCallbackFunction* functionPtr)
		: m_functionPtr(functionPtr) {}

	EventCallbackFunction* m_functionPtr = nullptr;
};

typedef std::vector<EventSubscription*> SubscriptionList;

//Function templates

struct EventSystemConfig
{

};

class EventSystem
{
public:
	EventSystem(EventSystemConfig const& config);
	~EventSystem();
	void Startup(); 
	void Shutdown(); 
	void BeginFrame();
	void EndFrame();

	void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr); 
	void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr); 
	void FireEvent(std::string const& eventName, EventArgs& args);
	void FireEvent(std::string const& eventName);

protected:
	EventSystemConfig m_config;
	std::map < std::string, SubscriptionList >  m_subscriptionListsByEventName;
};


// Standalone global-namespace helper functions; these forward to "the" event system, if it exists //
void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr);
void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr);
void FireEvent(std::string const& eventName, EventArgs& args);
void FireEvent(std::string const& eventName);
