/**
 * Copyright (c) 2012 - 2014 TideSDK contributors 
 * http://www.tidesdk.org
 * Includes modified sources under the Apache 2 License
 * Copyright (c) 2008 - 2012 Appcelerator Inc
 * Refer to LICENSE for details of distribution and use.
 **/

#include "../tide.h"

using namespace tide;

ScopeMethodDelegate::ScopeMethodDelegate(MethodDelegateType type,
                                         TiObjectRef global,
                                         TiObjectRef scope,
                                         TiMethodRef delegate) :
    type(type), global(global), scope(scope), delegate(delegate)
{
}

ScopeMethodDelegate::~ScopeMethodDelegate()
{
}


void ScopeMethodDelegate::Set(const char *name, ValueRef value)
{
    delegate->Set(name,value);
}

ValueRef ScopeMethodDelegate::Get(const char *name)
{
    return delegate->Get(name);
}

SharedStringList ScopeMethodDelegate::GetPropertyNames()
{
    return delegate->GetPropertyNames();
}

bool ScopeMethodDelegate::IsGlobalKey(std::string& key)
{
    std::string::size_type pos = key.find_first_of(".");
    return (pos!=std::string::npos);
}

ValueRef ScopeMethodDelegate::Call(const ValueList& args)
{
    std::string key = args.at(0)->ToString();
    TiObjectRef obj = IsGlobalKey(key) ? global : scope;
    if (type == GET)
    {
        // not found, look inside scope
        return obj->Get(key.c_str());
    }
    else
    {
        ValueRef result = args.at(1);
        obj->SetNS(key.c_str(),result);
        return Value::Undefined;
    }
}

AutoPtr<StaticBoundObject> ScopeMethodDelegate::CreateDelegate(TiObjectRef global, TiObjectRef bo)
{
    AutoPtr<StaticBoundObject> scope = new StaticBoundObject();
    SharedStringList keys = bo->GetPropertyNames();
    StringList::iterator iter = keys->begin();

    while(iter!=keys->end())
    {
        SharedString key_ptr = (*iter++);
        std::string key = *key_ptr;
        ValueRef value = bo->Get(key.c_str());

        if (key == "set")
        {
            TiMethodRef d = new ScopeMethodDelegate(SET, global, scope, value->ToMethod());
            ValueRef v = Value::NewMethod(d);
            scope->Set(key.c_str(), v);
        }
        else if (key == "get")
        {
            TiMethodRef d = new ScopeMethodDelegate(GET, global, scope, value->ToMethod());
            ValueRef v = Value::NewMethod(d);
            scope->Set(key.c_str(), v);
        }
        else
        {
            scope->Set(key.c_str(), value);
        }
    }
    return scope;
}
