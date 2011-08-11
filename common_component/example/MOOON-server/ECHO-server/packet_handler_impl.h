#ifndef MOOON_ECHO_SERVER_PACKET_HANDLER_IMPL_H
#define MOOON_ECHO_SERVER_PACKET_HANDLER_IMPL_H
#include <server/server.h>

class CPakcetHandlerImpl: public server::IPacketHandler
{
public:    
    CPakcetHandlerImpl(server::IConnection* connection);
    ~CPakcetHandlerImpl();

private:  
    virtual void reset();

    virtual char* get_request_buffer();
    virtual size_t get_request_size() const;        
    virtual util::handle_result_t on_handle_request(size_t data_size, server::Indicator& indicator);
             
    virtual const char* get_response_buffer() const;
    virtual size_t get_response_size() const;
    virtual size_t get_response_offset() const;
    virtual void move_response_offset(size_t offset);
    virtual util::handle_result_t on_response_completed(server::Indicator& indicator);

private:
    server::IConnection* _connection; // ����������
    size_t _request_size;    // �������󻺳���������ֽ���
    size_t _response_size;   // ��Ҫ���͵���Ӧ�����ֽ���
    size_t _response_offset; // ��ǰ�Ѿ����͵���Ӧ�����ֽ���
    char* _request_buffer;   // ��������������ݵĻ�����
};

#endif // MOOON_ECHO_SERVER_PACKET_HANDLER_IMPL_H
