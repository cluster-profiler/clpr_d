/**
 * @file utilities.hpp
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
 * helper functions etc
 */




#ifndef _UTILITIES
#define _UTILITIES


#include "key_defines.hpp"
#include "pid_data.hpp"



namespace clpr_d{

	/// Whoami
	string get_uid(const vector<string>& input);
	/// When did I start
	string get_start_time(vector<string>& input);
	/// Return the unique index for db push/pop
	string get_grp_proc_label(vector<string> &input);
	/// Return the pid blob for this input line
	pid_data get_pid_data(vector<string> &input);
}

#endif
