cmake_minimum_required(VERSION 3.10.0)
project(D3DLAB VERSION 0.1.0 LANGUAGES C CXX)

find_package(directxtk CONFIG REQUIRED)


add_executable(D3DLAB  WIN32 main.cpp wind3d.cpp Render.cpp d3dwrap.cpp Direction.cpp LabMesh.cpp Map.cpp Phys.cpp Rndroute.cpp)



target_link_libraries(D3DLAB PRIVATE winmm.lib d3d11.lib d3dcompiler.lib uuid.lib  dxgi.lib dxguid.lib  Microsoft::DirectXTK)