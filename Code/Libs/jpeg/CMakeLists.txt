start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Includes"
		"jconfig.h"
		"../../SDKs/jpeg/jpeglib.h" 
		"../../SDKs/jpeg/jmorecfg.h"
		"../../SDKs/jpeg/jerror.h"
		"../../SDKs/jpeg/jinclude.h" 
		"../../SDKs/jpeg/jpegint.h" 
		"../../SDKs/jpeg/jdct.h"
		"../../SDKs/jpeg/jmemsys.h" 
		"../../SDKs/jpeg/jversion.h" 
		"../../SDKs/jpeg/cdjpeg.h" 
		"../../SDKs/jpeg/cderror.h" 
		"../../SDKs/jpeg/transupp.h"
	SOURCE_GROUP "Sources"
		"../../SDKs/jpeg/jaricom.c" 
		"../../SDKs/jpeg/jcarith.c" 
		"../../SDKs/jpeg/jdarith.c" 
		"../../SDKs/jpeg/jcapimin.c" 
		"../../SDKs/jpeg/jcapistd.c" 
		"../../SDKs/jpeg/jdapimin.c" 
		"../../SDKs/jpeg/jdapistd.c" 
		"../../SDKs/jpeg/jcomapi.c" 
		"../../SDKs/jpeg/jcparam.c" 
		"../../SDKs/jpeg/jctrans.c" 
		"../../SDKs/jpeg/jdtrans.c" 
		"../../SDKs/jpeg/jcinit.c" 
		"../../SDKs/jpeg/jcmaster.c"
		"../../SDKs/jpeg/jcmainct.c" 
		"../../SDKs/jpeg/jcprepct.c" 
		"../../SDKs/jpeg/jccoefct.c" 
		"../../SDKs/jpeg/jccolor.c" 
		"../../SDKs/jpeg/jcsample.c" 
		"../../SDKs/jpeg/jcdctmgr.c" 
		"../../SDKs/jpeg/jfdctint.c" 
		"../../SDKs/jpeg/jfdctfst.c" 
		"../../SDKs/jpeg/jfdctflt.c" 
		"../../SDKs/jpeg/jchuff.c" 
		"../../SDKs/jpeg/jcmarker.c" 
		"../../SDKs/jpeg/jdatadst.c" 
		"../../SDKs/jpeg/jdmaster.c" 
		"../../SDKs/jpeg/jdinput.c" 
		"../../SDKs/jpeg/jdmainct.c" 
		"../../SDKs/jpeg/jdcoefct.c" 
		"../../SDKs/jpeg/jdpostct.c" 
		"../../SDKs/jpeg/jdmarker.c" 
		"../../SDKs/jpeg/jdhuff.c" 
		"../../SDKs/jpeg/jddctmgr.c" 
		"../../SDKs/jpeg/jidctint.c" 
		"../../SDKs/jpeg/jidctfst.c" 
		"../../SDKs/jpeg/jidctflt.c" 
		"../../SDKs/jpeg/jdsample.c" 
		"../../SDKs/jpeg/jdcolor.c" 
		"../../SDKs/jpeg/jdmerge.c" 
		"../../SDKs/jpeg/jquant1.c" 
		"../../SDKs/jpeg/jquant2.c" 
		"../../SDKs/jpeg/jdatasrc.c" 
		"../../SDKs/jpeg/jerror.c" 
		"../../SDKs/jpeg/jmemmgr.c"
		"../../SDKs/jpeg/jutils.c" 
		"../../SDKs/jpeg/jmemnobs.c" # alternatives: jmemansi.c jmemname.c  jmemdos.c jmemmac.c
	SOURCE_GROUP "Doc"
		"../../SDKs/jpeg/README"
		"../../SDKs/jpeg/install.txt"
		"../../SDKs/jpeg/usage.txt"
		"../../SDKs/jpeg/cjpeg.1"
		"../../SDKs/jpeg/djpeg.1"
		"../../SDKs/jpeg/jpegtran.1"
		"../../SDKs/jpeg/rdjpgcom.1"
		"../../SDKs/jpeg/wrjpgcom.1"
		"../../SDKs/jpeg/wizard.txt"
		"../../SDKs/jpeg/libjpeg.txt"
		"../../SDKs/jpeg/structure.txt"
		"../../SDKs/jpeg/coderules.txt"
		"../../SDKs/jpeg/filelist.txt"
		"../../SDKs/jpeg/change.log"
)

end_sources()

CryEngineStaticModule(jpeg SOLUTION_FOLDER "Libs")
target_include_directories(${THIS_PROJECT} INTERFACE "${SDK_DIR}/jpeg" "${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
target_compile_definitions(${THIS_PROJECT} PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_compile_options(${THIS_PROJECT} PRIVATE
	/wd4267 # conversion from 'size_t' to 'long', possible loss of data
)
endif()

if(LINUX)
	target_compile_options(${THIS_PROJECT} PRIVATE
		-Wno-shift-negative-value
		-Wno-main
	)
endif()
