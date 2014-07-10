#include <string>
#include <exception>

namespace clpr_d {

class mv_exception : public std::exception {
	public:
	explicit mv_exception(const char* message): msg_(message) {}
	explicit mv_exception(const std::string& message): msg_(message) {}
	virtual ~mv_exception() throw() {}
	virtual const char *what() const throw() {
		return msg_.c_str();
	}

	protected:
	std::string msg_;
}; // End of class mv_exception

}; // End of namespace clpr_d
