// Copyright (c) 2017-2018 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <phylanx/config.hpp>
#include <phylanx/execution_tree/primitives/primitive_argument_type.hpp>

#include <hpx/include/serialization.hpp>

namespace phylanx { namespace execution_tree
{
    ///////////////////////////////////////////////////////////////////////////
    void topology::serialize(hpx::serialization::output_archive& ar, unsigned)
    {
        ar & children_ & name_;
    }

    void topology::serialize(hpx::serialization::input_archive& ar, unsigned)
    {
        ar & children_ & name_;
    }

    ///////////////////////////////////////////////////////////////////////////
    void primitive_argument_type::serialize(
        hpx::serialization::output_archive& ar, unsigned)
    {
        ar & variant();
    }

    void primitive_argument_type::serialize(
        hpx::serialization::input_archive& ar, unsigned)
    {
        ar & variant();
    }
}}
