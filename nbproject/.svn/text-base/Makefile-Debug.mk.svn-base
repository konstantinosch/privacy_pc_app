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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
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
	${TESTDIR}/TestFiles/security \
        build/security

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
LDLIBSOPTIONS=-L/usr/lib -L/usr/local/lib -lgmp -lpbc -pthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/libUTLibrary.so

dist/Debug/GNU-Linux-x86/libUTLibrary.so: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.c} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libUTLibrary.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/source/common.o: src/source/common.c 
	${MKDIR} -p ${OBJECTDIR}/src/source
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/common.o src/source/common.c

${OBJECTDIR}/src/source/utma_weak.o: src/source/utma_weak.c 
	${MKDIR} -p ${OBJECTDIR}/src/source
	${RM} $@.d
	$(COMPILE.c) -g -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/utma_weak.o src/source/utma_weak.c

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/weak: ${TESTDIR}/tests/utmaweaktest.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/weak $^ ${LDLIBSOPTIONS} -lgmp -lpbc 

${TESTDIR}/TestFiles/ca: ${TESTDIR}/tests/ca.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/ca $^ ${LDLIBSOPTIONS} -lgmp -lpbc

${TESTDIR}/TestFiles/target: ${TESTDIR}/tests/target.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/target $^ ${LDLIBSOPTIONS} -lgmp -lpbc 

${TESTDIR}/TestFiles/helper: ${TESTDIR}/tests/helper.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/helper $^ ${LDLIBSOPTIONS} -lgmp -lpbc

${TESTDIR}/TestFiles/security: ${TESTDIR}/tests/security.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/security $^ ${LDLIBSOPTIONS} -lgmp -lpbc

${TESTDIR}/TestFiles/genapar: ${TESTDIR}/tests/genapar.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.c} -o ${TESTDIR}/TestFiles/genapar $^ ${LDLIBSOPTIONS} -lgmp -lpbc

build/security: ${TESTDIR}/TestFiles/security
	cp ${TESTDIR}/TestFiles/security build/security

${TESTDIR}/tests/utmaweaktest.o: tests/utmaweaktest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/utmaweaktest.o tests/utmaweaktest.c

${TESTDIR}/tests/ca.o: tests/utmaweaktest.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/ca.o tests/ca.c

${TESTDIR}/tests/target.o: tests/target.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/target.o tests/target.c

${TESTDIR}/tests/helper.o: tests/helper.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/helper.o tests/helper.c

${TESTDIR}/tests/security.o: privacy/security.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/security.o privacy/security.c

${TESTDIR}/tests/genapar.o: tests/genapar.c 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.c) -g -I. -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -MMD -MP -MF $@.d -o ${TESTDIR}/tests/genapar.o tests/genapar.c


${OBJECTDIR}/src/source/common_nomain.o: ${OBJECTDIR}/src/source/common.o src/source/common.c 
	${MKDIR} -p ${OBJECTDIR}/src/source
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/source/common.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.c) -g -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/common_nomain.o src/source/common.c;\
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
	    $(COMPILE.c) -g -I/usr/local/include/pbc/ -Isrc/header -I. -I. -I. -fPIC  -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/source/utma_weak_nomain.o src/source/utma_weak.c;\
	else  \
	    ${CP} ${OBJECTDIR}/src/source/utma_weak.o ${OBJECTDIR}/src/source/utma_weak_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/weak || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} build/security
	${RM} dist/Debug/GNU-Linux-x86/libUTLibrary.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
