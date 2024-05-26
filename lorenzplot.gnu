set term qt persist
set datafile separator ","
set title "Lorenz Model Output"
set xlabel "x"
set ylabel "y"
set zlabel "z"

set xrange [-30:30]
set yrange [-30:30]
set zrange [0:50]

# filename and n=number of lines of your data
filedata = 'test.csv'
n = system(sprintf('cat %s | wc -l', 'test.csv'))

set term gif animate delay 10
set output 'centerofmassdelayedwithcolor.gif'

set linetype 1 lw 2 lc rgb '#9D72AC' pt 1
set linetype 2 lw 4 lc rgb '#2AOC43' pt 7 ps 2
set linetype 3 lw 4 lc rgb '#9E2B25' pt 7 ps 2

do for [j=1:20000:10] {
    set title 'time '.j
    splot filedata u 1:2:3 every ::1::j w l lt 1 title "Lorenz Model" , \
          filedata u 1:2:3 every ::j::j w p lt 2 notitle, \
          filedata u 4:5:6 every ::j::j w p lt 3 title "Center of Mass"
}
