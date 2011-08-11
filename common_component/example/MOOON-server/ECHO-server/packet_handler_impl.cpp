#include <sys/util.h>
#include "packet_handler_impl.h"

CPakcetHandlerImpl::CPakcetHandlerImpl(server::IConnection* connection)
    :_connection(connection)
{
    _response_size = 0;
    _response_offset = 0;
    _request_size = sys::CUtil::get_page_size();
    _request_buffer = new char[_request_size];
}

CPakcetHandlerImpl::~CPakcetHandlerImpl()
{
    delete []_request_buffer;
}

void CPakcetHandlerImpl::reset()
{
    _response_size = 0;
    _response_offset = 0;
}

char* CPakcetHandlerImpl::get_request_buffer()
{
    return _request_buffer;
}

size_t CPakcetHandlerImpl::get_request_size() const
{
    return _request_size;
}

util::handle_result_t CPakcetHandlerImpl::on_handle_request(size_t data_size, server::Indicator& indicator)
{
    _response_size = data_size;
    return util::handle_finish; /** finish��ʾ�����Ѿ�������ɣ��ɽ�����Ӧ���� */
}

const char* CPakcetHandlerImpl::get_response_buffer() const
{
    return _request_buffer;
}

size_t CPakcetHandlerImpl::get_response_size() const
{
    return _response_size;
}

size_t CPakcetHandlerImpl::get_response_offset() const
{
    return _response_offset;
}

void CPakcetHandlerImpl::move_response_offset(size_t offset)
{
    _response_offset += offset;
}

util::handle_result_t CPakcetHandlerImpl::on_response_completed(server::Indicator& indicator)
{
    // ����յ�quitָ���ر�����
    return 0 == strncmp(_request_buffer, "quit", sizeof("quit")-1)
        ? util::handle_finish /** finish��ʾ�ɹر����� */
        : util::handle_continue; /** continue��ʾ���ӱ��֣���Ҫ�ر� */
}
