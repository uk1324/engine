# https://stackoverflow.com/questions/4222326/cmake-compiling-generated-files/8748478#8748478
function(targetAddGenerated targetName directoryToScan)
	set(GENERATED_FILES)
	file(GLOB_RECURSE DATA_FILES "${directoryToScan}/*.data")

	foreach(DATA_FILE ${DATA_FILES})
		get_filename_component(DATA_FILE_NAME ${DATA_FILE} NAME_WE)
		get_filename_component(DATA_FILE_DIRECTORY ${DATA_FILE} DIRECTORY)
		set(OUT_FILE "${GENERATED_PATH}/${DATA_FILE_NAME}DataGenerated.cpp")
		list(APPEND GENERATED_FILES ${OUT_FILE})
		set(OUT_HPP, "${DATA_FILE_DIRECTORY}/${DATA_FILE_NAME}Data.hpp")
		add_custom_command(
			PRE_BUILD 
			OUTPUT ${OUT_FILE} ${OUT_HPP}
			
			COMMAND java -Dfile.encoding=UTF-8 -classpath "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/out/production/codeGenTool;${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/ST-4.3.4.jar;${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib/antlr-4.13.0-complete.jar" Main ${EXECUTABLE_WORKING_DIRECTORY} ${GENERATED_PATH} ${DATA_FILE}
			VERBATIM
			WORKING_DIRECTORY ${CODEGEN_TOOL_WORKING_DIRECTORY}
			DEPENDS ${DATA_FILE}
			MAIN_DEPENDENCY ${DATA_FILE}
		)
		add_custom_target(
			"sharedTarget${DATA_FILE_NAME}"
			DEPENDS ${OUT_FILE} ${OUT_HPP}
		)
		add_dependencies(${targetName} "sharedTarget${DATA_FILE_NAME}")
	endforeach(DATA_FILE)
	target_sources(${targetName} PUBLIC ${GENERATED_FILES})
endfunction()

# # This function only adds the dependencies. You cannot create the targets twice. The reason this needs to exsit is because for example
# # game links with gfx2d 
# # a .data file in game includes Vertex2dPt
# #
# # game won't compile, because Vertex2dPtData.hpp wan't generated yet, because gfx2d wasn't generated yet.
# # using this function would fix it, because it would make game depend directly on Vertex2dPt
# function(targetAddGeneratedDependencies targetName directoryToScan) 
# 	set(GENERATED_FILES)
# 	file(GLOB_RECURSE DATA_FILES "${directoryToScan}/*.data")
# 
# 	foreach(DATA_FILE ${DATA_FILES})
# 		get_filename_component(DATA_FILE_NAME ${DATA_FILE} NAME_WE)
# 		add_dependencies(${targetName} "sharedTarget${DATA_FILE_NAME}")
# 	endforeach(DATA_FILE)
# 	target_sources(${targetName} PUBLIC ${GENERATED_FILES})
# endfunction()
