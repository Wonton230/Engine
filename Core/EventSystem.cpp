#include "EventSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"

EventSystem::EventSystem(EventSystemConfig const& config)
	:m_config(config)
{
}

EventSystem::~EventSystem()
{
}

void EventSystem::Startup()
{
}

void EventSystem::Shutdown()
{
}

void EventSystem::BeginFrame()
{
}

void EventSystem::EndFrame()
{
}

void EventSystem::SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr)
{
	SubscriptionList& subscribersForThisEvent = m_subscriptionListsByEventName[eventName];
	EventSubscription* newEventSubscription = new EventSubscription(functionPtr);
	subscribersForThisEvent.push_back(newEventSubscription);
}

void EventSystem::UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr)
{
	std::map< std::string, SubscriptionList >::iterator found = m_subscriptionListsByEventName.find(eventName);
	if (found == m_subscriptionListsByEventName.end())
	{
		return;
	}

	SubscriptionList& subscribersForThisEvent = found->second;
	int numSubscribers = static_cast<int>(subscribersForThisEvent.size());
	for (int i = 0; i < numSubscribers; ++i)
	{
		EventSubscription*& subscriber = subscribersForThisEvent[i];
		if (subscriber && subscriber->m_functionPtr == functionPtr)
		{
			subscriber = nullptr;
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName, EventArgs& args)
{
	std::map< std::string, SubscriptionList >::iterator found = m_subscriptionListsByEventName.find(eventName);
	if (found == m_subscriptionListsByEventName.end())
	{
		return;
	}

	// Found a list of subscribers for this event; call each one in turn (or until someone "consumes" the event)
	SubscriptionList& subscribersForThisEvent = found->second;
	int numSubscribers = static_cast<int>(subscribersForThisEvent.size());
	for (int i = 0; i < numSubscribers; ++i)
	{
		EventSubscription* subscriber = subscribersForThisEvent[i];
		if (subscriber)
		{
			bool wasConsumed = subscriber->m_functionPtr(args); // Execute the subscriber's callback function!
			if (wasConsumed)
			{
				break; // Event was "consumed" by this subscriber; stop notifying any other subscribers!
			}
		}
	}
}

void EventSystem::FireEvent(std::string const& eventName)
{
	EventArgs emptyArgs;
	FireEvent(eventName, emptyArgs);
}

void SubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr)
{
	g_theEventSystem->SubscribeEventCallbackFunction(eventName, functionPtr);
}

void UnsubscribeEventCallbackFunction(std::string const& eventName, EventCallbackFunction* functionPtr)
{
	g_theEventSystem->UnsubscribeEventCallbackFunction(eventName, functionPtr);
}

void FireEvent(std::string const& eventName, EventArgs& args)
{
	g_theEventSystem->FireEvent(eventName, args);
}

void FireEvent(std::string const& eventName)
{
	g_theEventSystem->FireEvent(eventName);
}
