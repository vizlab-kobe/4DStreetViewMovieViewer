#!/bin/sh
cp ${KVS_DIR}/include/Core/Visualization/Shader/qualifire.h Viewer.app/Contents/MacOS/
cp ${KVS_DIR}/include/Core/Visualization/Shader/texture.h Viewer.app/Contents/MacOS/
cp ../../Lib/spherical_map.* Viewer.app/Contents/MacOS/
macdeployqt Viewer.app
