# QSolanizer
A tool to visualize and analyze SMA power converter data. 
Currently it only parses CSV data from the (German?) [Sunny Explorer](http://www.sma.de/produkte/monitoring-control/sunny-explorer.html)

Please provide some exported files from your power converter, so other power converters are supported.

## Used Libraries
* [Qt5](http://www.qt.io/download/)
* [QCustomPlot](http://qcustomplot.com/) library to plot the graphs

## TODO

### First release
* ~~save settings~~ done
* ~~(de-)serialize data in background~~ not neccessary
* ~~add option to autosize diagrams~~
* ~~compare different days~~ done
* ~~show "energy distribution" of day~~ done
* ~~clean up code (primary qsolanizer.cpp)~~ done
* ~~make bars clickable~~ done
* additional user input to calculate kWp/kWh
* show theoretical power curve
* add dialog which shows missing data (gaps)


### Later

* save colors in settings file (partially done)
* do not reread everything when additional data is added
