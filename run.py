from sys import argv
import os

cmd = "./{} >> {} {}".format(argv[1], argv[2], '{}')

for eps in ['1e-5', '5e-6', '2e-6', '1e-6']:
    for n_theads in range(1, 9):
        os.environ["OMP_NUM_THREADS"] = str(n_theads)
        os.system(cmd.format(eps))