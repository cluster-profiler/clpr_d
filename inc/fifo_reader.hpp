#ifndef _FIFO_READER_
#define _FIFO_READER_

#include <string>
#include <fstream>

#include "clpr_log.hpp"
#include "clpr_db.hpp"

#define CLPR_FIFO_PATH "/gpfs/work/p/pzt5044/Github/clpr_d/bin/input"
#define CLPR_NUM_TOKENS_PER_LINE_MIN 27

namespace clpr_d {

class fifo_reader {

	public:
		fifo_reader(const std::string& fifo_path, const clpr_d::p_log p_log_file); 
		~fifo_reader();

		void read(clpr_d::db_ptr p_db);

		bool check_tokens(const std::vector<std::string>& tokens);


	private:
		std::ifstream fifo;
		clpr_d::p_log p_log_file;

		
		

}; // End of fifo_reader

} // End of namespace clpr_d

#endif // _FIFO_READER_
