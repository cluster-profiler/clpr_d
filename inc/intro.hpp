/*! \mainpage 
 *
 * \section Overview
 * This code comprises the clpr_d daemon for the Cluster Profiler project, an Orwellian attempt to develop time series and statistics for all running processes on a single system or many systems. Process data is gathered or clustered according to process birthdate (rounded to the minute) and uid.  The daemon uses several threads to work on a boost::multi_index data structure, containing the acquired process data. The main thread reads from a named pipe specified in key_defines.h, data produced by running and re-directing output from pidstat in (eg.,) an external shell loop. Appropriately formatted pidstat output may be produced from this forked code, the utility originally produced by Sebastien Godard : https://github.com/wjb19/sysstat. An example usage is the following, removing process data from root, and redirecting to a fifo in the bin directory of this distribution :
 *  
 *  pidstat -d -u -h -l -r -w -U  -v | grep -v root > bin/clpr_input 
 * 
 * An archiving thread works asynchronously to write port 80 using tcp/ipv4 whenever queried, according to the format specified in the ostream operator for clpr_db, the wrapper around boost::multi_index. Acquired process data from the fifo is 'blobbed' together by the reader and statistics developed on the fly. A logging class will write a log file periodically as well, with filename specified in key_defines.h. Top utilization statistics are reported in the log file, using the multiple search/sort indices of boost::multi_index. Finally, a manager thread periodically monitors the size of the database, trimming/deleting entries according to timestamps and a maximum size. 
 *
 * Keep in mind this daemon can be a security risk and compute as well as i/o intensive. It has been designed with flume + solr in mind; for the overall project, flume is used to query various instances of clpr_d, and solr used for indexing and search on records - WJB 03/14
 *  
 * \section Installation
 * Install fork of pidstat specified, 'make' this distribution, specifying compile and linker paths for boost as needed.
 */

