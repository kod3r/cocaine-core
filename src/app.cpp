//
// Copyright (C) 2011-2012 Andrey Sibiryov <me@kobology.ru>
//
// Licensed under the BSD 2-Clause License (the "License");
// you may not use this file except in compliance with the License.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <boost/algorithm/string/join.hpp>
#include <boost/assign.hpp>

#include "cocaine/app.hpp"

#include "cocaine/context.hpp"

using namespace cocaine::engine;

// Application
// -----------

app_t::app_t(context_t& ctx, const std::string& name_, const Json::Value& manifest_):
	name(name_),
	manifest(manifest_),
    log(ctx.log(name_))
{
    type = manifest["type"].asString();

    endpoint = boost::algorithm::join(
        boost::assign::list_of
            (std::string("ipc:///var/run/cocaine"))
            (ctx.config.core.instance)
            (name),
        "/");

    policy.suicide_timeout = manifest["engine"].get(
        "suicide-timeout",
        ctx.config.engine.suicide_timeout
    ).asDouble();
    
    policy.pool_limit = manifest["engine"].get(
        "pool-limit",
        ctx.config.engine.pool_limit
    ).asUInt();
    
    policy.queue_limit = manifest["engine"].get(
        "queue-limit",
        ctx.config.engine.queue_limit
    ).asUInt();
}