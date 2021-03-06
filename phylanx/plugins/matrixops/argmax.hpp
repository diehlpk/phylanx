// Copyright (c) 2018 Parsa Amini
// Copyright (c) 2018 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PHYLANX_PRIMITIVES_ARGMAX)
#define PHYLANX_PRIMITIVES_ARGMAX

#include <phylanx/config.hpp>
#include <phylanx/execution_tree/primitives/base_primitive.hpp>
#include <phylanx/execution_tree/primitives/primitive_component_base.hpp>

#include <hpx/lcos/future.hpp>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace phylanx { namespace execution_tree { namespace primitives
{
    /// \brief Implementation of argmax as a Phylanx primitive.
    /// Returns the index of the largest element of argument a.
    /// This implementation is intended to behave like [NumPy implementation of argmax]
    /// (https://docs.scipy.org/doc/numpy-1.14.0/reference/generated/numpy.argmax.html).
    /// \param a It may be a scalar value, vector, or matrix
    /// \param axis The dimension along which the indices of the max value(s) should be found
    class argmax
      : public primitive_component_base
      , public std::enable_shared_from_this<argmax>
    {
    protected:
        hpx::future<primitive_argument_type> eval(
            primitive_arguments_type const& operands,
            primitive_arguments_type const& args) const;

        using val_type = double;
        using arg_type = ir::node_data<val_type>;
        using args_type = std::vector<arg_type, arguments_allocator<arg_type>>;

    public:
        static match_pattern_type const match_data;

        argmax() = default;

        argmax(primitive_arguments_type&& operands,
            std::string const& name, std::string const& codename);

        hpx::future<primitive_argument_type> eval(
            primitive_arguments_type const& args) const override;

    private:
        primitive_argument_type argmax0d(args_type && args) const;
        primitive_argument_type argmax1d(args_type && args) const;
        primitive_argument_type argmax2d_flatten(arg_type && arg_a) const;
        primitive_argument_type argmax2d_x_axis(arg_type && arg_a) const;
        primitive_argument_type argmax2d_y_axis(arg_type && arg_a) const;
        primitive_argument_type argmax2d(args_type && args) const;
    };

    inline primitive create_argmax(hpx::id_type const& locality,
        primitive_arguments_type&& operands,
        std::string const& name = "", std::string const& codename = "")
    {
        return create_primitive_component(
            locality, "argmax", std::move(operands), name, codename);
    }
}}}

#endif
