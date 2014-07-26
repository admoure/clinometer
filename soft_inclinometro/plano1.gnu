clear
reset


#set title "Inclinación" font "Helvetica,20"


unset xtics
unset ytics
unset ztics

set xzeroaxis
set yzeroaxis
set zzeroaxis


set border 0 
#set border lw 0.5


set xlabel "X+" font "Helvetica,10"
set ylabel "Y+" font "Helvetica,10"
#set zlabel "z"

set xlabel offset 60,0
set ylabel offset 0,5
#set zlabel offset 0,0,1

set angle radians

alfa=-111.17e-6
beta=-54.07e-6


set xrange [-1:1]
set yrange [-1:1]


maximo=abs(sin(alfa)+sin(beta))
#print maximo

set zrange [-maximo:maximo]

set xyplane at 0

splot 'C:\Users\ignpc2013b\Desktop\datos.txt' using 1:2:3 with points
set nokey


#marco en el plano xy
set arrow from 1,1,0 to -1,1,0 nohead
set arrow from -1,1,0 to -1,-1,0 nohead
set arrow from -1,-1,0 to 1,-1,0 nohead
set arrow from 1,-1,0 to 1,1,0 nohead

#ejes x e y
set arrow from 0,0,0 to 2,0,0
set arrow from 0,0,0 to 0,2,0


#vector X
set arrow from 0,0,0 to cos(alfa),0,sin(alfa) nohead lw 4 lt 3

#vector Y
set arrow from 0,0,0 to 0,cos(beta),sin(beta) nohead lw 4 lt 3

set arrow from 0,0,0 to -cos(alfa),0,-sin(alfa) nohead lw 4 lt 3
set arrow from 0,0,0 to 0,-cos(beta),-sin(beta) nohead lw 4 lt 3

set arrow from cos(alfa),cos(beta),sin(alfa)+sin(beta) to -cos(alfa),cos(beta),-sin(alfa)+sin(beta) nohead lw 4 lt 3
set arrow from cos(alfa),cos(beta),sin(alfa)+sin(beta) to cos(alfa),-cos(beta),sin(alfa)-sin(beta) nohead lw 4 lt 3
set arrow from cos(alfa),-cos(beta),sin(alfa)-sin(beta) to -cos(alfa),-cos(beta),-sin(alfa)-sin(beta) nohead lw 4 lt 3
set arrow from -cos(alfa),-cos(beta),-sin(alfa)-sin(beta) to -cos(alfa),cos(beta),-sin(alfa)+sin(beta) nohead lw 4 lt 3


#Texto angulos
set label sprintf("Xº = %.2f urad ; Yº = %.2f urad",alfa/1e-6,beta/1e-6) at 5.2,-4.2,0
#set label sprintf("Yº = %.2f urad",beta/1e-6) at 4.5,-3.4,0

set view 73,49 #Done implicitly by mousing.
replot