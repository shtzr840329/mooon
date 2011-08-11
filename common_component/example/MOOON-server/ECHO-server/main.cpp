#include <util/string_util.h>
#include <sys/main_template.h>
#include "config_impl.h"
#include "factory_impl.h"

/***
  * ʹ��main����ģ�壬��Ҫʵ��sys::IMainHelper�ӿ�
  */
class CMainHelper: public sys::IMainHelper
{
public:
    CMainHelper();

private:        
    virtual bool init(int argc, char* argv[]);    
    virtual void fini();
    virtual int get_exit_signal() const { return SIGUSR1; }

private:
    uint16_t get_listen_port(int argc, char* argv[]);

private:
    server::server_t _server;
    CConfigImpl _config_impl;
    CFactoryImpl _factory_impl;
};

/***
  * �ɴ�һ���˿ںŲ�����Ҳ�ɲ����κβ�����Ĭ�϶˿ں�Ϊ2012
  */
int main(int argc, char* argv[])
{
    CMainHelper main_helper;
    return sys::main_template(&main_helper, argc, argv);
}

CMainHelper::CMainHelper()
    :_server(NULL)
{
}

bool CMainHelper::init(int argc, char* argv[])
{
    uint16_t port = get_listen_port(argc, argv);
    _config_impl.init(port);

    // ����һ��MOOON-server���ʵ��
    _server = server::create(&_config_impl, &_factory_impl);
    return _server != NULL;
}

void CMainHelper::fini()
{
    if (_server != NULL)
    {
        // ����MOOON-server���ʵ��
        server::destroy(_server);
        _server = NULL;
    }
}

uint16_t CMainHelper::get_listen_port(int argc, char* argv[])
{
    uint16_t port = 0;

    if (argc > 1)
    {
        (void)util::CStringUtil::string2int(argv[1], port);
    }

    return port;
}
