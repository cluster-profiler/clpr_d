/**
 * @file main.cpp
 * @author  Bill Brouwer <whiskeyjulietb@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 * Copyright 2014 William J. Brouwer
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

#include "reader.hpp"
#include "logger.hpp"
#include "archiver.hpp"
#include "manager.hpp"

#include "clpr_proc_db.hpp"

#include <boost/thread/thread.hpp>

using namespace clpr_d;
using namespace boost;

int main(void) {

	pid_t pid, sid;


	if ((pid=fork()) < 0) 
		exit(EXIT_FAILURE);

	if (pid > 0) 
		exit(EXIT_SUCCESS);

	//
	umask(0);

	//the child process       
	if ((sid=setsid()) < 0) 
		exit(EXIT_FAILURE);


	// close the std streams
	close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	//close(STDERR_FILENO);

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
}
