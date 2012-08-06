#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/source/common.o \
	${OBJECTDIR}/src/source/utma_weak.o

# Test Directory
TESTDIR=build/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk build/Release/GNU-MacOSX/tests/TestFiles/f2

build/Release/GNU-MacOSX/tests/TestFiles/f2: ${OBJECTFILES}
	${MKDIR} -p build/Release/GNU-MacOSX/tests/TestFiles
	${LINK.c} -dynamiclib -install_name f2 -o ${TESTDIR}/TestFiles/f2 -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/source/common.o: src/source/common.c 
	${MKDIR} -p ${OBJECTDIR}/src/source
	${RM} $@.d
	$(COMPILE.c) -O2 -I. -I. -I. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/common.o src/source/common.c

${OBJECTDIR}/src/source/utma_weak.o: src/source/utma_weak.c 
	${MKDIR} -p ${OBJECTDIR}/src/source
	${RM} $@.d
	$(COMPILE.c) -O2 -I. -I. -I. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/utma_weak.o src/source/utma_weak.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/utmaweaktest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} 


${TESTDIR}/tests/utmaweaktest.o: tests/utmaweaktest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -O2 -I. -I. -I. -I. -I. -I. -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/utmaweaktest.o tests/utmaweaktest.c


${OBJECTDIR}/src/source/common_nomain.o: ${OBJECTDIR}/src/source/common.o src/source/common.c 
	${MKDIR} -p ${OBJECTDIR}/src/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/source/common.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -I. -I. -I. -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/common_nomain.o src/source/common.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/source/common.o ${OBJECTDIR}/src/source/common_nomain.o;\
	fi

${OBJECTDIR}/src/source/utma_weak_nomain.o: ${OBJECTDIR}/src/source/utma_weak.o src/source/utma_weak.c 
	${MKDIR} -p ${OBJECTDIR}/src/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/source/utma_weak.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -O2 -I. -I. -I. -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/utma_weak_nomain.o src/source/utma_weak.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/source/utma_weak.o ${OBJECTDIR}/src/source/utma_weak_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} build/Release/GNU-MacOSX/tests/TestFiles/f2

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
