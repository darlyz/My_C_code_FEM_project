# My_C_code_FEM_project

An easy persenal FEM project to make test.


It use AztecOO to solve equations, which is a package of Trilinos.

Mpi is not enable when Trilinos is being installed.

It use Gid to make pre-post-treatment, and "./TinyFem" is the interface script.

It has tested 16 million quadrilateral mesh of Poisson equation through Gid files "./mesh/exam1.gid", averge 8gb memory and 50 minutes. But, the resualt file is too big to upload.