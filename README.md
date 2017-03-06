# Map Generation
A map generated by GPS data

## This is a little map generation program based on GPS data. Inspired by the Google Map, Baidu Map which can show the real time traffic situation, I try to generate a map with GPS data collected by taxi. The program has following files:

1. generateMap.cpp: Generate the raw map with GPS data
2. createTrace.cpp: Using OpenCV to create the trace with the raw map
3. voronoi.cpp: Get the skeleton of the contoured map (in developing...)
4. mymodule.py: Using scikit-image to get skeleton from the threshold map
