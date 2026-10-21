


//delegate with one parameter
template <typename RT, typename P1>

class delegate<RT, P1, void, void, void, void, void >
{

  private:

     typedef RT (*executorFuncPtr)(void*, P1);

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
    template <class T, RT (T::*memberFunction)(P1)>
    void setCallback(T *iPtr = 0)
    {

      instancePtr = (void*)iPtr;

      executorPtr = &executor<T, memberFunction>;

      callbackIsSet = true;

    }


    //Set callback for free functions
    template <RT (*freeFunction)(P1)>
    void setCallback(void *i = 0)
    {

      instancePtr = (void*)freeFunction;

      executorPtr = &executor<freeFunction>;

      callbackIsSet = true;

    }


    //Executor for member functions
    template <class T, RT (T::*memberFunction)(P1)>
    inline static RT executor(void *ptr, P1 param1)
    {

      T *p = static_cast<T*>(ptr);

      return (p->*memberFunction)(param1);

    }


    //Executor for free functions
    template <RT (*freeFunction)(P1)>
    inline static RT executor(void *ptr, P1 param1)
    {

      return (*freeFunction)(param1);

    }



    RT operator ()(P1 param1)
    {

      if(callbackIsSet == true)

        (*executorPtr)(instancePtr, param1);

    }


    bool operator == (const delegate<RT, P1> &d)
    {

      if(d.instancePtr == instancePtr)
        return true;

      else
        return false;

    }

};
