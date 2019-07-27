# IE03 Integarted Exercise for software I  
## Welcome to SpaceA group
### **Complete program is in submission directory**
### ** Things you need **
 - gcc compiler
 - python3
 - python module: matplotlib


### We have three functions:
  1. Query for shortest paths  
  2. Optimal road construction to the new points  
  3. Highways detection 
  
### How to use 
  1. Compile query.c with "-lm" option:
    ```
    gcc query.c -o query -lm
    ```
  2. Run the program:
    ```
    ./query
    ```
  3. Follow the instructions in the program

**Follow the following tips to enter data**  
  ```
  N is number of locations.  
  M is number of roads.  
  P is number of new locations.  
  Q is number of path queries.  
  From x_1 y_1 to x_n y_n are coordinates of locations  
  From b_1 e_1 to b_m e_m are start and end location IDs of line segments  
  From new_x_1 new_y_1 to new_x_p new_y_p are coordinates of new locations  
  From s_1 d_1 k_1 to s_q d_q k_q are start and end location IDs and number of k-th shortest paths for query.  
  ```
  
**Data input structure:**  
  ```
  N M P Q  
  x_1 y_1  
  x_2 y_2  
  x_n y_n  
  b_1 e_1  
  b_2 e_2  
  b_m e_m  
  new_x_1 new_y_1  
  new_x_2 new_y_2  
  new_x_p new_y_p  
  s_1 d_1 k_1  
  s_2 d_2 k_2  
  s_q d_q k_q  
  ```
  
#### Example 1 (k-th shortest paths)
##### Data
  ```
  6 5 0 5
  0 0
  2 5
  4 7
  5 5
  7 1
  9 5
  1 4
  1 6
  2 5
  3 5
  4 6
  1 4 5 
  C1 C3 10
  ```
#### Example 2 (new road constructions suggestion)
##### Data
  ```
  6 5 4 0
  0 0
  2 5
  4 7
  5 5
  7 1
  9 5
  1 4
  1 6
  2 5
  3 5
  4 6
  5 1
  11 5
  5 4
  3 6
  ```
#### Example 3 (Highways detection)
##### Data
  ```
  6 5 0 0
  0 0
  2 5
  4 7
  5 5
  7 1
  9 5
  1 4
  1 6
  2 5
  3 5
  4 6
  ```
------

