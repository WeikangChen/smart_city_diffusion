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




unset label
unset format
set size ratio -1 1,1
set xlabel ""
set ylabel ""
set xrange [0:1000]
set yrange [0:1000]
set xtics ""
set ytics ""
set point
plot "lights_dist.txt" u 1:2 notitle with points pt 7 ps 1 lc rgb "blue", \
	 "circle.txt" u 1:2:4 notitle with circles lw 3 lc rgb "red"


