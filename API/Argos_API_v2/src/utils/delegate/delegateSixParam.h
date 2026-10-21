


//delegate with one parameter
template <typename RT , typename P1 = void, typename P2 = void, typename P3 = void, typename P4 = void, typename P5 = void, typename P6 = void>

class delegate
{

  private:

     typedef RT (*executorFuncPtr)(void*, P1, P2, P3, P4, P5, P6);

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
    template <class T, RT (T::*memberFunction)(P1, P2, P3, P4, P5, P6)>
    void setCallback(T *iPtr = 0)
    {

      instancePtr = (void*)iPtr;

      executorPtr = &executor<T, memberFunction>;

      callbackIsSet = true;

    }


    //Set callback for free functions
    template <RT (*freeFunction)(P1, P2, P3, P4, P5, P6)>
    void setCallback(void *i = 0)
    {

      instancePtr = (void*)freeFunction;

      executorPtr = &executor<freeFunction>;

      callbackIsSet = true;

    }


    //Executor for member functions
    template <class T, RT (T::*memberFunction)(P1, P2, P3, P4, P5, P6)>
    inline static RT executor(void *ptr, P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6)
    {

      T *p = static_cast<T*>(ptr);

      return (p->*memberFunction)(param1, param2, param3, param4, param5, param6);

    }


    //Executor for free functions
    template <RT (*freeFunction)(P1, P2)>
    inline static RT executor(void *ptr, P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6)
    {

      return (*freeFunction)(param1, param2, param3, param4, param5, param6);

    }



    RT operator ()(P1 param1, P2 param2, P3 param3, P4 param4, P5 param5, P6 param6)
    {

      if(callbackIsSet == true)

        (*executorPtr)(instancePtr, param1, param2, param3, param4, param5, param6);

    }


    bool operator == (const delegate<RT, P1, P2, P3, P4, P5, P6> &d)
    {

      if(d.instancePtr == instancePtr)
        return true;

      else
        return false;

    }

};
