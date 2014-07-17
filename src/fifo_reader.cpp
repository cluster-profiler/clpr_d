#include <boost/regex.hpp>

#include "fifo_reader.hpp"

namespace clpr_d {

fifo_reader::fifo_reader(const std::string& fifo_path, const clpr_d::p_log p_log_file) {

	this->p_log_file = p_log_file;
	this->p_log_file->write(CLPR_LOG_DEBUG,"Calling fifo_reader constructor");


	// Open the path to the fifo
	fifo.open(fifo_path, std::ifstream::in); 

	if(!fifo.is_open()) {
		p_log_file->write(CLPR_LOG_ERROR,"Could not open fifo for reading");
	}	
} // End of fifo_reader::fifo_reader


fifo_reader::~fifo_reader() {
	p_log_file->write(CLPR_LOG_DEBUG,"Calling fifo_reader destructor");

	if(fifo.is_open()) {
		fifo.close();
	}	
} // End of fifo_reader::~fifo_reader
	

void fifo_reader::read(clpr_d::db_ptr p_db) {

	std::string line;
	std::string msg;

	uint64_t global_label = 0;
	bool input_checked = false;

	std::istringstream iss;

	while(1) {
		// Open the path to the fifo
		if( !fifo.is_open() ) 
			fifo.open(CLPR_FIFO_PATH, std::ifstream::in); 

		// Read content of FIFO
		while(std::getline(fifo, line)) {
			msg = "Processing line " + line;
			p_log_file->write(CLPR_LOG_INFO,msg);

			/// Store the line in an istringstream
			iss.str(line);

			/// Input is the result of "pidstat -duhlrwUv | grep -v root"
			// C++11 uniform initialization
			std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} }; 

			/// Check the validity of the input; report errors
			input_checked = fifo_reader::check_tokens(tokens);

			if(input_checked) {
				msg = "Success";
				p_log_file->write(CLPR_LOG_INFO,msg);

				/// Populate the database !
				// index = get_process_grp_label(tokens);
				// p_db->update(tokens, index, host_info, global_label);
			
			} else {
				msg = "Parsing failed";
				p_log_file->write(CLPR_LOG_ERROR,msg);
			}	

			/// Let's not forget to clear the stream
			iss.clear();
		}
		fifo.close();
	}

} // End of fifo_reader::read

