# Geographical Map Course Work
Course work for Technical University - Sofia (second semester) 

## Task
Write a C program with a menu to show and manipulate terrain portion. Terrain portion data must be stored in binary files.
The file must start with the size of the given terrain portion - width in meters & length in meters. After this you must add the terrain data:
Every meter (cell) from the terrain must have: 
 - **height**
 - **terrain type**: `0-plant`, `1-desert`, `2-water`, `3-road`, `4-building`
 - **attendence a day**
 
Make the following manipulations:
 1. show the map by its terrain type
 2. show the map by its height
 3. build continuous road by sequence of coordinates typed by the user;
    if the user type -1 then end the process;
    can't build road on water or building
 4. Early in the mornings bikers like to jump in the desert. Because it is morning the sun rises from East and that's why they ride in direction to West.
    Show the coordinates of appropriate hills for jumps, analysing terrain's height but in east->west direction.
    Appropriate jumps are 3 cells, one next to other, each one of them lower than the next one by max 45 degrees; followed by 2 cells - first one must be higher by -80 degrees.
    Cells must be of type `desert`.
    
## Mandatory Condition
Must use dynammically allocated data strutures to store the data and dynamic array for the map

## Documentation
Docs and description of the program in Bulgarian can find [here](https://docs.google.com/document/d/1m3Js1-4fzH1UaWE589b6JFQxRT8ex8DbfF1Dd-1rgKM/edit?usp=sharing).
