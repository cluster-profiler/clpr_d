Overview
========

This code comprises the clpr_d daemon for the Cluster Profiler project, an Orwellian attempt to develop time series and statistics for all running processes on a single system or many systems. Process data is gathered or clustered according to process group ID an UID. The daemon uses several threads to work on a map data structure, containing the acquired process data. The main thread reads from the /proc directory. Acquired process data is 'blobbed' together by the reader and statistics developed on the fly. 

A logging class will write a log file periodically as well. Finally, a manager thread periodically monitors the size of the database, trimming/deleting entries according to timestamps and a maximum size.

Keep in mind this daemon can be a security risk and compute as well as i/o intensive. It has been designed with flume + solr in mind; for the overall project, flume is used to query various instances of clpr_d, and solr used for indexing and search on records - WJB, PYT 08/14 

Installation
============
'make' this distribution, specifying compile and linker paths for boost as needed. 

Changelog from v0.1
===================
o Shared pointers all the way.
o Const-correctness.
o Class names were changed.
o FIFO listener to input data from pidstat was removed. The class proc_reader now provides the parsing capabilities of pidstat.
o TCP server was removed. An output file is written to disk. 
o Config class and file provide customization capabilities.
o Log class and file provide logging capabilities.
o Database design has been simplified.
o Process aggregation is at the process-group level, not by user id and start time.
