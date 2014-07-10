/**
 * @file reader.hpp
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
 * input reader, fill in db
 */



#ifndef _READER
#define _READER

#include <boost/thread/thread.hpp>
#include "clpr_proc_db.hpp"
#include "utilities.hpp"

namespace clpr_d {

	/// a class for reading pidstat data from a named pipe
	class reader{
		
		private:

		public :
			reader(clpr_proc_db &input);
			~reader(){};



		
	};
};

#endif
