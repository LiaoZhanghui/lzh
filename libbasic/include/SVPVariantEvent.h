
#pragma once

#include <functional>
#include <map>
#include <mutex>
//#include <future>

template<typename... Arguments_>
class SVPVariantEvent {
  public:
    typedef std::function<void(const Arguments_&...)> Listener;

    SVPVariantEvent() : m_nextSubscription(0) {}

    uint32_t subscribe(Listener listener);

    void unsubscribe(uint32_t subscritpion);

  public:/*protected:*/
    void fire(const Arguments_&... _eventArguments);

    void fireAsync(const Arguments_&... _eventArguments);

  private:
    uint32_t                        m_nextSubscription;
    std::map<uint32_t, Listener>    m_Listeners;
    std::recursive_mutex            m_mtx;
};

template<typename... Arguments_>
uint32_t SVPVariantEvent<Arguments_...>::subscribe(Listener listener) {
    std::lock_guard<std::recursive_mutex> lock(m_mtx);

    int32_t subscription = m_nextSubscription++;
    m_Listeners[subscription] = std::move(listener);

    return subscription;
}

template<typename... Arguments_>
void SVPVariantEvent<Arguments_...>::unsubscribe(uint32_t subscription) {
    std::lock_guard<std::recursive_mutex> lock(m_mtx);

    m_Listeners.erase(subscription);
}

template<typename ... Arguments_>
void SVPVariantEvent<Arguments_...>::fire(const Arguments_&... eventArguments) {
    std::lock_guard<std::recursive_mutex> lock(m_mtx);

    for (auto& x : m_Listeners) {
        x.second(eventArguments...);
    }
}

template<typename ... Arguments_>
void SVPVariantEvent<Arguments_...>::fireAsync(const Arguments_&... eventArguments) {
    //auto handle = std::async(std::launch::async, [&] { fire(eventArguments...); });
}
