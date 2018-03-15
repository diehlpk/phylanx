//  Copyright (c) 2018 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PHYLANX_PRIMITIVES_GENERIC_FUNCTION_FEB_15_2018_0125PM)
#define PHYLANX_PRIMITIVES_GENERIC_FUNCTION_FEB_15_2018_0125PM

#include <phylanx/config.hpp>
#include <phylanx/execution_tree/primitives/base_primitive.hpp>
#include <phylanx/execution_tree/primitives/primitive_component_base.hpp>

#include <hpx/async.hpp>
#include <hpx/lcos/future.hpp>

#include <string>
#include <utility>
#include <vector>

namespace phylanx { namespace execution_tree { namespace primitives
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Action, bool direct = Action::direct_execution::value>
    class generic_function;

    // wrapping non-direct actions
    template <typename Action>
    class generic_function<Action, false>
      : public primitive_component_base
    {
    public:
        static match_pattern_type const match_data;

        generic_function() = default;

        generic_function(std::vector<primitive_argument_type>&& operands,
                std::string const& name, std::string const& codename)
          : primitive_component_base(std::move(operands), name, codename)
        {}

        // Create a new instance of the variable and initialize it with the
        // value as returned by evaluating the given body.
        hpx::future<primitive_argument_type> eval(
            std::vector<primitive_argument_type> const& args) const override
        {
            return hpx::async(
                Action(), hpx::find_here(), operands_, args, name_, codename_);
        }

    private:
        std::string name_;
    };

    // wrapping direct actions
    template <typename Action>
    class generic_function<Action, true>
      : public primitive_component_base
    {
    public:
        static match_pattern_type const match_data;

        generic_function() = default;

        generic_function(std::vector<primitive_argument_type>&& operands,
                std::string const& name, std::string const& codename)
          : primitive_component_base(std::move(operands), name, codename)
        {}

        // Create a new instance of the variable and initialize it with the
        // value as returned by evaluating the given body.
        primitive_argument_type eval_direct(
            std::vector<primitive_argument_type> const& args) const override
        {
            return hpx::async(hpx::launch::sync, Action(), hpx::find_here(),
                operands_, args, name_, codename_).get();
        }

    private:
        std::string name_;
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Action>
    primitive create_generic_function(
        hpx::id_type const& locality,
        std::vector<primitive_argument_type>&& operands,
        std::string const& name = "", std::string const& codename = "")
    {
        return create_primitive_component(locality,
            hpx::actions::detail::get_action_name<Action>(),
            std::move(operands), name, codename);
    }
}}}

#endif


