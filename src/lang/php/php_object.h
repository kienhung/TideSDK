/**
 * Copyright (c) 2012 - 2014 TideSDK contributors
 * http://www.tidesdk.org
 * Includes modified sources under the Apache 2 License
 * Copyright (c) 2008 - 2012 Appcelerator Inc
 * Refer to LICENSE for details of distribution and use.
 **/

#ifndef _PHP_OBJECT_H_
#define _PHP_OBJECT_H_

namespace tide
{
    class KPHPObject : public TiObject
    {
        public:
        KPHPObject(zval* object);
        virtual ~KPHPObject();

        virtual void Set(const char *name, ValueRef value);
        virtual ValueRef Get(const char *name);
        virtual SharedStringList GetPropertyNames();
        virtual SharedString DisplayString(int);
        virtual bool Equals(TiObjectRef);
        bool PropertyExists(const char* property TSRMLS_DC);
        bool MethodExists(const char* methodName TSRMLS_DC);
        zval* ToPHP();
        
        private:
        zval* object;
    };
}

#endif
