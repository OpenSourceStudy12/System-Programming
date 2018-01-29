#include"file.h"

File::File(const string & filename,int flag):
	filename(filename),
	flag(flag)
{
	fd = -1;
}

File::~File()
{
	if(fd !=-1)
	close();
	
}

bool File::open()
{
	fd = ::open(filename.c_str(),flag,0640);
	if(fd < 0)
		return false;
	return true;
}

int File::read(char* buff,int size)
{
	return ::read(fd,buff,size);
}
int File::write(char* buff,int size)
{
	return ::write(fd,buff,size);
}

void File::close()
{
	::close(fd);
	fd = -1;
}
