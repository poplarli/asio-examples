/*
 * timer1.cpp
 *
 *  Created on: 2019��12��4��
 *      Author: poplarli
 */


#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>


unsigned long getThreadId(){
    std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    unsigned long threadNumber = 0;
    sscanf(threadId.c_str(), "%lx", &threadNumber);
    return threadNumber;
}

class printer
{
public:
  printer(boost::asio::io_context& io)
    : strand_(io),
	  strand2_(io),
      timer1_(io, boost::asio::chrono::seconds(1)),
      timer2_(io, boost::asio::chrono::seconds(1)),
      count_(0)
  {
	  //ͨ��strand_��Աdispatch handlers���Ա�֤handlerһ����һ����ִ�У����Ტ��ִ�С��������ﲻ�ں��Ƿ��ж���̶߳�ִ����io_context::run()
	  //�����ͨ��strand_dispatch����ͨ����ͬ��strand������ܻᲢ��ִ�С�
    timer1_.async_wait(boost::asio::bind_executor(strand_,
          boost::bind(&printer::print1, this)));

    timer2_.async_wait(boost::asio::bind_executor(strand2_,
          boost::bind(&printer::print2, this)));
  }

  ~printer()
  {
    std::cout << "Final count is " << count_ << std::endl;
  }

  void print1()
  {
    if (count_ < 100)
    {
      std::cout << getThreadId() << " Timer 1: " << count_ << std::endl;
      ++count_;

      timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));

      timer1_.async_wait(boost::asio::bind_executor(strand_,
            boost::bind(&printer::print1, this)));
    }
  }

  void print2()
  {
    if (count_ < 100)
    {
      std::cout << getThreadId() << " Timer 2: " << count_ << std::endl;
      ++count_;

      timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));

      timer2_.async_wait(boost::asio::bind_executor(strand_,
            boost::bind(&printer::print2, this)));
    }
  }

private:
  boost::asio::io_context::strand strand_;
  boost::asio::io_context::strand strand2_;
  boost::asio::steady_timer timer1_;
  boost::asio::steady_timer timer2_;
  int count_;
};

int main()
{
	std::cout << "main thread " <<  getThreadId() << std::endl;
	//�����߳�ʹ��ͬһ��io_context����
	boost::asio::io_context io;
	printer p(io);
	boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
	io.run();

	t.join();

	return 0;
}
