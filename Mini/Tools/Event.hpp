#pragma once
#include <vector>
#include <functional>

namespace Mini {
template<typename... T>
class Event {
private:
    
    template<typename...Args>
    struct IDelegate {
        int type;
        virtual ~IDelegate() {}
        virtual void Invoke(Args... values) = 0;
    };
    using Delegate = IDelegate<T...>;
    using Delegates = std::vector<Delegate*>;
    Delegates delegates;
    int defaultDelegatesCount;
    
    template<typename Obj>
    struct DelegateMember : public IDelegate<T...> {
        Obj* object;
        void (Obj::*method)(T...);
        
        void Invoke(T... values) override {
            (object->*method)(std::forward<T>(values)...);
        }
    };
    
    template<typename Obj, typename Context>
    struct DelegateMemberContext : public IDelegate<T...> {
        Obj* object;
        Context context;
        void (Obj::*method)(T..., Context);
        
        void Invoke(T... values) override {
            (object->*method)(std::forward<T>(values)..., context);
        }
    };
    
    struct DelegateLambda : public IDelegate<T...> {
        std::function<void(T...)> function;
        void Invoke(T... values) override {
            function(std::forward<T>(values)...);
        }
    };
    
    static int objectIDCounter;
    
    template<typename O>
    int GetObjectID() {
        static int id = objectIDCounter++;
        return id;
    }
    
public:
    
    Event() : defaultDelegatesCount(0) {}
    Event(const Event<T...>& other) {
        ClearNonDefaults();
    }
    Event(Event<T...>& other) {
        ClearNonDefaults();
    }
    Event<T...>& operator=(const Event<T...>& other) {
        ClearNonDefaults();
        return *this;
    }
    
    Event(Event&& other) {
        delegates = other.delegates;
        defaultDelegatesCount = other.defaultDelegatesCount;
        other.delegates.clear();
    }
    
    ~Event() { Clear(); }
    
    void Clear() noexcept {
        for(auto d : delegates) delete d;
        delegates.clear();
        defaultDelegatesCount = 0;
    }
    
    void ClearNonDefaults() noexcept {
        for(int i=defaultDelegatesCount; i<delegates.size(); ++i) {
            delete delegates[i];
        }
        delegates.resize(defaultDelegatesCount);
    }
    
    void MarkDefaults() {
        defaultDelegatesCount = (int)delegates.size();
    }
    
    bool Empty() const noexcept {
        return delegates.empty();
    }
    
    void operator () (T... values) {
        for(auto d : delegates) {
            d->Invoke(values...);
        }
    }
    
    template<typename Obj>
    void Bind(Obj* object, void (Obj::*method)(T...)) {
        DelegateMember<Obj>* delegate = new DelegateMember<Obj>();
        delegate->type = GetObjectID<Obj>();
        delegate->object = object;
        delegate->method = method;
        delegates.push_back(delegate);
    }
    
    template<typename Obj>
    void Unbind(Obj* object, void (Obj::*method)(T...)) {
        short typeID = GetObjectID<Obj>();
        int size = (int)delegates.size();
        for(int i=0; i<size; ++i) {
            auto d = delegates[i];
            if (d->type != typeID) continue;
            DelegateMember<Obj>* delegate = (DelegateMember<Obj>*)(d);
            if (delegate->object != object) continue;
            if (delegate->method != method) continue;
            delegates.erase(delegates.begin() + i);
            delete d;
            return;
        }
    }
    
    template<typename Obj, typename Context>
    void Bind(Obj* object, void (Obj::*method)(T..., Context), Context context) {
        DelegateMemberContext<Obj, Context>* delegate = new DelegateMemberContext<Obj, Context>();
        delegate->type = 10000 + GetObjectID<Obj>() + GetObjectID<Context>() * 10000;
        delegate->object = object;
        delegate->method = method;
        delegate->context = context;
        delegates.push_back(delegate);
    }
    
    template<typename Obj, typename Context>
    void Unbind(Obj* object, void (Obj::*method)(T..., Context), Context context) {
        int typeID = 10000 + GetObjectID<Obj>() + GetObjectID<Context>() * 10000;
        int size = (int)delegates.size();
        for(int i=0; i<size; ++i) {
            auto d = delegates[i];
            if (d->type != typeID) continue;
            DelegateMemberContext<Obj, Context>* delegate = (DelegateMemberContext<Obj, Context>*)(d);
            if (delegate->object != object) continue;
            if (delegate->method != method) continue;
            if (delegate->context != context) continue;
            delegates.erase(delegates.begin() + i);
            delete d;
            return;
        }
    }
    
    template<typename Lambda>
    void Bind(Lambda&& lambda) {
        DelegateLambda* delegate = new DelegateLambda();
        delegate->type = -1;
        delegate->function = lambda;
        delegates.push_back(delegate);
    }
};

template<typename...T>
int Event<T...>::objectIDCounter = 0;
}

