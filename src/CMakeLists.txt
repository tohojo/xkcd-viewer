

set(xkcd-viewer_SOURCES
  main.cpp
  message-handler.cpp
  gui.cpp
  widgets/graphicsview.cpp
  widgets/graphicsscene.cpp
  widgets/signalingdockwidget.cpp)
set(xkcd-viewer_HEADERS
  message-handler.h
  gui.h
  widgets/graphicsview.h
  widgets/graphicsscene.h
  widgets/signalingdockwidget.h
  )

set(xkcd-viewer_FORMS ui/gui.ui)

# QT4-specific preprocessor headers
qt4_wrap_cpp(xkcd-viewer_HEADERS_MOC ${xkcd-viewer_HEADERS})
qt4_wrap_ui(xkcd-viewer_FORMS_HEADERS ${xkcd-viewer_FORMS})

# make sure the compiler can find the generated _ui* files
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(widgets/)
include_directories(./)

add_executable(xkcd-viewer
  ${xkcd-viewer_HEADERS_MOC}
  ${xkcd-viewer_FORMS_HEADERS}
  ${xkcd-viewer_SOURCES})
target_link_libraries(xkcd-viewer ${QT_LIBRARIES})
