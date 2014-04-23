#include "reader.hpp"

namespace clpr_d{

	reader::reader(clpr_proc_db & db){


		uint64_t global_label=0;
		while(1){

			ifstream pipe_in(CLPR_INPUT_PIPE);
			
			//FIXME
			//struct stat st;

			//fstat(pipe_in, &st);
			//if (!(S_ISFIFO(st.st_mode)))
			//	std::invalid_argument("bad or non-existant fifo");

			string in;

			//input:
			//pidstat -d -u -h -l -r -w -U  -v | grep -v root
			string host_info="?"; 
			string index;
			while (getline(pipe_in,in)){

				istringstream iss(in);
				vector<string> tokens{istream_iterator<string>{iss},
					istream_iterator<string>{}};




				//not a blank line nor a header
				if ((tokens.size()>0) && (tokens.size()!=23)){

					if (tokens.size()<=20){
						host_info=in;
					}
					if (tokens.size()>20){
		
						index = get_uid_blob_label(tokens);
						uid_blob blob = db.get(tokens,index,global_label);
						blob.set(host_info,tokens);
						db.insert(blob,index);

					} //else {
										
					//	host_info=in;
					//}
				}
			}
		}
	};
};
