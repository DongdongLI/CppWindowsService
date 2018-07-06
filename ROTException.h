class ROTException{
public:
	ROTException() :
		message(""){}
	virtual ~ROTException() {}
	ROTException(char * msg) :
		message(msg){}
	const char * what(){
		return message;
	}
private:
	const char* message;
};