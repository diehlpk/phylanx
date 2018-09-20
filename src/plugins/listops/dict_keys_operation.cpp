// Copyright (c) 2018 Weile Wei
// Copyright (c) 2018 Parsa Amini
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <phylanx/config.hpp>
#include <phylanx/ir/dictionary.hpp>
#include <phylanx/plugins/listops/dict_keys_operation.hpp>
#include <phylanx/plugins/listops/dictionary_operation.hpp>
#include <phylanx/util/matrix_iterators.hpp>

#include <hpx/include/lcos.hpp>
#include <hpx/include/util.hpp>
#include <hpx/throw_exception.hpp>

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <blaze/Math.h>

///////////////////////////////////////////////////////////////////////////////
namespace phylanx { namespace execution_tree { namespace primitives {
    ///////////////////////////////////////////////////////////////////////////
    match_pattern_type const dict_keys_operation::match_data = {
        hpx::util::make_tuple("dict_keys",
            std::vector<std::string>{"dict_keys(_1)"},
            &create_dict_keys_operation, &create_primitive<dict_keys_operation>,
            "lili\n"
            "Args:\n"
            "\n"
            "    lili (list of lists, optional) : a list of 2-element lists\n"
            "\n"
            "Returns:\n"
            "\n"
            "The dict_keys primitive returns a list of dictionary key objects "
            "constructed "
            "from dictionary.")};

    ///////////////////////////////////////////////////////////////////////////
    dict_keys_operation::dict_keys_operation(
        primitive_arguments_type&& operands, std::string const& name,
        std::string const& codename)
      : primitive_component_base(std::move(operands), name, codename)
    {
    }

    ///////////////////////////////////////////////////////////////////////////
    primitive_argument_type dict_keys_operation::generate_dict_keys(
        primitive_arguments_type&& args) const
    {
        //using arg_type = ir::dictionary;
        auto args_dict =
            extract_dictionary_value(std::move(args[0]), name_, codename_);
        //if (is_dictionary_operand(args))
        //{
        //}


        //phylanx::ir::range r(args_dict.begin(), args_dict.end());

        //for (ir::rang_iterator it = args_dict.begin(); it != r.end(); ++it)
        //{
        //    std::cout << *it << std::endl;
        //}
        //std::cout << r.size() << std::endl;
        std::cout << "========" << std::endl;
        std::cout << "======== " << std::endl;
        //ir::range args_list =
        //    extract_list_value_strict(std::move(args[0]), name_, codename_);

        //if (args_list.empty())
        //{
        //    return primitive_argument_type{ir::dictionary{}};
        //}

        //phylanx::ir::dictionary dict;
        //dict.reserve(args_list.size());
        //for (auto it = args_list.begin(); it != args_list.end(); ++it)
        //{
        //    if (!is_list_operand(*it))
        //    {
        //        HPX_THROW_EXCEPTION(hpx::bad_parameter,
        //            "dictionary_operation::generate_dict",
        //            util::generate_error_message(
        //                "dict_operation expects a list of lists with exactly "
        //                "elements each",
        //                name_, codename_));
        //    }

        //    auto&& element = extract_list_value(*it);
        //    auto&& p = element.args();

        //    if (p.size() != 2)
        //    {
        //        HPX_THROW_EXCEPTION(hpx::bad_parameter,
        //            "dictionary_operation::generate_dict",
        //            util::generate_error_message(
        //                "dict_operation needs exactly two values for each of "
        //                "the key/value pairs",
        //                name_, codename_));
        //    }

        //    dict[std::move(p[0])] = std::move(p[1]);
        //}
        return primitive_argument_type{
            phylanx::ir::node_data<std::int64_t>(42)};
    }

    //////////////////////////////////////////////////////////////////////////
    hpx::future<primitive_argument_type> dict_keys_operation::eval(
        primitive_arguments_type const& operands,
        primitive_arguments_type const& args) const
    {
        if (operands.size() > 1 || operands.empty())
        {
            HPX_THROW_EXCEPTION(hpx::bad_parameter,
                "dict_keys_operation::eval",
                util::generate_error_message(
                    "the dict_keys primitive accepts exactly one argument",
                    name_, codename_));
        }

        if (!operands.empty() && !valid(operands[0]))
        {
            HPX_THROW_EXCEPTION(hpx::bad_parameter,
                "dict_keys_operation::eval",
                util::generate_error_message(
                    "the dict_keys primitive requires that the "
                    "argument given by the operand is a valid dictionary object",
                    name_, codename_));
        }

        auto this_ = this->shared_from_this();
        return hpx::dataflow(hpx::launch::sync,
            hpx::util::unwrapping([this_](primitive_arguments_type&& args)
                                      -> primitive_argument_type {
                return this_->generate_dict_keys(std::move(args));
            }),
            detail::map_operands(
                operands, functional::value_operand{}, args, name_, codename_));
    }

    hpx::future<primitive_argument_type> dict_keys_operation::eval(
        primitive_arguments_type const& args) const
    {
        if (this->no_operands())
        {
            return eval(args, noargs);
        }
        return eval(this->operands(), args);
    }
}}}