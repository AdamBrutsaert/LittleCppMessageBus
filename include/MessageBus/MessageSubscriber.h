#pragma once

#include <memory>

class MessageCallable
{
public:
    virtual ~MessageCallable() = default;
    virtual void operator()(void const*) const = 0;
};

template <typename M, typename F> class MessageCallableCaster : public MessageCallable
{
public:
    MessageCallableCaster(F&& func) : m_Func(std::forward<F>(func)) 
    {
    }

    void operator()(void const* msg) const
    {
        m_Func(*static_cast<M const*>(msg));
    }

private:
    F m_Func;
};

// TODO - Allocate / Free MessageCallableCaster in a MessageCallablePool instead
template <typename M> class MessageSubscriberFunction : public MessageCallable
{
public:
    template <typename F>
    MessageSubscriberFunction(F&& func) : m_Func(new MessageCallableCaster<M, F>{std::forward<F>(func)})
    {
    }

    // TODO - Implements copy constructor
    // The problem is that the copy constructor is resolved to be the forwarding
    // constructer above
    // https://stackoverflow.com/questions/70267685/generic-constructor-template-called-instead-of-copy-move-constructor
    // https://stackoverflow.com/questions/17842478/select-class-constructor-using-enable-if

    MessageSubscriberFunction(MessageSubscriberFunction&& subscriber) 
    {
        m_Func = subscriber.m_Func;
        subscriber.m_Func = nullptr;
    }

    ~MessageSubscriberFunction() override final 
    {
        delete m_Func;
        m_Func = nullptr;
    }

    void operator()(void const* msg) const override final
    { 
        (*m_Func)(msg); 
    }

    // Utility function to allow us to pass argument of type M
    void operator()(M const& msg) const 
    { 
        (*m_Func)(&msg); 
    }

private:
    MessageCallable* m_Func;
};

template <typename M>
using MessageSubscriber = std::shared_ptr<MessageSubscriberFunction<M>>;

// TODO - Allocate / free Subscriber in a MessageCallablePool instead
template <typename M, typename F>
inline MessageSubscriber<M> MakeMessageSubscriber(F &&func) 
{
    return std::make_shared<MessageSubscriberFunction<M>>(std::forward<F>(func));
}
