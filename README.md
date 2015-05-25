# QSolanizer
A tool to visualize and analyze SMA power converter data. 
Currently it only parses CSV data from the (German?) [Sunny Explorer](http://www.sma.de/produkte/monitoring-control/sunny-explorer.html)

Please provide some exported files from your power converter, so other power converters are supported.

## Used Libraries
* [Qt5](http://www.qt.io/download/)
* [QCustomPlot](http://qcustomplot.com/) library to plot the graphs

## TODO

### First release
* save settings
* (de-)serialize data in background
* add option to autosize diagrams
* compare different days
* show "energy distribution" of day

### Later
* additional user input to calculate kWp/kWh
* show theoretical power curve