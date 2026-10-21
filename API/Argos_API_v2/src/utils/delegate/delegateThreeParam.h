


//delegate with one parameter
template <typename RT , typename P1, typename P2, typename P3>

class delegate<RT, P1, P2, P3, void, void, void>
{

  private:

     typedef RT (*executorFuncPtr)(void*, P1, P2, P3);

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
    template <class T, RT (T::*memberFunction)(P1, P2, P3)>
    void setCallback(T *iPtr = 0)
    {

      instancePtr = (void*)iPtr;

      executorPtr = &executor<T, memberFunction>;

      callbackIsSet = true;

    }


    //Set callback for free functions
    template <RT (*freeFunction)(P1, P2, P3)>
    void setCallback(void *i = 0)
    {

      instancePtr = (void*)freeFunction;

      executorPtr = &executor<freeFunction>;

      callbackIsSet = true;

    }


    //Executor for member functions
    template <class T, RT (T::*memberFunction)(P1, P2, P3)>
    inline static RT executor(void *ptr, P1 param1, P2 param2, P3 param3)
    {

      T *p = static_cast<T*>(ptr);

      return (p->*memberFunction)(param1, param2, param3);

    }


    //Executor for free functions
    template <RT (*freeFunction)(P1, P2, P3)>
    inline static RT executor(void *ptr, P1 param1, P2 param2, P3 param3)
    {

      return (*freeFunction)(param1, param2, param3);

    }



    RT operator ()(P1 param1, P2 param2, P3 param3)
    {

      if(callbackIsSet == true)

        (*executorPtr)(instancePtr, param1, param2, param3);

    }


    bool operator == (const delegate<RT, P1, P2, P3> &d)
    {

      if(d.instancePtr == instancePtr)
        return true;

      else
        return false;

    }

};
