#ifndef SINGLETON_HPP
#define SINGLETON_HPP

/**
* Singleton template class.  It is designed to be inherited to create
* a singleton class out of the child class.
*/
template<typename T>
class Singleton
{
    public:
        virtual ~Singleton() { }

    /**
     * Returns an instance of the object type.
     */
    static T* getInstance()
        {
            static T theOnlyInstance;
            return &theOnlyInstance;
        }

    protected:
        Singleton() { }
};

#endif
