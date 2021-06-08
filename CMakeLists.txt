cmake_minimum_required(VERSION 3.0.0)
project(CAVO VERSION 0.1.0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "../bin")
add_compile_definitions(
    _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
)

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# set(INCLUDE_LIST 
#   "header_files/ec/edu/ups/model"
#   "header_files/ec/edu/ups/controller"
#   "header_files/ec/edu/ups/view"
#   "header_files/ec/edu/ups"
# )

# include_directories( ${OpenCV_INCLUDE_DIRS} ${INCLUDE_LIST})

include_directories( ${OpenCV_INCLUDE_DIRS})

# IF (WIN32)
#   add_compile_definitions(CH_PLATFORM_WINDOWS)
# ELSE()
#   # set stuff for other systems
# ENDIF()

add_executable(${PROJECT_NAME}
  "source_files/ec/edu/ups/controller/chroma_render_controller.cpp"
  "source_files/ec/edu/ups/controller/morphological_operation.cpp"
  "source_files/ec/edu/ups/model/frame.cpp"
  "source_files/ec/edu/ups/model/camera.cpp"
  "source_files/ec/edu/ups/model/mat_render.cpp"
  "source_files/ec/edu/ups/view/main.cpp"
  "source_files/ec/edu/ups/model/slider_group.cpp"
  "source_files/ec/edu/ups/view/main_cavo_gui.cpp"
)

# target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_LIST}
# )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

if (WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
    
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/styles/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsVistaStylePlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/styles/
  )
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
