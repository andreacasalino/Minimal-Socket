
#ifndef __STREAM_SOCKET_H__
#define __STREAM_SOCKET_H__

#include <string>
#include <memory>

namespace ssk {

	class Channel {
	public:

		virtual ~Channel();

		Channel(const Channel&) = delete;
		void operator=(const Channel&) = delete;

		virtual void initialize();

/////////////
// RECEIVE //
/////////////

		void Recv(char* buffer, const std::size_t& bufferSize);

		int RecvInt();

		std::string RecvStr();

/////////////
//   SEND  //
/////////////

		void Send(const char* buffer, const std::size_t& bufferSize);

		void Send(const std::string& message);

		void Send(const int& num);

	protected:

		Channel(const std::string& server_address, const int& port);

		struct Data;
		std::unique_ptr<Data> channelData;

	private:

		bool initDone;

	};

}






#endif
