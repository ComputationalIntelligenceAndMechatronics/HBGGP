cmake_minimum_required(VERSION 2.4)
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project (HBGGP)

set( CMAKE_VERBOSE_MAKEFILE ON )
set(CMAKE_BUILD_TYPE "Debug")


##########################################
# Some system dependent install variables
#########################################

## Need to be change to more generic install using Find.cmake

if( CMAKE_SYSTEM_NAME STREQUAL Darwin )
	set( PROJECTSROOT $ENV{HOME}/hbggp_programming CACHE PATH "Path to the root of the sources for all subproject" )
	set( MATLABSYSTEM maci )
	set( MATLAB /Applications/MATLAB74 CACHE PATH "Matlab root directory" )
else ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
	set( PROJECTSROOT $ENV{HOME}/library CACHE PATH "Path to the root of the sources for all subproject" )
	set( MATLAB /appl/gmatlab/matlab77  CACHE PATH "Matlab root directory" )
	if( CMAKE_SYSTEM_NAME STREQUAL SunOS )
		add_definitions( -xarch=sol64 )
		set( MATLABSYSTEM sol64 )
	else( CMAKE_SYSTEM_NAME STREQUAL SunOS )
		set( MATLABSYSTEM glnxa64 )
	endif( CMAKE_SYSTEM_NAME STREQUAL SunOS )
endif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
#######################




option( WITHOUT_GRAPHVIZ "Build the project without graphviz" ON )
option( USE_RKF "Build the project using Runge-Kutta Fehlberg integration methods" ON )
option( USE_RKFVS "Build the project using Runge-Kutta Fehlberg with variable time step integration methods" OFF )
option( USE_MPI "Build the project using distributed fitness evaluation using MPI" OFF )
option( USE_SYMBOLS "Build the project using FSA processing symbols" OFF )
option( USE_GSL "Build the project using GNU Scientific Library" ON )
option( ALLOW_DIFFCAUSALITY "Allow differential causality during evaluation of the bond graph" OFF )
option( USE_JUNCTIONPAIR "Build the project for using junction pair" OFF )
option( INSERT_RESISTANCE_WITH_SWITCH "Insert a resistance at the same junction of a newly added switch" OFF )


if( INSERT_RESISTANCE_WITH_SWITCH )
    add_definitions(-DINSERT_RESISTANCE_WITH_SWITCH)
endif( INSERT_RESISTANCE_WITH_SWITCH )

if( USE_JUNCTIONPAIR )
    add_definitions(-DUSE_JUNCTIONPAIR)
endif( USE_JUNCTIONPAIR )

if( ALLOW_DIFFCAUSALITY )
	add_definitions(-DALLOW_DIFFCAUSALITY)
endif( ALLOW_DIFFCAUSALITY )

if( USE_SYMBOLS )
	add_definitions(-DUSE_SYMBOL)
endif( USE_SYMBOLS )

if( WITHOUT_GRAPHVIZ )
	add_definitions(-DWITHOUT_GRAPHVIZ)
endif( WITHOUT_GRAPHVIZ )

if( USE_GSL )
	add_definitions(-DHAVE_GSL)
endif( USE_GSL )




set( MPIBEAGLE_INCLUDE_DIR ${PROJECTSROOT}/MPIBeagle/Source CACHE PATH "MPIBeagle headers" )
set( MPIBEAGLE_LIB_DIR ${PROJECTSROOT}/MPIBeagle/build CACHE PATH "MPIBeagle build"  )

set( BONDGRAPH_INCLUDE_DIR ${PROJECTSROOT}/libBondGraph/Source/libBondGraph CACHE PATH "libBondGraph headers" )
set( BONDGRAPH_LIB_DIR ${PROJECTSROOT}/libBondGraph/build/Source/libBondGraph CACHE PATH "libBondGraph build"  )

set(PACC_DIR "$ENV{HOME}/local")
set(PACC_INCLUDE_DIR "${PACC_DIR}/include/PACC")
set(PACC_LIB_DIR "${PACC_DIR}/lib")

set( SIMULATION_INCLUDE_DIR ${PROJECTSROOT}/libBondGraph/Source/Simulation CACHE PATH "Simulation headers" )


