# q-learning-9 - C++ programming project #

Directory structure:

  * `plan/` -- Here is your project plan.

  * `doc/` -- here are the documentation sources related to your project.
    You may use different tools for documentation (e.g. latex),
    but we hope to get the final version of the document
    **also as pdf file**.

  * `src/` -- Here are the C++ source files.

  * `lib/` -- Here are the libraries.

  * `test/` -- Here are the tests.


## Compiling the project and its tests ##

* Create build folder in the q-learning-9 folder.
`mkdir build_Makefile`

* Move to build folder.

* Create Makefile.
`cmake ..`

* List targets that can be built.
`make help`

* Compile everything.
`make` or `make all`

* Compile main.
`make main`

* Compile tests.
`make qtests`

* Remove compiled targets.
`make clean`

* Executable compiled targets will be in the build folder.
