/**
 * @file archiver.hpp
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
 * archiver; send db details to port when queried through boost::asio
 */



#ifndef _ARCHIVER_HPP
#define _ARCHIVER_HPP

#include "tcp_server.hpp"

namespace clpr_d{

	/// Archiving class; write a socket thru boost::asio
	class archiver{
		
		private:

			db_pointer db_;
		public :
			/**
			 * Construct the archiver, which will listen on some port 
                         * and write out the db when queried.
                         * @param db_pointer db input database
			 */
			archiver(db_pointer& db);
			~archiver(){};

			void operator()();

		
	};
};

#endif
