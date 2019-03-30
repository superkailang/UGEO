cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()
project(UNGEO)  
  
#find_package(VTK COMPONENTS 
#  vtkCommonCore   
#  vtkFiltersCore  
# vtkInfovisCore
#  vtkInteractionStyle 
#  vtkViewsQt     
#)  
find_package(VTK REQUIRED)   
include(${VTK_USE_FILE}) 

if("${VTK_QT_VERSION}" STREQUAL "")
  message(FATAL_ERROR "VTK was not built with Qt")
endif() 

# Use the include path and library for Qt that is used by VTK.
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set your files and resources here
set( Srcs_ModelSetup 
	./UGEO/main.cpp
	./UGEO/cellpara3dshow.cpp
	./UGEO/commands.cpp
	./UGEO/fileReader.cpp
	./UGEO/mywidget.cpp
	./UGEO/ProjectFile.cpp
	./UGEO/BaseFunction.cpp
	./UGEO/GridProcess.cpp
	./UGEO/IJKGrid.cpp
	./UGEO/Tree.cpp
	./UGEO/GridPlot.cpp
	./UGEO/SerachDialog.cpp
	./UGEO/PropertySet.cpp
	) 	
set( Hdrs_ModelSetup 
	./UGEO/cellpara3dshow.h
	./UGEO/commands.h
	./UGEO/Constants.h
	./UGEO/fileReader.h
	./UGEO/mywidget.h
	./UGEO/ProjectFile.h
	./UGEO/Struct.h
	./UGEO/BaseFunction.h
	./UGEO/GridProcess.h
	./UGEO/IJKGrid.h
	./UGEO/Tree.h
	./UGEO/GridPlot.h
	./UGEO/Gridstruct.h
	./UGEO/SerachDialog.h
	./UGEO/PropertySet.h
	./UGEO/EnumDef.h
	)	
set( MOC_Hdrs_ModelSetup 
	./UGEO/cellpara3dshow.h
	./UGEO/commands.h
	./UGEO/Constants.h
	./UGEO/fileReader.h
	./UGEO/mywidget.h
	./UGEO/ProjectFile.h
	./UGEO/Struct.h
	./UGEO/BaseFunction.h
	./UGEO/GridProcess.h
	./UGEO/IJKGrid.h
	./UGEO/Tree.h
	./UGEO/GridPlot.h
	./UGEO/Gridstruct.h
	./UGEO/SerachDialog.h
	./UGEO/PropertySet.h
	./UGEO/EnumDef.h
	)
	
set( UIs_ModelSetup 
	./UGEO/cellpara3dshow.ui
	)
	

set( QRCs_ModelSetup 	 ./UGEO/Icons/Grid3DToolIcon.qrc )

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# The rest should just work (sure...)
if(VTK_QT_VERSION VERSION_GREATER "4")
  # We have ui files, this will bring in the macro: qt5_wrap_ui
  find_package(Qt5 COMPONENTS Widgets REQUIRED QUIET)
  qt5_wrap_ui(UI_Srcs ${UIs_ModelSetup})
  qt5_add_resources(QRC_Srcs ${QRCs_ModelSetup})
  
  source_group("UGEO\\Header Files" FILES
		${Hdrs_ModelSetup}
  )
  source_group("UGEO\\Resource" FILES
		${UIs_ModelSetup}
		${QRCs_ModelSetup}
  )
  source_group("UGEO\\Source Files" FILES
		${Srcs_ModelSetup}
  ) 

  add_executable(UNGEO MACOSX_BUNDLE
    ${Srcs_ModelSetup} ${Hdrs_ModelSetup} 
	${UI_Srcs} ${MOC_Hdrs_ModelSetup} ${QRC_Srcs})
  qt5_use_modules(UNGEO Core Gui Widgets)
  target_link_libraries(UNGEO ${VTK_LIBRARIES})
  
 else()
 
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
  # Use what VTK built with
  set(QT_QMAKE_EXECUTABLE ${VTK_QT_QMAKE_EXECUTABLE} CACHE FILEPATH "")
  set(QT_MOC_EXECUTABLE ${VTK_QT_MOC_EXECUTABLE} CACHE FILEPATH "")
  set(QT_UIC_EXECUTABLE ${VTK_QT_UIC_EXECUTABLE} CACHE FILEPATH "")
  
  qt4_wrap_ui(UI_Srcs ${UIs_ModelSetup} )
  qt4_wrap_cpp(MOC_Srcs ${MOC_Hdrs_ModelSetup}  )
  qt4_add_resources(QRC_Srcs ${QRCs_ModelSetup})

  source_group("UGEO\\Header Files" FILES
		${Hdrs_ModelSetup}
  )
  source_group("UGEO\\Resource" FILES
		${UIs_ModelSetup}
		${QRCs_ModelSetup}
  )
  source_group("UGEO\\Source Files" FILES
		${Srcs_ModelSetup}
  )

#  add_definitions(-DQT_GUI_LIBS -DQT_CORE_LIB -DQT3_SUPPORT)

  add_executable(UNGEO MACOSX_BUNDLE
    ${Srcs_ModelSetup}  ${Hdrs_ModelSetup} ${UI_Srcs} ${MOC_Hdrs_ModelSetup} ${QRC_Srcs})
  target_link_libraries(UNGEO
    ${QT_LIBRARIES}
    ${VTK_LIBRARIES}
  )
endif()