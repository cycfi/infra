/*=============================================================================
   Copyright (c) 2016-2018 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_TIMER_JUNE_15_2019)
#define CYCFI_TIMER_JUNE_15_2019

#include <chrono>
#include <thread>

namespace cycfi
{
   template <typename F>
   class timer
   {
   public:

      using clock = std::chrono::high_resolution_clock;

      timer(F task_)
       : _task(task_)
       , _start(clock::now())
       , _loop([this]{ run(); })
      {}

      ~timer()
      {
         _running = false;
         _loop.join();
      }

   private:

      void run()
      {
         using namespace std::chrono_literals;

         while (_running)
         {
            std::this_thread::sleep_for(1ms);
            auto now = clock::now();
            if (now >= (_start + _period))
            {
               _start = now;
               if (!_task())
                  _running = false;
            }
         }
      }

      using milliseconds = std::chrono::milliseconds;
      using time_point = std::chrono::time_point<clock>;
      using atomic_bool = std::atomic<bool>;

      F              _task;
      time_point     _start;
      milliseconds   _period;
      std::thread    _loop;
      atomic_bool    _running = true;
   };

   template <typename F>
   inline timer<F> make_timer(F f)
   {
      return { f };
   }
}

#endif