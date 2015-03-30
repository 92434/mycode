//Ϊ�˷����ͷ�ļ����嵽��Mutex.h�ļ�����ȥ�ˣ��޸�Mutex.h�硢//�£�
#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.SetUnlock())//��仰������õ�
class Mutex
{
public:
    Mutex(void);
    ~Mutex(void);
    void Lock();
    void UnLock();
private:
    unsigned long m_criticalSection;
};

