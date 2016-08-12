
/*
 *	This file was automatically generated by dbusxx-xml2cpp; DO NOT EDIT!
 */

#ifndef __dbusxx__genivi_session_adaptor_h__ADAPTOR_MARSHAL_H
#define __dbusxx__genivi_session_adaptor_h__ADAPTOR_MARSHAL_H

#include <dbus-c++/dbus.h>
#include <cassert>

namespace org {
namespace genivi {
namespace navigationcore {

class Session_adaptor
: public ::DBus::InterfaceAdaptor
{
public:

    Session_adaptor()
    : ::DBus::InterfaceAdaptor("org.genivi.navigationcore.Session")
    {
        register_method(Session_adaptor, GetVersion, _GetVersion_stub);
        register_method(Session_adaptor, CreateSession, _CreateSession_stub);
        register_method(Session_adaptor, DeleteSession, _DeleteSession_stub);
        register_method(Session_adaptor, GetSessionStatus, _GetSessionStatus_stub);
        register_method(Session_adaptor, GetAllSessions, _GetAllSessions_stub);
    }

    ::DBus::IntrospectedInterface *introspect() const 
    {
        static ::DBus::IntrospectedArgument GetVersion_args[] = 
        {
            { "version", "(qqqs)", false },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedArgument CreateSession_args[] = 
        {
            { "client", "s", true },
            { "sessionHandle", "u", false },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedArgument DeleteSession_args[] = 
        {
            { "sessionHandle", "u", true },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedArgument GetSessionStatus_args[] = 
        {
            { "sessionHandle", "u", true },
            { "sessionStatus", "i", false },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedArgument GetAllSessions_args[] = 
        {
            { "sessionsList", "a(us)", false },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedArgument SessionDeleted_args[] = 
        {
            { "sessionHandle", "u", false },
            { 0, 0, 0 }
        };
        static ::DBus::IntrospectedMethod Session_adaptor_methods[] = 
        {
            { "GetVersion", GetVersion_args },
            { "CreateSession", CreateSession_args },
            { "DeleteSession", DeleteSession_args },
            { "GetSessionStatus", GetSessionStatus_args },
            { "GetAllSessions", GetAllSessions_args },
            { 0, 0 }
        };
        static ::DBus::IntrospectedMethod Session_adaptor_signals[] = 
        {
            { "SessionDeleted", SessionDeleted_args },
            { 0, 0 }
        };
        static ::DBus::IntrospectedProperty Session_adaptor_properties[] = 
        {
            { 0, 0, 0, 0 }
        };
        static ::DBus::IntrospectedInterface Session_adaptor_interface = 
        {
            "org.genivi.navigationcore.Session",
            Session_adaptor_methods,
            Session_adaptor_signals,
            Session_adaptor_properties
        };
        return &Session_adaptor_interface;
    }

public:

    /* properties exposed by this interface, use
     * property() and property(value) to get and set a particular property
     */

public:

    /* methods exported by this interface,
     * you will have to implement them in your ObjectAdaptor
     */
    virtual ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > GetVersion() = 0;
    virtual uint32_t CreateSession(const std::string& client) = 0;
    virtual void DeleteSession(const uint32_t& sessionHandle) = 0;
    virtual int32_t GetSessionStatus(const uint32_t& sessionHandle) = 0;
    virtual std::vector< ::DBus::Struct< uint32_t, std::string > > GetAllSessions() = 0;

public:

    /* signal emitters for this interface
     */
    void SessionDeleted(const uint32_t& arg1)
    {
        ::DBus::SignalMessage sig("SessionDeleted");
        ::DBus::MessageIter wi = sig.writer();
        wi << arg1;
        emit_signal(sig);
    }

private:

    /* unmarshalers (to unpack the DBus message before calling the actual interface method)
     */
    ::DBus::Message _GetVersion_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        ::DBus::Struct< uint16_t, uint16_t, uint16_t, std::string > argout1 = GetVersion();
        ::DBus::ReturnMessage reply(call);
        ::DBus::MessageIter wi = reply.writer();
        wi << argout1;
        return reply;
    }
    ::DBus::Message _CreateSession_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        std::string argin1; ri >> argin1;
        uint32_t argout1 = CreateSession(argin1);
        ::DBus::ReturnMessage reply(call);
        ::DBus::MessageIter wi = reply.writer();
        wi << argout1;
        return reply;
    }
    ::DBus::Message _DeleteSession_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        uint32_t argin1; ri >> argin1;
        DeleteSession(argin1);
        ::DBus::ReturnMessage reply(call);
        return reply;
    }
    ::DBus::Message _GetSessionStatus_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        uint32_t argin1; ri >> argin1;
        int32_t argout1 = GetSessionStatus(argin1);
        ::DBus::ReturnMessage reply(call);
        ::DBus::MessageIter wi = reply.writer();
        wi << argout1;
        return reply;
    }
    ::DBus::Message _GetAllSessions_stub(const ::DBus::CallMessage &call)
    {
        ::DBus::MessageIter ri = call.reader();

        std::vector< ::DBus::Struct< uint32_t, std::string > > argout1 = GetAllSessions();
        ::DBus::ReturnMessage reply(call);
        ::DBus::MessageIter wi = reply.writer();
        wi << argout1;
        return reply;
    }
};

} } } 
#endif //__dbusxx__genivi_session_adaptor_h__ADAPTOR_MARSHAL_H