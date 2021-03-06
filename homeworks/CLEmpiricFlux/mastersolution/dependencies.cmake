# Dependencies of mastersolution:

# DIR will be provided by the calling file.

set(SOURCES
  ${DIR}/clempiricflux_main.cc
  ${DIR}/clempiricflux.h
  ${DIR}/clempiricflux.cc
  ${DIR}/solvecauchyproblem.h
  ${DIR}/solvecauchyproblem.cc
  ${DIR}/uniformcubicspline.h
  ${DIR}/uniformcubicspline.cc
)

set(LIBRARIES
  Eigen3::Eigen
)
