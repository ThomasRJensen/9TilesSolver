# 9TilesSolver

## Billede af de 9 brikker (ikke placeret korrekt)
![9tiles](https://github.com/user-attachments/assets/2d032850-7560-45f3-b6bb-c7db779c07df)

## Compilering og kørsel af programmet  
  
### Option 1: Online (copy/paste koden)
https://www.programiz.com/cpp-programming/online-compiler/  
  
### Option 2: På Linux
```
g++ -std=c++17 main.cpp -O2 -o tile_solver
./tile_solver
```

## Output fra kørsel af programmet
```
Løsning fundet (hver brik: Pos(row,col): id [top,right,bottom,left] : color/part):  
  
Pos(0,0): id=0 [ Purple/Bottom, Blue/Top, Yellow/Top, Green/Bottom ]  
Pos(0,1): id=2 [ Purple/Bottom, Green/Top, Yellow/Top, Blue/Bottom ]  
Pos(0,2): id=5 [ Yellow/Bottom, Green/Top, Purple/Top, Green/Bottom ]  
  
Pos(1,0): id=7 [ Yellow/Bottom, Purple/Bottom, Green/Top, Blue/Top ]  
Pos(1,1): id=6 [ Yellow/Bottom, Green/Bottom, Blue/Top, Purple/Top ]  
Pos(1,2): id=3 [ Purple/Bottom, Yellow/Bottom, Blue/Top, Green/Top ]  
  
Pos(2,0): id=8 [ Green/Bottom, Purple/Bottom, Blue/Top, Yellow/Top ]  
Pos(2,1): id=4 [ Blue/Bottom, Yellow/Bottom, Green/Top, Purple/Top ]  
Pos(2,2): id=1 [ Blue/Bottom, Purple/Bottom, Blue/Top, Yellow/Top ]  
  
Elapsed time: 98 microseconds
```
