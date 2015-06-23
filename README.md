# QSolanizer
A tool to visualize and analyze SMA power converter data. 
Currently it only parses CSV data from the (German?) [Sunny Explorer](http://www.sma.de/produkte/monitoring-control/sunny-explorer.html)

Please provide some exported files from your power converter, so other power converters are supported.

## Screenshots
<img src="http://lostbit.de/uploads/qsolanizer/qsolanizer_day_multi.PNG" width="350">
<img src="http://lostbit.de/uploads/qsolanizer/qsolanizer_day_theo.PNG" width="350">
<img src="http://lostbit.de/uploads/qsolanizer/qsolanizer_month_energy.PNG" width="350">
<img src="http://lostbit.de/uploads/qsolanizer/qsolanizer_customrange_distribution.PNG" width="350">
<img src="http://lostbit.de/uploads/qsolanizer/qsolanizer_all_years.PNG" width="350">
<img src="http://lostbit.de/uploads/qsolanizer/qsolanizer_total.PNG" width="350">

## Used Libraries
* [Qt5](http://www.qt.io/download/)
* [QCustomPlot](http://qcustomplot.com/) library to plot the graphs

## TODO

### Later
* remove single day plot if multiple plots are shown - undone, have to think about consistent behaviour
* save colors in settings file (partially done)
* do not reread everything when additional data is added
* restructure project, so that Solarpart also contains the IO
* add dialog which shows missing data (gaps) - delayed
* create installer for windows users - delayed
