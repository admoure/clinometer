set term dumb
set output "/var/www/3Dview.png"
unset xtics
unset ytics
unset ztics
set xzeroaxis
set yzeroaxis
set zzeroaxis
set border 0
set label "X+" at 2.5,0,0
set label "Y+" at 0,2.5,0
set angle radians
alfa = 144.3238e-6
beta = 274.1465e-6
set xrange [-1:1]
set yrange [-1:1]
maximo=abs(sin(144.3238e-6)+sin(274.1465e-6))
set zrange [-maximo:maximo]
set xyplane at 0
set nokey
splot "/home/pi/soft_inclinometro/sero.txt" using 1:2:3 notitle with points
set terminal png truecolor size 1024,300
set multiplot layout 1,3
set arrow from 1,1,0 to -1,1,0 nohead
set arrow from -1,1,0 to -1,-1,0 nohead
set arrow from -1,-1,0 to 1,-1,0 nohead
set arrow from 1,-1,0 to 1,1,0 nohead
set arrow from 0,0,0 to 2,0,0 linecolor rgb "red"
set arrow from 0,0,0 to 0,2,0 linecolor rgb "green"
set arrow from 0,0,0 to cos(144.3238e-6),0,sin(144.3238e-6) nohead lw 4 lt 3
set arrow from 0,0,0 to 0,cos(274.1465e-6),sin(274.1465e-6) nohead lw 4 lt 3
set arrow from 0,0,0 to -cos(144.3238e-6),0,-sin(144.3238e-6) nohead lw 4 lt 3
set arrow from 0,0,0 to 0,-cos(274.1465e-6),-sin(274.1465e-6) nohead lw 4 lt 3
set arrow from cos(144.3238e-6),cos(274.1465e-6),sin(144.3238e-6)+sin(274.1465e-6) to -cos(144.3238e-6),cos(274.1465e-6),-sin(144.3238e-6)+sin(274.1465e-6) nohead lw 4 lt 3
set arrow from cos(144.3238e-6),cos(274.1465e-6),sin(144.3238e-6)+sin(274.1465e-6) to cos(144.3238e-6),-cos(274.1465e-6),sin(144.3238e-6)-sin(274.1465e-6) nohead lw 4 lt 3
set arrow from cos(144.3238e-6),-cos(274.1465e-6),sin(144.3238e-6)-sin(274.1465e-6) to -cos(144.3238e-6),-cos(274.1465e-6),-sin(144.3238e-6)-sin(274.1465e-6) nohead lw 4 lt 3
set arrow from -cos(144.3238e-6),-cos(274.1465e-6),-sin(144.3238e-6)-sin(274.1465e-6) to -cos(144.3238e-6),cos(274.1465e-6),-sin(144.3238e-6)+sin(274.1465e-6) nohead lw 4 lt 3
set label font "Times-Roman, 9" sprintf ("X = %.2f urad\nY = %.2f urad",144.3238e-6/1e-6,274.1465e-6/1e-6) at 0,0,-maximo*2
set view 73,49
replot
set view 73,229
replot
set view 64,135
replot
unset multiplot
