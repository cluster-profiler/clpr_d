/**
 * @file clpr_log.hpp
 * @author Bill Brouwer <whiksyejulietb@gmail.com>
 * @author Pierre-Yves Taunay <py.taunay@psu.edu>
 * @version 0.2
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
 * @section LICENSE
 *
 * 
 *
 * @section DESCRIPTION
 *
 * A logging class that adds entries to a log file, whose location is defined with the CLPR_LOG_PATH define 
 *
 */
#ifndef _CLPR_LOG_HPP_
#define _CLPR_LOG_HPP_

//// STD
#include <string>
#include <fstream>

//// Boost
#include <boost/shared_ptr.hpp>

#include "clpr_conf.hpp"

//// Defines
// Path to the log file
#define CLPR_LOG_PATH  "/gpfs/work/p/pzt5044/Github/clpr_d/log/clprd.log"
#define CLPR_LOG_PATH_OLD  "/gpfs/work/p/pzt5044/Github/clpr_d/log/clprd.log."

//// Enums
// Logging types
enum { CLPR_LOG_UNKNOWN = -1, CLPR_LOG_INFO = 0, CLPR_LOG_WARN = 1, CLPR_LOG_ERROR = 2, CLPR_LOG_CRITICAL = 3, CLPR_LOG_DEBUG = 4 };

//// Class
namespace clpr_d {

class clpr_conf;
typedef boost::shared_ptr<clpr_conf> conf_ptr;


class clpr_log {

	private:
		std::ofstream log_file;
		bool is_debug;

	public:
		clpr_log(const std::string& path);
		~clpr_log();

		void write(const int& severity, const std::string& msg); 
		void set_debug(const clpr_d::conf_ptr& conf_file);



}; // End of class clpr_log

typedef boost::shared_ptr<clpr_log> log_ptr;

}; // End of namespace clpr_d

#endif
