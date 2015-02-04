#ifndef NOTCOPYABLE_HPP
#define NOTCOPYABLE_HPP

/**
 * Design pattern to ensures derived classes are not copyable.
 */
class NotCopyable
{
    public:
        NotCopyable() { }
        ~NotCopyable() { }

    private:
        NotCopyable(const NotCopyable&);
        const NotCopyable& operator= (const NotCopyable&);
};

#endif