if( USE_RKF )
	add_definitions(-DUSE_RKF)
endif( USE_RKF )

if( USE_RKFVS )
    add_definitions(-DUSE_RKFVS)
endif( USE_RKFVS )


if( CMAKE_BUILD_TYPE STREQUAL Debug )
	add_definitions(-DBEAGLE_FULL_DEBUG)
endif( CMAKE_BUILD_TYPE STREQUAL Debug )

if( USE_MPI )
	include(FindMPI)
	if( NOT MPI_FOUND )
		set( MPI_DIR /usr/mpi/gcc/openmpi-1.3.2 CACHE FILEPATH "MPI directory" )
		set( MPI_COMPILER ${MPI_DIR}/bin/mpic++ CACHE FILEPATH "MPI compiler" )
		set( CC ${MPI_COMPILER} )
		set( MPI_INCLUDE_PATH ${MPI_DIR}/include )
		set( MPI_LINK_FLAGS ${MPI_DIR}/lib64 )
		set( MPI_LIB_DIR ${MPI_DIR}/lib64 )
		set( MPI_LIBRARIES mpi mpi_cxx)
	endif( NOT MPI_FOUND )
	
	include_directories( 
		${MPI_INCLUDE_PATH} 
		${PACC_INCLUDE_DIR} 
		$ENV{HOME}/local/include 
		/sw/include 
		${BONDGRAPH_INCLUDE_DIR}  
		${SIMULATION_INCLUDE_DIR} 
		#${MATLAB}/extern/include 
		${CMAKE_CURRENT_SOURCE_DIR} 
		${CMAKE_CURRENT_BINARY_DIR}
		${MPIBEAGLE_INCLUDE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/Source
		${CMAKE_CURRENT_SOURCE_DIR}/Source/TwoTanks
		${CMAKE_CURRENT_SOURCE_DIR}/Source/DCDCBoost
	)
	link_directories( 
		${CMAKE_CURRENT_BINARY_DIR} 
		${MPIBEAGLE_LIB_DIR} 
		${PACC_LIB_DIR} 
		${MPI_LIB_DIR}
		/opt/SUNWhpc/lib 
		$ENV{HOME}/local/lib 
		/sw/lib 
		${BONDGRAPH_LIB_DIR}  
		${SIMULATIONBUILD} 
		#${MATLAB}/extern/lib/${MATLABSYSTEM} 
		#${MATLAB}/bin/${MATLABSYSTEM}
	)

	set( CMAKE_CXX_FLAGS -DUSE_MPI )
	set( ThreeTanks_LIBS  pacc BondGraph openbeagle-MPI openbeagle-GA openbeagle-GP openbeagle pthread gsl gslcblas z ${MPI_LIBRARIES})
	set( DCDCBoost_LIBS   pacc BondGraph openbeagle-MPI openbeagle-GA openbeagle-GP openbeagle pthread gsl gslcblas z ${MPI_LIBRARIES})
	set( ANALOGFILTER_LIBS pacc BondGraph openbeagle-MPI openbeagle-GA openbeagle-GP openbeagle z pthread mwmclmcrrt AnalogFilterDesignEval ${MPI_LIBRARIES} )
	
else( USE_MPI )
	include_directories( 
		$ENV{HOME}/local/include 
		/sw/include 
		${BONDGRAPH_INCLUDE_DIR} 
		${PACC_INCLUDE_DIR} 
		${SIMULATION_INCLUDE_DIR} 
		#${MATLAB}/extern/include
		${CMAKE_CURRENT_SOURCE_DIR}/Source
		${CMAKE_CURRENT_SOURCE_DIR}/Source/TwoTanks
		${CMAKE_CURRENT_SOURCE_DIR}/Source/DCDCBoost
		${CMAKE_CURRENT_BINARY_DIR}
	)
	
	link_directories( 
		${CMAKE_CURRENT_BINARY_DIR}  
		$ENV{HOME}/local/lib 
		${BONDGRAPH_LIB_DIR}
		/sw/lib 
		/opt/SUNWhpc/lib
		#${MATLAB}/extern/lib/${MATLABSYSTEM}
		#${MATLAB}/bin/${MATLABSYSTEM}
	)


	if( WITHOUT_GRAPHVIZ )
		set( ThreeTanks_LIBS  pacc BondGraph openbeagle-GA openbeagle-GP openbeagle pthread gsl gslcblas z)
		set( DCDCBoost_LIBS   pacc BondGraph openbeagle-GA openbeagle-GP openbeagle pthread gsl gslcblas z)
		set( ANALOGFILTER_LIBS  pacc BondGraph openbeagle-GP openbeagle-GA openbeagle mwmclmcrrt z AnalogFilterDesignEval )
	else( WITHOUT_GRAPHVIZ )
		set( ThreeTanks_LIBS  pacc BondGraph openbeagle-GA openbeagle-GP openbeagle graph gvc pthread gsl gslcblas z)
		set( DCDCBoost_LIBS  pacc BondGraph openbeagle-GA openbeagle-GP openbeagle graph gvc pthread gsl gslcblas z)
		set( ANALOGFILTER_LIBS pacc BondGraph openbeagle-GP openbeagle-GA openbeagle gvc graph z mwmclmcrrt AnalogFilterDesignEval)
	endif( WITHOUT_GRAPHVIZ )
endif( USE_MPI )
    

file(GLOB BONDGRAPH_SRCS		${BONDGRAPH_INCLUDE_DIR}/*.cpp)
file(GLOB SIMULATION_SRCS		${SIMULATION_INCLUDE_DIR}/*.cpp)

set ( BGGP_SRCS
	Source/LookaheadController.cpp
	Source/SimulationCase.cpp
	Source/ParametersHolder.cpp
	Source/stringcompression.cpp
	Source/stringutil.cpp
	Source/VectorUtil.cpp
	Source/LogFitness.cpp
	Source/TreeSTag.cpp
	Source/RootReturn.cpp
	Source/NOP.cpp
	Source/EmbryoPrimitive.cpp
	Source/BGPrimitive.cpp
	Source/ReplaceComponent.cpp
	Source/ReplaceSwitch.cpp
	Source/RemoveComponent.cpp
	Source/AddComponent.cpp
	Source/InsertJunction.cpp
	Source/InsertJunctionPair.cpp
	Source/BGContext.cpp
	Source/BondGraphEvalOp.cpp
	Source/BGFitness.cpp
	Source/GrowingBondGraph.cpp
	Source/GrowingHybridBondGraph.cpp
	Source/GrowingBG.cpp
	Source/AddSwitch.cpp
	Source/AddSwitchE.cpp
	Source/FlipBond.cpp
	Source/SplitBond.cpp
	Source/SplitBondSame.cpp
	Source/CrossoverSelectiveConstrainedOp.cpp
	Source/MutationShrinkSelectiveConstrainedOp.cpp
	Source/MutationSwapSelectiveConstrainedOp.cpp
	Source/MutationStandardSelectiveConstrainedOp.cpp
	Source/SelectiveConstrainedSelectionOp.cpp
	Source/BGSpeciesHolder.cpp
	Source/BGSpeciationOp.cpp
	Source/StatsCalcStructuralFitnessOp.cpp
	Source/StructuralHierarchicalFairCompetitionOp.cpp
	Source/BGSpeciationVerificationOp.cpp
	Source/DepthDependentSelectionOp.cpp
	Source/CrossoverDepthSelectiveConstrainedOp.cpp
	Source/MutationShrinkDepthSelectiveConstrainedOp.cpp
	Source/MutationSwapDepthSelectiveConstrainedOp.cpp
	Source/MutationStandardDepthSelectiveConstrainedOp.cpp
	Source/LogIndividualOp.cpp
	Source/LogIndividualDataOp.cpp
)

set ( DCDCBoost_SRCS
	Source/DCDCBoost/DCDCBoostMain.cpp
	Source/DCDCBoost/DCDCBoostEvalOp.cpp
	Source/DCDCBoost/DCDCBoostEmbryo.cpp
	Source/DCDCBoost/DCDCBoostLookaheadController.cpp
)

set( DCDCBoost2xGA_SRCS
	Source/stringutil.cpp
	Source/VectorUtil.cpp
	Source/stringcompression.cpp
	Source/SimulationCase.cpp
	Source/LogFitness.cpp
	Source/DCDCBoost/DCDCBoost2xGA.cpp
	Source/DCDCBoost/DCDCBoost2xGAEvalOp.cpp
	Source/DCDCBoost/DCDCBoost2xGAController.cpp
)

# set( DCDCBoostGA_SRCS
# 	Source/stringutil.cpp
# 	Source/stringcompression.cpp
# 	Source/SimulationCase.cpp
# 	Source/LogFitness.cpp
# 	Source/DCDCBoost/DCDCBoostGA.cpp
# 	Source/DCDCBoost/DCDCBoostGAEvalOp.cpp
# 	Source/DCDCBoost/DCDCBoostGAController.cpp
# )

 set ( ThreeTanks_SRCS
 	Source/ThreeTanks/ThreeTanksMain.cpp
 	Source/ThreeTanks/ThreeTanksEvalOp.cpp
 	Source/ThreeTanks/ThreeTanksEmbryo.cpp
 	Source/ThreeTanks/ThreeTanksEmbryo2.cpp
	Source/ThreeTanks/ThreeTanksLookaheadController.cpp
)

# set( ANALOGFILTER_SRCS
# 	Source/AnalogFilter/AnalogFilterEmbryo.cpp
# 	Source/AnalogFilter/AnalogFilterEvalOp.cpp
# 	Source/AnalogFilter/AnalogFilter.cpp
# 	Source/BGContext.cpp
# 	Source/BGPrimitive.cpp
# 	Source/ReplaceComponent.cpp
# 	Source/AddComponent.cpp
# 	Source/InsertJunction.cpp
# 	Source/InsertJunctionPair.cpp
# 	Source/EmbryoPrimitive.cpp
# 	Source/RootReturn.cpp
# 	Source/LogFitness.cpp
# 	Source/BGFitness.cpp
# 	Source/BondGraphEvalOp.cpp
# 	Source/GrowingBG.cpp
# 	Source/GrowingBondGraph.cpp
# 	Source/GrowingHybridBondGraph.cpp
# 	Source/MatrixInterface.cpp
# 	Source/stringcompression.cpp
# 	Source/TreeSTag.cpp
# 	Source/MatlabInitializationOp.cpp
# )
# 
# set( ANALOGFILTER_MFILES
# 	Source/AnalogFilter/AnalogFilterEval.m
# )

if( CMAKE_SYSTEM_NAME STREQUAL Darwin )
	set( ANALOGFILTERDESIGN_LIB libAnalogFilterDesignEval.dylib )
else ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
	set( ANALOGFILTERDESIGN_LIB libAnalogFilterDesignEval.so )
endif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )

#add_custom_command( OUTPUT ${ANALOGFILTERDESIGN_LIB} libAnalogFilterDesignEval.h COMMAND ${MATLAB}/bin/mcc -v -W cpplib:libAnalogFilterDesignEval -T link:lib ${CMAKE_CURRENT_SOURCE_DIR}/${ANALOGFILTER_MFILES} DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${ANALOGFILTER_MFILES} )
#add_executable (AnalogFilter ${ANALOGFILTER_SRCS} ${ANALOGFILTERDESIGN_LIB})
#target_link_libraries(AnalogFilter ${ANALOGFILTER_LIBS})

add_executable (ThreeTanks ${ThreeTanks_SRCS} ${BGGP_SRCS} )
target_link_libraries(ThreeTanks ${ThreeTanks_LIBS})

add_executable (DCDCBoost ${DCDCBoost_SRCS} ${BGGP_SRCS} )
target_link_libraries(DCDCBoost ${DCDCBoost_LIBS})

#add_executable (DCDCBoostGA ${DCDCBoostGA_SRCS} ${BONDGRAPH_SRCS} ${SIMULATION_SRCS} )
#target_link_libraries(DCDCBoostGA ${DCDCBOOST_LIBS})

#add_executable (DCDCBoost2xGA ${DCDCBoost2xGA_SRCS} )
#target_link_libraries(DCDCBoost2xGA ${DCDCBoost_LIBS})


