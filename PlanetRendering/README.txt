Planet Rendering application
Christian Cosgrove

Description: 
This program allows one to visualize procedurally generated planets and perform n-body gravity simulations.  It uses OpenGL and SDL to render pseudorandom terrains based on a custom noise function.  Planets have random visual properties.

The physics engine is a Verlet integrator equipped with Newtonian gravity.  The integrator is numerically stable, so orbits should not unrealistically decay over time.  The starting system here is an unstable three-body system.

Controls:
Up/down arrows - adjust time step
WASD - movement
Shift - increase acceleration
Acceleration - increase acceleration
Space - decelerate
Mouse - rotate camera