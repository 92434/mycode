//为了方便把头文件定义到了Mutex.h文件里面去了，修改Mutex.h如、//下：
#define synchronized(M)  for(Lock M##_lock = M; M##_lock; M##_lock.SetUnlock())//这句话后面会用到
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

