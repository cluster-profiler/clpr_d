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

#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <fstream>

#include "reader.hpp"
#include "logger.hpp"
#include "archiver.hpp"
#include "manager.hpp"
#include "clpr_proc_db.hpp"
#include "clpr_config.hpp"

using namespace boost;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {

	//// Start up a daemon
	pid_t pid, sid;

	// If we are already a daemon, just exit
	if( getppid() == 1 ) {
		return EXIT_SUCCESS;
	}	

	//// 1. Fork from parent process
	pid = fork();
	// Can't fork - fork error
	if ( pid < 0 ) 
		exit(EXIT_FAILURE);
	// Otherwise, parent exits	
	if ( pid > 0 ) 
		exit(EXIT_SUCCESS);

	//// 2. Set permissions
	umask(0);

	//// 3. Open any logs
		
	
	//// 4. Get new session ID       
	sid = setsid();
	if ( sid < 0 ) 
		exit(EXIT_FAILURE);


	//// 5. Change to a safe working directory


	//// 6. Close the standard file descriptors	
	close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	close(STDERR_FILENO);

	//// 7. Actual daemon code
	
	// Check for configuration file, and parse it
	std::ifstream config_file(CLPR_CONF_PATH, ios::in);
	if (config_file.is_open()) {
		std::cout << "INFO Found configuration file ! Parsing..." << std::endl;
		clpr_d::p_conf p_config_file( new clpr_d::clpr_config(config_file) );
		config_file.close();
	} else {
		std::cout << "ERROR Can not find configuration file" << std::endl;
		exit(EXIT_FAILURE);
	}	

	/*
	// main db
	clpr_proc_db db;
	// a shared pointer to same
	db_pointer dbs(&db,null_deletor());
	//kickof threads
	// extra paranthesis for most vexing parse(?)
	thread write_log((logger(dbs)));
	thread write_socket((archiver(dbs)));
	thread manage_db((manager(dbs)));

	//
	reader parse_input(db);

	write_log.join();
	write_socket.join();
	manage_db.join();

	//
	exit(EXIT_FAILURE);
	*/
}
