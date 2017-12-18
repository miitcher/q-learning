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

  * `build/` -- Here the project is built.


## Build-instructions: Compiling the project and tests ##
Cmake creates the Makefiles, and make uses the Makefiles to build the project.

* Install required packages if it's not installed already
`sudo apt install cmake`

* Move to the build folder in the q-learning-9 folder.
`cd q-learning-9/build/`

* Create Makefile.
`cmake ..`

Build targets can be listed, built together, built separately and removed:

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

The executable compiled targets will be in the build folder as:
`main`
`qtests`


### Guides that may help if packages are missing ###
https://superuser.com/questions/1099227/installing-opengl-in-linux
https://stackoverflow.com/questions/5299989/x11-xlib-h-not-found-in-ubuntu
