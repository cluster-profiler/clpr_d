/**
 * @file manager.hpp
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
 * manager monitors and tidies the db
 */



#ifndef _MANAGER_HPP
#define _MANAGER_HPP

#include "clpr_proc_db.hpp"
#include <boost/thread/thread.hpp>

namespace clpr_d{

	/// manager class; check and tidy db
	class manager{
		
		private:

			db_pointer db_;
		public :
			manager(db_pointer& db);
			~manager(){};

			void operator()();


		
	};
};

#endif
