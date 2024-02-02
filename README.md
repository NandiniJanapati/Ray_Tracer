This is the fifth assignment I did for my Computer Graphics class at Texas A&M
The goal was to create a ray-tracer that could render implicit spheres, triangles, and planes.
The reflections go to a depth of 4 recursive calls.
Bounding Volume Hierarchy was implemented for the second scene where the bunny is drawn in a small portion of the screen.

To switch from the scene with the spheres to the bunny, in main.cpp on line 174 in the Init() function, replace the 0 to a 1 and re-run the program.
For the sphere scene:
line 174: Scene scene = Scene(SHAPES0, LIGHTS);

For the bunny scene:
line 174: Scene scene = Scene(SHAPES1, LIGHTS);
