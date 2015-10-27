class Lock
{
public:
    Lock(Mutex &mutex);
    ~Lock(void);
    void SetUnlock();
    operator bool () const;
private:
    Mutex &m_mutex;
    bool m_locked;
};
