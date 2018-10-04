# Raycasting-with-C-

Project Description:
  Renders .obj files using c++ (check "render" folder for examples)

Execution:
  This was done using BVH rendering techniques. It reads from an obj file and stores each triangle into the tree.
  Then, using anti-aliasing, shading detection, and other rendering features, it outputs a render of the target file.
  
Use:
  Run "Make" to compile the project, then run with the flags -o and optionally -high_res. High res increases res
  and adds anti-aliasing, but takes a long time. -o is used as I was using an STL to OBJ file converter and it wasn't
  working as well as the stock OBJ files. So the -o is for those OBJ files, which are in the obj directory.
