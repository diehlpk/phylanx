//   Copyright (c) 2018 Hartmut Kaiser
//
//   Distributed under the Boost Software License, Version 1.0. (See accompanying
//   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Fixing #244: Can not create a list or a vector of previously defined variables

#include <phylanx/phylanx.hpp>

#include <hpx/hpx_main.hpp>
#include <hpx/include/util.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

///////////////////////////////////////////////////////////////////////////////
std::string const randstr = R"(
    define(call, size, random(size, "uniform"))
    call
)";

std::string const randstr_2d = R"(
    define(call, size, random(list(size, size)))
    call
)";

std::string const bench1 = R"(
    define(run, y, k, block(
        define(x, constant(0.0, k)),
        define(local_y, y),
        define(z, 0),
        for_each(
            lambda(i, store(z, slice(x, slice(local_y, i)) + 1)),
            range(k)
        )
    ))
    run
)";

std::string const bench2 = R"(
    define(run, y, k, block(
        define(x, constant(0.0, k)),
        define(local_y, y),
        for_each(
            lambda(i, block(
                define(idx, slice(local_y, i)),
                store(slice(x, idx), slice(x, idx) + 1)
            )),
            range(k)
        )
    ))
    run
)";

std::string const bench_dot = R"(
    define(run, y, block(
        dot(y, y)
    ))
    run
)";

std::string const bench_inverse = R"(
    define(run, y, block(
        inverse(y)
    ))
    run
)";

std::string const bench_add = R"(
    define(run, y, block(
        y+y
    ))
    run
)";

#define ARRAY_SIZE std::int64_t(100000)

///////////////////////////////////////////////////////////////////////////////
void benchmark_1arg(std::string const& name,
               phylanx::execution_tree::compiler::function_list& snippets,
               std::string const& codestr, std::vector<std::int64_t> const& matrix_sizes)
{
    auto const& rand_code = phylanx::execution_tree::compile(randstr_2d, snippets);
    auto rand = rand_code.run();


    auto const& code = phylanx::execution_tree::compile(codestr, snippets);
    auto bench = code.run();

    std::uint64_t t;
    std::cout<<"\n"<<name<<std::endl;

    for (std::int64_t i : matrix_sizes)
    {
        phylanx::ir::node_data<std::int64_t> matrix_size(i);
        auto input_matrix = rand(matrix_size);

        t = hpx::util::high_resolution_clock::now();
        bench(input_matrix);
        t = hpx::util::high_resolution_clock::now() - t;

        std::cout << i << "       " << (t / 1e6) << " ms.\n";
    }
}



int main(int argc, char* argv[])
{
    std::cout<<"\nnumber of threads: "<<hpx::get_os_thread_count()<<std::endl;

    std::vector<std::int64_t> ARRAY_SIZES = {10, 100, 1000, 10000};

    phylanx::execution_tree::compiler::function_list snippets;
    benchmark_1arg("Dot product: ", snippets, bench_dot, ARRAY_SIZES);
    benchmark_1arg("Inverse: ", snippets, bench_inverse, ARRAY_SIZES);
    benchmark_1arg("Add: ", snippets, bench_add, ARRAY_SIZES);

    return 0;
}

