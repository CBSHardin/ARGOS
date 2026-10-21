
//

//delegate with one parameter
template <typename RT>

class delegate<RT, void, void, void, void, void, void >
{

  private:

     typedef RT (*executorFuncPtr)(void*);

     void *instancePtr;

     executorFuncPtr executorPtr;

     bool callbackIsSet;


  public:

    delegate()
    {

      instancePtr = 0;

      callbackIsSet = false;

    }

    bool isCallbackSet()
    {

      return callbackIsSet;

    }


    //Set callback for member function
    template <class T, RT (T::*memberFunction)()>
    void setCallback(T *iPtr = 0)
    {

      instancePtr = (void*)iPtr;

      executorPtr = &executor<T, memberFunction>;

      callbackIsSet = true;

    }


    //Set callback for free functions
    template <RT (*freeFunction)()>
    void setCallback(void *i = 0)
    {

      instancePtr = (void*)freeFunction;

      executorPtr = &executor<freeFunction>;

      callbackIsSet = true;

    }


    //Executor for member functions
    template <class T, RT (T::*memberFunction)()>
    inline static RT executor(void *ptr)
    {

      T *p = static_cast<T*>(ptr);

      return (p->*memberFunction)();

    }


    //Executor for free functions
    template <RT (*freeFunction)()>
    inline static RT executor(void *ptr)
    {

      return (*freeFunction)();

    }



    RT operator ()()
    {

      if(callbackIsSet == true)

        (*executorPtr)(instancePtr);

    }


    bool operator == (const delegate<RT> &d)
    {

      if(d.instancePtr == instancePtr)
        return true;

      else
        return false;

    }

};
