/**
 * @file logger.hpp
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
 * a logger class
 */

#ifndef _LOGGER_
#define _LOGGER_

#include "clpr_db.hpp"

namespace clpr_d{

	/// a logging class; write a file somewhere with stats
	class logger{
		
		private:
			/// main db
			db_pointer db_;
		public :
			/**
			 * Write a log file periodically with statistics; @see key_defines.h
			 * @param db_pointer & db database
			 */	 
			logger(db_pointer &db);
			/// bye bye birdie
			~logger(){};
			/// run
			void operator()();
	};
};

#endif
