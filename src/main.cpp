/**
 * @file main.cpp
 * @author  Bill Brouwer <whiskeyjulietb@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 * Copyright 2014 William J. Brouwer, Pierre-Yves Taunay
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 *
 */


#include <iostream>
#include <fstream>
#include <exception>

#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>

//#include "reader.hpp"
//#include "archiver.hpp"
//#include "manager.hpp"

#include "clpr_db.hpp"

#include "clpr_conf.hpp"
#include "clpr_log.hpp"
//#include "fifo_reader.hpp"
#include "proc_reader.hpp"

using namespace boost;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {

	////////////////////////////////////
	///////////  Start up a daemon
	////////////////////////////////////
	pid_t pid, sid;

	// If we are already a daemon, just exit
	if( getppid() == 1 ) {
		return EXIT_SUCCESS;
	}	

/*
	//// 1. Fork from parent process
	pid = fork();
	// Can't fork - fork error
	if ( pid < 0 ) 
		return EXIT_FAILURE;
	// Otherwise, parent exits	
	if ( pid > 0 ) 
		return EXIT_SUCCESS;
*/
	//// 2. Set permissions
	umask(0);

	//// 3. Open any logs
	clpr_d::p_log p_log_file;
	try {
		p_log_file = clpr_d::p_log( new clpr_d::clpr_log(CLPR_LOG_PATH));
	} catch (std::exception& e) {
		std::cout << "ERROR " << e.what() << std::endl;	
		return EXIT_FAILURE;
	}	
	
	//// 4. Get new session ID       
	/*
	sid = setsid();
	if ( sid < 0 ) 
		return EXIT_FAILURE;
	*/
	//// 5. Change to a safe working directory


	//// 6. Close the standard file descriptors	
	//close(STDIN_FILENO);
	// close(STDOUT_FILENO); // Leave it open for debugging purposes
	//close(STDERR_FILENO);
 
	////////////////////////////////////
	/////////// Actual daemon code
	////////////////////////////////////
	
	//// Check for configuration file, and parse it
	std::ifstream conf_file(CLPR_CONF_PATH, ios::in);
	std::string msg;
	clpr_d::p_conf p_conf_file;

	// Try opening the config file
	if (conf_file.is_open()) {

		// Found the config file; get the path, then pass it to logger
		msg = "Found configuration file";
		std::string clpr_conf_path(CLPR_CONF_PATH, strnlen(CLPR_CONF_PATH,511)); 
		msg += clpr_conf_path;
		msg += " !  Parsing...";

		p_log_file->write(CLPR_LOG_INFO, msg);

		// Set the config object
		p_conf_file = clpr_d::p_conf( new clpr_d::clpr_conf(conf_file,p_log_file) );
		// Now, add the config file to the log file
		p_log_file->set_debug(p_conf_file);

		// Log the closing of the log file
		msg = "Closing configuration file " + clpr_conf_path;
		p_log_file->write(CLPR_LOG_INFO, msg);

		conf_file.close();

	} else {
		p_log_file->write(CLPR_LOG_ERROR,"Can not find or open configuration file");
		return EXIT_FAILURE;
	}	

	//// Start the database
	// Main db -- shared pointer
	clpr_d::db_ptr p_clpr_db;
	p_clpr_db = clpr_d::db_ptr(new clpr_d::clpr_db(p_log_file, p_conf_file));


	//// Start the reader
	clpr_d::proc_reader reader(p_log_file);	
	reader.read(p_clpr_db);




	//// Worker threads
	// extra paranthesis for most vexing parse(?)
	
	// 1. Logger
	// boost::thread write_log((logger(dbs)));
	// 2. Listener
	//boost::thread write_socket((archiver(dbs)));
	//boost::thread manage_db((manager(dbs)));

	//reader parse_input(db);

	//write_log.join();
	//write_socket.join();
	//manage_db.join();

//	exit(EXIT_SUCCESS);
	return 0;
}
