# MapGeneration
A map generated by GPS data

###This is a map generation program based on GPS data. Inspired by the Google Map, Baidu Map which can show the real time traffic situation, I try to generate a map with GPS data collected by taxi. The program has following files:

##generateMap.cpp
Generate the raw map with GPS data

##createTrace.cpp
Using OpenCV to create the trace with the raw map

##voronoi.cpp
Get the skeleton of the contoured map
