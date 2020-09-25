set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(SDK_ROOT "${CMAKE_BINARY_DIR}/SDK/")
set(ULTRALIGHT_INCLUDE_DIR "${SDK_ROOT}/include")
set(ULTRALIGHT_BINARY_DIR "${SDK_ROOT}/bin")
set(ULTRALIGHT_INSPECTOR_DIR "${SDK_ROOT}/inspector")

set(PORT UltralightWin)
set(PLATFORM "win")
set(ULTRALIGHT_LIBRARY_DIR "${SDK_ROOT}/lib")

set(ARCHITECTURE "x64")

set(S3_DOMAIN ".sfo2.cdn.digitaloceanspaces.com")

ExternalProject_Add(UltralightSDK
  URL https://ultralight-sdk${S3_DOMAIN}/ultralight-sdk-latest-${PLATFORM}-${ARCHITECTURE}.7z
  SOURCE_DIR "${SDK_ROOT}"
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
)

MACRO(ADD_APP source_list)
  set(APP_NAME ${CMAKE_PROJECT_NAME})

  include_directories("${ULTRALIGHT_INCLUDE_DIR}")
  link_directories("${ULTRALIGHT_LIBRARY_DIR}")
  link_libraries(UltralightCore AppCore Ultralight WebCore)

  add_executable(${APP_NAME} WIN32 ${source_list})
  
  if (MSVC)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${APP_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
  endif()

  # Copy all binaries to target directory
  add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_BINARY_DIR}" $<TARGET_FILE_DIR:${APP_NAME}>) 

    set(ASSETS_PATH "$<TARGET_FILE_DIR:${APP_NAME}>/assets") 

  # Copy assets to assets path
  add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets/" "${ASSETS_PATH}")

  if(${ENABLE_INSPECTOR})
    # Copy inspector to assets directory
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_INSPECTOR_DIR}" "${ASSETS_PATH}/inspector")
  endif ()

    set(RESOURCES_PATH "$<TARGET_FILE_DIR:${APP_NAME}>/resources") 

  # Copy resources to resources path
  add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ULTRALIGHT_BINARY_DIR}/resources/" "${RESOURCES_PATH}")

  add_dependencies(${APP_NAME} UltralightSDK)
ENDMACRO()