reset
unset multiplot
#set term postscript eps enhanced color
#set output 'curr_e.eps'
set style line 1 lt 1 lw 5 pt 8 linecolor rgb "green"
set style line 2 lt 1 lw 5 pt 6 linecolor rgb "blue"
set style line 3 lt 1 lw 5 pt 4 linecolor rgb "red"


unset log                              # remove any log-scaling
unset label                            # remove any previous label
set key default                        # remove any key config

set key at 0.22,17.63
set key spacing 1.5
set key font "Helvetica,28"
set pointsize 2
set xrange [0.:5.0]
set yrange [0.0001:0.005]
set xlabel "time" font "Times-Italic, 30"
set ylabel "density" font "Times-Italic, 30"
set xtics font "Helvetica,20"
set ytics font "Helvetica,20"
set format y "%2.1e"


set ytics ("1" 0.001, "3" 0.003, "5" 0.005)

set multiplot
pi = 3.141593
filename(n)=sprintf("density_%d.txt",n)
file1 = filename(1)
file2 = filename(3)
file3 = filename(5)

fun1(x) = 1 / sqrt(4*pi*d1*x) * exp(-r1/(4*d1*x))
fun2(x) = 1 / sqrt(4*pi*d2*x) * exp(-r2/(4*d2*x))
fun3(x) = 1 / sqrt(4*pi*d3*x) * exp(-r3/(4*d3*x))

fit [0.01:5.0] fun1(x) file1 u 1:2 via d1, r1
fit [0.01:5.0] fun2(x) file2 u 1:2 via d2, r2
fit [0.01:5.0] fun3(x) file3 u 1:2 via d3, r3


set size 1.0, 1.0
set origin 0.02, 0.0
set label "With Particles" at 0.68,0.4 font "Times,28" 
plot file1 u 1:2 t " " ls 3, \
     fun1(x) notitle ls 3, \
     file2  u 1:2 t " " ls 2, \
     fun2(x) notitle ls 2, \
     file3  u 1:2 t " " ls 1, \
     fun3(x) notitle ls 1


unset label
unset format
set size ratio -1 1,1
set xlabel ""
set ylabel ""
set size 0.45,0.45
set origin 0.58,0.5
set xrange [0:1000]
set yrange [0:1000]
set xtics ""
set ytics ""
set point
#plot "lights_dist.txt" u 1:2 notitle with points pt 7 ps 1 lc rgb "blue", \
#	 "lights_dist.txt" every ::1::3  notitle with circles lw 3 lc rgb "red"


print d1
print d2
print d3

print sqrt(r1)
print sqrt(r2)
print sqrt(r3)