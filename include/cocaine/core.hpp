#ifndef COCAINE_CORE_HPP
#define COCAINE_CORE_HPP

#define EV_MINIMAL 0
#include <ev++.h>

#include "cocaine/common.hpp"
#include "cocaine/forwards.hpp"
#include "cocaine/networking.hpp"
#include "cocaine/security/signatures.hpp"

namespace cocaine { namespace core {

class core_t:
    public boost::noncopyable
{
    friend class future_t;
    
    public:
        core_t();
        ~core_t();

        // Event loop
        void run();
        
    private:
        // Signal processing
        void terminate(ev::sig& sig, int revents);
        void reload(ev::sig& sig, int revents);
        void purge(ev::sig& sig, int revents);

        // Request processing
        void request(ev::io& io, int revents);

        // Request dispatching
        void dispatch(future_t* future, const Json::Value& root);
        
        // Request handling
        void push(future_t* future, const std::string& target, const Json::Value& args);
        void drop(future_t* future, const std::string& target, const Json::Value& args);
        void past(future_t* future, const std::string& target, const Json::Value& args);
        void stat(future_t* future);

        // Called by a deferred future when all the keys are fulfilled
        void seal(const std::string& future_id);

        // Publishing
        void event(ev::io& io, int revents);

        // Engine request processing and dispatching
        void interthread(ev::io& io, int revents);

        void future(const std::string& future_id, const std::string& key, const Json::Value& value);
        void reap(const std::string& engine_id, const std::string& thread_id);
        
        // Task recovery
        void recover();

    private:
        security::signatures_t m_signatures;

        // Engine management (URI -> Engine)
        typedef boost::ptr_map<const std::string, engine::engine_t> engine_map_t;
        engine_map_t m_engines;

        // Future management
        typedef boost::ptr_map<const std::string, future_t> future_map_t;
        future_map_t m_futures;

        // History
        typedef std::deque< std::pair<ev::tstamp, Json::Value> > history_t;
        typedef boost::ptr_map<std::string, history_t> history_map_t;
        history_map_t m_histories;

        // Networking
        zmq::context_t m_context;
        lines::socket_t s_requests, s_publisher;
        lines::channel_t s_events, s_interthread;
        
        // Event loop
        ev::default_loop m_loop;
        ev::io e_events, e_requests, e_interthread;
        ev::sig e_sigint, e_sigterm, e_sigquit, e_sighup, e_sigusr1;

        // Hostname
        std::string m_hostname;
};

}}

#endif