// FIXME: clean up a bit that routine
bool fifo_reader::check_tokens(const std::vector<std::string>& tokens) {

	bool ret = false;
	std::string test_str;
	std::string msg;
	std::vector<int> enum_subset;
	int pos_enum;

	// We need to have at lease CLPR_NUM_TOKENS_PER_LINE_MIN tokens on a single line
	if( tokens.size() >= CLPR_NUM_TOKENS_PER_LINE_MIN ) {
		//// Tokens 0 1 2 3 4 : start date -> Tue Jul 15 10:01:02 2014
		for(int i = 0; i < TIME_POS; ++i) {
			test_str += tokens[i];
			if( i < TIME_POS-1) {
				test_str += " ";
			}
		}	

		// Test that we have a valid date
		boost::regex rex_date("^(Mon|Tue|Wed|Thu|Fri|Sat|Sun) (Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec) ([0-9]{2}) ([0-9]{2}:[0-9]{2}:[0-9]{2}) [0-9]{4}" , boost::regex::extended);

		if(!boost::regex_match(test_str,rex_date)) {
			msg = test_str + " can not be parsed !";
			p_log_file->write(CLPR_LOG_ERROR, msg);

			return false;
		}	
					

		//// Tokens 5,7,12,15,16,21,24,25: timestamp, PID, cpu number, vsz, rss, iodelay, threads, fd -> 140789741, 1234, 2, 45678, 12546, 10, 2, 10
		//// Regexes:  [1-9][0-9]* -> time, pid, vsz, rss 
		////  cpu#: [0-9] | [1-9][0-9], iodelay: [0-9]*, 
		////  threads:, fd: -([0-9] | [1-9][0-9]*)
		test_str.clear();

		/// Test timestamp, PID, vsz, rss
		boost::regex regex1("[0-9]|[1-9][0-9]*", boost::regex::extended);
		enum_subset.clear();
		enum_subset = {TIME_POS, PID_POS, VSZ_POS, RSS_POS};

		for(auto it = enum_subset.begin(); it != enum_subset.end(); ++it) {
			test_str = tokens[*it];
			if(!boost::regex_match(test_str, regex1)) {
				ret = false;
				pos_enum = *it;
				break;
			} else {
				ret = true;
			}	
		}	

		if(!ret) {
			msg = "Expression \"" + test_str + "\" (position " + std::to_string(pos_enum+1) + ") can not be parsed !";
			p_log_file->write(CLPR_LOG_ERROR,msg);

			return false;
		} 

		/// Test cpu#, iodelay 
		boost::regex regex2("([0-9])|([1-9][0-9]*)",boost::regex::extended);
		enum_subset.clear();
		enum_subset = {CORE_CPU_POS, IODELAY_POS};
		for(auto it = enum_subset.begin(); it != enum_subset.end(); ++it) {
			test_str = tokens[*it];
			if(!boost::regex_match(test_str, regex2)) {
				ret = false;
				pos_enum = *it;
				break;
			} else {
				ret = true;
			}	
		}	

		if(!ret) {
			msg = "Expression \"" + test_str + "\" (position " + std::to_string(pos_enum+1) + ") can not be parsed !";
			p_log_file->write(CLPR_LOG_ERROR,msg);

			return false;
		} 

		/// Test threads
		boost::regex regex_th("([1-9][0-9]*)", boost::regex::extended);
		test_str = tokens[NTH_POS];
		if(!boost::regex_match(test_str,regex_th)) {
			msg = "Expression \"" + test_str + "\" (position " + std::to_string(NTH_POS+1) + ") can not be parsed !";
			p_log_file->write(CLPR_LOG_ERROR, msg);

			return false;
		}	

		/// Test fd
		test_str = tokens[FDS_POS];
		// First test we don't have a negative number. If so, check pidstat privileges
		boost::regex regex_neg("-", boost::regex::extended);
		if(boost::regex_search(test_str,regex_neg)) {
			msg = "Expression \"" + test_str + "\" (position " + std::to_string(FDS_POS+1) + ", file descriptor) is a negative number. Check that pidstat has enough privileges to be run.";
			p_log_file ->write(CLPR_LOG_ERROR, msg);
			return false;
		}	

		boost::regex regex_fd("[0-9]|[1-9][0-9]*", boost::regex::extended);
		if(!boost::regex_match(test_str,regex_fd)) {
			msg = "Expression \"" + test_str + "\" (position " + std::to_string(FDS_POS+1) + ") can not be parsed !";
			p_log_file ->write(CLPR_LOG_ERROR, msg);
			return false;
		}	

		//// Tokens 8,9,10,11,13,14,17,18,19,20,22,23: %usr, %sys, %gst, %cpu, minflt/s, majflt/s, %mem, kbrd/s, kbwr/s, cswch/s, nvcswch/s
		enum_subset.clear();
		enum_subset = {USER_POS, SYSTEM_POS, PCPU_POS, MINFLT_S_POS, MAJFLT_S_POS, MEM_POS, KB_RD_S_POS, KB_WR_S_POS, KB_CCWR_S_POS, CSWCH_S_POS, NVCSWCH_S_POS};
		boost::regex regex_float("[0-9]+\.[0-9]{2,}", boost::regex::extended);
		for(auto it = enum_subset.begin(); it != enum_subset.end(); ++it) {
			test_str = tokens[*it];
			// Check the negative
			if(boost::regex_search(test_str, regex_neg)) {
				ret = false;
				pos_enum = *it;
				msg = "Expression \"" + test_str + "\" (position " + std::to_string(pos_enum+1) + ") is a negative number. Check that pidstat has enough privileges to be run.";
				p_log_file->write(CLPR_LOG_ERROR,msg);
				break;
			}
			// Check for a correct floating point format
			else if(!boost::regex_match(test_str,regex_float)) {
				ret = false;
				pos_enum = *it;
				msg = "Expression \"" + test_str + "\" (position " + std::to_string(pos_enum+1) + ") can not be parsed !";
				p_log_file->write(CLPR_LOG_ERROR,msg);
				break;
			} else {
				ret = true;
			}
		}	

		if(!ret) {
			return false;
		}	
		
		// We passed everything ! 
		ret = true;

		//// Token 6: user
		// Maybe have the config file specifying a user regex ?

		//// Token 26+: cmd
		// No need to try to parse that -- can be anything
		// Check that field 26 is not empty, though
	} else {
		msg = "Not enough columns (Currently: " + std::to_string(tokens.size()) + "; should be greater than " + std::to_string(CLPR_NUM_TOKENS_PER_LINE_MIN) + ")"; 
		p_log_file->write(CLPR_LOG_ERROR,msg);

		ret = false;
	}	

	return ret;

} // End of fifo_reader::check_tokens



} // End of namespace clpr_d
