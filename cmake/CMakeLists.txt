cmake_minimum_required(VERSION 2.8)



project(cmaketry_project  )


###############################################################################
## file globbing ##############################################################
###############################################################################
# from https://stackoverflow.com/questions/21163188/most-simple-but-complete-cmake-HWorld
# these instructions search the directory tree when cmake is
# invoked and put all files that match the pattern in the variables 
# `sources` and `data`


# generate the lib 

file(GLOB_RECURSE simplelibs  ../../PeripheralDevices/*.cpp  ../../PeripheralDevices/*.h )
# for the moment don't include  MCP4728setaddr.cpp 

#message ( "${CMAKE_CURRENT_SOURCE_DIR}/../../PeripheralDevices/MCP4728//MCP4728setaddr.cpp" )


# message( ${simplelibs})

# check if there is a i2c device  
# to compile for dummy cmake . -DDUMMY=true 
# but once used , make sure you remove it from cache or cmake . -DDUMMY=FALSE 
if ( DUMMY )
message ("compile for dummy") 
set( TARGET_FLAG __DUMMY__) 
message ("forced to compile for dummy") 

else()
	if (UNIX)
		# for Raspberrypi  user has to be member of the i2c group , see Readme.md
		if (EXISTS "/dev/i2c-1")
			set(TARGET_FLAG  __LINUX__)
			message(">>> compile for LINUX i2cdev")
		else() 
       			set(TARGET_FLAG  __DUMMY__ )
			message (">>>> /dev/i2c-1 not found compile for dummy") 
			exit()
		endif()
	else()
		message("UNIX not found build for MBED not supported ") 
		exit()
	endif() 
endif()

list(REMOVE_ITEM simplelibs "${CMAKE_CURRENT_SOURCE_DIR}/../../PeripheralDevices/MCP4728/MCP4728setaddr.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/../../PeripheralDevices/MCP4728/MCP4728setaddr.h")


add_library(tstlib SHARED ${simplelibs})
target_include_directories(tstlib PUBLIC  ../../PeripheralDevices/DevInterfaces/)
#set_target_properties(tstlib PROPERTIES LINKER_LANGUAGE CXX)
#set_target_properties(tstlib PROPERTIES COMPILER_LANGUAGE CXX)

# you can use set(sources src/main.cpp) etc if you don't want to
# use globing to find files automatically

# collect all main programs 
file(GLOB_RECURSE ALLPROGS  ../*/*.cpp)


## build the include directories , 

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list ../../PeripheralDevices/*/*h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# macro source https://stackoverflow.com/questions/53013529/how-to-tell-cmake-to-recursively-search-include-directory-when-specificying-incl

#get the list 

HEADER_DIRECTORIES ( dev_incl_dirs)

##############################################################################
## target definitions #########################################################
###############################################################################


foreach ( testprog ${ALLPROGS}) 
	#string( REPLACE ".cpp" "" testname ${testprog} )
	get_filename_component(testname  ${testprog} NAME_WE )
	add_executable( ${testname}  ${testprog}  )
	# just add some compiler flags
	#target_compile_options(${testname}  PUBLIC -std=c++1y -Wall -D${TARGET_FLAG})
	target_compile_options(${testname}  PUBLIC -Wall -D${TARGET_FLAG})
	# this lets me include files relative to the root src dir with a <> pair
	#target_include_directories(${testname} PUBLIC  ../../PeripheralDevices/*/)
	target_include_directories(${testname} PUBLIC  ${dev_incl_dirs}  )
	target_link_libraries( ${testname} tstlib )
 	#install(TARGETS  ${testname}  DESTINATION ../BUILD/${testname} )
endforeach ( testprog ${allprogs}) 
###############################################################################
## testing ####################################################################
###############################################################################



