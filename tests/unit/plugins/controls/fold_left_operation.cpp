// Copyright (c) 2018 Hartmut Kaiser
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <phylanx/phylanx.hpp>

#include <hpx/hpx_main.hpp>
#include <hpx/util/lightweight_test.hpp>

#include <cstdint>
#include <string>

///////////////////////////////////////////////////////////////////////////////
phylanx::execution_tree::primitive_argument_type compile_and_run(
    std::string const& codestr)
{
    phylanx::execution_tree::compiler::function_list snippets;
    phylanx::execution_tree::compiler::environment env =
        phylanx::execution_tree::compiler::default_environment();

    auto const& code = phylanx::execution_tree::compile(codestr, snippets, env);
    return code.run();
}

///////////////////////////////////////////////////////////////////////////////
void test_fold_left_operation_lambda()
{
    std::string const code = R"(
            fold_left(lambda(x, y, x + y), 0, list(1, 2, 3, 4))
        )";

    auto result =
        phylanx::execution_tree::extract_numeric_value(compile_and_run(code));

    HPX_TEST_EQ(result[0], 10.0);
}

/// <image url="$(ItemDir)/images/test_fold_left_operation_builtin.dot.png" />
//
void test_fold_left_operation_builtin()
{
    std::string const code = R"(
            fold_left(__add, 0, '(1, 2, 3, 4))
        )";

    auto result =
        phylanx::execution_tree::extract_numeric_value(compile_and_run(code));

    HPX_TEST_EQ(result[0], 10.0);
}

void test_fold_left_operation_func()
{
    std::string const code = R"(block(
            define(f, x, y, x + y),
            fold_left(f, 0, list(1, 2, 3, 4))
        ))";

    auto result =
        phylanx::execution_tree::extract_numeric_value(compile_and_run(code));

    HPX_TEST_EQ(result[0], 10.0);
}

void test_fold_left_operation_func_lambda()
{
    std::string const code = R"(block(
            define(f, lambda(x, y, x + y)),
            fold_left(f, 0, list(1, 2, 3, 4))
        ))";

    auto result =
        phylanx::execution_tree::extract_numeric_value(compile_and_run(code));

    HPX_TEST_EQ(result[0], 10.0);
}

void test_fold_left_operation_lambda_list()
{
    std::string const code = R"(
            fold_left(lambda(x, y, make_list(x, y)), list(), list(1, 2, 3, 4))
        )";

    auto result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(compile_and_run(code))};

    std::string const expected_str = R"(
            list(list(list(list(list(), 1), 2), 3), 4)
        )";

    auto expected_result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(
            compile_and_run(expected_str))};

    HPX_TEST_EQ(result, expected_result);
}

void test_fold_left_operation_builtin_list()
{
    std::string const code = R"(
            fold_left(make_list, list(), list(1, 2, 3, 4))
        )";

    auto result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(compile_and_run(code))};

    std::string const expected_str = R"(
            list(list(list(list(list(), 1), 2), 3), 4)
        )";

    auto expected_result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(
            compile_and_run(expected_str))};

    HPX_TEST_EQ(result, expected_result);
}

void test_fold_left_operation_func_list()
{
    std::string const code = R"(block(
            define(f, x, y, make_list(x, y)),
            fold_left(f, list(), list(1, 2, 3, 4))
        ))";

    auto result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(compile_and_run(code))};

    std::string const expected_str = R"(
            list(list(list(list(list(), 1), 2), 3), 4)
        )";

    auto expected_result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(
            compile_and_run(expected_str))};

    HPX_TEST_EQ(result, expected_result);
}

void test_fold_left_operation_func_lambda_list()
{
    std::string const code = R"(block(
            define(f, lambda(x, y, make_list(x, y))),
            fold_left(f, list(), list(1, 2, 3, 4))
        ))";

    auto result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(compile_and_run(code))};

    std::string const expected_str = R"(
            list(list(list(list(list(), 1), 2), 3), 4)
        )";

    auto expected_result = phylanx::execution_tree::primitive_argument_type{
        phylanx::execution_tree::extract_list_value(
            compile_and_run(expected_str))};

    HPX_TEST_EQ(result, expected_result);
}

void test_fold_left_list_length()
{
    std::string const code = R"(block(
            define(list_length, list,
                fold_left(lambda(sum, element, sum + 1), 0, list)),
            list_length(list(1, 2, 3, 4))
        ))";

    HPX_TEST_EQ(phylanx::execution_tree::extract_scalar_integer_value(
                    compile_and_run(code)), 4);
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    test_fold_left_operation_lambda();
    test_fold_left_operation_builtin();
    test_fold_left_operation_func();
    test_fold_left_operation_func_lambda();

    test_fold_left_operation_lambda_list();
    test_fold_left_operation_builtin_list();
    test_fold_left_operation_func_list();
    test_fold_left_operation_func_lambda_list();

    test_fold_left_list_length();

    return hpx::util::report_errors();
}

