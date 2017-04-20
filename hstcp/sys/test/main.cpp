#include "../thread.h"
#include <iostream>
#include <string>
using namespace std;

class CTestThread:public CThread
{
public:
	long Run()
	{
		while(!isTerminated())
		{
			cout<<"in CTestThread :: Runn() ----------- 0 "<<endl;
			usleep(500);
		}
		return 0;
	}




};
bool g_bIsRun = false;
void signint_handler(int sig)
{

	g_bIsRun = false;
}
int main(int argc,char* argv[])
{
	CTestThread *pThread = new CTestThread();
	pThread->start();

	g_bIsRun = true;
	signal(SIGINT,signint_handler);
	int sum = 0;
	while(g_bIsRun)
	{	
		if (sum >=10)
			g_bIsRun = false;
		if(g_bIsRun == false)
		{
			pThread->stop();
		}
		sleep(1);
		sum++;
	}

	cout<<"it's over"<<endl;
	return 0;

}
