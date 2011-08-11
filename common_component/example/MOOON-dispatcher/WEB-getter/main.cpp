#include <sys/logger.h>
#include <util/args_parser.h>
#include <sys/main_template.h>
#include <dispatcher/dispatcher.h>
#include "getter.h"

INTEGER_ARG_DEFINE(true, uint16_t, port, 80, 1, 65535, "web server port")
STRING_ARG_DEFINE(false, dn, "", "domain name")
STRING_ARG_DEFINE(false, url, "", "URL")

class CMainHelper: public sys::IMainHelper
{
public:
    CMainHelper();

private:
    virtual bool init(int argc, char* argv[]);
    virtual void fini();
    virtual int get_exit_signal() const;

private:
    sys::CLogger _logger;
    dispatcher::IDispatcher* _dispatcher;
};

// ���main��������ĳ��namespace�ڲ�������Ҫextern "C"����
// ����������ʱ���ᱨmain����δ���壬ԭ����C++��������Ժ��������б���
extern "C" int main(int argc, char* argv[])
{
    CMainHelper main_helper;
    return sys::main_template(&main_helper, argc, argv);
}

CMainHelper::CMainHelper()
    :_dispatcher(NULL)
{
}

bool CMainHelper::init(int argc, char* argv[])
{    
    // ���������в���
    if (!ArgsParser::parse(argc, argv))
    {
        fprintf(stderr, "Command parameter error: %s.\n", ArgsParser::g_error_message.c_str());
        return false;
    }

    // ȷ����־�ļ����Ŀ¼
    // �������ｫ��־�ļ��ͳ����ļ�����ͬһ��Ŀ¼��
    std::string logdir = sys::CUtil::get_program_path();

    // ������־�������ɵ���־�ļ���Ϊweg_getter.log
    _logger.create(logdir.c_str(), "web_getter.log");

    // ������־��
    dispatcher::logger = &_logger;

    // ����MOOON-dispatcher���ʵ��
    _dispatcher = dispatcher::create(2);
    if (_dispatcher != NULL)
    {
        CGetter::get_singleton()->set_port(ArgsParser::port->get_value());
        CGetter::get_singleton()->set_domain_name(ArgsParser::dn->get_value());
        CGetter::get_singleton()->set_url(ArgsParser::url->get_value());
        CGetter::get_singleton()->set_dispatcher(_dispatcher);

        return CGetter::get_singleton()->start();
    }
    
    return false;
}

void CMainHelper::fini()
{
    if (_dispatcher != NULL)
    {
        // ����MOOON-dispatcher���ʵ��
        dispatcher::destroy(_dispatcher);
        _dispatcher = NULL;
    }
}

int CMainHelper::get_exit_signal() const
{ 
    // ����������һ���˳��źţ�������ֵΪ0ʱ��
    // ������Ҫ�л���ȥ��֤���̲߳�ֱ���˳�
    // ������ͨ������£�������һ���˳��ź�
    return SIGUSR1; 
}
