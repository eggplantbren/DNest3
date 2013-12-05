#ifndef _Data_
#define _Data_

class Data
{
	private:

	public:
		Data();

	// Singleton
	private:
		static Data instance;
	public:
		static const Data& get_instance() { return instance; }
};

#endif

