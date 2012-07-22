#!/bin/sh
# Writed by JianYi On 2008-5-20

# ��������
log_filename= # ��־�ļ���
log_bakeup_nubmer=10 # ��־���ݸ���
log_rotate_bytes=102400000 # ��־�����ֽ���

# ��;���õ��ļ���С
# �������ļ���
# ����ֵ�����ʧ�ܷ���0�����򷵻��ļ���С
get_file_size()
{
	file_size=`ls --time-style=long-iso -l $1 2>/dev/null|cut -d" " -f5`
	if test -z $size; then
		echo "0"
	else
		echo $file_size
	fi
}

# ��;���õ���־�ļ�����
# ��������־�ļ����Ļ��������������������ֵ����֣��磺test.log��������test.log.1
# ����ֵ���ɹ���������ֵ��ʧ�ܷ���-1
get_log_file_index()
{
    log_file_basename=$1
    index=`ls $log_file_basename.[0-9] 2>/dev/null|cut -d"." -f3|sort -n|sed -n '$p'`
    if test -z $index; then
        echo "-1"
    else
        echo $index
    fi
}

# ��;��������־�ļ�
# ��������־�ļ���
rotate_log_file()
{
	log_file_basename=$1
	index=`get_log_file_index $log_file_basename`
	if test $index -eq -1; then
		mv $log_file_basename $log_file_basename.0
		return
	elif test $index -ge $log_file_roll_num; then
        index=$log_file_roll_num
	fi
	
	# ѭ�������ļ�, ������Խ���ļ�����Խ��
    while true
    do
        let index_next=$index+1
        if test -f $log_filename.$index; then
            mv $log_file_basename.$index $log_file_basename.$index_next
        fi
        
        if test $index -eq 0; then
            # ����ѭ��
            if test -f $log_file_basename; then
                mv $log_file_basename $log_file_basename.0
            fi
            return
        else
            # ��������һ
            let index=$index-1
        fi                
    done
}

# ��;��д��־����
# ��������־�����ַ���
log_write()
{
	# �õ���־�ļ���С
	log_size=`get_file_size $log_filename`
	
	# ������ڹ�����С
	if test $log_size -gt $log_roll_bytes; then
		roll_log_file $log_filename
	else
		# �õ���ǰʱ��
		now="`date +'%Y-%m-%d/%H:%M:%S'`"
		echo "[$now]$1" >> $log_filename
	fi
}
