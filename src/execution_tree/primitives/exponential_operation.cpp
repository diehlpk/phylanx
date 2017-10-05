//   Copyright (c) 2017 Bibek Wagle
//
//   Distributed under the Boost Software License, Version 1.0. (See accompanying
//   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <phylanx/config.hpp>
#include <phylanx/ast/detail/is_literal_value.hpp>
#include <phylanx/execution_tree/primitives/exponential_operation.hpp>
#include <phylanx/ir/node_data.hpp>
#include <phylanx/util/serialization/eigen.hpp>

#include <hpx/include/components.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/util.hpp>

#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <cmath>

#include <unsupported/Eigen/MatrixFunctions>

////////////////////////////////////////////////////////////////////////////////////////
typedef hpx::components::component<
    phylanx::execution_tree::primitives::exponential_operation>
    exponential_operation_type;
HPX_REGISTER_DERIVED_COMPONENT_FACTORY(
    exponential_operation_type, phylanx_exponential_operation_component,
    "phylanx_primitive_component", hpx::components::factory_enabled)
HPX_DEFINE_GET_COMPONENT_TYPE(exponential_operation_type::wrapped_type)
///////////////////////////////////////////////////////////////////////////////////////

namespace phylanx { namespace execution_tree { namespace primitives
    {
      ///////////////////////////////////////////////////////////////////////////
      exponential_operation::exponential_operation
          (std::vector<primitive_argument_type> &&operands)
          : operands_(std::move(operands))
      {
          if (operands_.size() > 1)
          {
            HPX_THROW_EXCEPTION(hpx::bad_parameter,
                                "exponential_operation::exponential_operation",
                                "the exponential_operation primitive requires"
                                "exactly one operand");
          }

          if (!valid(operands_[0]))
          {
            HPX_THROW_EXCEPTION(hpx::bad_parameter,
                                "exponential_operation::exponential_operation",
                                "the exponential_operation primitive requires "
                                "that the arguments given by the operands array"
                                " is valid");
          }
      }

      ///////////////////////////////////////////////////////////////////////////
      ir::node_data<double> exponential_operation::exponential0d
          (operands_type const& ops) const
      {
        return std::exp(ops[0][0]);
      }

      ///////////////////////////////////////////////////////////////////////////
      ir::node_data<double> exponential_operation::exponential1d(operands_type const&
      ops) const
      {
        using array_type = Eigen::Array<double, Eigen::Dynamic, 1>;
        using matrix_type = Eigen::Matrix<double, Eigen::Dynamic, 1>;

        array_type data = ops[0].matrix();
        matrix_type result =  data.exp();

        return ir::node_data<double>(std::move(result));
      }

      ir::node_data<double> exponential_operation::exponentialxd(operands_type const&
      ops) const
      {
        using matrix_type = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

        matrix_type result =  ops[0].matrix().exp();;

        return ir::node_data<double>(std::move(result));
      }

      hpx::future<ir::node_data<double>> exponential_operation::eval() const
      {
          return hpx::dataflow(hpx::util::unwrapping(
              [this](std::vector<ir::node_data<double>> && ops)
          {
            std::size_t dims = ops[0].num_dimensions();
            switch (dims)
            {
              case 0:
                return exponential0d(ops);

              case 1:
                return exponential1d(ops);

              case 2:
                return exponentialxd(ops);;

              default:
                HPX_THROW_EXCEPTION(hpx::bad_parameter,
                                    "exponential_operation::eval",
                                    "something wrong with the dimentions");
            }
          }),
          detail::map_operands(operands_,
                               [](primitive_argument_type const& val)
                                   ->  hpx::future<ir::node_data<double>>
                               {
                                 primitive const* p = util::get_if<primitive>(&val);
                                 if (p != nullptr)
                                   return p->eval();

                                 HPX_ASSERT(valid(val));
                                 return hpx::make_ready_future(extract_literal_value(val));
                               })
          );
      }

    }}}