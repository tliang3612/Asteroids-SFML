# Asteroids
A remake of the classic game 'Asteroids', written in C++ using the SFML graphics library for simple
handling of audio, window, system, and graphics.

# Collision Detection
Collision detection is based on the SAT method, where each vertex of the two polygons 
is projected onto each normal axis of the two polygons to find overlap. If there is 
no overlap on any of the axes, then the two polygons are not colliding.

Example below


![alt text](https://dyn4j.org/assets/posts/2010-01-01-sat-separating-axis-theorem/sat-ex-3.png)
