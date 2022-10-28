# Convex Hull
Implementation of two different algorithms to calculate and visualize a 2D convex hull and comparison regarding their performance
### **Algorithms:**
- Gift wrapping
- Quickhull
<br></br>

## **3 possible methods of generating input data (float values for 2D points):**
- Random generated test data
- Drawing points
- File import with simple file format
    * First line: number of points n
    * Line 2 .. n+1: x,y comma seperated float values for each point
<br></br>

## **2 modes of operation:**
- Performance optimized mode with just printed result and time measurement for
both algorithms
- Visual mode which draws with SDL the convex hull step by step interactively and
visualizes the algorithms 
<br></br>

## **Build instructions:**
 - Open ConvexHull.sln -build with visual studio 2022
- Prebuilt executables are in /x64/debug and /x64/release 
- For manual
builds the SDL2.dll must be copied into these folders
