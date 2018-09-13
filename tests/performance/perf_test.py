#  Copyright (c) 2018 Shahrzad Shirzad
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

import numpy as np
import time


def dot_product(matrix_sizes):
    print("\ndot product: ")
    for i in matrix_sizes:
        y=np.random.rand(i,i)
        start_time=time.time()
        np.dot(y, y)
        end_time=time.time()
        print(str(i) +'       '+str((end_time-start_time)*1000)+' ms.\n')

def inverse(matrix_sizes):
    print("\ninverse: ")
    for i in matrix_sizes:
        y=np.random.rand(i,i)
        start_time=time.time()
        np.linalg.inv(y)
        end_time=time.time()
        print(str(i)+'       '+str((end_time-start_time)*1000)+' ms.\n')

def add(matrix_sizes):
    print("\ninverse: ")
    for i in matrix_sizes:
        y=np.random.rand(i,i)
        start_time=time.time()
        y+y
        end_time=time.time()
        print(str(i)+'       '+str((end_time-start_time)*1000)+' ms.\n')

matrix_sizes=[10, 100, 1000, 10000]
dot_product(matrix_sizes)
inverse(matrix_sizes)
add(matrix_sizes)
