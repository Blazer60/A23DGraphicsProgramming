/**
 * @file Callback.h
 * @author Ryan Purse
 * @date 14/04/2022
 */


#pragma once

#include "Pch.h"

#include <functional>

/**
 * Allows functions to be 'subscribed' to an event that can be triggered by calling broadcast.
 * @author Ryan Purse
 * @date 14/04/2022
 */
template<typename ...TArgs>
class Callback
{
public:
    typedef std::function<void(const TArgs &...)> Signature;
    typedef uint32_t Token;
    
    uint32_t subscribe(const Signature &subscriber);
    
    void unsubscribe(const Token token);
    
    void broadcast(const TArgs &... args) const;
    
protected:
    std::vector<Signature>  mSubscribers;
    std::vector<Token>      mTokens;
    Token                   mNextToken      { 0 };
};

template<typename ...TArgs>
uint32_t Callback<TArgs...>::subscribe(const Callback::Signature &subscriber)
{
    mSubscribers.push_back(subscriber);
    mTokens.emplace_back(++mNextToken);
    return mNextToken;
}

template<typename... TArgs>
void Callback<TArgs...>::unsubscribe(const Callback::Token token)
{
    const auto it       = std::find(mTokens.begin(), mTokens.end(), token);
    const auto offset   = std::distance(mTokens.begin(), it);
    
    mSubscribers.erase(it);
    mTokens.erase(mTokens.begin() + offset);
}

template<typename... TArgs>
void Callback<TArgs...>::broadcast(const TArgs &... args) const
{
    for (const auto &subscriber : mSubscribers)
        subscriber(args...);
}


